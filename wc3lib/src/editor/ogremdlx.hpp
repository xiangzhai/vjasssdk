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

#ifndef WC3LIB_EDITOR_OGREMDLX_HPP
#define WC3LIB_EDITOR_OGREMDLX_HPP

#include <list>
#include <map>

#include <kurl.h>

#include <Ogre.h>

#include "resource.hpp"
#include "../mdlx.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace editor
{

using namespace mdlx;

/**
* This class can be used to display MDLX models by using the OGRE 3d rendering engine.
* It maintains a single mesh instance in a scene which contains all converted data of the original
* MDLX model.
* Geosets are implemented as sub meshes.
* Each MDLX instance can have its own team color and glow which is required for proper unit displays and model testings.
* @todo Use inherited event functions of frame listener to apply animation track data (each model instance should have its own time marker for sequences).
*/
class OgreMdlx  : public Resource, public Ogre::FrameListener
{
	public:
		enum TeamColor
		{
			Red,
			Blue,
			Teal,
			Purple,
			Yellow,
			Orange,
			Green,
			Pink,
			Gray,
			LightBlue,
			DarkGreen,
			Brown,
			Black,
			MaxTeamColors
		};
		
		typedef std::map<const class Camera*, Ogre::Camera*> Cameras;

		OgreMdlx(const KUrl &url, const class Mdlx &mdlx, class ModelView *modelView);
		~OgreMdlx();

		const class Mdlx* mdlx() const;
		class ModelView* modelView() const;

		const Cameras cameras() const;

		void setTeamColor(enum TeamColor teamColor);
		enum TeamColor teamColor() const;
		void setTeamGlow(enum TeamColor teamGlow);
		enum TeamColor teamGlow() const;

		/**
		* Loads and analyses all data of corresponding MDLX model and refreshes displayed OGRE mesh.
		*/
		void refresh() throw (class Exception, class Ogre::Exception);

		void saveAs(const KUrl &url) throw (class Exception);

	protected:
		typedef std::pair<const class Node*, Ogre::Node*> NodePairType;

		QString namePrefix() const;
		/**
		 * Searches in member list of \p block for member \p member and returns its corresponding id.
		 * Id is always its index in member list (starting with 0).
		 */
		mdlx::long32 mdlxId(const mdlx::GroupMdxBlockMember &member, const mdlx::GroupMdxBlock *block) const;

		Ogre::TexturePtr createTexture(const class Texture &texture) throw (class Exception);
		Ogre::MaterialPtr createMaterial(const class Material &material) throw (class Exception);
		/**
		* Creates manual object for specified geoset.
		*/
		Ogre::ManualObject* createGeoset(const class Geoset &geoset) throw (class Exception);

		Ogre::Camera* createCamera(const class Camera &camera) throw (class Exception);

		/**
		 * Collision shapes are required for "hit tests".
		 */
		Ogre::ManualObject* createCollisionShape(const class CollisionShape &collisionShape) throw (class Exception);
		
		Ogre::Node* createNode(const class Node &node);

		/**
		* Creates all necessary OGRE nodes with correct inheritane and returns the resulting map with all nodes and objects.
		* @todo Allocate objects by using type information (Object::type).
		*/
		std::map<const class Node*, Ogre::Node*> setupInheritance(const std::list<const class Node*> &nodes);

		const class Mdlx *m_mdlx;
		class ModelView *m_modelView;
		Ogre::SceneNode *m_sceneNode;

		std::map<const class Texture*, Ogre::TexturePtr> m_textures;
		std::map<const class Material*, Ogre::MaterialPtr> m_materials;
		std::map<const class Geoset*, Ogre::ManualObject* > m_geosets;
		Cameras m_cameras;

		std::map<const class Node*, Ogre::Node*> m_nodes;
		std::map<const class Bone*, Ogre::Bone*> m_bones;

		class GlobalSequence *m_globalSequence; /// Current global sequence which is played.

		enum TeamColor m_teamColor;
		enum TeamColor m_teamGlow;
		std::list<Ogre::TexturePtr> m_teamColorTextures;
		std::list<Ogre::TexturePtr> m_teamGlowTextures;
};

inline const class Mdlx* OgreMdlx::mdlx() const
{
	return this->m_mdlx;
}

inline class ModelView* OgreMdlx::modelView() const
{
	return this->m_modelView;
}

inline const OgreMdlx::Cameras OgreMdlx::cameras() const
{
	return this->m_cameras;
}

inline enum OgreMdlx::TeamColor OgreMdlx::teamColor() const
{
	return this->m_teamColor;
}

inline enum OgreMdlx::TeamColor OgreMdlx::teamGlow() const
{
	return this->m_teamGlow;
}

}

}

#endif
