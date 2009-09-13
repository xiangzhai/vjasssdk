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

#include <cstdio>

#include "mdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{

MdxBlock::MdxBlock(byte blockName[4], bool optional = false) : m_blockName(blockName), m_optional(optional)
{
}

/// @todo Consider optional like in Python script.
void MdxBlock::readMdx(std::fstream &fstream) throw (class Exception)
{
	byte identifier[sizeof(this->m_blockName)];
	//identifier[4] = '\0';
	fstream.read(identifier, sizeof(this->m_blockName));

	if (strcmp(identifier, this->m_blockName) != 0)
	{
		char message[50];
		sprintf(message, "Missing \"%s\" block name.", this->blockName);
		throw Exception(message);
	}
}

void MdxBlock::writeMdx(std::fstream &fstream) throw (class Exception)
{
	fstream.write(this->m_blockName, sizeof(this->m_blockName));
}

}

}
