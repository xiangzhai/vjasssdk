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

#include "vertices.hpp"

namespace wc3lib
{

namespace mdlx
{

Vertices::Vertices(class Geoset *geoset) : MdxBlock("VRTX"), m_geoset(geoset)
{
}

Vertices::~Vertices()
{
}

void Vertices::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Vertices::readMdx(std::fstream &fstream) throw (class Exception)
{
}

void Vertices::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

void Vertices::writeMdx(std::fstream &fstream) throw (class Exception)
{
}

}

}
