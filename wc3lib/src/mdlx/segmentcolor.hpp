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

#ifndef WC3LIB_MDLX_SEGMENTCOLOR_HPP
#define WC3LIB_MDLX_SEGMENTCOLOR_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class SegmentColor
{
	public:
		SegmentColor(class Mdlx *mdlx);
		virtual ~SegmentColor();

		class Mdlx* mdlx() const;
		float32 red() const;
		float32 green() const;
		float32 blue() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		// SegmentColor usually 3 segments
		// Inverse order from MDL
		float32 m_red, m_green, m_blue;
};

inline class Mdlx* SegmentColor::mdlx() const
{
	return this->m_mdlx;
}

inline float32 SegmentColor::red() const
{
	return this->m_red;
}

inline float32 SegmentColor::green() const
{
	return this->m_green;
}

inline float32 SegmentColor::blue() const
{
	return this->m_blue;
}

}

}

#endif
