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

#ifndef WC3LIB_MDLX_PRIMITIVESIZES_HPP
#define WC3LIB_MDLX_PRIMITIVESIZES_HPP

#include <list>

#include "mdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{

class Geoset;
class PrimitiveSize;

//PCNT
class PrimitiveSizes : public MdxBlock
{
	public:
		PrimitiveSizes(class Geoset *geoset);
		virtual ~PrimitiveSizes();

		class Geoset* geoset() const;
		const std::list<class PrimitiveSize*>& primitiveSizes() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class Geoset *m_geoset;
		std::list<class PrimitiveSize*> m_primitiveSizes;
};

inline class Geoset* PrimitiveSizes::geoset() const
{
	return this->m_geoset;
}

inline const std::list<class PrimitiveSize*>& PrimitiveSizes::primitiveSizes() const
{
	return this->m_primitiveSizes;
}

}

}

#endif

