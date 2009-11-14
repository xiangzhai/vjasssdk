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

#include "primitivevertices.hpp"
#include "primitivevertex.hpp"

namespace wc3lib
{

namespace mdlx
{

PrimitiveVertices::PrimitiveVertices(class Geoset *geoset) : MdxBlock("PVTX"), m_geoset(geoset)
{
}

PrimitiveVertices::~PrimitiveVertices()
{
	for (std::list<class PrimitiveVertex*>::iterator iterator = this->m_primitiveVertices.begin(); iterator != this->m_primitiveVertices.end(); ++iterator)
		delete *iterator;
}

void PrimitiveVertices::readMdl(std::istream &istream) throw (class Exception)
{
}

void PrimitiveVertices::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 PrimitiveVertices::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;

	long32 ntris = 0;
	istream.read(reinterpret_cast<char*>(&ntris), sizeof(ntris));
	bytes += istream.gcount();
	std::cout << "Primitive vertices " << ntris << std::endl;
	
	for ( ; ntris > 0; --ntris)
	{
		class PrimitiveVertex *primitiveVertex = new PrimitiveVertex(this);
		bytes += primitiveVertex->readMdx(istream);
		this->m_primitiveVertices.push_back(primitiveVertex);
	}
	
	std::cout << "Primitive vertices bytes " << bytes << std::endl;
	
	return bytes;
}

long32 PrimitiveVertices::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	return bytes;
}

}

}
