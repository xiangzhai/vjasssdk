/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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

#ifndef WC3LIB_MDLX_GROUPVERTICES_HPP
#define WC3LIB_MDLX_GROUPVERTICES_HPP

#include "groupmdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{

/// GNDX
class GroupVertices : public GroupMdxBlock
{
	public:
		GroupVertices(class Geoset *geoset);
		virtual ~GroupVertices();

		class Geoset* geoset() const;
		const std::list<class GroupVertex*>& groupVertices() const;

		virtual std::streamsize readMdl(istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(ostream &ostream) const throw (class Exception);

	protected:
		virtual class GroupMdxBlockMember* createNewMember();

		class Geoset *m_geoset;
};

inline class Geoset* GroupVertices::geoset() const
{
	return this->m_geoset;
}

inline const std::list<class GroupVertex*>& GroupVertices::groupVertices() const
{
	return reinterpret_cast<const std::list<class GroupVertex*>&>(this->m_members);
}

}

}

#endif
