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

#include <OgreSubMesh.h>

#include "ogremdlx.hpp"
#include "mdlx.hpp"
#include "geoset.hpp"

namespace wc3lib
{
	
namespace mdlx
{

OgreMdlx::OgreMdlx(class Mdlx *mdlx) : m_mdlx(mdlx), m_mesh(0)
{
}

void setUpGeosetOnMesh(Geoset *geoset, Ogre::MeshPtr mesh)
{
	mesh->
}

void OgreMdlx::refresh(class Ogre::MeshManager *meshManager)
{
	if (this->m_mesh == 0)
		this->m_mesh = meshManager->createManual(this->m_mdlx->model()->name(), "MDLX");
	
	this->m_mesh->setBoundingSphereRadius(this->m_mdlx->model()->boundsRadius());
	this->m_mesh->_setBounds(
		this->m_mdlx->model()->minExtX(),
		this->m_mdlx->model()->minExtY(),
		this->m_mdlx->model()->minExtZ(),
		this->m_mdlx->model()->maxExtX(),
		this->m_mdlx->model()->maxExtY(),
		this->m_mdlx->model()->maxExtZ()
	);
	
	/// ...
	std::size_t index = 0;
	
	BOOST_FOREACH(const class Geoset *geoset, this->m_mdlx->geosets()->geosets())
	{
		Ogre::SubMesh *subMesh = this->m_mesh->createSubMesh(boost::str(boost::format(_("Geoset - %1%")) % i).c_str());
		
		subMesh->vertexData()void 	setMaterialName (const String &matName)		
		
		++i;
	}
}

}

}
