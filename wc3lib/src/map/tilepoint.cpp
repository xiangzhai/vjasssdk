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

#include "tilepoint.hpp"

namespace wc3lib
{
	
namespace map
{
	
Tilepoint::Tilepoint(class Environment *environment) : m_environment(environment)
{
}

/// @todo Missing one byte?
std::streamsize Tilepoint::read(std::istream &istream) throw (class Exception)
{
	istream.read(reinterpret_cast<char*>(&this->m_groundHeight), sizeof(this->m_groundHeight));
	std::streamsize bytes = istream.gcount();
	short16 waterLevel;
	istream.read(reinterpret_cast<char*>(&waterLevel), sizeof(waterLevel));
	bytes += istream.gcount();
	this->m_waterLevel = (waterLevel & 0x3FFF); // extract water level
	this->m_flags = Tilepoint::Flags(waterLevel & 0xC000); // extract boundary flag
	
	istream >> this->m_groundTextureType;
	
	istream.read(reinterpret_cast<char*>(&this->m_textureDetails), sizeof (this->m_textureDetails));
	bytes += istream.gcount();
	
	istream >> this->m_cliffTextureType;
	++bytes; // 2* 4 bits
	
	istream >> this->m_layerHeight;
	
	return bytes;
}

}

}
