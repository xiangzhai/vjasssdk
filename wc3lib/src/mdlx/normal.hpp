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

#ifndef WC3LIB_MDLX_NORMAL_HPP
#define WC3LIB_MDLX_NORMAL_HPP

#include "groupmdxblockmember.hpp"

namespace wc3lib
{

namespace mdlx
{
	
class Normals;

class Normal : public GroupMdxBlockMember
{
	public:
		Normal(class Normals *normals);
		virtual ~Normal();

		//class Normals* normals() const;
		float32 x() const;
		float32 y() const;
		float32 z() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		float32 m_x, m_y, m_z;
};

/*
inline class Normals* Normal::normals() const
{
	return dynamic_cast<class Normals*>(this->m_parent);
}
*/

inline float32 Normal::x() const
{
	return this->m_x;
}

inline float32 Normal::y() const
{
	return this->m_y;
}

inline float32 Normal::z() const
{
	return this->m_z;
}

}

}

#endif
