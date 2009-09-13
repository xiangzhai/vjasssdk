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

#ifndef WC3LIB_MDLX_PIVOTPOINT_HPP
#define WC3LIB_MDLX_PIVOTPOINT_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class PivotPoint
{
	public:
		PivotPoint(class Mdlx *mdlx);
		virtual ~PivotPoint();

		class Mdlx* mdlx() const;
		float32 x() const;
		float32 y() const;
		float32 z() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		float32 m_x, m_y, m_z;
};

inline class Mdlx* PivotPoint::mdlx() const
{
	return this->m_mdlx;
}

inline float32 PivotPoint::x() const
{
	return this->m_x;
}

inline float32 PivotPoint::y() const
{
	return this->m_y;
}

inline float32 PivotPoint::z() const
{
	return this->m_z;
}

}

}

#endif

