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

#include "pivotpoints.hpp"
#include "pivotpoint.hpp"

namespace wc3lib
{

namespace mdlx
{

PivotPoints::PivotPoints(class Mdlx *mdlx) : MdxBlock("PIVT"), m_mdlx(mdlx)
{
}

PivotPoints::~PivotPoints()
{
}

void PivotPoints::readMdl(std::istream &istream) throw (class Exception)
{
}

void PivotPoints::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize PivotPoints::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	std::streamsize nbytes = 0;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();
	
	while (nbytes > 0)
	{
		class PivotPoint *pivotPoint = new PivotPoint(this);
		long32 readBytes = pivotPoint->readMdx(istream);
		bytes += readBytes;
		nbytes -= readBytes;
		this->m_pivotPoints.push_back(pivotPoint);
	}
	
	return bytes;
}

std::streamsize PivotPoints::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	if (!this->exists())
		return 0;
	
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	
	return bytes;
}

}

}
