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

#include "geosetanimationcolors.hpp"
#include "geosetanimationcolor.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

GeosetAnimationColors::GeosetAnimationColors(class GeosetAnimation *geosetAnimation) : MdxBlock("KGAC"), m_geosetAnimation(geosetAnimation)
{
}

GeosetAnimationColors::~GeosetAnimationColors()
{
	for (std::list<class GeosetAnimationColor*>::iterator iterator = this->m_geosetAnimationColors.begin(); iterator != this->m_geosetAnimationColors.end(); ++iterator)
		delete *iterator;
}

void GeosetAnimationColors::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void GeosetAnimationColors::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 GeosetAnimationColors::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nunks = 0;
	fstream.read(reinterpret_cast<char*>(&nunks), sizeof(nunks));
	bytes += fstream.gcount();
	
	if (nunks <= 0)
	{
		char message[50];
		sprintf(message, _("Geoset animation colors: Count error, %d geoset animation colors.\n"), nunks);
		
		throw Exception(message);
	}
	
	fstream.read(reinterpret_cast<char*>(&this->m_lineType), sizeof(this->m_lineType));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_globalSequenceId), sizeof(this->m_globalSequenceId));
	bytes += fstream.gcount();
	
	for ( ; nunks > 0; --nunks)
	{
		class GeosetAnimationColor *geosetAnimationColor = new GeosetAnimationColor(this);
		bytes += geosetAnimationColor->readMdx(fstream);
		this->m_geosetAnimationColors.push_back(geosetAnimationColor);
	}
	
	return bytes;
}

long32 GeosetAnimationColors::writeMdx(std::fstream &fstream) throw (class Exception)
{
	if (!this->exists())
		return 0;
	
	long32 bytes = MdxBlock::writeMdx(fstream);
}

}

}
