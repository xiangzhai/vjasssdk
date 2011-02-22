/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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
#include <boost/filesystem/fstream.hpp>

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

Mpq::Mpq() : m_size(0), m_path(), m_format(Mpq::Mpq1), m_extendedAttributes(Mpq::None), m_sectorSize(0), m_strongDigitalSignature(0), m_isOpen(false)
{
}

Mpq::~Mpq()
{
	this->close();
}

std::streamsize Mpq::create(const boost::filesystem::path &path, bool overwriteExisting, std::streampos startPosition, enum Format format, enum ExtendedAttributes extendedAttributes, int32 sectorSize, bool hasStrongDigitalSignature, bool containsListfileFile, bool containsSignatureFile) throw (class Exception)
{
	this->close();

	if (boost::filesystem::exists(path) && !overwriteExisting)
		throw Exception(boost::str(boost::format(_("Unable to create file \"%1%\". File does already exist.")) % path.string()));

	/// @todo Lock file.
	try
	{
		boost::filesystem::ofstream ofstream(path, std::ios_base::binary);

		if (!ofstream)
			throw Exception(boost::str(boost::format(_("Unable to create file \"%1%\".")) % path.string()));

		ofstream.seekp(startPosition);

		if (ofstream.tellp() != startPosition)
			throw Exception(boost::str(boost::format(_("Unable to start in file \"%1%\" at position %2%.")) % path.string() % startPosition));

		this->m_size = boost::filesystem::file_size(path);
		this->m_path = path;
		this->m_format = format;
		this->m_extendedAttributes = extendedAttributes;
		this->m_sectorSize = sectorSize;
		std::streamsize streamSize = 0;

		try
		{
			streamSize = this->writeMpq(ofstream);

			/// @todo FIXME, set stream size!!!
			if (hasStrongDigitalSignature)
				;
				//this->createStrongDigitalSignature();

			if (containsListfileFile)
				this->createListfileFile();

			if (containsSignatureFile)
				;
				//this->createSignatureFile();
		}
		catch (class Exception &exception)
		{
			this->clear();

			throw exception;
		}

		this->m_isOpen = true;

		return streamSize;
	}
	catch (...)
	{
		/// @todo Unlock file.

		throw;
	}
}

std::streamsize Mpq::open(const boost::filesystem::path &path, istream *listfileIstream) throw (class Exception)
{
	this->close();

	/// @todo Lock file.

	try
	{
		boost::filesystem::ifstream ifstream(path, std::ios_base::binary);

		if (!ifstream)
			throw Exception(boost::str(boost::format(_("Unable to open file \"%1%\".")) % path.string()));

		this->m_size = boost::filesystem::file_size(path);
		this->m_path = path;
		std::streamsize streamSize = 0;

		try
		{
			streamSize = this->readMpq(ifstream, listfileIstream);
		}
		catch (class Exception &exception)
		{
			this->clear();

			throw exception;
		}

		this->m_isOpen = true;

		return streamSize;
	}
	catch (...)
	{
		/// @todo Unlock file.

		throw;
	}
}

void Mpq::close()
{
	if (!this->m_isOpen)
		return;

	std::cout << "Closing archive." << std::endl;
	/// @todo Unlock file.
	this->clear();
}

std::streamsize Mpq::readMpq(istream &istream, istream *listfileIstream) throw (class Exception)
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

	istream.seekg(-4, std::ios::cur);
	this->m_startPosition = istream.tellg();
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

	this->m_sectorSize = pow(2, header.sectorSizeShift) * 512;
	struct ExtendedHeader extendedHeader;

	if (this->m_format == Mpq::Mpq2)
	{
		istream.read(reinterpret_cast<char*>(&extendedHeader), sizeof(extendedHeader));
		bytes += istream.gcount();
	}

	int64 offset = header.blockTableOffset;

	if (this->m_format == Mpq::Mpq2 && extendedHeader.blockTableOffsetHigh > 0)
		offset += int64(extendedHeader.blockTableOffsetHigh) << 32;

	istream.seekg(this->m_startPosition + offset);
	std::size_t encryptedBytesSize = header.blockTableEntries * sizeof(struct BlockTableEntry);
	char *encryptedBytes = new char[encryptedBytesSize];
	istream.read(encryptedBytes, encryptedBytesSize);
	uint32 hashValue = HashString(Mpq::cryptTable(), "(block table)", FileKey);
	DecryptData(Mpq::cryptTable(), reinterpret_cast<void*>(encryptedBytes), encryptedBytesSize, hashValue);
	std::stringstream sstream;
	sstream.write(const_cast<const char*>(encryptedBytes), encryptedBytesSize);
	delete[] encryptedBytes;
	encryptedBytes = 0;

	for (std::size_t i = 0; i < header.blockTableEntries; ++i)
	{
		class Block *block = new Block(this);
		bytes += block->read(sstream);
		this->m_blocks.push_back(block);
		this->m_blockMap[this->m_blocks.size() - 1] = block;
	}

	/*
	read extended block table
	As of the Burning Crusade Friends and Family beta, this table is not encrypted.
	*/
	if (this->m_format == Mpq::Mpq2)
	{
		istream.seekg(this->m_startPosition + extendedHeader.extendedBlockTableOffset);

		BOOST_FOREACH(class Block *block, this->m_blocks)
		{
			struct ExtendedBlockTableEntry extendedBlockTableEntry;
			istream.read(reinterpret_cast<char*>(&extendedBlockTableEntry), sizeof(extendedBlockTableEntry));
			bytes += istream.gcount();
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
	istream.read(encryptedBytes, encryptedBytesSize);
	hashValue = HashString(Mpq::cryptTable(), "(hash table)", FileKey);
	DecryptData(Mpq::cryptTable(), reinterpret_cast<void*>(encryptedBytes), encryptedBytesSize, hashValue);
	sstream.flush();
	sstream.write(const_cast<const char*>(encryptedBytes), encryptedBytesSize);
	delete[] encryptedBytes;
	encryptedBytes = 0;

	for (std::size_t i = 0; i < header.hashTableEntries; ++i)
	{
		class Hash *hash = new Hash(this);
		bytes += hash->read(sstream);
		this->m_hashes.push_back(hash);
	}

	// file data / add file instances
	BOOST_FOREACH(class Hash *hash, this->m_hashes)
	{
		if (!hash->empty() && !hash->deleted())
		{
			class MpqFile *mpqFile = new MpqFile(this, hash);
			//mpqFile->m_path = // path can only be set if there is a listfile file or if we're able to convert its hash into file path
			this->m_files.push_back(mpqFile);

			// seek to file data beginning
			istream.seekg(int32(this->m_startPosition) + mpqFile->m_hash->m_block->m_blockOffset);
			/// @todo Decrypt and unimplode data?

			bytes += mpqFile->read(istream);
		}
	}

	class MpqFile *listfileFile = const_cast<class MpqFile*>(this->listfileFile());

	if (listfileIstream == 0)
	{
		if (listfileFile != 0)
		{
			std::cout << "CONTAINS LISTFILE FILE! (Block flags " << listfileFile->m_hash->m_block->flags() << ")" << std::endl;
			// read listfile file and create path entries
			std::stringstream sstream;
			listfileFile->writeData(sstream);
			std::cout << "Adding " << this->readListfilePathEntries(sstream) << " path entries" << std::endl; // (sstream " << sstream.str() << ")." << std::endl;
		}
	}
	else
	{
		// read listfile file and create path entries
		this->readListfilePathEntries(*listfileIstream);
		// append list file file
		const_cast<class MpqFile*>(this->createListfileFile())->appendData(*listfileIstream);
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

std::streamsize Mpq::openTar(const boost::filesystem::path &path, istream *listfileIstream) throw (class Exception)
{
	return 0;
}

std::streamsize Mpq::readTar(istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Mpq::writeTar(ostream &ostream) const throw (class Exception)
{
	return 0;
}

const class MpqFile* Mpq::findFile(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform) const
{
	class Hash *hash = const_cast<class Mpq*>(this)->findHash(path, locale, platform);

	if (hash == 0 || hash->deleted() || hash->empty())
		return 0;

	if (hash->m_mpqFile->m_path != path) // path has not been set yet
		hash->m_mpqFile->m_path = path;

	return const_cast<const class MpqFile*>(hash->m_mpqFile);
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

const class MpqFile* Mpq::findFile(const class MpqFile &mpqFile) const
{
	return this->findFile(mpqFile.path(), mpqFile.locale(), mpqFile.platform());
}

class MpqFile* Mpq::addFile(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform, const istream *istream, bool overwriteExisting, int32 reservedSpace) throw (class Exception)
{
	class MpqFile *mpqFile = const_cast<class MpqFile*>(this->findFile(path, locale, platform));

	// In the event of a collision (the home entry is occupied by another file), progressive overflow is used, and the file is placed in the next available hash table entry
	if (mpqFile != 0)
	{
		if (overwriteExisting)
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
	hash->m_locale = MpqFile::localeToInt(locale);
	hash->m_platform = MpqFile::platformToInt(platform);
	hash->m_mpqFile = new MpqFile(this, hash);
	this->m_files.push_back(hash->m_mpqFile);

	/// @todo Write block table entry into file. This can take a lot of time since the whole MPQ data after the entry has to be written newly.
	if (newBlock)
		;

	/// @todo Write file data/free reserved space in MPQ.
	if (istream != 0)
		;

	// Add "(listfile)" file entry.
	if (this->containsListfileFile())
	{
		std::stringstream sstream(path.string());
		this->listfileFile()->appendData(sstream);
	}

	// Add "(attributes)" file entries.
	if (this->extendedAttributes() != Mpq::None && this->containsAttributesFile())
	{
		/// @todo Write meta data/extended attributes
		if (this->extendedAttributes() & Mpq::FileCrc32s)
			//block->m_crc32;
			;

		if (this->extendedAttributes() & Mpq::FileTimeStamps)
			block->setFileTime(time(0));

		if (this->extendedAttributes() & Mpq::FileMd5s)
			//block->m_md5 = ;
			;

		this->refreshAttributesFile();
	}

	// Refresh "(signature)" file.
	if (this->containsSignatureFile())
		this->refreshSignatureFile();

	return hash->m_mpqFile;
}

class MpqFile* Mpq::addFile(const MpqFile &mpqFile, bool addData, bool overwriteExisting) throw (class Exception)
{
	std::stringstream sstream;

	if (addData)
		mpqFile.writeData(sstream);

	return this->addFile(mpqFile.path(), mpqFile.locale(), mpqFile.platform(), &sstream, overwriteExisting, mpqFile.size());
}

bool Mpq::removeFile(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform)
{
	class MpqFile *mpqFile = this->findFile(path, locale, platform);

	if (mpqFile == 0)
		return false;

	mpqFile->remove();

	return true;
}

bool Mpq::removeFile(const boost::filesystem::path &path)
{
	class MpqFile *mpqFile = this->findFile(path);

	if (mpqFile == 0)
		return false;

	mpqFile->remove();

	return true;
}

bool Mpq::removeFile(const std::string &name)
{
	class MpqFile *mpqFile = this->findFile(name);

	if (mpqFile == 0)
		return false;

	mpqFile->remove();

	return true;
}

bool Mpq::removeFile(const MpqFile &mpqFile)
{
	class MpqFile *searchedMpqFile = this->findFile(mpqFile);

	if (searchedMpqFile == 0)
		return false;

	searchedMpqFile->remove();

	return true;
}

bool Mpq::operator!() const
{
	return !this->isOpen();
}

class Mpq& Mpq::operator<<(const class MpqFile &mpqFile) throw (class Exception)
{
	// Copy all file data, existing files won't be overwritten.
	this->addFile(mpqFile, false);

	return *this;
}

class Mpq& Mpq::operator<<(const class Mpq &mpq) throw (class Exception)
{
	// Copy all file data of the other MPQ, existing files won't be overwritten.
	BOOST_FOREACH(const class MpqFile *mpqFile, mpq.files())
		this->addFile(*mpqFile, false);

	return *this;
}

class Mpq& Mpq::operator>>(class Mpq &mpq) throw (class Exception)
{
	// Copy all file data into the other MPQ, existing files won't be overwritten.
	BOOST_FOREACH(const class MpqFile *mpqFile, this->files())
		mpq.addFile(*mpqFile, false);

	return *this;
}

void Mpq::clear()
{
	if (this->m_strongDigitalSignature != 0)
		delete[] this->m_strongDigitalSignature;

	//BOOST_FOREACH(class Block *block, this->m_blocks)
	//	delete block;

	//BOOST_FOREACH(class Hash *hash, this->m_hashes)
	//	delete hash;

	//BOOST_FOREACH(class MpqFile *file, this->m_files)
	//	delete file;

	this->m_size = 0;
	this->m_path.clear();
	this->m_format = Mpq::Mpq1;
	this->m_extendedAttributes = Mpq::None;
	this->m_sectorSize = 0;
	this->m_strongDigitalSignature = 0;
	this->m_isOpen = false;
}

/// @todo There seems to be some MPQ archives which do contain hash tables which do start with an empty entry. Therefore I commented checks for such tables.
class Hash* Mpq::findHash(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform)
{
	if (this->m_hashes.empty()) //|| this->m_hashes.front()->empty())
	{
		if (this->m_hashes.empty())
			std::cout << "Hashes are empty." << std::endl;

		if (this->m_hashes.front()->empty())
			std::cout << "Front hash is empty (Block address " << this->m_hashes.front()->m_block << ")." << std::endl;

		return 0;
	}

	// Compute the hashes to compare the hash table entry against
	uint32 nameHashA = HashString(Mpq::cryptTable(), path.string().c_str(), NameA);
	uint32 nameHashB = HashString(Mpq::cryptTable(), path.string().c_str(), NameB);
	int16 realLocale = MpqFile::localeToInt(locale);
	int16 realPlatform = MpqFile::platformToInt(platform);
	std::cout << "Searching for hash " << path << " with value a " << nameHashA << " and value b " << nameHashB << " and locale " << locale << " and platform " << realPlatform << std::endl;

	// Check each entry in the hash table till a termination point is reached
	std::list<class Hash*>::iterator iterator = this->m_hashes.begin();

	do
	{
		if (!(*iterator)->deleted())
		{
			if ((*iterator)->m_filePathHashA == nameHashA && (*iterator)->m_filePathHashB == nameHashB && (*iterator)->m_locale == realLocale && (*iterator)->m_platform == realPlatform)
				return *iterator;
		}

		++iterator;
	}
	while (iterator != this->m_hashes.end()); // && !(*iterator)->empty()

	return 0;
}

class Hash* Mpq::findHash(const Hash &hash)
{
	return this->findHash(hash.mpqFile()->path(), hash.mpqFile()->locale(), hash.mpqFile()->platform());
}

class MpqFile* Mpq::findFile(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform)
{
	return const_cast<class MpqFile*>(const_cast<const Mpq*>(this)->findFile(path, locale, platform));
}

class MpqFile* Mpq::findFile(const boost::filesystem::path &path)
{
	return const_cast<class MpqFile*>(const_cast<const Mpq*>(this)->findFile(path));
}

class MpqFile* Mpq::findFile(const std::string &name)
{
	return const_cast<class MpqFile*>(const_cast<const Mpq*>(this)->findFile(name));
}

class MpqFile* Mpq::findFile(const class MpqFile &mpqFile)
{
	return const_cast<class MpqFile*>(const_cast<const Mpq*>(this)->findFile(mpqFile));
}

bool Mpq::removeFile(class MpqFile *&mpqFile)
{
	if (mpqFile->mpq() != this)
		return false;

	mpqFile->remove();
	delete mpqFile;
	mpqFile = 0;

	return true;
}

class MpqFile* Mpq::refreshAttributesFile()
{
	throw Exception(_("Mpq: refreshAttributesFile is not implemented yet!"));

	return 0;
}

class MpqFile* Mpq::refreshSignatureFile()
{
	throw Exception(_("Mpq: refreshSignatureFile is not implemented yet!"));

	return 0;
}

std::size_t Mpq::readListfilePathEntries(istream &istream)
{
	// read list file file and create path entries
	std::size_t count = 0;
	std::string line;

	// The listfile is contained in the file "(listfile)" (default language and platform), and is simply a text file with file paths separated by ';', 0Dh, 0Ah, or some combination of these.
	char character;
	bool newEntry = false;

	while (istream.get(character))
	{
		if (character == ';' || character == '\r' || character == '\n')
		{
			if (!newEntry)
			{
				//std::cout << "(listfile) line: " << line << std::endl;
				//this->findFile(line, MpqFile::Neutral, MpqFile::Default);
				newEntry = true;
				++count;
				line.clear();
			}
		}
		else
		{
			line.append(1, character);

			if (newEntry)
				newEntry = false;

			if (count == 0)
				count = 1;
		}
	}

	// last entry
	if (!newEntry)
	{
		//std::cout << "(listfile) line: " << line << std::endl;
		//this->findFile(line, MpqFile::Neutral, MpqFile::Default);
		++count;
	}

	return count;
}

}

}
