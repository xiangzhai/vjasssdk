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

#ifndef WC3LIB_MPQ_MPQ_HPP
#define WC3LIB_MPQ_MPQ_HPP

#include <iostream>
#include <list>
#include <string>
#include <sstream>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include "platform.hpp"
#include "hash.hpp"
#include "block.hpp"
#include "mpqfile.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mpq
{

class Mpq
{
	public:
		enum Format
		{
			Mpq1, // original format
			Mpq2 // Burning Crusade, large files
		};
		
		enum ExtendedAttributes
		{
			None = 0x0,
			FileCrc32s = 0x00000001,
			FileTimeStamps = 0x00000002,
			FileMd5s = 0x00000004
		};
		
		static bool hasStrongDigitalSignature(std::istream &istream);
		static std::streamsize strongDigitalSignature(std::istream &istream, char signature[256]) throw (class Exception);

		/**
		* Does not read the MPQ archive data!
		* Use @fn Mpq.readMpq to do this.
		*/
		Mpq(const boost::filesystem::path &path);
		~Mpq();
		
		/**
		* @return Returns MPQ's size in bytes.
		*/
		std::streamsize readMpq(std::istream &istream, const std::istream *listfileIstream = 0) throw (class Exception);
		/**
		* Writes the whole MPQ archive into output stream @param ostream. Note that you don't have to call this function each time you want to save your changed data of the opened MPQ archive.
		* If you change some data of the opened MPQ archive it's written directly into the corresponding file (the whole archive is not loaded into memory!).
		* @return Returns MPQ's size in bytes.
		*/
		std::streamsize writeMpq(std::ostream &ostream) const throw (class Exception);
		
#ifdef TAR
		std::streamsize readTar(std::istream &istream, enum Mode mode) throw (class Exception);
		std::streamsize writeTar(std::istream &istream, enum Mode mode) throw (class Exception);		
#endif
		
		bool check() const;
		bool fix() const;
		/**
		* @return Returns true if the archive contains a "(listfile)" file.
		*/
		bool containsListfileFile() const;
		/**
		* Creates a "(listfile)" if there isn't already one. All file entries which do have corresponding file paths will be added to this file.
		* @return If the file does already exist it returns 0.
		*/
		const class MpqFile* createListfileFile() throw (class Exception);
		const class MpqFile* listfileFile() const;
		/**
		* @return Returns true if the archive contains a "(attributes)" file. This can also be checked by checking if @fn Mpq.extendedAttributes() does not return @enum Mpq.None.
		*/
		bool containsAttributesFile() const;
		const class MpqFile* attributesFile() const;
		
		bool containsSignatureFile() const;
		const class MpqFile* signatureFile() const;

		std::size_t unusedSpace() const;
		
		const class MpqFile* findFile(const boost::filesystem::path &path) const;
		const class MpqFile* findFileByName(const std::string &name) const;
		/**
		* Note that the MPQ archive has no information about file paths if there is no "(listfile)" file. This function is the best way to get your required file.
		* @return Returns the corresponding @class MpqFile instance of the searched file. If no file was found it returns 0.		
		*/
		const class MpqFile* findFileByHash(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform) const;
		/**
		* Addes a new file to the MPQ archive with path @param path, locale @param locale and platform @param platform.
		* @param istream This input stream is used for reading the initial file data.
		* @todo Replace reservedSpace by size of istream?
		*/
		class MpqFile* addFile(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform, const std::istream *istream = 0, bool overwriteExisting = false, std::size_t reservedSpace = 0) throw (class Exception);
		
		/**
		* @return Returns the size of the whole MPQ archive file.
		*/
		std::size_t size() const;
		const boost::filesystem::path& path() const;
		enum Format format() const;
		enum ExtendedAttributes extendedAttributes() const;
		bool hasStrongDigitalSignature() const;
		const char* strongDigitalSignature() const;
		const std::list<class Block*>& blocks() const;
		const std::list<class Hash*>& hashes() const;
		const std::list<class MpqFile*>& files() const;
		
		
		/**
		* @todo Use these two operators for writing/reading files into/from streams.
		* operator<< // global operator for std::istream
		* operator<< // operator for one file, existing files won't be overwritten.
		* operator<< // operator for one archive, existing files won't be overwritten.
		* operator>> // global operator for std::ostream
		*/
		class Mpq& operator<<(const class MpqFile &mpqFile) throw (class Exception);
		class Mpq& operator<<(const class Mpq &mpq) throw (class Exception);
		class Mpq& operator>>(class Mpq &mpq) throw (class Exception);

		static const byte identifier[4];
		static const int16 formatVersion1Identifier;
		static const int16 formatVersion2Identifier;
		static const int32 extendedAttributesVersion;
	protected:
		friend class MpqFile;
		friend class Hash;
		friend class Block;
		
		static const uint32* cryptTable();

		Mpq(const Mpq &mpq);
		bool checkBlocks() const;
		bool checkHashes() const;
		/**
		* Empty space entries should have BlockOffset and BlockSize nonzero, and FileSize and Flags zero.
		*/
		class Block* firstEmptyBlock() const;
		/**
		* Unused block table entries should have BlockSize, FileSize, and Flags zero.
		*/
		class Block* firstUnusedBlock() const;
		/**
		* @return Returns the block with the biggest offset.
		*/
		class Block* lastOffsetBlock() const;
		class Hash* firstEmptyHash() const;
		class Hash* firstDeletedHash() const;
		/**
		* @return Returns the next block offset as large unsigned integer. Next block offset means the offset of the next added block (last block offset + last block size / header size).
		*/
		uint64 nextBlockOffset() const;
		/**
		* Same as function @fn Mpq.nextBlockOffset but divides large offset value into @param blockOffset and @param extendedBlockOffset.
		*/
		void nextBlockOffsets(int32 &blockOffset, int16 &extendedBlockOffset) const;
		
		class Mpq& operator=(const class Mpq &mpq);
		
		/// This value is required for getting the first block offset (relative to the beginning of archive).
		static const std::size_t headerSize;
		
		std::size_t m_size;
		boost::filesystem::path m_path;
		enum Format m_format;
		enum ExtendedAttributes m_extendedAttributes;
		char *m_strongDigitalSignature;
		std::list<class Block*> m_blocks;
		std::list<class Hash*> m_hashes;
		std::list<class MpqFile*> m_files;
};

inline std::ostream& operator<<(std::ostream &ostream, const class Mpq &mpq) throw (class Exception)
{
	mpq.writeMpq(ostream);
	
	return ostream;
}

inline std::istream& operator>>(std::istream &istream, class Mpq &mpq) throw (class Exception)
{
	mpq.readMpq(istream);
	
	return istream;
}

inline bool operator>(const class Mpq &mpq1, const class Mpq &mpq2)
{
	return mpq1.size() > mpq2.size();
}

inline bool operator<(const class Mpq &mpq1, const class Mpq &mpq2)
{
	return mpq1.size() < mpq2.size();
}

inline bool operator>=(const class Mpq &mpq1, const class Mpq &mpq2)
{
	return mpq1.size() >= mpq2.size();
}

inline bool operator<=(const class Mpq &mpq1, const class Mpq &mpq2)
{
	return mpq1.size() <= mpq2.size();
}

inline bool operator==(const class Mpq &mpq1, const class Mpq &mpq2)
{
	return mpq1.size() == mpq2.size();
}

inline bool operator!=(const class Mpq &mpq1, const class Mpq &mpq2)
{
	return mpq1.size() != mpq2.size();
}

inline bool Mpq::check() const
{
	/// @todo Check format, size, extended attributes and signature.
	
	if (!this->checkBlocks())
		return false;
	
	if (!this->checkHashes())
		return false;
	
	return true;
}

/// @todo Fix corrupted entries if possible.
inline bool Mpq::fix() const
{
	return false;
}

inline bool Mpq::containsListfileFile() const
{
	return this->listfileFile() != 0;
}

inline const class MpqFile* Mpq::createListfileFile() throw (class Exception)
{
	if (this->containsListfileFile())
		return 0;
	
	std::list<std::string> entries;
	
	BOOST_FOREACH(const class MpqFile *mpqFile, this->m_files)
	{
		if (!mpqFile->m_path.empty() && mpqFile->m_path.string() != "(attributes)") /// @todo Exclude directories?
			entries.push_back(mpqFile->m_path.string());
	}
	
	entries.sort(); /// @todo Sort alphabetically?
	std::stringstream sstream;
	
	BOOST_FOREACH(std::string entry, entries)
		sstream << entry << std::endl;	
			
	return this->addFile("(listfile)", MpqFile::Neutral, MpqFile::Default, &sstream);
}

inline const class MpqFile* Mpq::listfileFile() const
{
	return this->findFileByHash("(listfile)", MpqFile::Neutral, MpqFile::Default);
}


inline bool Mpq::containsAttributesFile() const
{
	return this->attributesFile() != 0;
}

inline const class MpqFile* Mpq::attributesFile() const
{
	return this->findFileByHash("(attributes)", MpqFile::Neutral, MpqFile::Default);
}

inline bool Mpq::containsSignatureFile() const
{
	return this->signatureFile() != 0;
}

inline const class MpqFile* Mpq::signatureFile() const
{
	return this->findFileByHash("(signature)", MpqFile::Neutral, MpqFile::Default);
}

inline std::size_t Mpq::size() const
{
	return this->m_size;
}

inline const boost::filesystem::path& Mpq::path() const
{
	return this->m_path;
}

inline enum Mpq::Format Mpq::format() const
{
	return this->m_format;
}

inline enum Mpq::ExtendedAttributes Mpq::extendedAttributes() const
{
	return this->m_extendedAttributes;
}

inline bool Mpq::hasStrongDigitalSignature() const
{
	return this->m_strongDigitalSignature != 0;
}

inline const char* Mpq::strongDigitalSignature() const
{
	return this->m_strongDigitalSignature;
}

inline const std::list<class Block*>& Mpq::blocks() const
{
	return this->m_blocks;
}

inline const std::list<class Hash*>& Mpq::hashes() const
{
	return this->m_hashes;
}

inline const std::list<class MpqFile*>& Mpq::files() const
{
	return this->m_files;
}

inline bool Mpq::checkBlocks() const
{
	BOOST_FOREACH(class Block *block, this->m_blocks)
	{
		if (!block->check())
			return false;
	}
	
	return true;
}

inline bool Mpq::checkHashes() const
{
	BOOST_FOREACH(class Hash *hash, this->m_hashes)
	{
		if (!hash->check())
			return false;
	}
	
	return true;
}

inline class Block* Mpq::firstEmptyBlock() const
{
	BOOST_FOREACH(class Block *block, this->m_blocks)
	{
		if (block->empty())
			return block;
	}
	
	return 0;
}

inline class Block* Mpq::firstUnusedBlock() const
{
	BOOST_FOREACH(class Block *block, this->m_blocks)
	{
		if (block->unused())
			return block;
	}
	
	return 0;
}

inline class Block* Mpq::lastOffsetBlock() const
{
	uint64 offset = 0;
	class Block *result = 0;
	
	BOOST_FOREACH(class Block *block, this->m_blocks)
	{
		uint64 newOffset = block->largeOffset();
		
		if (newOffset > offset)
			result = block;
	}
	
	return result;
}
	

inline class Hash* Mpq::firstEmptyHash() const
{
	BOOST_FOREACH(class Hash *hash, this->m_hashes)
	{
		if (hash->empty())
			return hash;
	}
	
	return 0;
}

inline class Hash* Mpq::firstDeletedHash() const
{
	BOOST_FOREACH(class Hash *hash, this->m_hashes)
	{
		if (hash->deleted())
			return hash;
	}
	
	return 0;
}

inline uint64 Mpq::nextBlockOffset() const
{
	class Block *block = this->lastOffsetBlock();
	
	if (block == 0)
		return Mpq::headerSize;
	
	return block->largeOffset() + block->m_blockSize;
}

inline void Mpq::nextBlockOffsets(int32 &blockOffset, int16 &extendedBlockOffset) const
{
	uint64 offset = this->nextBlockOffset();
	blockOffset = int32(offset << 16); // die hinteren Bits werden abgeschnitten???
	extendedBlockOffset = int16(offset >> 32);
}

}

}

#endif
