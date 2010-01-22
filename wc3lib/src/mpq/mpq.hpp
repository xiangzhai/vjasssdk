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
#include <ctime>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include "platform.hpp"
#include "mpqfile.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mpq
{

class MpqFile;

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
			FileCrc32s = 0x00000001,
			FileTimeStamps = 0x00000002,
			FileMd5s = 0x00000004
		};

		static const char identifier[4];
		static const int16 formatVersion1Identifier;
		static const int16 formatVersion2Identifier;
		static const int32 extendedAttributesVersion;

		/**
		* Does not read the MPQ archive data!
		* Use @fn Mpq.readMpq to do this.
		*/
		Mpq(const boost::filesystem::path &path);
		~Mpq();
		
		/**
		* @return Returns MPQ's size in bytes.
		*/
		std::streamsize readMpq(std::istream &istream) throw (class Exception);
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
		std::size_t unusedSpace() const;
		std::size_t blockCount() const;
		std::size_t hashCount() const;
		
		const class MpqFile* findFile(const boost::filesystem::path &path) const;
		const class MpqFile* findFileByName(const std::string &name) const;
		/**
		* Note that the MPQ archive has no information about file paths if there is no "(listfile)" file. This function is the best way to get your required file.
		* @return Returns the corresponding @class MpqFile instance of the searched file. If no file was found it returns 0.		
		*/
		const class MpqFile* findFileByHash(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform) const;
		class MpqFile* addFile(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform, bool overwriteExisting = false, std::size_t reservedSpace = 0) throw (class Exception);
		
		/**
		* @return Returns the size of the whole MPQ archive file.
		*/
		std::size_t size() const;
		const boost::filesystem::path& path() const;
		enum Format format() const;
		enum ExtendedAttributes extendedAttributes() const;
		const std::list<class MpqFile*>& files() const;

	protected:
		friend class MpqFile;
		
		class Block
		{
			public:
				enum Flags
				{
					None = 0x0,
					IsFile = 0x80000000,
					IsSingleUnit = 0x01000000,
					IsEncrypted = 0x00020000,
					IsCompressed = 0x00000200,
					IsImploded = 0x00000100
				};

				Block(class Mpq *mpq);

				std::streamsize read(std::istream &istream) throw (class Exception);

				/**
				* @todo Check size, flags and required properties.
				*/
				bool check() const;
				bool empty() const
				{
					return this->m_blockOffset > 0 && this->m_blockSize > 0 && this->m_fileSize == 0 && this->m_flags == Mpq::Block::None;
				};
				bool unused() const
				{
					return this->m_blockSize == 0 && this->m_fileSize == 0 && this->m_flags == Mpq::Block::None;
				};
				uint64 largeOffset() const
				{
					return ((uint64)this->m_extendedBlockOffset << 32) + (uint64)this->m_blockOffset;
				};
				bool time(time_t &time)
				{
					// The FILETIME represents a 64-bit integer: the number of 100 ns units since January 1, 1601
					uint64 nTime = ((uint64)this->m_fileTime.highDateTime << 32) + this->m_fileTime.lowDateTime;
				
					if (nTime < EPOCH_OFFSET)
						return false;
				
					nTime -= EPOCH_OFFSET;	// Convert the time base from 01/01/1601 to 01/01/1970
					nTime /= 10000000ULL;	// Convert 100 ns to sec
				
					time = (time_t)nTime;
				
					// Test for overflow (FILETIME is 64 bits, time_t is 32 bits)
					if ((nTime - (uint64)time) > 0)
						return false;
				
					return true;
				};
				void setFileTime(const time_t &time)
				{
					uint64 nTime = (uint64)time;
					
					nTime *= 10000000ULL;
					nTime += EPOCH_OFFSET;
				
					this->m_fileTime.lowDateTime = (uint32_t)nTime;
					this->m_fileTime.highDateTime = (uint32_t)(nTime >> 32);
				}
						
			protected:
				friend class Mpq;
				
				static const uint64 EPOCH_OFFSET = 116444736000000000ULL; // Number of 100 ns units between 01/01/1601 and 01/01/1970
				
				class Mpq *m_mpq;
				int32 m_blockOffset;
				int16 m_extendedBlockOffset;
				int32 m_blockSize;
				int32 m_fileSize;
				Flags m_flags;
				// extended attributes
				int32 m_crc32;
				FILETIME m_fileTime;
				MD5 m_md5;
		};
		
		class Hash
		{
			public:
				Hash(class Mpq *mpq);
				
				std::streamsize read(std::istream &istream) throw (class Exception);
				void clear();
				
				bool check() const;
				bool deleted() const
				{
					return this->m_deleted;
				};
				bool empty() const
				{
					return !this->m_deleted && this->m_block == 0;
				};
				
			protected:
				friend class Mpq;
				
				static const int32 blockIndexDeleted = 0xFFFFFFFE;
				static const int32 blockIndexEmpty = 0xFFFFFFFF;
				
				class Mpq *m_mpq;
				class MpqFile *m_mpqFile;
				int32 m_filePathHashA;
				int32 m_filePathHashB;
				int16 m_language; // enum?
				int8 m_platform;
				class Block *m_block; // if this value is 0 it has never been used
				bool m_deleted; // can not be true if m_block is 0
		};
		
		static const uint32* cryptTable();

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
		
		/// This value is required for getting the first block offset (relative to the beginning of archive).
		static const std::size_t headerSize;
		
		std::size_t m_size;
		boost::filesystem::path m_path;
		enum Format m_format;
		enum ExtendedAttributes m_extendedAttributes;
		std::list<class Block*> m_blocks;
		std::list<class Hash*> m_hashes;
		std::list<class MpqFile*> m_files;
};

inline bool Mpq::check() const
{
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

inline std::size_t Mpq::blockCount() const
{
	return this->m_blocks.size();
}

inline std::size_t Mpq::hashCount() const
{
	return this->m_hashes.size();
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

inline class Mpq::Block* Mpq::firstEmptyBlock() const
{
	BOOST_FOREACH(class Mpq::Block *block, this->m_blocks)
	{
		if (block->empty())
			return block;
	}
	
	return 0;
}

inline class Mpq::Block* Mpq::firstUnusedBlock() const
{
	BOOST_FOREACH(class Mpq::Block *block, this->m_blocks)
	{
		if (block->unused())
			return block;
	}
	
	return 0;
}

inline class Mpq::Block* Mpq::lastOffsetBlock() const
{
	uint64 offset = 0;
	class Mpq::Block *result = 0;
	
	BOOST_FOREACH(class Mpq::Block *block, this->m_blocks)
	{
		uint64 newOffset = block->largeOffset();
		
		if (newOffset > offset)
			result = block;
	}
	
	return result;
}
	

inline class Mpq::Hash* Mpq::firstEmptyHash() const
{
	BOOST_FOREACH(class Mpq::Hash *hash, this->m_hashes)
	{
		if (hash->empty())
			return hash;
	}
	
	return 0;
}

inline class Mpq::Hash* Mpq::firstDeletedHash() const
{
	BOOST_FOREACH(class Mpq::Hash *hash, this->m_hashes)
	{
		if (hash->deleted())
			return hash;
	}
	
	return 0;
}

inline uint64 Mpq::nextBlockOffset() const
{
	class Mpq::Block *block = this->lastOffsetBlock();
	
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
