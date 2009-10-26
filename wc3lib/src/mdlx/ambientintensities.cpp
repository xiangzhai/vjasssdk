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

#include "ambientintensities.hpp"

namespace wc3lib
{

namespace mdlx
{

AmbientIntensities::AmbientIntensities(class Light *light) : MdxBlock("KLBI"), m_light(light)
{
}

AmbientIntensities::~AmbientIntensities()
{
}

void AmbientIntensities::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void AmbientIntensities::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 AmbientIntensities::readMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

long32 AmbientIntensities::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
