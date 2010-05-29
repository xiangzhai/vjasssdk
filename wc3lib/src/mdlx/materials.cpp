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

#include "materials.hpp"
#include "material.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Materials::Materials(class Mdlx *mdlx) : MdxBlock("MTLS"), m_mdlx(mdlx)
{
}

Materials::~Materials()
{
}

void Materials::readMdl(std::istream &istream) throw (class Exception)
{
}

void Materials::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize Materials::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;

	long32 nbytes = 0; //nbytes
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();
	
	while (nbytes > 0)
	{
		class Material *material = new Material(this);
		long32 readBytes = material->readMdx(istream);
		
		if (readBytes == 0)
			throw Exception(_("Materials: 0 byte material"));
		
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_materials.push_back(material);
	}
	
	return bytes;
}

std::streamsize Materials::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	
	return bytes;
}

}

}
