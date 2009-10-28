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

//#include <cstdio>
#include <cstring>
#include <iostream> //debug

#include "mdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{

MdxBlock::MdxBlock(byte blockName[4], bool optional) : m_optional(optional), m_exists(false)
{
	memcpy(this->m_blockName, blockName, sizeof(blockName));
	/*
	for (unsigned int i = 0; i < sizeof(this->m_blockName); ++i)
		this->m_blockName[i] = blockName[i];
	*/
}

/// @todo Consider optional like in Python script.
long32 MdxBlock::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = 0;
	byte identifier[sizeof(this->m_blockName)];
	std::fstream::pos_type position = fstream.tellg();
	fstream.read(identifier, sizeof(identifier));
	bytes += fstream.gcount();
	//std::cout << "Read bytes: " << fstream.gcount() << std::endl;

	if (memcmp(identifier, this->m_blockName, sizeof(this->m_blockName)) != 0)
	{
		if (this->m_optional)
		{
			fstream.seekg(position);
			std::cout << "Block " << this->m_blockName << " is optional and doesn't exist." << std::endl;
			std::cout << "Block name " << this->m_blockName << " is not equal to " << identifier << std::endl;
			
			return 0;
		}
		else
		{
			char message[50];
			sprintf(message, "Unexptected identifier \"%s\". Missing \"%s\" block name.", identifier, this->m_blockName);
			
			throw Exception(message);
		}
	}
	
	this->m_exists = true;
	
	return bytes;
}

long32 MdxBlock::writeMdx(std::fstream &fstream) throw (class Exception)
{
	fstream.write(this->m_blockName, sizeof(this->m_blockName));
	
	return sizeof(this->m_blockName);
	
}

bool MdxBlock::moveToBlockName(std::fstream &fstream)
{
	byte readBlockName[4];
	
	while (fstream)
	{
		fstream.read(readBlockName, sizeof(readBlockName));
		
		if (memcmp(readBlockName, this->m_blockName, sizeof(this->m_blockName)) == 0)
			return true;
	}
	
	return true;
}

}

}
