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

#ifndef WC3LIB_MDLX_SCALING_HPP
#define WC3LIB_MDLX_SCALING_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;

class Scaling
{

	public:
		Scaling(class Mdlx *mdlx);
		virtual ~Scaling();

		class Mdlx* mdlx() const;
		long32 frame() const;
		float32 x() const;
		float32 y() const;
		float32 z() const;
		float32 inTanX() const;
		float32 inTanY() const;
		float32 inTanZ() const;
		float32 outTanX() const;
		float32 outTanY() const;
		float32 outTanZ() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		long32	m_frame;
		float32	m_x, m_y, m_z;
		//if (LineType > 1) {
		float32	m_inTanX, m_inTanY, m_inTanZ;
		float32	m_outTanX, m_outTanY, m_outTanZ;
		//}
};

inline class Mdlx* Scaling::mdlx() const
{
	return this->m_mdlx;
}

inline long32 Scaling::frame() const
{
	return this->m_frame;
}

inline float32 Scaling::x() const
{
	return this->m_x;
}

inline float32 Scaling::y() const
{
	return this->m_y;
}

inline float32 Scaling::z() const
{
	return this->m_z;
}

inline float32 Scaling::inTanX() const
{
	return this->m_inTanX;
}

inline float32 Scaling::inTanY() const
{
	return this->m_inTanY;
}

inline float32 Scaling::inTanZ() const
{
	return this->m_inTanZ;
}

inline float32 Scaling::outTanX() const
{
	return this->m_outTanX;
}

inline float32 Scaling::outTanY() const
{
	return this->m_outTanY;
}

inline float32 Scaling::outTanZ() const
{
	return this->m_outTanZ;
}

}

}

#endif
