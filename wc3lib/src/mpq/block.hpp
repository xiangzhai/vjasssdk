/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#ifndef WC3LIB_MPQ_BLOCK_HPP
#define WC3LIB_MPQ_BLOCK_HPP

#include <istream>
#include <ctime>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{
	
namespace mpq
{
	
class Mpq;

class Block
{
	public:
		enum Flags
		{
			None = 0x0,
			IsFile = 0x80000000,
			IsSingleUnit = 0x01000000,
			UsesEncryptionKey = 0x00020000,
			IsEncrypted = 0x00010000,
			IsCompressed = 0x00000200,
			IsImploded = 0x00000100
		};

		Block(class Mpq *mpq);

		std::streamsize read(std::istream &istream) throw (class Exception);

		/**
		* @todo Check size, flags and required properties.
		*/
		bool check() const;
		bool empty() const;
		bool unused() const;
		uint64 largeOffset() const;
		bool time(time_t &time);
		void setFileTime(const time_t &time);
				
	protected:
		friend class Mpq;
		friend class Hash;
		
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

inline bool Block::empty() const
{
	return this->m_blockOffset > 0 && this->m_blockSize > 0 && this->m_fileSize == 0 && this->m_flags == Block::None;
}

inline bool Block::unused() const
{
	return this->m_blockSize == 0 && this->m_fileSize == 0 && this->m_flags == Block::None;
}

inline uint64 Block::largeOffset() const
{
	return ((uint64)this->m_extendedBlockOffset << 32) + (uint64)this->m_blockOffset;
}

inline bool Block::time(time_t &time)
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
}

inline void Block::setFileTime(const time_t &time)
{
	uint64 nTime = (uint64)time;
	
	nTime *= 10000000ULL;
	nTime += EPOCH_OFFSET;

	this->m_fileTime.lowDateTime = (uint32_t)nTime;
	this->m_fileTime.highDateTime = (uint32_t)(nTime >> 32);
}

}

}

#endif
