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

#include <iostream> // debug

#include "primitivesizes.hpp"
#include "primitivesize.hpp"

namespace wc3lib
{

namespace mdlx
{

PrimitiveSizes::PrimitiveSizes(class Geoset *geoset) : MdxBlock("PCNT"), m_geoset(geoset)
{
}

PrimitiveSizes::~PrimitiveSizes()
{
	for (std::list<class PrimitiveSize*>::iterator iterator = this->m_primitiveSizes.begin(); iterator != this->m_primitiveSizes.end(); ++iterator)
		delete *iterator;
}

void PrimitiveSizes::readMdl(std::istream &istream) throw (class Exception)
{
}

void PrimitiveSizes::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 PrimitiveSizes::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 npcnts = 0;
	istream.read(reinterpret_cast<char*>(&npcnts), sizeof(npcnts));
	bytes += istream.gcount();
	std::cout << "Primitive sizes " << npcnts << std::endl;
	
	for ( ; npcnts > 0; --npcnts)
	{
		class PrimitiveSize *primitiveSize = new PrimitiveSize(this);
		bytes += primitiveSize->readMdx(istream);
		this->m_primitiveSizes.push_back(primitiveSize);
	}
	
	std::cout << "Primitive sizes bytes " << bytes << std::endl;
	
	return bytes;
}

long32 PrimitiveSizes::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	return bytes;
}

}

}
