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

#include <iostream>

#include <boost/format.hpp>

#include "texture.hpp"
#include "textures.hpp"
#include "../internationalisation.hpp"
#include "../utilities.hpp"

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

void Texture::readMdl(std::istream &istream) throw (class Exception)
{
}

void Texture::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize Texture::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read(istream, *reinterpret_cast<long32*>(&this->m_replaceableId), size);
	wc3lib::read(istream, this->m_texturePath, size);
	wc3lib::read(istream, this->m_unknown0, size);
	wc3lib::read(istream, *reinterpret_cast<long32*>(&this->m_wrapping), size);

	return size;
}

std::streamsize Texture::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::write(ostream, *reinterpret_cast<const long32*>(&this->m_replaceableId), size);
	wc3lib::write(ostream, this->m_texturePath, size);
	wc3lib::write(ostream, this->m_unknown0, size);
	wc3lib::write(ostream, *reinterpret_cast<const long32*>(&this->m_wrapping), size);

	return size;
}

}

}
