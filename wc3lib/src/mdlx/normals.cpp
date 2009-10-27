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

#include "normals.hpp"
#include "normal.hpp"

namespace wc3lib
{

namespace mdlx
{

Normals::Normals(class Geoset *geoset) : MdxBlock("NRMS"), m_geoset(geoset)
{
}

Normals::~Normals()
{
	for (std::list<class Normal*>::iterator iterator = this->m_normals.begin(); iterator != this->m_normals.end(); ++iterator)
		delete *iterator;
}

void Normals::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Normals::writeMdl(std::fstream &fstream) throw (class Exception)
{
}


long32 Normals::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nnrms = 0;
	fstream.read(reinterpret_cast<char*>(&nnrms), sizeof(nnrms));
	bytes += fstream.gcount();
	
	for ( ; nnrms > 0; --nnrms)
	{
		class Normal *normal = new Normal(this);
		bytes += normal->readMdx(fstream);
		this->m_normals.push_back(normal);
	}
	
	return bytes;
}

long32 Normals::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(fstream);
	
	return bytes;
}

}

}
