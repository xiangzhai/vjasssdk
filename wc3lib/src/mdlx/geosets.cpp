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

#include <iostream> //debug

#include "geosets.hpp"
#include "geoset.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Geosets::Geosets(class Mdlx *mdlx) : MdxBlock("GEOS"), m_mdlx(mdlx)
{
}

Geosets::~Geosets()
{
	for (std::list<class Geoset*>::iterator iterator = this->m_geosets.begin(); iterator != this->m_geosets.end(); ++iterator)
		delete *iterator;
}

void Geosets::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Geosets::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Geosets::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
	{
		std::cout << "No geosets." << std::endl;
		
		return 0;
	}
	
	long32 nbytes = 0; //nbytes
	fstream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	
	if (nbytes <= 0)
	{
		char message[50];
		sprintf(message, _("Geosets: 0 byte geosets.\n"));
		
		throw Exception(message);
	}
	
	std::cout << "Read " << nbytes << " geoset bytes." << std::endl;
	
	bytes += fstream.gcount();
	
	while (nbytes > 0)
	{
		class Geoset *geoset = new Geoset(this);
		long32 readBytes = geoset->readMdx(fstream); 
		std::cout << "Read geoset with " << readBytes << " bytes." << std::endl;
		
		nbytes -= readBytes;
		std::cout << nbytes  << " bytes left." << std::endl;
		bytes += readBytes;
		this->m_geosets.push_back(geoset);
	}
	
	std::cout << "Canceled" << std::endl;
	
	return bytes;
}

long32 Geosets::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
