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

#include "segmentcolor.hpp"

namespace wc3lib
{

namespace mdlx
{

SegmentColor::SegmentColor(class ParticleEmitter2 *particleEmitter) : m_particleEmitter(particleEmitter)
{
}

SegmentColor::~SegmentColor()
{
}

void SegmentColor::readMdl(std::istream &istream) throw (class Exception)
{
}

void SegmentColor::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize SegmentColor::readMdx(std::istream &istream) throw (class Exception)
{
	istream.read(reinterpret_cast<char*>(&this->m_red), sizeof(this->m_red));
	std::streamsize bytes = istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_green), sizeof(this->m_green));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_blue), sizeof(this->m_blue));
	bytes += istream.gcount();
	
	return bytes;
}

std::streamsize SegmentColor::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
