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

#ifndef WC3LIB_MDLX_COLLISIONSHAPE_HPP
#define WC3LIB_MDLX_COLLISIONSHAPE_HPP

#include "object.hpp"

namespace wc3lib
{

namespace mdlx
{

class CollisionShapes;

class CollisionShape : public Object
{
	public:
		enum Shape
		{
			Box = 0,
			Sphere = 2
		};

		CollisionShape(class CollisionShapes *collisionShapes);
		virtual ~CollisionShape();
		
		class CollisionShapes* collisionShapes() const;
		long32 shape() const;
		float32 x() const;
		float32 y() const;
		float32 z() const;
		float32 x2() const;
		float32 y2() const;
		float32 z2() const;
		float32 boundsRadius() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

	protected:
		class CollisionShapes *m_collisionShapes;
		long32 m_shape; //(0:box;2:sphere)
		float32 m_x, m_y, m_z;
		//if (Shape == 0)
		float32 m_x2, m_y2, m_z2;
		//else
		float32 m_boundsRadius;
};

inline class CollisionShapes* CollisionShape::collisionShapes() const
{
	return this->m_collisionShapes;
}

inline long32 CollisionShape::shape() const
{
	return this->m_shape;
}

inline float32 CollisionShape::x() const
{
	return this->m_x;
}

inline float32 CollisionShape::y() const
{
	return this->m_y;
}

inline float32 CollisionShape::z() const
{
	return this->m_z;
}

inline float32 CollisionShape::x2() const
{
	return this->m_x2;
}

inline float32 CollisionShape::y2() const
{
	return this->m_y2;
}

inline float32 CollisionShape::z2() const
{
	return this->m_z2;
}

inline float32 CollisionShape::boundsRadius() const
{
	return this->m_boundsRadius;
}

}

}

#endif
