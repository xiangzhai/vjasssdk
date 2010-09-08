/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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

#ifndef WC3LIB_MDLX_OGREMDLX_HPP
#define WC3LIB_MDLX_OGREMDLX_HPP

#include <map>
#include <list>

#include <Ogre.h>

#include "../mdlx.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;

/**
* This class can be used to display MDLX models by using the OGRE 3d rendering engine.
* It maintains a single mesh instance in a scene which contains all converted data of the original
* MDLX model.
* Geosets are implemented as sub meshes.
* @todo This class should be moved to wc3lib module "editor".
*/
class OgreMdlx
{
	public:
		OgreMdlx(const class Mdlx &mdlx);

		const class Mdlx* mdlx() const;
		const Ogre::MeshPtr mesh() const;

		/**
		* Loads and analyses all data of corresponding MDLX model and refreshes displayed OGRE mesh.
		*/
		void refresh() throw (class Exception);

	protected:
		typedef std::pair<const class Node*, Ogre::Node*> NodePairType;

		Ogre::Node* createNode(const class Node &node);

		/**
		* Creates all necessary OGRE nodes with correct inheritane and returns the resulting map with all nodes and objects.
		* @todo Allocate objects by using type information (Object::type).
		*/
		std::map<const class Node*, Ogre::Node*> setupInheritance(const std::list<const class Node*> &nodes);

		const class Mdlx *m_mdlx;
		Ogre::MeshPtr m_mesh;
		std::map<const class Geoset*, Ogre::MeshPtr> m_geosets;
		std::map<const class Node*, Ogre::Node*> m_nodes;
		std::map<const class Bone*, Ogre::Bone*> m_bones;
};

inline const class Mdlx* OgreMdlx::mdlx() const
{
	return this->m_mdlx;
}

inline const Ogre::MeshPtr OgreMdlx::mesh() const
{
	return this->m_mesh;
}

}

}

#endif
