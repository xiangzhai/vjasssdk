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

#include "collisionshape.hpp"
#include "collisionshapes.hpp"

namespace wc3lib
{

namespace mdlx
{

CollisionShape::CollisionShape(class CollisionShapes *collisionShapes) : Object(collisionShapes->mdlx()), m_collisionShapes(collisionShapes)
{
}

CollisionShape::~CollisionShape()
{
}

void CollisionShape::readMdl(std::istream &istream) throw (class Exception)
{
}

void CollisionShape::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 CollisionShape::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = Object::readMdx(istream);
	istream.read(reinterpret_cast<char*>(&this->m_shape), sizeof(this->m_shape));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_x), sizeof(this->m_x));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_y), sizeof(this->m_y));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_z), sizeof(this->m_z));
	bytes += istream.gcount();
	
	if (this->m_shape == Box)
	{
		istream.read(reinterpret_cast<char*>(&this->m_x2), sizeof(this->m_x2));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_y2), sizeof(this->m_y2));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_z2), sizeof(this->m_z2));
		bytes += istream.gcount();
	}
	else
	{
		istream.read(reinterpret_cast<char*>(&this->m_boundsRadius), sizeof(this->m_boundsRadius));
		bytes += istream.gcount();
	}
	
	return bytes;
}

long32 CollisionShape::writeMdx(std::ostream &ostream) throw (class Exception)
{
	return 0;
}

}

}
