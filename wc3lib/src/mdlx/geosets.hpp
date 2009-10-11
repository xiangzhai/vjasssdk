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

#ifndef WC3LIB_MDLX_GEOSETS_HPP
#define WC3LIB_MDLX_GEOSETS_HPP

#include <list>

#include "mdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;

class Geosets : public MdxBlock
{
	public:
		Geosets(class Mdlx *mdlx);
		virtual ~Geosets();
		
		std::list<class Geoset*> geosets() const;
		class Vertices* vertices() const;
		class Normals* normals() const;
		class PrimitiveTypes* primitiveTypes() const;
		class PrimitiveSizes* primitiveSizes() const;
		class PrimitiveVertices* primitiveVertices() const;
		class GroupVertices* groupVertices() const;
		class MaterialGroupLengths* materialGroupLengths() const;
		class Matrices* matrices() const;
		class TexturePatches* texturePatches() const;
		class TextureVertices* textureVertices() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual long32 readMdx(std::fstream &fstream) throw (class Exception);
		virtual long32 writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		//long	nbytes;
		std::list<class Geoset*> m_geosets;
		class Vertices *m_vertices; //VRTX
		class Normals *m_normals;
		class PrimitiveTypes *m_primitiveTypes; //PTYP
		class PrimitiveSizes *m_primitiveSizes; //PCNT
		class PrimitiveVertices *m_primitiveVertices; //PVTX
		class GroupVertices *m_groupVertices; //GNDX
		class MaterialGroupLengths *m_materialGroupLengths; //MTGC
		class Matrices *m_matrices; //MATS
		/*
		--(BIDX) ???
		--(BWGT) ???
		*/
		class TexturePatches *m_texturePatches; //UVAS, texture patches?
		class TextureVertices *m_textureVertices; //UVBS
};

inline std::list<class Geoset*> Geosets::geosets() const
{
	return this->m_geosets;
}

inline class Vertices* Geosets::vertices() const
{
	return this->m_vertices;
}

inline class Normals* Geosets::normals() const
{
	return this->m_normals;
}

inline class PrimitiveTypes* Geosets::primitiveTypes() const
{
	return this->m_primitiveTypes;
}

inline class PrimitiveSizes* Geosets::primitiveSizes() const
{
	return this->m_primitiveSizes;
}

inline class PrimitiveVertices* Geosets::primitiveVertices() const
{
	return this->m_primitiveVertices;
}

inline class GroupVertices* Geosets::groupVertices() const
{
	return this->m_groupVertices;
}

inline class MaterialGroupLengths* Geosets::materialGroupLengths() const
{
	return this->m_materialGroupLengths;
}

inline class Matrices* Geosets::matrices() const
{
	return this->m_matrices;
}

inline class TexturePatches* Geosets::texturePatches() const
{
	return this->m_texturePatches;
}

inline class TextureVertices* Geosets::textureVertices() const
{
	return this->m_textureVertices;
}

}

}

#endif
