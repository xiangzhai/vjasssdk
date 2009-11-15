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

#ifndef WC3LIB_MDLX_ROTATION0_HPP
#define WC3LIB_MDLX_ROTATION0_HPP

#include <iostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Rotation0s;

class Rotation0
{
	public:
		Rotation0(class Rotation0s *rotations);
		virtual ~Rotation0();

		class Rotation0s* rotations() const;
		float32 frame() const;
		float32 a() const;
		float32 b() const;
		float32 c() const;
		float32 d() const;
		float32 inTanA() const;
		float32 inTanB() const;
		float32 inTanC() const;
		float32 inTanD() const;
		float32 outTanA() const;
		float32 outTanB() const;
		float32 outTanC() const;
		float32 outTanD() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

	protected:
		class Rotation0s *m_rotations;
		long32 m_frame;
		float32 m_a, m_b, m_c, m_d;
		//if (LineType > 1)
		//{
		float32 m_inTanA, m_inTanB, m_inTanC, m_inTanD;
		float32 m_outTanA, m_outTanB, m_outTanC, m_outTanD;
		//}
};

inline class Rotation0s* Rotation0::rotations() const
{
	return this->m_rotations;
}

inline float32 Rotation0::frame() const
{
	return this->m_frame;
}

inline float32 Rotation0::a() const
{
	return this->m_a;
}

inline float32 Rotation0::b() const
{
	return this->m_b;
}

inline float32 Rotation0::c() const
{
	return this->m_c;
}

inline float32 Rotation0::d() const
{
	return this->m_d;
}

inline float32 Rotation0::inTanA() const
{
	return this->m_inTanA;
}

inline float32 Rotation0::inTanB() const
{
	return this->m_inTanB;
}

inline float32 Rotation0::inTanC() const
{
	return this->m_inTanC;
}

inline float32 Rotation0::inTanD() const
{
	return this->m_inTanD;
}

inline float32 Rotation0::outTanA() const
{
	return this->m_outTanA;
}

inline float32 Rotation0::outTanB() const
{
	return this->m_outTanB;
}

inline float32 Rotation0::outTanC() const
{
	return this->m_outTanC;
}

inline float32 Rotation0::outTanD() const
{
	return this->m_outTanD;
}

}

}

#endif
