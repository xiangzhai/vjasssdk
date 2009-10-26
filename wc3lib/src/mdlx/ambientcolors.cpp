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

#include "ambientcolors.hpp"
#include "ambientcolor.hpp"

namespace wc3lib
{

namespace mdlx
{

AmbientColors::AmbientColors(class Light *light) : MdxBlock("KLBC"), m_light(light)
{
}

AmbientColors::~AmbientColors()
{
	for (std::list<class AmbientColor*>::iterator iterator = this->m_colors.begin(); iterator != this->m_colors.end(); ++iterator)
		delete *iterator;
}

void AmbientColors::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void AmbientColors::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 AmbientColors::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	return bytes;
}

long32 AmbientColors::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(fstream);
	
	return bytes;
}

}

}
