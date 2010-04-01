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

#ifndef WC3LIB_MPQ_HASH_HPP
#define WC3LIB_MPQ_HASH_HPP

#include <istream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{
	
namespace mpq
{
	
class Mpq;
class MpqFile;
class Block;

class Hash
{
	public:
		Hash(class Mpq *mpq);
		
		std::streamsize read(std::istream &istream) throw (class Exception);
		void clear();
		
		bool check() const;
		bool deleted() const;
		bool empty() const;
		
		class Mpq* mpq() const;
		class MpqFile* mpqFile() const;
		class Block* block() const;
		
	protected:
		friend class Mpq;
		
		static const int32 blockIndexDeleted = 0xFFFFFFFE;
		static const int32 blockIndexEmpty = 0xFFFFFFFF;
		
		class Mpq *m_mpq;
		class MpqFile *m_mpqFile;
		int32 m_filePathHashA;
		int32 m_filePathHashB;
		int16 m_language; // enum?
		int16 m_platform;
		class Block *m_block; // if this value is 0 it has never been used
		bool m_deleted; // can not be true if m_block is 0
};

inline bool Hash::deleted() const
{
	return this->m_deleted;
}

inline bool Hash::empty() const
{
	return !this->m_deleted && this->m_block == 0;
}

inline class Mpq* Hash::mpq() const
{
	return this->m_mpq;
}

inline class MpqFile* Hash::mpqFile() const
{
	return this->m_mpqFile;
}
		
inline class Block* Hash::block() const
{
	return this->m_block;
}

}

}

#endif
