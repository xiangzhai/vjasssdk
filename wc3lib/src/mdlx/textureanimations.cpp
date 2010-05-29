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

#include <boost/foreach.hpp>

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
	BOOST_FOREACH(class TextureAnimation *textureAnimation, this->m_textureAnimations)
		delete textureAnimation;
}

void TextureAnimations::readMdl(std::istream &istream) throw (class Exception)
{
}

void TextureAnimations::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize TextureAnimations::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
	{
		std::cout << "No texture animations." << std::endl;
		
		return 0;
	}
	
	long32 nbytes = 0;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();
	std::cout << "Texture animation bytes: " << nbytes << std::endl;
	
	while (nbytes > 0)
	{
		class TextureAnimation *textureAnimation = new TextureAnimation(this);
		long32 readBytes = textureAnimation->readMdx(istream);
		
		if (readBytes == 0)
			throw Exception(_("Texture animations: 0 byte texture animation."));
		
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_textureAnimations.push_back(textureAnimation);
	}
	
	return bytes;
}

std::streamsize TextureAnimations::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	return bytes;
}

}

}
