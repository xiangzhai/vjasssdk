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

#include <boost/format.hpp>
#include <boost/foreach.hpp>

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
	BOOST_FOREACH(class Geoset *geoset, this->m_geosets)
		delete geoset;
}

void Geosets::readMdl(std::istream &istream) throw (class Exception)
{
}

void Geosets::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize Geosets::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes = 0; //nbytes
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	
	if (nbytes <= 0)
		throw Exception(_("Geosets: 0 byte geosets.\n"));
	
	bytes += istream.gcount();
	
	while (nbytes > 0)
	{
		class Geoset *geoset = new Geoset(this);
		long32 readBytes = geoset->readMdx(istream); 	
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_geosets.push_back(geoset);
	}
	
	return bytes;
}

std::streamsize Geosets::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
