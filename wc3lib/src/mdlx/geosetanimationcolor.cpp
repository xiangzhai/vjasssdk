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

#include "geosetanimationcolor.hpp"
#include "geosetanimationcolors.hpp"

namespace wc3lib
{

namespace mdlx
{

GeosetAnimationColor::GeosetAnimationColor(class GeosetAnimationColors *geosetAnimationColors) : m_geosetAnimationColors(geosetAnimationColors)
{
}

void GeosetAnimationColor::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void GeosetAnimationColor::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 GeosetAnimationColor::readMdx(std::fstream &fstream) throw (class Exception)
{
	fstream.read(reinterpret_cast<char*>(&this->m_frame), sizeof(this->m_frame));
	long32 bytes = fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_x), sizeof(this->m_x));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_y), sizeof(this->m_y));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_z), sizeof(this->m_z));
	bytes += fstream.gcount();
	
	if (this->geosetAnimationColors()->lineType() > 1)
	{
		fstream.read(reinterpret_cast<char*>(&this->m_inTanX), sizeof(this->m_inTanX));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_inTanY), sizeof(this->m_inTanY));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_inTanZ), sizeof(this->m_inTanZ));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_outTanX), sizeof(this->m_outTanX));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_outTanY), sizeof(this->m_outTanY));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_outTanZ), sizeof(this->m_outTanZ));
		bytes += fstream.gcount();
	}
	// init against access errors
	else
	{
		this->m_inTanX = 0.0;
		this->m_inTanY = 0.0;
		this->m_inTanZ = 0.0;
		this->m_outTanX = 0.0;
		this->m_outTanY = 0.0;
		this->m_outTanZ = 0.0;
	}
	
	return bytes;
}

long32 GeosetAnimationColor::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
