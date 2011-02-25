/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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
#include <boost/cast.hpp>

#include "geosetanimations.hpp"
#include "geosetanimation.hpp"
#include "../utilities.hpp"
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
	BOOST_FOREACH(class GeosetAnimation *geosetAnimation, this->m_geosetAnimations)
		delete geosetAnimation;
}

std::streamsize GeosetAnimations::readMdl(istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize GeosetAnimations::writeMdl(ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize GeosetAnimations::readMdx(istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);
	
	if (size == 0)
		return 0;
	
	long32 nbytes = 0;
	wc3lib::read(istream, nbytes, size);
	
	if (nbytes <= 0)
		throw Exception(boost::str(boost::format( _("Geoset animations: Byte count error, %1% bytes.\n")) % nbytes));
	
	while (nbytes > 0)
	{
		class GeosetAnimation *geosetAnimation = new GeosetAnimation(this);
		std::streamsize readBytes = geosetAnimation->readMdx(istream);
		size += readBytes;
		nbytes -= boost::numeric_cast<long32>(readBytes);
		this->m_geosetAnimations.push_back(geosetAnimation);
	}
	
	std::cout << "After all geoset animations with count " << this->m_geosetAnimations.size() << std::endl;
	
	return size;
}

std::streamsize GeosetAnimations::writeMdx(ostream &ostream) const throw (class Exception)
{
	std::streamsize size = MdxBlock::writeMdx(ostream);
	
	if (size == 0)
		return 0;
	
	std::streampos position;
	skipByteCount<long32>(ostream, position);
	
	BOOST_FOREACH(const class GeosetAnimation *geosetAnimation, geosetAnimations())
		size += geosetAnimation->writeMdx(ostream);
	
	writeByteCount(ostream, static_cast<long32>(size), position, size);
	
	return size;
}

}

}
