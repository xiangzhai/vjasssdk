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

#ifndef WC3LIB_MDLX_MATRIXGROUPCOUNTS_HPP
#define WC3LIB_MDLX_MATRIXGROUPCOUNTS_HPP

#include <fstream>
#include <list>

#include "mdxblock.hpp"
#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Geoset;
class MatrixGroupCount;

//MTGC
class MatrixGroupCounts : public MdxBlock
{
	public:
		MatrixGroupCounts(class Geoset *geoset);
		virtual ~MatrixGroupCounts();

		class Geoset* geoset() const;
		std::list<class MatrixGroupCount*> matrixGroupCounts() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual long32 readMdx(std::fstream &fstream) throw (class Exception);
		virtual long32 writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Geoset *m_geoset;
		std::list<class MatrixGroupCount*> m_matrixGroupCounts;
};

inline class Geoset* MatrixGroupCounts::geoset() const
{
	return this->m_geoset;
}

inline std::list<class MatrixGroupCount*> MatrixGroupCounts::matrixGroupCounts() const
{
	return this->m_matrixGroupCounts;
}

}

}

#endif