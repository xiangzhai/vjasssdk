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

#include "layer.hpp"
#include "layers.hpp"
#include "materialalphas.hpp"
#include "textureids.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Layer::Layer(class Layers *layers) : m_layers(layers), m_alphas(new MaterialAlphas(this)), m_textureIds(new TextureIds(this))
{
}

Layer::~Layer()
{
	delete this->m_alphas;
	delete this->m_textureIds;
}

void Layer::readMdl(std::istream &istream) throw (class Exception)
{
}

void Layer::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 Layer::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = 0;
	long32 nbytesi = 0;
	istream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	std::cout << "nbytesi is " << nbytesi << std::endl;
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_filterMode), sizeof(this->m_filterMode)); //(0:none;1:transparent;2:blend;3:additive;4:addalpha;5:modulate)
	bytes += istream.gcount();
	std::cout << "filterMode is " << this->m_filterMode << std::endl;
	
	if (this->m_filterMode < 0 || this->m_filterMode > 5)
		std::cerr << boost::format(_("Layer: Warning, unknown filter mode.\nFilter mode %1%.")) % this->m_filterMode << std::endl;
	
	istream.read(reinterpret_cast<char*>(&this->m_shading), sizeof(this->m_shading)); //+1:unshaded;+2:SphereEnvMap;+16:twosided;
	bytes += istream.gcount();
	
	if (this->m_shading != 1 && this->m_shading != 2 && this->m_shading != 16)
		std::cerr << boost::format(_("Layer: Warning, unknown shading.\nShading %1%.")) % this->m_shading << std::endl;
	
	istream.read(reinterpret_cast<char*>(&this->m_textureId), sizeof(this->m_textureId)); //  +32:unfogged;+64:NoDepthTest;+128:NoDepthSet)
	bytes += istream.gcount();
	
	if (this->m_textureId != 32 && this->m_textureId != 64 && this->m_textureId != 128)
		std::cerr << boost::format(_("Layer: Warning, unknown texture id.\nTexture id %1%.")) % this->m_textureId << std::endl;
	
	istream.read(reinterpret_cast<char*>(&this->m_tvertexAnimationId), sizeof(this->m_tvertexAnimationId)); // 0xFFFFFFFF if none
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_coordinatesId), sizeof(this->m_coordinatesId));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_alpha), sizeof(this->m_alpha));
	bytes += istream.gcount();
	bytes += this->m_alphas->readMdx(istream);
	std::cout << "Before texture ids with " << bytes << " bytes." << std::endl;
	bytes += this->m_textureIds->readMdx(istream);
	std::cout << "After texture ids with " << bytes << " bytes." << std::endl;
	
	if (nbytesi != bytes)
		std::cerr << boost::format(_("Layer: Real byte count is not equal to file byte count.\nReal byte count %1%.\nFile byte count %2%.")) % bytes % nbytesi << std::endl;
	
	return bytes;
}

long32 Layer::writeMdx(std::ostream &ostream) throw (class Exception)
{
	return 0;
}

}

}
