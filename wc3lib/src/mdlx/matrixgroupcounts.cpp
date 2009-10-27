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

#include "matrixgroupcounts.hpp"
#include "matrixgroupcount.hpp"

namespace wc3lib
{

namespace mdlx
{

MatrixGroupCounts::MatrixGroupCounts(class Geoset *geoset) : MdxBlock("MTGC"), m_geoset(geoset)
{
}

MatrixGroupCounts::~MatrixGroupCounts()
{
	for (std::list<class MatrixGroupCount*>::iterator iterator = this->m_matrixGroupCounts.begin(); iterator != this->m_matrixGroupCounts.end(); ++iterator)
		delete *iterator;
}

void MatrixGroupCounts::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void MatrixGroupCounts::writeMdl(std::fstream &fstream) throw (class Exception)
{
}


long32 MatrixGroupCounts::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nmtrcs = 0;
	fstream.read(reinterpret_cast<char*>(&nmtrcs), sizeof(nmtrcs));
	bytes += fstream.gcount();
	
	for ( ; nmtrcs > 0; --nmtrcs)
	{
		class MatrixGroupCount *matrixGroupCount = new MatrixGroupCount(this);
		bytes += matrixGroupCount->readMdx(fstream);
		this->m_matrixGroupCounts.push_back(matrixGroupCount);
	}
	
	return bytes;
}

long32 MatrixGroupCounts::writeMdx(std::fstream &fstream) throw (class Exception)
{
	if (!this->exists())
		return 0;
	
	long32 bytes = MdxBlock::writeMdx(fstream);
	
	return bytes;
}

}

}
