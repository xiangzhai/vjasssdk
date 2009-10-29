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

#include "pivotpoint.hpp"

namespace wc3lib
{

namespace mdlx
{

PivotPoint::PivotPoint(class PivotPoints *pivotPoints) : m_pivotPoints(pivotPoints)
{
}

PivotPoint::~PivotPoint()
{
}

void PivotPoint::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void PivotPoint::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 PivotPoint::readMdx(std::fstream &fstream) throw (class Exception)
{
	fstream.read(reinterpret_cast<char*>(&this->m_x), sizeof(this->m_x));
	long32 bytes = fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_y), sizeof(this->m_y));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_z), sizeof(this->m_z));
	bytes += fstream.gcount();
	
	return bytes;
}

long32 PivotPoint::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
