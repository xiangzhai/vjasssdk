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

#include <iostream> // debug

#include "geosetanimationalpha.hpp"
#include "geosetanimationalphas.hpp"

namespace wc3lib
{

namespace mdlx
{

GeosetAnimationAlpha::GeosetAnimationAlpha(class GeosetAnimationAlphas *geosetAnimationAlphas) : m_geosetAnimationAlphas(geosetAnimationAlphas)
{
}

void GeosetAnimationAlpha::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void GeosetAnimationAlpha::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 GeosetAnimationAlpha::readMdx(std::fstream &fstream) throw (class Exception)
{
	fstream.read(reinterpret_cast<char*>(&this->m_frame), sizeof(this->m_frame));
	long32 bytes = fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_state), sizeof(this->m_state));
	bytes += fstream.gcount();
	
	if (this->m_geosetAnimationAlphas->lineType() > 1)
	{
		fstream.read(reinterpret_cast<char*>(&this->m_inTan), sizeof(this->m_inTan));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_outTan), sizeof(this->m_outTan));
		bytes += fstream.gcount();
	}
	// init against access errors
	else
	{
		this->m_inTan = 0.0;
		this->m_outTan = 0.0;
	}
	
	std::cout << "Geoset animation alpha with " << bytes << " bytes " << std::endl;
	
	return bytes;
}

long32 GeosetAnimationAlpha::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
