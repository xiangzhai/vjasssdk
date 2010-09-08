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

#include "collisionshapes.hpp"
#include "collisionshape.hpp"

namespace wc3lib
{

namespace mdlx
{

CollisionShapes::CollisionShapes(class Mdlx *mdlx) : MdxBlock("CLID"), m_mdlx(mdlx)
{
}

CollisionShapes::~CollisionShapes()
{
	BOOST_FOREACH(class CollisionShape *collisionShape, this->m_collisionShapes)
		delete collisionShape;
}

std::streamsize CollisionShapes::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize CollisionShapes::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize CollisionShapes::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);

	if (bytes == 0)
		return 0;

	long32 nbytes;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();

	while (nbytes > 0)
	{
		class CollisionShape *collisionShape = new CollisionShape(this);
		long32 readBytes = collisionShape->readMdx(istream);
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_collisionShapes.push_back(collisionShape);
	}

	return bytes;
}

std::streamsize CollisionShapes::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = MdxBlock::writeMdx(ostream);

	if (bytes == 0)
		return 0;

	return bytes;
}

}

}
