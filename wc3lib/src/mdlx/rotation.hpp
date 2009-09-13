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

#ifndef WC3LIB_MDLX_NORMAL_HPP
#define WC3LIB_MDLX_NORMAL_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Rotation
{
	public:
		Rotation(class Mdlx *mdlx);
		virtual ~Rotation();

		class Mdlx* mdlx() const;
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

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		long32 m_frame;
		float32 m_a, m_b, m_c, m_d;
		//if (LineType > 1)
		//{
		float32 m_inTanA, m_inTanB, m_inTanC, m_inTanD;
		float32 m_outTanA, m_outTanB, m_outTanC, m_outTanD;
		//}
};

class Mdlx* Rotation::mdlx() const
{
	return this->m_mdlx;
}

float32 Rotation::frame() const
{
	return this->m_frame;
}

float32 Rotation::a() const
{
	return this->m_a;
}

float32 Rotation::b() const
{
	return this->m_b;
}

float32 Rotation::c() const
{
	return this->m_c;
}

float32 Rotation::d() const
{
	return this->m_d;
}

float32 Rotation::inTanA() const
{
	return this->m_inTanA;
}

float32 Rotation::inTanB() const
{
	return this->m_inTanB;
}

float32 Rotation::inTanC() const
{
	return this->m_inTanC;
}

float32 Rotation::inTanD() const
{
	return this->m_inTanD;
}

float32 Rotation::outTanA() const
{
	return this->m_outTanA;
}

float32 Rotation::outTanB() const
{
	return this->m_outTanB;
}

float32 Rotation::outTanC() const
{
	return this->m_outTanC;
}

float32 Rotation::outTanD() const
{
	return this->m_outTanD;
}

}

}

#endif
