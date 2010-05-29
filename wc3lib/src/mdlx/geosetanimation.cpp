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

#include "geosetanimation.hpp"
#include "geosetanimationalphas.hpp"
#include "geosetanimationcolors.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

GeosetAnimation::GeosetAnimation(class GeosetAnimations *geosetAnimations) : m_geosetAnimations(geosetAnimations), m_staticAlpha(0.0), m_colorAnimation(GeosetAnimation::None), m_colorRed(0.0), m_colorGreen(0.0), m_colorBlue(0.0), m_geosetId(0),  m_alphas(new GeosetAnimationAlphas(this)),  m_colors(new GeosetAnimationColors(this))
{
}

GeosetAnimation::~GeosetAnimation()
{
	delete this->m_alphas;
	delete this->m_colors;
}

void GeosetAnimation::readMdl(std::istream &istream) throw (class Exception)
{
}

void GeosetAnimation::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize GeosetAnimation::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytesi;
	istream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	std::streamsize bytes = istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_staticAlpha), sizeof(this->m_staticAlpha));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_colorAnimation), sizeof(this->m_colorAnimation));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_colorRed), sizeof(this->m_colorRed));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_colorGreen), sizeof(this->m_colorGreen));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_colorBlue), sizeof(this->m_colorBlue));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_geosetId), sizeof(this->m_geosetId));
	bytes += istream.gcount();
	
	std::cout << "Static alpha is " << this->m_staticAlpha << std::endl;
	
	if (this->m_staticAlpha == 1.0)
	{
		bytes += this->m_alphas->readMdx(istream);
	}
	
	bytes += this->m_colors->readMdx(istream); /// @todo Seems to be optional, file Krieger.mdx doesn't have this block.
	std::cout << "After colors" << std::endl;
	
	if (nbytesi != bytes)
		throw Exception(boost::str(boost::format(_("Geoset animation: File byte count isn't equal to real byte count:\nFile byte count %1%.\nReal byte count %2%.\n")) % nbytesi % bytes));
	
	return bytes;
}

std::streamsize GeosetAnimation::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
