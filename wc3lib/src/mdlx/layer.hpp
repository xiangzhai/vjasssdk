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

#ifndef WC3LIB_MDLX_LAYER_HPP
#define WC3LIB_MDLX_LAYER_HPP

#include <iostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Layers;
class MaterialAlphas;
class TextureIds;

class Layer
{
	public:
		Layer(class Layers *layers);
		virtual ~Layer();

		class Layers* layers() const;
		long32 filterMode() const;
		long32 shading() const;
		long32 textureId() const;
		long32 tvertexAnimationId() const;
		long32 coordinatesId() const;
		float32	alpha() const;
		class MaterialAlphas* alphas() const;
		class TextureIds* textureIds() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

	protected:
		class Layers *m_layers;
		long32	m_filterMode; //(0:none;1:transparent;2:blend;3:additive;4:addalpha;5:modulate)
		long32	m_shading; //+1:unshaded;+2:SphereEnvMap;+16:twosided;
		long32	m_textureId; //  +32:unfogged;+64:NoDepthTest;+128:NoDepthSet)
		long32 	m_tvertexAnimationId; // 0xFFFFFFFF if none
		long32	m_coordinatesId;
		float32	m_alpha; //(0(transparent)->1(opaque))
		class MaterialAlphas *m_alphas; //(KMTA)
		class TextureIds *m_textureIds; //(KMTF) // state is long not float
};

inline class Layers* Layer::layers() const
{
	return this->m_layers;
}

inline long32 Layer::filterMode() const
{
	return this->m_filterMode;
}

inline long32 Layer::shading() const
{
	return this->m_shading;
}

inline long32 Layer::textureId() const
{
	return this->m_textureId;
}

inline long32 Layer::tvertexAnimationId() const
{
	return this->m_tvertexAnimationId;
}

inline long32 Layer::coordinatesId() const
{
	return this->m_coordinatesId;
}

inline float32 Layer::alpha() const
{
	return this->m_alpha;
}

inline class MaterialAlphas* Layer::alphas() const
{
	return this->m_alphas;
}

inline class TextureIds* Layer::textureIds() const
{
	return this->m_textureIds;
}

}

}

#endif
