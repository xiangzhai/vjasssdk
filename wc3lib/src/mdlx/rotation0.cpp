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

#include "rotation0.hpp"
#include "rotation0s.hpp"

namespace wc3lib
{

namespace mdlx
{

Rotation0::Rotation0(class Rotation0s *rotations) : m_rotations(rotations)
{
}

Rotation0::~Rotation0()
{
}

void Rotation0::readMdl(std::istream &istream) throw (class Exception)
{
}

void Rotation0::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 Rotation0::readMdx(std::istream &istream) throw (class Exception)
{
	istream.read(reinterpret_cast<char*>(&this->m_frame), sizeof(this->m_frame));
	long32 bytes = istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_a), sizeof(this->m_a));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_b), sizeof(this->m_b));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_c), sizeof(this->m_c));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_d), sizeof(this->m_d));
	bytes += istream.gcount();
	
	if (this->m_rotations->lineType() > 1)
	{
		istream.read(reinterpret_cast<char*>(&this->m_inTanA), sizeof(this->m_inTanA));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_inTanB), sizeof(this->m_inTanB));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_inTanC), sizeof(this->m_inTanC));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_inTanD), sizeof(this->m_inTanD));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_outTanA), sizeof(this->m_outTanA));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_outTanB), sizeof(this->m_outTanB));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_outTanC), sizeof(this->m_outTanC));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_outTanD), sizeof(this->m_outTanD));
		bytes += istream.gcount();
	}
	
	return bytes;
}

long32 Rotation0::writeMdx(std::ostream &ostream) throw (class Exception)
{
	return 0;
}

}

}
