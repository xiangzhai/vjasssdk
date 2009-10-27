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

#include "groupvertices.hpp"
#include "groupvertex.hpp"

namespace wc3lib
{

namespace mdlx
{

GroupVertices::GroupVertices(class Geoset *geoset) : MdxBlock("GNDX"), m_geoset(geoset)
{
}

GroupVertices::~GroupVertices()
{
	for (std::list<class GroupVertex*>::iterator iterator = this->m_groupVertices.begin(); iterator != this->m_groupVertices.end(); ++iterator)
		delete *iterator;
}

void GroupVertices::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void GroupVertices::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 GroupVertices::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nvgrps = 0;
	fstream.read(reinterpret_cast<char*>(&nvgrps), sizeof(nvgrps));
	bytes += fstream.gcount();
	
	for ( ; nvgrps > 0; --nvgrps)
	{
		class GroupVertex *groupVertex = new GroupVertex(this);
		bytes += groupVertex->readMdx(fstream);
		this->m_groupVertices.push_back(groupVertex);
	}
	
	return bytes;
}

long32 GroupVertices::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
