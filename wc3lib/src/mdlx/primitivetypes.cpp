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

#include "primitivetypes.hpp"
#include "primitivetype.hpp"

namespace wc3lib
{

namespace mdlx
{

PrimitiveTypes::PrimitiveTypes(class Geoset *geoset) : MdxBlock("PTYP"), m_geoset(geoset)
{
}

PrimitiveTypes::~PrimitiveTypes()
{
	for (std::list<class PrimitiveType*>::iterator iterator = this->m_primitiveTypes.begin(); iterator != this->m_primitiveTypes.end(); ++iterator)
		delete *iterator;
}

void PrimitiveTypes::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void PrimitiveTypes::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 PrimitiveTypes::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nptyps = 0;
	fstream.read(reinterpret_cast<char*>(&nptyps), sizeof(nptyps));
	bytes += fstream.gcount();
	
	for ( ; nptyps > 0; --nptyps)
	{
		class PrimitiveType *primitiveType = new PrimitiveType(this);
		bytes += primitiveType->readMdx(fstream);
		this->m_primitiveTypes.push_back(primitiveType);
	}
	
	return bytes;
}

long32 PrimitiveTypes::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = 0;
	bytes += MdxBlock::writeMdx(fstream);
	
	return bytes;
}

}

}
