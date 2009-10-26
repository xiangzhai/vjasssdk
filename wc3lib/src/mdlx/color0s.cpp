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

#include "color0s.hpp"
#include "color0.hpp"

namespace wc3lib
{

namespace mdlx
{

Color0s::Color0s(class Mdlx *mdlx) : MdxBlock("KGAC"), m_mdlx(mdlx)
{
}

Color0s::~Color0s()
{
	for (std::list<class Color0*>::iterator iterator = this->m_colors.begin(); iterator != this->m_colors.end(); ++iterator)
		delete *iterator;
}

void Color0s::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Color0s::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Color0s::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	return bytes;
}

long32 Color0s::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(fstream);
	
	return bytes;
}

}

}
