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

#include "textureanimations.hpp"
#include "textureanimation.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

TextureAnimations::TextureAnimations(class Mdlx *mdlx) : MdxBlock("TXAN"), m_mdlx(mdlx)
{
}

TextureAnimations::~TextureAnimations()
{
	for (std::list<class TextureAnimation*>::iterator iterator = this->m_textureAnimations.begin(); iterator != this->m_textureAnimations.end(); ++iterator)
		delete *iterator;
}

void TextureAnimations::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void TextureAnimations::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 TextureAnimations::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes = 0;
	fstream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += fstream.gcount();
	std::cout << "Texture animation bytes: " << nbytes << std::endl;
	
	while (nbytes > 0)
	{
		class TextureAnimation *textureAnimation = new TextureAnimation(this);
		long32 readBytes = textureAnimation->readMdx(fstream);
		
		if (readBytes == 0)
			throw Exception(_("Texture animations: 0 byte texture animation."));
		
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_textureAnimations.push_back(textureAnimation);
	}
	
	return bytes;
}

long32 TextureAnimations::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(fstream);
	
	return bytes;
}

}

}
