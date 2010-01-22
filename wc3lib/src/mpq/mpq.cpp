/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.de                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <cstring>
#include <cmath>
#include <sstream>

#include <boost/format.hpp>

#include "mpq.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mpq
{

// The encryption and hashing functions use a number table in their procedures. This table must be initialized before the functions are called the first time.
static void InitializeCryptTable(uint32 dwCryptTable[0x500])
{
    uint32 seed   = 0x00100001;
    uint32 index1 = 0;
    uint32 index2 = 0;
    int16   i;

    for (index1 = 0; index1 < 0x100; index1++)
    {
        for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
        {
            uint32 temp1, temp2;

            seed  = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;

            seed  = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);

            dwCryptTable[index2] = (temp1 | temp2);
        }
    }
}

static void EncryptData(const uint32 dwCryptTable[0x500], void *lpbyBuffer, uint32 dwLength, uint32 dwKey)
{
    assert(lpbyBuffer);

    uint32 *lpdwBuffer = (uint32 *)lpbyBuffer;
    uint32 seed = 0xEEEEEEEE;
    uint32 ch;

    dwLength /= sizeof(uint32);

    while(dwLength-- > 0)
    {
        seed += dwCryptTable[0x400 + (dwKey & 0xFF)];
        ch = *lpdwBuffer ^ (dwKey + seed);

        dwKey = ((~dwKey << 0x15) + 0x11111111) | (dwKey >> 0x0B);
        seed = *lpdwBuffer + seed + (seed << 5) + 3;

		*lpdwBuffer++ = ch;
    }
}

static void DecryptData(const uint32 dwCryptTable[0x500], void *lpbyBuffer, uint32 dwLength, uint32 dwKey)
{
    assert(lpbyBuffer);

    uint32 *lpdwBuffer = (uint32 *)lpbyBuffer;
    uint32 seed = 0xEEEEEEEEL;
    uint32 ch;

    dwLength /= sizeof(uint32);

    while(dwLength-- > 0)
    {
        seed += dwCryptTable[0x400 + (dwKey & 0xFF)];
        ch = *lpdwBuffer ^ (dwKey + seed);

        dwKey = ((~dwKey << 0x15) + 0x11111111L) | (dwKey >> 0x0B);
        seed = ch + seed + (seed << 5) + 3;

		*lpdwBuffer++ = ch;
    }
}

enum HashType
{
	TableOffset = 0,
	NameA = 1,
	NameB = 2,
	FileKey = 3
};

// Based on code from StormLib.
uint32 HashString(const uint32 dwCryptTable[0x500], const char *lpszString, enum HashType dwHashType)
{
    assert(lpszString);
    
    uint32  seed1 = 0x7FED7FEDL;
    uint32  seed2 = 0xEEEEEEEEL;
    int16    ch;

    while (*lpszString != 0)
    {
        ch = toupper(*lpszString++);

        seed1 = dwCryptTable[(dwHashType * 0x100) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
    }
    
    return seed1;
}

struct Header
{
	char magic[4];
	int32 headerSize;
	int32 archiveSize;
	int16 formatVersion;
	int8 sectorSizeShift;
	int32 hashTableOffset;
	int32 blockTableOffset;
	int32 hashTableEntries;
	int32 blockTableEntries;
};

struct ExtendedHeader // MPQ format 2
{
	int64 extendedBlockTableOffset;
	int16 hashTableOffsetHigh;
	int16 blockTableOffsetHigh;
};

struct BlockTableEntry
{
	int32 blockOffset;
	int32 blockSize;
	int32 fileSize;
	int32 flags;
};

struct ExtendedBlockTableEntry
{
	int16 extendedBlockOffset;
};

struct HashTableEntry
{
	int32 filePathHashA;
	int32 filePathHashB;
	int16 language;
	int8 platform;
	int32 fileBlockIndex;
};

struct FileData
{
	int32 *sectorOffsetTable;
};

struct ExtendedAttributes
{
	int32 version;
	int32 attributesPresent;
};

struct WeakDigitalSignature
{	
	int32 unknown0;
	int32 unknown1;
};

struct StrongDigitalSignature
{
	char magic[4];
	char signature[256];
//	int2048 signature; //int2048, little-endian format
};

const char Mpq::identifier[4] = { 'M', 'P', 'Q',  0x1 };
const int16 Mpq::format1Identifier = 0x0000;
const int16 Mpq::format2Identifier = 0x0001;
const int32 Mpq::extendedAttributesVersion = 100;
const std::size_t Mpq::headerSize = sizeof(struct Header);

Mpq::Mpq(const boost::filesystem::path &path) : m_path(path), m_size(boost::filesystem::file_size(path))
{
}

Mpq::~Mpq()
{
	BOOST_FOREACH(class Block *block, this->m_blocks)
		delete block;
	
	BOOST_FOREACH(class Hash *hash, this->m_hashes)
		delete hash;

	BOOST_FOREACH(class MpqFile *file, this->m_files)
		delete file;
}

std::streamsize Mpq::readMpq(std::istream &istream) throw (class Exception)
{
	struct Header header;
	istream.read(reinterpret_cast<char*>(&header), sizeof(header));
	std::streamsize bytes = istream.gcount();
	
	if (bytes < sizeof(header))
		throw Exception(_("Error while reading MPQ header."));

	if (memcmp(header.magic, Mpq::identifier, sizeof(Mpq::identifier)))
		throw Exception(boost::str(boost::format(_("Missing MPQ identifier \"%1%\".")) % Mpq::identifier));

	if (header.formatVersion == Mpq::formatVersion1Identifier)
		this->m_format = Mpq::Mpq1;
	else if (header.formatVersion == Mpq::formatVersion2Identifier)
		this->m_format = Mpq::Mpq2;
	else
		throw Exception(boost::str(boost::format(_("Unknown MPQ format \"%1%\".")) % header.formatVersion));
	
	// Number of entries in the hash table. Must be a power of two, and must be less than 2^16 for the original MoPaQ format, or less than 2^20 for the Burning Crusade format.
	if ((this->m_format == Mpq::Mpq1 && header.hashTableEntries >= pow(2, 16)) || (this->m_format == Mpq::Mpq2 && header.hashTableEntries >= pow(2, 20)))
		std::cerr << boost::format(_("Warning: There are too many MPQ hash table entries in MPQ file \"%1%\".\nEntries: %2%.")) % this->m_path.string() % header.hashTableEntries() << std::endl;
	
	// According to the StormLib this value is sometimes changed by map creators to protect their maps. As in the StormLib this value is ignored.
	if (header.headerSize != sizeof(header))
		std::cerr << boost::format(_("Warning: MPQ header size is not equal to real header size.\nContained header size: %1%.\nReal header size: %2%.")) % header.headerSize % sizeof(header) << std::endl;

	if (header.archiveSize != this->m_size)
		std::cerr << boost::format(_("Warning: MPQ file size of MPQ file \"%1%\" is not equal to its internal header file size.\nFile size: %2%.\nInternal header file size: %3%.")) % this->m_path.string() % this->m_size % header.archiveSize << std::endl;
	
	struct ExtendedHeader extendedHeader;
	
	if (this->m_format == Mpq::Mpq2)
	{
		istream.read(reinterpret_cast<char*>(&extendedHeader), sizeof(extendedHeader));
		bytes += istream.gcount();
	}
	
	int64 offset = header.blockTableOffset;
	
	if (this->m_format == Mpq::Mpq2 && extendedHeader.blockTableOffsetHigh > 0)
		offset += int64(extendedHeader.blockTableOffsetHigh) << 32;
	
	istream.seekg(offset);

	std::size_t encryptedBytesSize = header.blockTableEntries * sizeof(struct BlockTableEntry);
	char *encryptedBytes = new char[encryptedBytesSize];
	
	try
	{
		istream.read(encryptedBytes, encryptedBytesSize);
		uint32 hashValue = HashString(Mpq::cryptTable(), "(block table)", HashType(1)); // HashType.NameA
		DecryptData(Mpq::cryptTable(), reinterpret_cast<void*>(encryptedBytes), encryptedBytesSize, hashValue);
		std::stringstream sstream(encryptedBytes);
		
		for (std::size_t i = 0; i < header.blockTableEntries; ++i)
		{
			class Mpq::Block *block = new Block(this);
			bytes += block->read(sstream);
			this->m_blocks.push_back(block);
		}
	}
	catch (...)
	{
		delete[] encryptedBytes;
	}
	
	delete[] encryptedBytes;
	encryptedBytes = 0;
	
	/*
	read extended block table
	As of the Burning Crusade Friends and Family beta, this table is not encrypted.
	*/
	if (this->m_format == Mpq::Mpq2)
	{
		istream.seekg(extendedHeader.extendedBlockTableOffset);
		
		BOOST_FOREACH(class Mpq::Block *block, this->m_blocks)
		{
			struct ExtendedBlockTableEntry extendedBlockTableEntry;
			bytes += istream.read(reinterpret_cast<char*>(&extendedBlockTableEntry), sizeof(extendedBlockTableEntry));
			block->m_extendedBlockOffset = extendedBlockTableEntry.extendedBlockOffset;
		}
	}
			
	// read encrypted hash table
	offset = header.hashTableOffset;
		 
	if (this->m_format == Mpq::Mpq2 && extendedHeader.hashTableOffsetHigh > 0)
		offset += int64(extendedHeader.hashTableOffsetHigh) << 32;
	
	istream.seekg(offset);
	
	encryptedBytesSize = header.hashTableEntries * sizeof(struct HashTableEntry);
	encryptedBytes = new char[encryptedBytesSize];
	
	try
	{
		istream.read(encryptedBytes, encryptedBytesSize);
		uint32 hashValue = HashString(Mpq::cryptTable(), "(hash table)", HashType(1)); // HashType.NameA
		DecryptData(Mpq::cryptTable(), reinterpret_cast<void*>(encryptedBytes), encryptedBytesSize, hashValue);
		std::stringstream sstream(encryptedBytes);
		
		for (std::size_t i = 0; i < header.hashTableEntries; ++i)
		{
			class Mpq::Hash *hash = new Hash(this);
			bytes += hash->read(sstream);
			this->m_hashes.push_back(hash);
		}
	}
	catch (...)
	{
		delete[] encryptedBytes;
	}
	
	delete[] encryptedBytes;
	encryptedBytes = 0;
	
	//this->findFile("(listfile)"); listfile contains file paths
	
	// file data / add file instances
	BOOST_FOREACH(class Mpq::Hash *hash, this->m_hashes)
	{
		if (!hash->empty() && !hash->deleted())
		{
			class MpqFile *mpqFile = new MpqFile(this, hash);
			mpqFile->m_size = hash->m_block->m_fileSize; // uncompressed size
			//mpqFile->m_compressedSize = hash->m_block->m_blockSize
			mpqFile->m_locale = MpqFile::intToLocale(hash->m_language);
			mpqFile->m_platform = MpqFile::intToPlatform(hash->m_platform);
			//mpqFile->m_path = // path can only be set if there is a listfile file or if we're able to convert its hash into file path
			
			// sector offset table
			if (!(hash->m_block->m_flags & Mpq::Block::IsSingleUnit))
			{
			}
			
			this->m_files.push_back(mpqFile);
		}
	}

	
	/// @todo Skip file data (sectors)
	
	// extended block attributes
	struct ExtendedAttributes extendedAttributes;
	istream.read(reinterpret_cast<char*>(&extendedAttributes), sizeof(extendedAttributes));
	bytes += istream.gcount();
	
	if (extendedAttributes.version != Mpq::extendedAttributesVersion)
		std::cout << boost::format(_("Warning: Unknown extended attributes version \"%1%\".\nCurrent version is \"%2%\".")) % extendedAttributes.version % Mpq::extendedAttributesVersion << std::endl;
	
	this->m_extendedAttributes = extendedAttributes.attributesPresent; /// @todo Convert to enum
	
	if (this->m_extendedAttributes & Mpq::FileCrc32s)
	{
		BOOST_FOREACH(class Block *block, this->m_blocks)
			bytes += istream.read(reinterpret_cast<char*>(&block->m_crc32), sizeof(block->m_crc32));
	}
		
	if (this->m_extendedAttributes & Mpq::FileTimeStamps)
	{
		BOOST_FOREACH(class Block *block, this->m_blocks)
			bytes += istream.read(reinterpret_cast<char*>(&block->m_fileTime), sizeof(block->m_fileTime));
	}
	
	if (this->m_extendedAttributes & Mpq::FileMd5s)
	{
		BOOST_FOREACH(class Block *block, this->m_blocks)
			bytes += istream.read(reinterpret_cast<char*>(&block->m_md5), sizeof(block->m_md5));
	}
	
	return bytes;
	
}

/// @todo Write with format this->m_format!
std::streamsize Mpq::writeMpq(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

const class MpqFile* Mpq::findFile(const boost::filesystem::path &path) const
{
	BOOST_FOREACH(const class MpqFile *mpqFile, this->m_files)
	{
		if (mpqFile->path() == path)
			return mpqFile;
	}
	
	return 0;
}

const class MpqFile* Mpq::findFileByName(const std::string &name) const
{
	BOOST_FOREACH(const class MpqFile *mpqFile, this->m_files)
	{
		if (mpqFile->name() == name)
			return mpqFile;
	}
	
	return 0;
}

const class MpqFile* Mpq::findFileByHash(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform) const
{
	if (this->m_hashes.empty() || this->m_hashes.front()->empty())
		return 0;
		
	// Compute the hashes to compare the hash table entry against
	uint32 nameHashA = HashString(Mpq::cryptTable(), path.string().c_str(), HashType.NameA);
	uint32 nameHashB = HashString(Mpq::cryptTable(), path.string().c_str(), HashType.NameB);	
	int16 language = MpqFile::localeToInt(locale);
	int8 realPlatform = MpqFile::platformToInt(platform);
	
	// Check each entry in the hash table till a termination point is reached
	std::list<class Mpq::Hash*>::const_iterator iterator = this->m_hashes.begin();
	
	do
	{
		if (!(*iterator)->deleted())
		{
			if ((*iterator)->m_filePathHashA == nameHashA 
				&& (*iterator)->m_filePathHashB == nameHashB
				&& (*iterator)->m_language == language
				&& (*iterator)->m_platform == realPlatform)
				return const_cast<const class MpqFile*>((*iterator)->m_mpqFile);
		}
			
		++iterator;
	}
	while (iterator != this->m_hashes.end() && !(*iterator)->empty());
	
	return 0;
}

class MpqFile* Mpq::addFile(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform, bool overwriteExisting, std::size_t reservedSpace) throw (class Exception)
{
	class MpqFile *mpqFile = this->findFileByHash(path, locale, platform);
	
	if (mpqFile != 0)
	{
		if (!overwriteExisting)
			return 0;
		
		// overwrite existing
		mpqFile->remove();
	}
	
	class Mpq::Block *block = 0;
	
	if (reservedSpace == 0)
	{
		block = this->firstEmptyBlock();
		
		if (block == 0)
			block = this->firstUnusedBlock();
	}
	else
	{
		// get block with the best size
		BOOST_FOREACH(class Block *iteratedBlock, this->m_blocks)
		{
			if ((iteratedBlock->empty() || iteratedBlock->unused()) && iteratedBlock->m_blockSize >= reservedSpace && (block == 0 || iteratedBlock->m_blockSize - reservedSpace < block->m_blockSize - reservedSpace))
			{
				block = iteratedBlock;
				block->m_fileSize = reservedSpace;
			}
		}
	}
	
	bool newBlock = false;
	
	// no block was found, create new one
	if (block == 0)
	{
		newBlock = true;
		block = new Mpq::Block(this);
		this->nextBlockOffsets(block->m_blockOffset, block->m_extendedBlockOffset);
		
		if (this->m_format != Mpq::Mpq2 && block->m_extendedOffset > 0)
			throw Exception(_("Extended block offset can not be used in MPQ format 1."));
		
		block->m_blockSize = reservedSpace;
		block->m_fileSize = reservedSpace;
		block->m_flags = Mpq::Block::IsFile; /// @todo Set right flags (user-defined).
		this->m_blocks.push_back(block);
		
	}
	
	class Mpq::Hash *hash = this->firstDeletedHash();
		
	if (hash == 0)
		hash = this->firstEmptyHash();
	else
		hash->m_deleted = false;
	
	
	if (hash == 0)
	{
		if (newBlock)
			delete block;
		
		throw Exception(_("There isn't any hash table entry left."));
	}
	
	hash->m_block = block;
	hash->m_filePathHashA = HashString(Mpq::cryptTable(), path.string(), HashType.NameA);
	hash->m_filePathHashB = HashString(Mpq::cryptTable(), path.string(), HashType.NameB);
	hash->m_language = MpqFile::localeToInt(locale);
	hash->m_platform = MpqFile::platformToInt(platform);
	hash->m_mpqFile = new MpqFile(this, hash);
	this->m_files.push_back(hash->m_mpqFile);
	
	/// @todo Write block table entry into file. This can take a lot of time since the whole MPQ data after the entry has to be written newly.
	if (newBlock)
		;
	
	/// @todo Write file data/free reserved space in MPQ.
		
	return hash->m_mpqFile;
}

const uint32* Mpq::cryptTable()
{
	static uint32 cryptTable[0x500];
	static bool isInitialized = false;
	
	if (!isInitialized)
	{
		InitializeCryptTable(cryptTable);
		isInitialized = true;
	}
	
	return const_cast<const uint32*>(cryptTable);
}

Mpq::Block::Block(class Mpq *mpq) : m_mpq(mpq), m_blockOffset(0), m_extendedBlockOffset(0), m_blockSize(0), m_fileSize(0) m_flags(Mpq::Block::None)
{
}

std::streamsize Mpq::Block::read(std::istream &istream) throw (class Exception)
{
	struct BlockTableEntry entry;
	istream.read(reinterpret_cast<char*>(&entry), sizeof(entry));
	std::streamsize bytes = istream.gcount();

	if (bytes != sizeof(entry))
		throw Exception(_("Error while reading block table entry."));

	this->m_blockOffset = entry.blockOffset;
	this->m_blockSize = entry.blockSize;
	this->m_fileSize = entry.fileSize;
	this->m_flags = static_cast<enum Mpq::Block::Flags>(entry.fileSize);

	return bytes;
}

Mpq::Hash::Hash(class Mpq *mpq) : m_mpq(mpq), m_mpqFile(0), m_block(0)
{
}

std::streamsize Mpq::Hash::read(std::istream &istream) throw (class Exception)
{
	struct HashTableEntry entry;
	istream.read(reinterpret_cast<char*>(&entry), sizeof(entry));
	std::streamsize bytes = istream.gcount();
	
	if (bytes != sizeof(entry))
		throw Exception(_("Error while reading hash table entry."));
	
	this->m_filePathHashA = entry.filePathHashA;
	this->m_filePathHashB = entry.filePathHashB;
	this->m_language = entry.language;
	this->m_platform = entry.platform;
	
	if (entry.fileBlockIndex == Mpq::Hash::blockIndexDeleted)
		this->m_deleted = true;
	else if (entry.fileBlockIndex != Mpq::Hash::blockIndexEmpty)
	{
		int32 index = 0;
		
		BOOST_FOREACH(class Mpq::Block *block, this->m_mpq->m_blocks)
		{
			if (index == entry.fileBlockIndex)
			{
				this->m_block = block;
				
				break;
			}
			
			++index;
		}
		
		if (this->m_block == 0)
			throw Exception(_("Error while searching for corresponding block of hash table entry."));
	}
	// otherwise it's empty
	
	return bytes;
}

/// @todo Clear or write file hash and block data!
void Mpq::Hash::clear()
{
	// If the next entry is empty, mark this one as empty; otherwise, mark this as deleted.
	for (std::list<class Mpq::Hash*>::const_iterator iterator = this->m_mpq->m_hashes.begin(); iterator != this->m_mpq->m_hashes.end(); ++iterator)
	{
		if (*iterator == this)
		{
			if (iterator + 1 != this->m_mpq->m_hashes.end() && !(*(iterator + 1))->empty())
				this->m_deleted = true;
			
			break;
		}
	}
	
	// If the block occupies space, mark the block as free space; otherwise, clear the block table entry.
	if (this->m_block->m_blockSize > 0)
	{
		this->m_block->m_fileSize = 0;
		this->m_block->m_flags = Mpq::Block::None;
	}
	else
	{
		/// @todo Change file size?
		this->m_mpq->m_blocks.remove(this->m_block);
		delete this->m_block;
	}
		
	this->m_block = 0;
}

}

}
