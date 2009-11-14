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

#ifndef WC3LIB_MDLX_GEOSET_HPP
#define WC3LIB_MDLX_GEOSET_HPP

#include <list>
#include <iostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Geosets;
class Vertices;
class Normals;
class PrimitiveTypes;
class PrimitiveSizes;
class PrimitiveVertices;
class GroupVertices;
class MatrixGroupCounts;
class Matrices;
class Ganimation;
class TexturePatches;
class TextureVertices;

class Geoset
{
	public:
		enum Selectable
		{
			None = 0,
			Unselectable = 4
		};

		Geoset(class Geosets *geosets);
		virtual ~Geoset();

		class Geosets* geosets() const;
		class Vertices* vertices() const;
		class Normals* normals() const;
		class PrimitiveTypes* primitiveTypes() const;
		class PrimitiveSizes* primitiveSizes() const;
		class PrimitiveVertices* primitiveVertices() const;
		class GroupVertices* groupVertices() const;
		class MatrixGroupCounts* matrixGroupCounts() const;
		class Matrices* matrices() const;
		long32 materialId() const;
		long32 selectionGroup() const;
		long32 selectable() const;
		float32 boundsRadius() const;
		float32 minExtentX() const;
		float32 minExtentY() const;
		float32 minExtentZ() const;
		float32 maxExtentX() const;
		float32 maxExtentY() const;
		float32 maxExtentZ() const;
		std::list<class Ganimation*> ganimations() const;
		class TexturePatches* texturePatches() const;
		class TextureVertices* textureVertices() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

	protected:
		class Geosets *m_geosets;
		class Vertices *m_vertices; //VRTX
		class Normals *m_normals; //NRMS
		class PrimitiveTypes *m_primitveTypes; //PTYP
		class PrimitiveSizes *m_primitiveSizes; //PCNT
		class PrimitiveVertices *m_primitiveVertices; //PVTX
		class GroupVertices *m_groupVertices; //GNDX
		class MatrixGroupCounts *m_matrixGroupCounts; //MTGC
		class Matrices *m_matrices; //MATS
		long32 m_materialId;
		long32 m_selectionGroup;
		long32 m_selectable; //(0:none;4:Unselectable)
		float32 m_boundsRadius;
		float32 m_minExtentX, m_minExtentY, m_minExtentZ;
		float32 m_maxExtentX, m_maxExtentY, m_maxExtentZ;
		//long32 nanim;
		std::list<class Ganimation*> m_ganimations;
		//struct GAnimation *ganimations; //[nganim], naim?!?! Tamino Dauth
		class TexturePatches *m_texturePatches; //UVAS
		class TextureVertices *m_textureVertices; //UVBS
};

inline class Geosets* Geoset::geosets() const
{
	return this->m_geosets;
}

inline class Vertices* Geoset::vertices() const
{
	return this->m_vertices;
}

inline class Normals* Geoset::normals() const
{
	return this->m_normals;
}

inline class PrimitiveTypes* Geoset::primitiveTypes() const
{
	return this->m_primitveTypes;
}

inline class PrimitiveSizes* Geoset::primitiveSizes() const
{
	return this->m_primitiveSizes;
}

inline class PrimitiveVertices* Geoset::primitiveVertices() const
{
	return this->m_primitiveVertices;
}

inline class GroupVertices* Geoset::groupVertices() const
{
	return this->m_groupVertices;
}

inline class MatrixGroupCounts* Geoset::matrixGroupCounts() const
{
	return this->m_matrixGroupCounts;
}

inline class Matrices* Geoset::matrices() const
{
	return this->m_matrices;
}

inline long32 Geoset::materialId() const
{
	return this->m_materialId;
}

inline long32 Geoset::selectionGroup() const
{
	return this->m_selectionGroup;
}

inline long32 Geoset::selectable() const
{
	return this->m_selectable;
}

inline float32 Geoset::boundsRadius() const
{
	return this->m_boundsRadius;
}

inline float32 Geoset::minExtentX() const
{
	return this->m_minExtentX;
}

inline float32 Geoset::minExtentY() const
{
	return this->m_minExtentY;
}

inline float32 Geoset::minExtentZ() const
{
	return this->m_minExtentZ;
}

inline float32 Geoset::maxExtentX() const
{
	return this->m_maxExtentX;
}

inline float32 Geoset::maxExtentY() const
{
	return this->m_maxExtentY;
}

inline float32 Geoset::maxExtentZ() const
{
	return this->m_maxExtentZ;
}

inline std::list<class Ganimation*> Geoset::ganimations() const
{
	return this->m_ganimations;
}

inline class TexturePatches* Geoset::texturePatches() const
{
	return this->m_texturePatches;
}

inline class TextureVertices* Geoset::textureVertices() const
{
	return this->m_textureVertices;
}

}

}

#endif
