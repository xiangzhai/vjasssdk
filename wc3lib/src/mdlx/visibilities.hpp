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

#ifndef WC3LIB_MDLX_PIVOTPOINTS_HPP
#define WC3LIB_MDLX_PIVOTPOINTS_HPP

#include <list>

#include "mdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{
	
class PivotPoint;

/// @todo Add several visibility types!
class PivotPoints : public MdxBlock
{
	public:
		PivotPoints(class Mdlx *mdlx);
		virtual ~PivotPoints();

		class Mdlx* mdlx() const;
		std::list<class PivotPoint*> pivotPoints() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		std::list<class PivotPoint*> m_pivotPoints;
};

inline class Mdlx* PivotPoints::mdlx() const
{
	return this->m_mdlx;
}

inline std::list<class PivotPoint*> PivotPoints::pivotPoints() const
{
	return this->m_pivotPoints;
}

}

}

#endif
