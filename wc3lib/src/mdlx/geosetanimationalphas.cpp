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

#include "geosetanimationalphas.hpp"
#include "geosetanimationalpha.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

GeosetAnimationAlphas::GeosetAnimationAlphas(class GeosetAnimation *geosetAnimation) : MdxBlock("KGAO"), m_geosetAnimation(geosetAnimation)
{
}

GeosetAnimationAlphas::~GeosetAnimationAlphas()
{
	for (std::list<class GeosetAnimationAlpha*>::iterator iterator = this->m_geosetAnimationAlphas.begin(); iterator != this->m_geosetAnimationAlphas.end(); ++iterator)
		delete *iterator;
}

void GeosetAnimationAlphas::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void GeosetAnimationAlphas::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 GeosetAnimationAlphas::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nunks = 0;
	fstream.read(reinterpret_cast<char*>(&nunks), sizeof(nunks));
	bytes += fstream.gcount();
	
	std::cout << "Number of geoset animation alphas " << nunks << std::endl;
	
	if (nunks <= 0)
	{
		char message[50];
		sprintf(message, _("Geoset animation alphas: Count error, %d geoset animation alphas.\n"), nunks);
		
		throw Exception(message);
	}
	
	fstream.read(reinterpret_cast<char*>(&this->m_lineType), sizeof(this->m_lineType));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_globalSequenceId), sizeof(this->m_globalSequenceId));
	bytes += fstream.gcount();
	
	for ( ; nunks > 0; --nunks)
	{
		class GeosetAnimationAlpha *geosetAnimationAlpha = new GeosetAnimationAlpha(this);
		bytes += geosetAnimationAlpha->readMdx(fstream);
		this->m_geosetAnimationAlphas.push_back(geosetAnimationAlpha);
	}
	
	return bytes;
}

long32 GeosetAnimationAlphas::writeMdx(std::fstream &fstream) throw (class Exception)
{
	if (!this->exists())
		return 0;
	
	long32 bytes = MdxBlock::writeMdx(fstream);
	
	return bytes;
}

}

}
