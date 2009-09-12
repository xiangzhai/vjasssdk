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

#include "geoset.hpp"

namespace wc3lib
{

namespace mdlx
{

Geoset::Geoset()
{
}

Geoset::Geoset(class Mdlx *mdlx) : m_mdlx(mdlx), m_vertices(0), m_normals(0),  m_primitveTypes(0), m_primitiveSizes(0),  m_faces(0),  m_vertexGroup(0), m_matrixGroupCounts(0),  m_matrices(0), m_materialId(0),  m_selectionGroup(0),  m_selectable(Geoset::None), m_boundsRadius(0.0), m_minExtentX(0.0), m_minExtentY(0.0), m_minExtentZ(0.0), m_maxExtentX(0.0), m_maxExtentY(0.0), m_maxExtentZ(0.0), m_texturePatches(0),  m_tvertices(0)
{
}

Geoset::~Geoset()
{
	if (this->m_vertices != 0)
		delete this->m_vertices;

	if (this->m_normals != 0)
		delete this->m_normals;

	if (this->m_primitveTypes != 0)
		delete this->m_primitveTypes;

	if (this->m_primitiveSizes != 0)
		delete this->m_primitiveSizes;

	if (this->m_faces != 0)
		delete this->m_faces;

	if (this->m_vertexGroup != 0)
		delete this->m_vertexGroup;

	if (this->m_matrixGroupCounts != 0)
		delete this->m_matrixGroupCounts;

	if (this->m_matrices != 0)
		delete this->m_matrices;
}

void Geoset::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Geoset::readMdx(std::fstream &fstream) throw (class Exception)
{
}

void Geoset::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

void Geoset::writeMdx(std::fstream &fstream) throw (class Exception)
{
}

}

}
