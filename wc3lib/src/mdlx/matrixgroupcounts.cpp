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

#include <boost/foreach.hpp>

#include "matrixgroupcounts.hpp"
#include "matrixgroupcount.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

MatrixGroupCounts::MatrixGroupCounts(class Geoset *geoset) : MdxBlock("MTGC"), m_geoset(geoset)
{
}

MatrixGroupCounts::~MatrixGroupCounts()
{
	BOOST_FOREACH(class MatrixGroupCount *matrixGroupCount, this->m_matrixGroupCounts)
		delete matrixGroupCount;
}

void MatrixGroupCounts::readMdl(std::istream &istream) throw (class Exception)
{
}

void MatrixGroupCounts::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}


std::streamsize MatrixGroupCounts::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nmtrcs = 0;
	read(istream, nmtrcs, bytes);
	//istream.read(reinterpret_cast<char*>(&nmtrcs), sizeof(nmtrcs));
	//bytes += istream.gcount();
	
	for ( ; nmtrcs > 0; --nmtrcs)
	{
		class MatrixGroupCount *matrixGroupCount = new MatrixGroupCount(this);
		bytes += matrixGroupCount->readMdx(istream);
		this->m_matrixGroupCounts.push_back(matrixGroupCount);
	}
	
	return bytes;
}

std::streamsize MatrixGroupCounts::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	if (!this->exists())
		return 0;
	
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	
	return bytes;
}

}

}
