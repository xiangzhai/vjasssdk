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

#ifndef WC3LIB_MDLX_SCALING0_HPP
#define WC3LIB_MDLX_SCALING0_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Scaling0s;

class Scaling0
{
	public:
		Scaling0(class Scaling0s *mdlx);
		virtual ~Scaling0();

		class Scaling0s* scalings() const;
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
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual long32 readMdx(std::fstream &fstream) throw (class Exception);
		virtual long32 writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Scaling0s *m_scalings;
		long32	m_frame;
		float32	m_x, m_y, m_z;
		//if (LineType > 1) {
		float32	m_inTanX, m_inTanY, m_inTanZ;
		float32	m_outTanX, m_outTanY, m_outTanZ;
		//}
};

inline class Scaling0s* Scaling0::scalings() const
{
	return this->m_scalings;
}

inline long32 Scaling0::frame() const
{
	return this->m_frame;
}

inline float32 Scaling0::x() const
{
	return this->m_x;
}

inline float32 Scaling0::y() const
{
	return this->m_y;
}

inline float32 Scaling0::z() const
{
	return this->m_z;
}

inline float32 Scaling0::inTanX() const
{
	return this->m_inTanX;
}

inline float32 Scaling0::inTanY() const
{
	return this->m_inTanY;
}

inline float32 Scaling0::inTanZ() const
{
	return this->m_inTanZ;
}

inline float32 Scaling0::outTanX() const
{
	return this->m_outTanX;
}

inline float32 Scaling0::outTanY() const
{
	return this->m_outTanY;
}

inline float32 Scaling0::outTanZ() const
{
	return this->m_outTanZ;
}

}

}

#endif
