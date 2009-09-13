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

		virtual void readMdl(std::fstream &fstream) throw (Exception);
		virtual void readMdx(std::fstream &fstream) throw (Exception);
		virtual void writeMdl(std::fstream &fstream) throw (Exception);
		virtual void writeMdx(std::fstream &fstream) throw (Exception);

	protected:
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

}

}

#endif
