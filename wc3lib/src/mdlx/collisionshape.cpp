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

void CollisionShape::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void CollisionShape::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 CollisionShape::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = Object::readMdx(fstream);
	fstream.read(reinterpret_cast<char*>(&this->m_shape), sizeof(this->m_shape));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_x), sizeof(this->m_x));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_y), sizeof(this->m_y));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_z), sizeof(this->m_z));
	bytes += fstream.gcount();
	
	if (this->m_shape == Box)
	{
		fstream.read(reinterpret_cast<char*>(&this->m_x2), sizeof(this->m_x2));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_y2), sizeof(this->m_y2));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_z2), sizeof(this->m_z2));
		bytes += fstream.gcount();
	}
	else
	{
		fstream.read(reinterpret_cast<char*>(&this->m_boundsRadius), sizeof(this->m_boundsRadius));
		bytes += fstream.gcount();
	}
	
	return bytes;
}

long32 CollisionShape::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
