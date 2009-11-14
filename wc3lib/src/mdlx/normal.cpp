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

#include "normal.hpp"
#include "normals.hpp"

namespace wc3lib
{

namespace mdlx
{

Normal::Normal(class Normals *normals) : GroupMdxBlockMember(normals)
{
}

Normal::~Normal()
{
}

void Normal::readMdl(std::istream &istream) throw (class Exception)
{
}

void Normal::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 Normal::readMdx(std::istream &istream) throw (class Exception)
{
	istream.read(reinterpret_cast<char*>(&this->m_x), sizeof(this->m_x));
	long32 bytes = istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_y), sizeof(this->m_y));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_z), sizeof(this->m_z));
	bytes += istream.gcount();
	
	return bytes;
}

long32 Normal::writeMdx(std::ostream &ostream) throw (class Exception)
{
	return 0;
}

}

}
