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
#include "geosets.hpp"
#include "vertices.hpp"
#include "normals.hpp"
#include "primitivetypes.hpp"
#include "primitivesizes.hpp"
#include "primitivevertices.hpp"
#include "groupvertices.hpp"
#include "matrixgroupcounts.hpp"
#include "matrices.hpp"
#include "ganimation.hpp"
#include "texturepatches.hpp"
#include "texturevertices.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Geoset::Geoset(class Geosets *geosets) : m_geosets(geosets), m_vertices(new Vertices(this)), m_normals(new Normals(this)),  m_primitveTypes(new PrimitiveTypes(this)), m_primitiveSizes(new PrimitiveSizes(this)),  m_primitiveVertices(new PrimitiveVertices(this)),  m_groupVertices(new GroupVertices(this)), m_matrixGroupCounts(new MatrixGroupCounts(this)),  m_matrices(new Matrices(this)), m_materialId(0),  m_selectionGroup(0),  m_selectable(Geoset::None), m_boundsRadius(0.0), m_minExtentX(0.0), m_minExtentY(0.0), m_minExtentZ(0.0), m_maxExtentX(0.0), m_maxExtentY(0.0), m_maxExtentZ(0.0), m_texturePatches(new TexturePatches(this)), m_textureVertices(new TextureVertices(this))
{
}

Geoset::~Geoset()
{
	delete this->m_vertices;
	delete this->m_normals;
	delete this->m_primitveTypes;
	delete this->m_primitiveSizes;
	delete this->m_primitiveVertices;
	delete this->m_groupVertices;
	delete this->m_matrixGroupCounts;
	delete this->m_matrices;
	delete this->m_texturePatches;
	delete this->m_textureVertices;
}

void Geoset::readMdl(std::istream &istream) throw (class Exception)
{
}

void Geoset::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 Geoset::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytes = 0;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	
	if (nbytes <= 0)
	{
		char message[50];
		sprintf(message, _("Geoset: 0 byte geoset.\n"));
		
		throw Exception(message);
	}
	
	std::cout << "Test 1" << std::endl;
	long32 bytes = istream.gcount();
	bytes += this->m_vertices->readMdx(istream);
	bytes += this->m_normals->readMdx(istream);
	bytes += this->m_primitveTypes->readMdx(istream);
	bytes += this->m_primitiveSizes->readMdx(istream);
	bytes += this->m_primitiveVertices->readMdx(istream);
	bytes += this->m_groupVertices->readMdx(istream);
	bytes += this->m_matrixGroupCounts->readMdx(istream);
	bytes += this->m_matrices->readMdx(istream);
	std::cout << "Test 2" << std::endl;
	istream.read(reinterpret_cast<char*>(&this->m_materialId), sizeof(this->m_materialId));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_selectionGroup), sizeof(this->m_selectionGroup));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_selectable), sizeof(this->m_selectable));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_boundsRadius), sizeof(this->m_boundsRadius));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_minExtentX), sizeof(this->m_minExtentX));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_minExtentY), sizeof(this->m_minExtentY));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_minExtentZ), sizeof(this->m_minExtentZ));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_maxExtentX), sizeof(this->m_maxExtentX));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_maxExtentY), sizeof(this->m_maxExtentY));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_maxExtentZ), sizeof(this->m_maxExtentZ));
	bytes += istream.gcount();
	std::cout << "Test 3" << std::endl;
	long32 nanim = 0;
	istream.read(reinterpret_cast<char*>(&nanim), sizeof(nanim));
	bytes += istream.gcount();
	std::cout << "Test 4 nanimations " << nanim << std::endl;
	
	for ( ; nanim > 0; --nanim)
	{
		class Ganimation *ganimation = new Ganimation(this);
		bytes += ganimation->readMdx(istream);
		this->m_ganimations.push_back(ganimation);
	}
	
	std::cout << "Test 5" << std::endl;
	bytes += this->m_texturePatches->readMdx(istream);
	/// @todo Doesn't exist!!!!
	bytes += this->m_textureVertices->readMdx(istream);
	
	std::cout << "Test 192832948" << std::endl;
	
	return bytes;
}

long32 Geoset::writeMdx(std::ostream &ostream) throw (class Exception)
{
	return 0;
}

}

}
