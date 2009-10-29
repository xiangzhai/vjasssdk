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

#include "geosetanimations.hpp"
#include "geosetanimation.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

GeosetAnimations::GeosetAnimations(class Mdlx *mdlx) : MdxBlock("GEOA"), m_mdlx(mdlx)
{
}

GeosetAnimations::~GeosetAnimations()
{
	for (std::list<class GeosetAnimation*>::iterator iterator = this->m_geosetAnimations.begin(); iterator != this->m_geosetAnimations.end(); ++iterator)
		delete *iterator;
}

void GeosetAnimations::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void GeosetAnimations::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 GeosetAnimations::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes = 0;
	fstream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += fstream.gcount();
	
	if (nbytes <= 0)
	{
		char message[50];
		sprintf(message, _("Geoset animations: Byte count error, %d bytes.\n"), nbytes);
		
		throw Exception(message);
	}
	
	while (nbytes > 0)
	{
		class GeosetAnimation *geosetAnimation = new GeosetAnimation(this);
		long32 readBytes = geosetAnimation->readMdx(fstream);
		bytes += readBytes;
		nbytes -= readBytes;
		this->m_geosetAnimations.push_back(geosetAnimation);
	}
	
	std::cout << "After all geoset animations with count " << this->m_geosetAnimations.size() << std::endl;
	
	return bytes;
}

long32 GeosetAnimations::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
