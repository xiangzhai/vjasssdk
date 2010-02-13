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

#include "block.hpp"
#include "mpq.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{
	
namespace mpq
{

Block::Block(class Mpq *mpq) : m_mpq(mpq), m_blockOffset(0), m_extendedBlockOffset(0), m_blockSize(0), m_fileSize(0), m_flags(Block::None)
{
}

std::streamsize Block::read(std::istream &istream) throw (class Exception)
{
	struct BlockTableEntry entry;
	istream.read(reinterpret_cast<char*>(&entry), sizeof(entry));
	std::streamsize bytes = istream.gcount();

	if (bytes != sizeof(entry))
		throw Exception(_("Error while reading block table entry."));

	this->m_blockOffset = entry.blockOffset;
	this->m_blockSize = entry.blockSize;
	this->m_fileSize = entry.fileSize;
	this->m_flags = static_cast<enum Block::Flags>(entry.fileSize);

	return bytes;
}

}

}