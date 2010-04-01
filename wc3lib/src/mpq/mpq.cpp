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

#include <iostream>
#include <cstring>
#include <cmath>
#include <sstream>

#include <boost/format.hpp>

#include "algorithm.hpp"
#include "mpq.hpp"
#include "sector.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mpq
{	

const byte Mpq::identifier[4] = { 'M', 'P', 'Q',  0x1A };
const int16 Mpq::formatVersion1Identifier = 0x0000;
const int16 Mpq::formatVersion2Identifier = 0x0001;
const int32 Mpq::extendedAttributesVersion = 100;
const std::size_t Mpq::headerSize = sizeof(struct Header);

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

Mpq::Mpq(const boost::filesystem::path &path) : m_size(boost::filesystem::file_size(path)), m_path(path), m_format(Mpq::Mpq1), m_extendedAttributes(Mpq::None), m_sectorSizeShift(0), m_strongDigitalSignature(0)
{
}

Mpq::~Mpq()
{
	if (this->m_strongDigitalSignature != 0)
		delete[] this->m_strongDigitalSignature;
	
	BOOST_FOREACH(class Block *block, this->m_blocks)
		delete block;
	
	BOOST_FOREACH(class Hash *hash, this->m_hashes)
		delete hash;

	BOOST_FOREACH(class MpqFile *file, this->m_files)
		delete file;
}

std::streamsize Mpq::readMpq(std::istream &istream, std::istream *listfileIstream) throw (class Exception)
{
	// find header structure by using file key
	byte identifier[4];
	
	do
	{
		istream.read(reinterpret_cast<char*>(identifier), 4);
		
		if (istream.gcount() < 4)
			throw Exception(boost::str(boost::format(_("Missing identifier \"%1%\".")) % Mpq::identifier));
	}
	while (memcmp(identifier, Mpq::identifier, 4) != 0);
	
	std::cout << "Found MPQ identifier at offset " << (istream.tellg() - std::streamoff(4)) << std::endl;
	
	istream.seekg(-4, std::ios::cur);
	std::streampos startPosition = istream.tellg();
	std::cout << "Is now at " << startPosition << std::endl;
	
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
		std::cerr << boost::format(_("Warning: There are too many MPQ hash table entries in MPQ file \"%1%\".\nEntries: %2%.")) % this->m_path.string() % header.hashTableEntries << std::endl;
	
	// According to the StormLib this value is sometimes changed by map creators to protect their maps. As in the StormLib this value is ignored.
	if (header.headerSize != sizeof(header))
		std::cerr << boost::format(_("Warning: MPQ header size is not equal to real header size.\nContained header size: %1%.\nReal header size: %2%.")) % header.headerSize % sizeof(header) << std::endl;

	if (header.archiveSize != this->m_size)
		std::cerr << boost::format(_("Warning: MPQ file size of MPQ file \"%1%\" is not equal to its internal header file size.\nFile size: %2%.\nInternal header file size: %3%.")) % this->m_path.string() % this->m_size % header.archiveSize << std::endl;
	
	this->m_sectorSizeShift = header.sectorSizeShift;
	std::cout << "Format version " << header.formatVersion << std::endl;
	std::cout << "Sector size shift " << header.sectorSizeShift << std::endl;
	
	struct ExtendedHeader extendedHeader;
	
	if (this->m_format == Mpq::Mpq2)
	{
		std::cout << "Has extended header!" << std::endl;
		istream.read(reinterpret_cast<char*>(&extendedHeader), sizeof(extendedHeader));
		bytes += istream.gcount();
	}
	
	int64 offset = header.blockTableOffset;
	std::cout << "1. Block table offset: " << header.blockTableOffset << std::endl;
	
	if (this->m_format == Mpq::Mpq2 && extendedHeader.blockTableOffsetHigh > 0)
		offset += int64(extendedHeader.blockTableOffsetHigh) << 32;
	
	std::cout << "2. Block table offset: " << offset << std::endl;
	istream.seekg(startPosition + offset);

	std::size_t encryptedBytesSize = header.blockTableEntries * sizeof(struct BlockTableEntry);
	char *encryptedBytes = new char[encryptedBytesSize];

	std::cout << "Block table entries: " << header.blockTableEntries << "\nSize of single block table entry: " << sizeof(struct BlockTableEntry) << "\nSize of char array: " << encryptedBytesSize << std::endl;
	//return 0;
	
	istream.read(encryptedBytes, encryptedBytesSize);
	//std::cout << "test" << std::endl;
	uint32 hashValue = HashString(Mpq::cryptTable(), "(block table)", FileKey);
	std::cout << "block table hash value " << hashValue << std::endl;
	DecryptData(Mpq::cryptTable(), reinterpret_cast<void*>(encryptedBytes), encryptedBytesSize, hashValue);
	//std::cout << "test 3" << std::endl;
	std::stringstream sstream;
	sstream.write(const_cast<const char*>(encryptedBytes), encryptedBytesSize);
	delete[] encryptedBytes;
	encryptedBytes = 0;
	//std::cout << "test 4" << std::endl;
	
	for (std::size_t i = 0; i < header.blockTableEntries; ++i)
	{
		//std::cout << "test 5" << std::endl;
		class Block *block = new Block(this);
		//std::cout << "test 6" << std::endl;
		bytes += block->read(sstream);
		//std::cout << "test 7" << std::endl;
		//std::cout << "Index " << i << std::endl;
		//std::cout << "Block table entries: " << header.blockTableEntries << "\nSize of single block table entry: " << sizeof(struct BlockTableEntry) << "\nSize of char array: " << encryptedBytesSize << std::endl;
		this->m_blocks.push_back(block);
		this->m_blockMap[this->m_blocks.size() - 1] = block;
	}
	
	std::cout << "Block list size: " << this->m_blocks.size() << std::endl;
		
	std::cout << "FINISH!" << std::endl;
	
	/*
	read extended block table
	As of the Burning Crusade Friends and Family beta, this table is not encrypted.
	*/
	if (this->m_format == Mpq::Mpq2)
	{
		istream.seekg(startPosition + extendedHeader.extendedBlockTableOffset);
		
		BOOST_FOREACH(class Block *block, this->m_blocks)
		{
			struct ExtendedBlockTableEntry extendedBlockTableEntry;
			istream.read(reinterpret_cast<char*>(&extendedBlockTableEntry), sizeof(extendedBlockTableEntry));
			bytes += istream.gcount();
			block->m_extendedBlockOffset = extendedBlockTableEntry.extendedBlockOffset;
		}
	}
	
	std::cout << "FINISH!" << std::endl;
			
	// read encrypted hash table
	offset = header.hashTableOffset;
	std::cout << "1. Hash table offset: " << offset << std::endl;
		 
	if (this->m_format == Mpq::Mpq2 && extendedHeader.hashTableOffsetHigh > 0)
		offset += int64(extendedHeader.hashTableOffsetHigh) << 32;
	
	std::cout << "2. Hash table offset: " << offset << std::endl;
	istream.seekg(offset);
	
	std::cout << "FINISH2!" << std::endl;
	
	encryptedBytesSize = header.hashTableEntries * sizeof(struct HashTableEntry);
	encryptedBytes = new char[encryptedBytesSize];
	
	std::cout << "FINISH2!" << std::endl;

	istream.read(encryptedBytes, encryptedBytesSize);
	std::cout << "FINISH2!" << std::endl;
	hashValue = HashString(Mpq::cryptTable(), "(hash table)", FileKey);
	std::cout << "Hash table with hash value " << hashValue << std::endl;
	DecryptData(Mpq::cryptTable(), reinterpret_cast<void*>(encryptedBytes), encryptedBytesSize, hashValue);
	std::cout << "FINISH2!" << std::endl;
	sstream.flush();
	sstream.write(const_cast<const char*>(encryptedBytes), encryptedBytesSize);
	delete[] encryptedBytes;
	encryptedBytes = 0;
	
	std::cout << "FINISH3!" << std::endl;
	
	for (std::size_t i = 0; i < header.hashTableEntries; ++i)
	{
		class Hash *hash = new Hash(this);
		//std::cout << "Index " << i << std::endl;
		bytes += hash->read(sstream);
		this->m_hashes.push_back(hash);
	}
	
	std::cout << "FINISH4!" << std::endl;	
	std::cout << "FINISH!!!!!" << std::endl;
	
	// file data / add file instances
	BOOST_FOREACH(class Hash *hash, this->m_hashes)
	{
		if (!hash->empty() && !hash->deleted())
		{
			class MpqFile *mpqFile = new MpqFile(this, hash);
			mpqFile->m_size = hash->m_block->m_fileSize; // uncompressed size
			//mpqFile->m_compressedSize = hash->m_block->m_blockSize
			mpqFile->m_locale = MpqFile::intToLocale(hash->m_language);
			mpqFile->m_platform = MpqFile::intToPlatform(hash->m_platform);
			//mpqFile->m_path = // path can only be set if there is a listfile file or if we're able to convert its hash into file path
			
			// seek to file data beginning
			istream.seekg(mpqFile->m_hash->m_block->m_blockOffset);
			/// @todo Decrypt and unimplode data?
			
			/*
			sector offset table
			This table is not present if this information can be calculated.
			*/
			if (!(hash->m_block->m_flags & Block::IsSingleUnit) || (!(hash->m_block->m_flags & Block::IsCompressed) && !(hash->m_block->m_flags & Block::IsImploded)))
			{
				int32 sectors = 0; /// @todo How to get this value?
				
				for (std::size_t i = 0; i < sectors; ++i)
				{
					class Sector *sector = new Sector(mpqFile);
					istream.read(reinterpret_cast<char*>(&sector->m_sectorOffset), sizeof(sector->m_sectorOffset));
					bytes += istream.gcount();
					mpqFile->m_sectors.push_back(sector);
				}
				
				// The last entry contains the file size, making it possible to easily calculate the size of any given sector.
				istream.read(reinterpret_cast<char*>(&mpqFile->m_size), sizeof(mpqFile->m_size));
				bytes += istream.gcount();
				int32 size = mpqFile->m_size;
				
				// calculate sector size, not required but maybe useful at some point
				BOOST_REVERSE_FOREACH(class Sector *sector, mpqFile->m_sectors)
				{
					sector->m_sectorSize = size - sector->m_sectorOffset;
					size = sector->m_sectorOffset;
				}
			}
			// If the file is not compressed/imploded, then the size and offset of all sectors is known, based on the archive's SectorSizeShift. If the file is stored as a single unit compressed/imploded, then the SectorOffsetTable is omitted, as the single file "sector" corresponds to BlockSize and FileSize, as mentioned previously. However, the SectorOffsetTable will be present if the file is compressed/imploded and the file is not stored as a single unit, even if there is only a single sector in the file (the size of the file is less than or equal to the archive's sector size).
			else
			{
				int32 sectors = 0;
				int16 sectorSize = this->sectorSize();
				
				if (hash->m_block->m_flags & Block::IsSingleUnit)
					sectors = 1;
				else
					sectors = hash->m_block->m_blockSize / sectorSize;
				
				int32 newOffset = 0;
				int32 lastSize = hash->m_block->m_blockSize % sectorSize;
				
				for (std::size_t i = 0; i < sectors; ++i)
				{
					class Sector *sector = new Sector(mpqFile);
					sector->m_sectorOffset = newOffset;
					
					if (hash->m_block->m_flags & Block::IsSingleUnit)
						sector->m_sectorSize = hash->m_block->m_blockSize;
					else
						sector->m_sectorSize = sectorSize;
					
					mpqFile->m_sectors.push_back(sector);
					
					newOffset += sectorSize;
				}
				
				// the last sector may contain less than this, depending on the size of the entire file's data.
				if (!(hash->m_block->m_flags & Block::IsSingleUnit) && lastSize > 0)
				{
					class Sector *sector = new Sector(mpqFile);
					sector->m_sectorOffset = newOffset;
					sector->m_sectorSize = lastSize;
					mpqFile->m_sectors.push_back(sector);
				}
				
			}
			
			BOOST_FOREACH(class Sector *sector, mpqFile->m_sectors)
			{
				if (mpqFile->m_hash->m_block->m_flags & Block::IsCompressed)
				{
					byte compression;
					istream.read(reinterpret_cast<char*>(&compression), sizeof(compression));
					bytes += istream.gcount();
					sector->setCompression(compression);
					istream.seekg(sector->m_sectorSize - 1, std::ios_base::cur); // skip sector data
				}
				else
				{
					sector->setCompression(0);
					istream.seekg(sector->m_sectorSize, std::ios_base::cur); // skip sector data
				}
			}
			
			this->m_files.push_back(mpqFile);
		}
	}
	
	class MpqFile *listfileFile = const_cast<class MpqFile*>(this->listfileFile());
	
	if (listfileIstream == 0)
	{
		if (listfileFile != 0)
		{
			std::cout << "CONTAINS LISTFILE FILE!" << std::endl;
			// read listfile file and create path entries
			std::stringstream sstream;
			listfileFile->write(sstream);
			std::cout << "Adding " << this->readListfilePathEntries(sstream) << " path entries." << std::endl;
		}
	}
	else
	{
		// read listfile file and create path entries
		this->readListfilePathEntries(*listfileIstream);
		// append list file file
		const_cast<class MpqFile*>(this->createListfileFile())->append(*listfileIstream);
	}
	
	/// @todo Single "(attributes)" file?
	/*
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
	*/
	
	/// @todo Read "(signature)" file.
	//The strong digital signature is stored immediately after the archive, in the containing file
	/// @todo Read strong digital signature.
	if (Mpq::hasStrongDigitalSignature(istream))
	{
		this->m_strongDigitalSignature = new char[256];
		bytes += strongDigitalSignature(istream, this->m_strongDigitalSignature);
	}
	else
		this->m_strongDigitalSignature = 0;
	
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

const class MpqFile* Mpq::findFile(const std::string &name) const
{
	BOOST_FOREACH(const class MpqFile *mpqFile, this->m_files)
	{
		if (mpqFile->path().filename() == name)
			return mpqFile;
	}
	
	return 0;
}

const class MpqFile* Mpq::findFile(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform) const
{
	class Hash *hash = const_cast<class Mpq*>(this)->findHash(path, locale, platform);
	
	std::cout << "Hash is " << hash << " when searching for file " << path.string() << std::endl;
	
	if (hash == 0)
		return 0;
				
	if (hash->m_mpqFile->m_path != path) // path has not been set yet
		hash->m_mpqFile->m_path = path;
				
	return const_cast<const class MpqFile*>(hash->m_mpqFile);
}

class MpqFile* Mpq::addFile(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform, const std::istream *istream, bool overwriteExisting, std::size_t reservedSpace) throw (class Exception)
{
	class MpqFile *mpqFile = const_cast<class MpqFile*>(this->findFile(path, locale, platform));
	
	if (mpqFile != 0)
	{
		if (!overwriteExisting)
			return 0;
		
		// overwrite existing
		mpqFile->remove();
	}
	
	class Block *block = 0;
	
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
		block = new Block(this);
		this->nextBlockOffsets(block->m_blockOffset, block->m_extendedBlockOffset);
		
		if (this->m_format != Mpq::Mpq2 && block->m_extendedBlockOffset > 0)
			throw Exception(_("Extended block offset can not be used in MPQ format 1."));
		
		block->m_blockSize = reservedSpace;
		block->m_fileSize = reservedSpace;
		block->m_flags = Block::IsFile; /// @todo Set right flags (user-defined).
		this->m_blocks.push_back(block);
		this->m_blockMap[this->m_blocks.size() - 1] = block;
		
	}
	
	class Hash *hash = this->firstDeletedHash();
		
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
	hash->m_filePathHashA = HashString(Mpq::cryptTable(), path.string().c_str(), NameA);
	hash->m_filePathHashB = HashString(Mpq::cryptTable(), path.string().c_str(), NameB);
	hash->m_language = MpqFile::localeToInt(locale);
	hash->m_platform = MpqFile::platformToInt(platform);
	hash->m_mpqFile = new MpqFile(this, hash);
	this->m_files.push_back(hash->m_mpqFile);
	
	/// @todo Write block table entry into file. This can take a lot of time since the whole MPQ data after the entry has to be written newly.
	if (newBlock)
		;
	
	/// @todo Write file data/free reserved space in MPQ.
	if (istream != 0)
		;
	
	/// @todo Write meta data/extended attributes
	if (this->extendedAttributes() & Mpq::FileCrc32s)
		//block->m_crc32;
		;
		
	if (this->extendedAttributes() & Mpq::FileTimeStamps)
		block->setFileTime(time(0));
		
	if (this->extendedAttributes() & Mpq::FileMd5s)
		//block->m_md5 = ;
		;
	
	// delete old (attributes) file/refresh
	if (this->containsAttributesFile())
		const_cast<class MpqFile*>(this->attributesFile())->remove();
	
	this->createAttributesFile();
		
	return hash->m_mpqFile;
}

bool Mpq::removeFile(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform)
{
	class MpqFile *mpqFile = const_cast<class MpqFile*>(this->findFile(path, locale, platform));
	
	if (mpqFile == 0)
		return false;
	
	mpqFile->remove();
	
	return true;
}

bool Mpq::removeFile(const boost::filesystem::path &path)
{
	class MpqFile *mpqFile = const_cast<class MpqFile*>(this->findFile(path));
	
	if (mpqFile == 0)
		return false;
	
	mpqFile->remove();
	
	return true;
}

bool Mpq::removeFile(const std::string &name)
{
	class MpqFile *mpqFile = const_cast<class MpqFile*>(this->findFile(name));
	
	if (mpqFile == 0)
		return false;
	
	mpqFile->remove();
	
	return true;
}

class Mpq& Mpq::operator<<(const class MpqFile &mpqFile) throw (class Exception)
{
	/// @todo Copy all file data, existing files won't be overwritten.
	
	return *this;
}

class Mpq& Mpq::operator<<(const class Mpq &mpq) throw (class Exception)
{
	/// @todo Copy all file data of the other MPQ, existing files won't be overwritten.
	
	return *this;
}

class Mpq& Mpq::operator>>(class Mpq &mpq) throw (class Exception)
{
	/// @todo Copy all file data into the other MPQ, existing files won't be overwritten.

	return *this;
}

class Hash* Mpq::findHash(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform)
{
	if (this->m_hashes.empty() || this->m_hashes.front()->empty())
	{
		std::cout << "Hashes are empty." << std::endl;
		std::cout << "Front hash is empty." << std::endl;
		
		return 0;
	}
		
	// Compute the hashes to compare the hash table entry against
	uint32 nameHashA = HashString(Mpq::cryptTable(), path.string().c_str(), NameA);
	uint32 nameHashB = HashString(Mpq::cryptTable(), path.string().c_str(), NameB);	
	int16 language = MpqFile::localeToInt(locale);
	int16 realPlatform = MpqFile::platformToInt(platform);
	std::cout << "Searching for hash " << path << " with value a " << nameHashA << " and value b " << nameHashB << " and language " << language << " and platform " << realPlatform << std::endl;
	
	// Check each entry in the hash table till a termination point is reached
	std::list<class Hash*>::iterator iterator = this->m_hashes.begin();
	
	do
	{
		if (!(*iterator)->deleted())
		{
			if ((*iterator)->m_filePathHashA == nameHashA && (*iterator)->m_filePathHashB == nameHashB && (*iterator)->m_language == language && (*iterator)->m_platform == realPlatform)	
				return *iterator;
		}
			
		++iterator;
	}
	while (iterator != this->m_hashes.end() && !(*iterator)->empty());
	
	return 0;
}

std::size_t Mpq::readListfilePathEntries(std::istream &istream)
{
	// read list file file and create path entries
	std::size_t count = 0;
	std::string line;
	
	while (std::getline(istream, line))
	{
		++count;
		this->findFile(line, MpqFile::Neutral, MpqFile::Default);
	}
	
	return count;
}

}

}
