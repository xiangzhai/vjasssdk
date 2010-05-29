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

#include "matrices.hpp"
#include "matrix.hpp"

namespace wc3lib
{

namespace mdlx
{

Matrices::Matrices(class Geoset *geoset) : MdxBlock("MATS"), m_geoset(geoset)
{
}

Matrices::~Matrices()
{
	BOOST_FOREACH(class Matrix *matrix, this->m_matrices)
		delete matrix;
}

void Matrices::readMdl(std::istream &istream) throw (class Exception)
{
}

void Matrices::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize Matrices::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nmtrcs = 0;
	istream.read(reinterpret_cast<char*>(&nmtrcs), sizeof(nmtrcs));
	bytes += istream.gcount();
	
	for ( ; nmtrcs > 0; --nmtrcs)
	{
		class Matrix *matrix = new Matrix(this);
		bytes += matrix->readMdx(istream);
		this->m_matrices.push_back(matrix);
	}
	
	return bytes;
}

std::streamsize Matrices::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	
	return bytes;
}

}

}
