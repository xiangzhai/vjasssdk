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

#include <boost/foreach.hpp>

#include "hash.hpp"
#include "mpq.hpp"
#include "mpqfile.hpp"
#include "block.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{
	
namespace mpq
{

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