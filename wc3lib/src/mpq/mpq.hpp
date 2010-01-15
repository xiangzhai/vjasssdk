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

#include <boost/foreach.hpp>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mpq
{

class MpqFile;

class Mpq
{
	public:
		enum Mode
		{
			Read = 1 << 0,
			Write = 1 << 1
		};

		enum Format
		{
			Mpq1, // original format
			Mpq2 // Burning Crusade, large files
		};

		static const int32 identifier1;
		static const int16 format1Identifier;
		static const int16 format2Identifier;

		Mpq();
		~Mpq();
		
		/**
		* @return Returns MPQ's size in bytes.
		*/
		std::streamsize readMpq(std::istream &istream, enum Mode mode) throw (class Exception);
		/**
		* @return Returns MPQ's size in bytes.
		*/
		std::streamsize writeMpq(std::ostream &ostream) const throw (class Exception);
		
#ifdef TAR
		std::streamsize readTar(std::istream &istream, enum Mode mode) throw (class Exception);
		std::streamsize writeTar(std::istream &istream, enum Mode mode) throw (class Exception);		
#endif
		
		const class MpqFile* findFile(const std::string &path) const;
		const class MpqFile* findFileByName(const std::string &name) const;
		
		std::streamsize size() const;
		std::string path() const;
		const std::list<class MpqFile*>& files() const;

	protected:
		class Block
		{
			public:
				enum Flags
				{
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
						
			protected:
				friend class Mpq;
				
				static uint32 hashKey;
				
				class Mpq *m_mpq;
				int32 m_blockOffset;
				int32 m_blockSize;
				int32 m_fileSize;
				Flags m_flags;
		};
		
		bool checkBlocks() const;
		class Block* firstEmptyBlock() const;
		class Block* firstUnusedBlock() const;
		
		enum Format m_format;
		std::list<class Block*> m_blocks;
		std::list<class MpqFile*> m_files;
};

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

/**
* Empty space entries should have BlockOffset and BlockSize nonzero, and FileSize and Flags zero.
*/
inline class Mpq::Block* Mpq::firstEmptyBlock() const
{
	BOOST_FOREACH(class Mpq::Block *block, this->m_blocks)
	{
		if (block->m_blockOffset > 0 && block->m_blockSize > 0 && block->m_fileSize == 0 && block->m_flags == 0)
			return block;
	}
	
	return 0;
}

/**
* Unused block table entries should have BlockSize, FileSize, and Flags zero.
*/
inline class Mpq::Block* Mpq::firstUnusedBlock() const
{
	BOOST_FOREACH(class Mpq::Block *block, this->m_blocks)
	{
		if (block->m_blockSize == 0 && block->m_fileSize == 0 && block->m_flags == 0)
			return block;
	}
	
	return 0;
}

}

}

#endif
