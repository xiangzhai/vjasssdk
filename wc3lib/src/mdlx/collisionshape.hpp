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
		enum Shape shape() const;
		const struct VertexData& vertexData() const;
		const struct VertexData& vertexData2() const;
		float32 boundsRadius() const;

		virtual std::streamsize readMdl(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class CollisionShapes *m_collisionShapes;
		enum Shape m_shape; //(0:box;2:sphere)
		struct VertexData m_vertexData;
		//if (Shape == 0)
		struct VertexData m_vertexData2;
		//else
		float32 m_boundsRadius;
};

inline class CollisionShapes* CollisionShape::collisionShapes() const
{
	return this->m_collisionShapes;
}

inline enum CollisionShape::Shape CollisionShape::shape() const
{
	return this->m_shape;
}

inline const struct VertexData& CollisionShape::vertexData() const
{
	return this->m_vertexData;
}

inline const struct VertexData& CollisionShape::vertexData2() const
{
	return this->m_vertexData2;
}

inline float32 CollisionShape::boundsRadius() const
{
	return this->m_boundsRadius;
}

}

}

#endif
