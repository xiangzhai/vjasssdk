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

#include "texturevertices.hpp"
#include "texturevertex.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

TextureVertices::TextureVertices(class Geoset *geoset) : MdxBlock("UVBS"), m_geoset(geoset)
{
}

TextureVertices::~TextureVertices()
{
	BOOST_FOREACH(class TextureVertex *textureVertex, this->m_textureVertices)
		delete textureVertex;
}

void TextureVertices::readMdl(std::istream &istream) throw (class Exception)
{
}

void TextureVertices::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize TextureVertices::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nvrts = 0;
	istream.read(reinterpret_cast<char*>(&nvrts), sizeof(nvrts));
	
	if (nvrts <= 0)
		throw Exception(_("Texture Vertices: 0 byte texture vertices."));
	
	bytes += istream.gcount();
	
	for ( ; nvrts > 0; --nvrts)
	{
		class TextureVertex *textureVertex = new TextureVertex(this);
		bytes += textureVertex->readMdx(istream);
		this->m_textureVertices.push_back(textureVertex);
	}
	
	return bytes;
}

std::streamsize TextureVertices::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	return bytes;
}

}

}
