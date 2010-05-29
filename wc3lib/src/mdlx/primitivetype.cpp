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

#include "primitivetype.hpp"

namespace wc3lib
{

namespace mdlx
{

PrimitiveType::PrimitiveType(class PrimitiveTypes *primitiveTypes) : GroupMdxBlockMember(primitiveTypes)
{
}

PrimitiveType::~PrimitiveType()
{
}

void PrimitiveType::readMdl(std::istream &istream) throw (class Exception)
{
}

void PrimitiveType::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize PrimitiveType::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = 0;
	istream.read(reinterpret_cast<char*>(&this->m_value), sizeof(this->m_value));
	bytes += istream.gcount();
	
	return bytes;
}

std::streamsize PrimitiveType::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
