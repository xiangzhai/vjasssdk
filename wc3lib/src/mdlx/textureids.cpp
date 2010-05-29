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

#include "textureids.hpp"
#include "textureid.hpp"

namespace wc3lib
{

namespace mdlx
{

TextureIds::TextureIds(class Layer *layer) : MdxBlock("KMTF"), m_layer(layer)
{
}

TextureIds::~TextureIds()
{
}

void TextureIds::readMdl(std::istream &istream) throw (class Exception)
{
}

void TextureIds::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize TextureIds::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nunks;
	istream.read(reinterpret_cast<char*>(&nunks), sizeof(nunks));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_lineType), sizeof(this->m_lineType));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_globalSequenceId), sizeof(this->m_globalSequenceId));
	bytes += istream.gcount();
	
	for ( ; nunks > 0; --nunks)
	{
		class TextureId *textureId = new TextureId(this);
		bytes += textureId->readMdx(istream);
		this->m_textureIds.push_back(textureId);
	}
	
	return bytes;
}

std::streamsize TextureIds::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	
	return bytes;
}

}

}
