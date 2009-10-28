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

#ifndef WC3LIB_MDLX_VERTEX_HPP
#define WC3LIB_MDLX_VERTEX_HPP

#include "groupmdxblockmember.hpp"
#include "vertices.hpp"

namespace wc3lib
{

namespace mdlx
{

class Vertex : public GroupMdxBlockMember
{
	public:
		Vertex(class Vertices *vertices);
		virtual ~Vertex();

		class Vertices* vertices() const;
		float32 x() const;
		float32 y() const;
		float32 z() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual long32 readMdx(std::fstream &fstream) throw (class Exception);
		virtual long32 writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		float32 m_x, m_y, m_z;
};

inline class Vertices* Vertex::vertices() const
{
	return dynamic_cast<class Vertices*>(this->m_parent);
}

inline float32 Vertex::x() const
{
	return this->m_x;
}

inline float32 Vertex::y() const
{
	return this->m_y;
}

inline float32 Vertex::z() const
{
	return this->m_z;
}

}

}

#endif
