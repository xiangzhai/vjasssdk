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

#include <boost/foreach.hpp>

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
	BOOST_FOREACH(class GroupVertex *groupVertex, this->m_groupVertices)
		delete groupVertex;
}

void GroupVertices::readMdl(std::istream &istream) throw (class Exception)
{
}

void GroupVertices::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize GroupVertices::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nvgrps = 0;
	istream.read(reinterpret_cast<char*>(&nvgrps), sizeof(nvgrps));
	bytes += istream.gcount();
	
	for ( ; nvgrps > 0; --nvgrps)
	{
		class GroupVertex *groupVertex = new GroupVertex(this);
		bytes += groupVertex->readMdx(istream);
		this->m_groupVertices.push_back(groupVertex);
	}
	
	return bytes;
}

std::streamsize GroupVertices::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
