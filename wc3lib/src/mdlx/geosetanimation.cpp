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

#include "geosetanimation.hpp"

namespace wc3lib
{

namespace mdlx
{

GeosetAnimation::GeosetAnimation(class GeosetAnimations *geosetAnimations) : m_geosetAnimations(geosetAnimations), m_staticAlpha(0.0), m_colorAnimation(GeosetAnimation::None), m_colorRed(0.0), m_colorGreen(0.0), m_colorBlue(0.0), m_geosetId(0),  m_alpha(0),  m_color(0)
{
}

GeosetAnimation::~GeosetAnimation()
{
}

void GeosetAnimation::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void GeosetAnimation::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 GeosetAnimation::readMdx(std::fstream &fstream) throw (class Exception)
{
	long	nbytesi;
}

long32 GeosetAnimation::writeMdx(std::fstream &fstream) throw (class Exception)
{
}

}

}
