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

#include "mdxscaling.hpp"
#include "mdxscalings.hpp"

namespace wc3lib
{
	
namespace mdlx
{

MdxScaling::MdxScaling(class MdxScalings *scalings) : m_scalings(scalings)
{
}
		
MdxScaling::~MdxScaling()
{
}

long32 MdxScaling::readMdx(std::istream &istream) throw (class Exception)
{
	istream.read(reinterpret_cast<char*>(&this->m_frame), sizeof(this->m_frame));
	long32 bytes = istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_x), sizeof(this->m_x));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_y), sizeof(this->m_y));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_z), sizeof(this->m_z));
	bytes += istream.gcount();
	
	if (this->m_scalings->lineType() > 1)
	{
		istream.read(reinterpret_cast<char*>(&this->m_inTanX), sizeof(this->m_inTanX));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_inTanY), sizeof(this->m_inTanY));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_inTanZ), sizeof(this->m_inTanZ));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_outTanX), sizeof(this->m_inTanX));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_outTanY), sizeof(this->m_inTanY));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_outTanZ), sizeof(this->m_outTanZ));
		bytes += istream.gcount();
	}
	
	return bytes;
}

long32 MdxScaling::writeMdx(std::ostream &ostream) throw (class Exception)
{
	ostream.write(reinterpret_cast<const char*>(&this->m_frame), sizeof(this->m_frame));
	long32 bytes = sizeof(this->m_frame);
	ostream.write(reinterpret_cast<const char*>(&this->m_x), sizeof(this->m_x));
	bytes += sizeof(this->m_x);
	ostream.write(reinterpret_cast<const char*>(&this->m_y), sizeof(this->m_y));
	bytes += sizeof(this->m_y);
	ostream.write(reinterpret_cast<const char*>(&this->m_z), sizeof(this->m_z));
	bytes += sizeof(this->m_z);
	
	if (this->m_scalings->lineType() > 1)
	{
		ostream.write(reinterpret_cast<const char*>(&this->m_inTanX), sizeof(this->m_inTanX));
		bytes += sizeof(this->m_inTanX);
		ostream.write(reinterpret_cast<char*>(&this->m_inTanY), sizeof(this->m_inTanY));
		bytes += sizeof(this->m_inTanY);
		ostream.write(reinterpret_cast<const char*>(&this->m_inTanZ), sizeof(this->m_inTanZ));
		bytes += sizeof(this->m_inTanZ);
		ostream.write(reinterpret_cast<const char*>(&this->m_outTanX), sizeof(this->m_inTanX));
		bytes += sizeof(this->m_inTanX);
		ostream.write(reinterpret_cast<const char*>(&this->m_outTanY), sizeof(this->m_inTanY));
		bytes += sizeof(this->m_inTanY);
		ostream.write(reinterpret_cast<const char*>(&this->m_outTanZ), sizeof(this->m_outTanZ));
		bytes += sizeof(this->m_outTanZ);
	}
	
	return bytes;
}

}

}
