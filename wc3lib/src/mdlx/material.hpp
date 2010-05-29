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

#ifndef WC3LIB_MDLX_MATERIAL_HPP
#define WC3LIB_MDLX_MATERIAL_HPP

#include <istream>
#include <ostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Materials;
class Layers;

class Material
{
	public:
		enum RenderMode
		{
			ConstantColor = 1,
			SortPrimsFarZ = 0x16,
			FullResolution = 0x32
		};

		Material(class Materials *materials);
		virtual ~Material();

		class Materials* materials() const;
		float32 priorityPlane() const;
		float32 renderMode() const;
		class Layers* layers() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class Materials *m_materials;
		//long nbytesi;
		long32 m_priorityPlane;
		long32 m_renderMode; //(+1:ConstantColor;+16:SortPrimsFarZ;+32:FullResolution)
		class Layers *m_layers;
};

inline class Materials* Material::materials() const
{
	return this->m_materials;
}

inline float32 Material::priorityPlane() const
{
	return this->m_priorityPlane;
}

inline float32 Material::renderMode() const
{
	return this->m_renderMode;
}

inline class Layers* Material::layers() const
{
	return this->m_layers;
}

}

}

#endif
