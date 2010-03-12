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

#include <OgreMeshManager.h>
#include <OgreSubMesh.h>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include "ogremdlx.hpp"
#include "mdlx.hpp"
#include "model.hpp"
#include "geosets.hpp"
#include "geoset.hpp"
#include "vertices.hpp"
#include "vertex.hpp"
#include "normals.hpp"
#include "normal.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{
	
namespace mdlx
{

OgreMdlx::OgreMdlx(class Mdlx *mdlx) : m_mdlx(mdlx), m_mesh(0)
{
}

void OgreMdlx::refresh() throw (class Exception)
{
	class Ogre::MeshManager *meshManager = Ogre::MeshManager::getSingletonPtr();
	
	if (this->m_mesh.isNull())
		this->m_mesh = meshManager->createManual(this->m_mdlx->model()->name(), "MDLX");
	
	//else if (this->m_mesh->
	this->m_mesh->_setBoundingSphereRadius(this->m_mdlx->model()->boundsRadius());
	this->m_mesh->_setBounds(Ogre::AxisAlignedBox(
		this->m_mdlx->model()->minExtX(),
		this->m_mdlx->model()->minExtY(),
		this->m_mdlx->model()->minExtZ(),
		this->m_mdlx->model()->maxExtX(),
		this->m_mdlx->model()->maxExtY(),
		this->m_mdlx->model()->maxExtZ()
		)
	);
	
	/// ...
	std::size_t i = 0;
	
	BOOST_FOREACH(const class Geoset *geoset, this->m_mdlx->geosets()->geosets())
	{
		std::string name = boost::str(boost::format(_("Geoset - %1%")) % i);
		Ogre::SubMesh *subMesh = this->m_mesh->createSubMesh(name.c_str());
		std::string verticesPoseName = boost::str(boost::format(_("%1% - Vertices Pose")) % name);
		class Ogre::Pose *verticesPose = this->m_mesh->createPose(i + 1, verticesPoseName.c_str());
		std::size_t j = 0;
		
		BOOST_FOREACH(const class Vertex *vertex, geoset->vertices()->vertices())
		{
			verticesPose->addVertex(j, Ogre::Vector3(vertex->x(), vertex->y(), vertex->z()));
			++j;
		}
		
		std::string normalsPoseName = boost::str(boost::format(_("%1% - Normals Pose")) % name);
		class Ogre::Pose *normalsPose = this->m_mesh->createPose(i + 1, normalsPoseName.c_str());
		j = 0;
		
		BOOST_FOREACH(const class Normal *normal, geoset->normals()->normals())
		{
			normalsPose->addVertex(j, Ogre::Vector3(normal->x(), normal->y(), normal->z()));
			++j;
		}
		
		// primitives data
		// vertex group data
			
		//subMesh->vertexData()
		//subMesh->vertexData()void 	setMaterialName (const String &matName)		
		
		++i;
	}
}

}

}
