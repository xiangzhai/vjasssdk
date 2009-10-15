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

#include "texture.hpp"
#include "textures.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Texture::Texture(class Textures *textures) : m_textures(textures)
{
}

Texture::~Texture()
{
}

void Texture::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Texture::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Texture::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = 0;
	fstream.read(reinterpret_cast<char*>(&this->m_replaceableId), sizeof(this->m_replaceableId));
	bytes += fstream.gcount();
	fstream.read(this->m_texturePath, sizeof(this->m_texturePath));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_unknown0), sizeof(this->m_unknown0));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_wrapping), sizeof(this->m_wrapping));
	bytes += fstream.gcount();
	
	if (this->m_wrapping != 1 && this->m_wrapping != 2 && this->m_wrapping != 3)
		fprintf(stderr, _("Texture: Warning, unknown wrapping %d.\n"), this->m_wrapping);
	
	return bytes;
}

long32 Texture::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = 0;
	
	return bytes;
}

}

}
