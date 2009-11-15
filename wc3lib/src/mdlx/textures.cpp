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

#include "textures.hpp"
#include "texture.hpp"
#include "../internationalisation.hpp"

#include <iostream> //debug

namespace wc3lib
{

namespace mdlx
{

Textures::Textures(class Mdlx *mdlx) : MdxBlock("TEXS"), m_mdlx(mdlx)
{
}

Textures::~Textures()
{
	for (std::list<class Texture*>::iterator iterator = this->m_textures.begin(); iterator != this->m_textures.end(); ++iterator)
		delete *iterator;
}

void Textures::readMdl(std::istream &istream) throw (class Exception)
{
}

void Textures::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 Textures::readMdx(std::istream &istream) throw (class Exception)
{
	std::cout << "TEXTURES" << std::endl;
	long32 bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32	nbytes = 0;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();
	std::cout << "Texture bytes: " << nbytes << std::endl;
	
	while (nbytes > 0)
	{
		class Texture *texture = new Texture(this);
		long32 readBytes = texture->readMdx(istream);
		std::cout << "Read texture with " << readBytes << " bytes." << std::endl;
		
		if (readBytes == 0)
			throw Exception(_("Textures: 0 byte texture."));
		
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_textures.push_back(texture);
	}
	
	return bytes;
}

long32 Textures::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	return bytes;
}

}

}
