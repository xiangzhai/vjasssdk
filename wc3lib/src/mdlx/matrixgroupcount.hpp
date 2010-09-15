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

#ifndef WC3LIB_MDLX_MATRIXGROUPCOUNT_HPP
#define WC3LIB_MDLX_MATRIXGROUPCOUNT_HPP

#include <list>

#include "groupmdxblockmember.hpp"
#include "matrixgroupcounts.hpp"

namespace wc3lib
{

namespace mdlx
{

class MatrixGroupCount : public GroupMdxBlockMember
{
	public:
		MatrixGroupCount(class MatrixGroupCounts *matrixGroupCounts);
		virtual ~MatrixGroupCount();

		class MatrixGroupCounts* matrixGroupCounts() const;
		long32 data() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		long32 m_data;
};

inline class MatrixGroupCounts* MatrixGroupCount::matrixGroupCounts() const
{
	return dynamic_cast<class MatrixGroupCounts*>(this->m_parent);
}

inline long32 MatrixGroupCount::data() const
{
	return this->m_data;
}

}

}

#endif
