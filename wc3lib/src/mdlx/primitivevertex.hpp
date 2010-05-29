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

#ifndef WC3LIB_MDLX_PRIMITIVEVERTEX_HPP
#define WC3LIB_MDLX_PRIMITIVEVERTEX_HPP

#include <istream>
#include <ostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class PrimitiveVertices;

class PrimitiveVertex
{
	public:
		PrimitiveVertex(class PrimitiveVertices *primitiveVertices);
		virtual ~PrimitiveVertex();

		class PrimitiveVertices* primitiveVertices() const;
		short16 triangle() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class PrimitiveVertices *m_primitiveVertices;
		short16 m_triangle;
};

inline class PrimitiveVertices* PrimitiveVertex::primitiveVertices() const
{
	return this->m_primitiveVertices;
}

inline short16 PrimitiveVertex::triangle() const
{
	return this->m_triangle;
}

}

}

#endif
