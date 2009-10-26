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

#include <iostream> //debug

#include "texturepatches.hpp"
#include "texturepatch.hpp"
#include "geoset.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

TexturePatches::TexturePatches(class Geoset *geoset) : MdxBlock("UVAS"), m_geoset(geoset)
{
}

TexturePatches::~TexturePatches()
{
	for (std::list<class TexturePatch*>::iterator iterator = this->m_texturePatches.begin(); iterator != this->m_texturePatches.end(); ++iterator)
		delete *iterator;
}

void TexturePatches::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void TexturePatches::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 TexturePatches::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 ntvrts = 0;
	fstream.read(reinterpret_cast<char*>(&ntvrts), sizeof(ntvrts));
	
	if (ntvrts <= 0)
	{
		char message[50];
		sprintf(message, _("Texture Patches: 0 byte texture patches.\n"));
		
		throw Exception(message);
	}
	
	bytes += fstream.gcount();
	
	for ( ; ntvrts > 0; --ntvrts)
	{
		class TexturePatch *texturePatch = new TexturePatch(this);
		bytes += texturePatch->readMdx(fstream);
		this->m_texturePatches.push_back(texturePatch);
	}
	
	return bytes;
}

long32 TexturePatches::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(fstream);
	
	return bytes;
}

}

}
