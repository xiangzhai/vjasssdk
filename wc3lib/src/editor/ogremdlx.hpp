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

#include <boost/cast.hpp>

#include <kurl.h>

#include <Ogre.h>

#include "resource.hpp"
#include "../mdlx.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace editor
{

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
		
		typedef std::map<const class mdlx::Camera*, Ogre::Camera*> Cameras;
		
		struct CollisionShape
		{
			CollisionShape() : box(0), shape(mdlx::CollisionShape::Box)
			{
			}
			
			~CollisionShape()
			{
				if (shape == mdlx::CollisionShape::Box)
					delete box;
				else
					delete sphere;
			}
			
			union { Ogre::AxisAlignedBox *box; Ogre::Sphere *sphere;  };
			mdlx::CollisionShape::Shape shape;
		};
		
		typedef std::map<const class mdlx::CollisionShape*, struct CollisionShape*> CollisionShapes;
		
		
		static QColor teamColor(enum TeamColor teamColor);
		static enum TeamColor teamColor(const QColor &color);
		/**
		 * Changes camera's \p ogreCamera to settings of camera \p camera.
		 * Might be used by camera actions in any editor which have to view a specific camera.
		 */
		static void updateCamera(const class mdlx::Camera &camera, Ogre::Camera *ogreCamera);

		OgreMdlx(const KUrl &url, const class mdlx::Mdlx &mdlx, class ModelView *modelView);
		~OgreMdlx();

		const class mdlx::Mdlx* mdlx() const;
		class ModelView* modelView() const;

		const Cameras& cameras() const;
		const CollisionShapes& collisionShapes() const;

		void setTeamColor(enum TeamColor teamColor);
		enum TeamColor teamColor() const;
		void setTeamGlow(enum TeamColor teamGlow);
		enum TeamColor teamGlow() const;

		/**
		* Loads and analyses all data of corresponding MDLX model and refreshes displayed OGRE mesh.
		*/
		void refresh() throw (class Exception, class Ogre::Exception);

		/**
		 * Provides serialization functionality of the corresponding \ref mdlx::Mdlx instance or the OGRE scene.
		 * Detects output format by file extension (no matter what case).
		 * Supported formats:
		 * <ul>
		 * <li>MDX</li>
		 * <li>MDL</li>
		 * </ul>
		 * Stores file at \p url.
		 * \throw Exception Is thrown when file could not be stored in \p url.
		 */
		void saveAs(const KUrl &url) throw (class Exception);

	protected:
		typedef std::pair<const class Node*, Ogre::Node*> NodePairType;

		QString namePrefix() const;
		/**
		 * Searches in member list of \p block for member \p member and returns its corresponding id.
		 * Id is always its index in member list (starting with 0).
		 */
		mdlx::long32 mdlxId(const mdlx::GroupMdxBlockMember &member, const mdlx::GroupMdxBlock *block) const;
		/**
		 * Sometimes models are loaded from local directories which do also contain their required textures, attachments, particle emitter effects etc.
		 * In that case you can't load those required files from \ref MpqPriorityList (\ref Editor -> \ref ModelView::editor()). Hence you must load them from the local filesystem using the model's directory URL as root directory.
		 * This function checks whether URL \p url exists in one of the MPQ archives using \ref MpqPriorityList::findFile and if not it replaces the given URL parameter by a local URL using \ref OgreMdlx::url()'s directory as root directory.
		 * \param url URL which is checked for and might be replaced by its local version using model's resource directory URL as root directory.
		 * \param showMessage If this value is true it will show a message dialog if file does not exist in one of the MPQ archives.
		 * \return Returns true if file does not exist in one of the MPQ archives and you have to try the local URL.
		 */
		bool useDirectoryUrl(KUrl &url, bool showMessage = true) const;

		Ogre::TexturePtr createTexture(const class mdlx::Texture &texture) throw (class Exception);
		Ogre::MaterialPtr createMaterial(const class mdlx::Material &material) throw (class Exception);
		/**
		* Creates manual object for specified geoset.
		*/
		Ogre::ManualObject* createGeoset(const class mdlx::Geoset &geoset) throw (class Exception);

		Ogre::Camera* createCamera(const class mdlx::Camera &camera) throw (class Exception);

		/**
		 * Collision shapes are required for "hit tests".
		 */
		CollisionShape* createCollisionShape(const class mdlx::CollisionShape &collisionShape) throw (class Exception);
		
		Ogre::Node* createNode(const class mdlx::Node &node);

		/**
		* Creates all necessary OGRE nodes with correct inheritane and returns the resulting map with all nodes and objects.
		* @todo Allocate objects by using type information (Object::type).
		*/
		std::map<const class mdlx::Node*, Ogre::Node*> setupInheritance(const std::list<const class mdlx::Node*> &nodes);

		const class mdlx::Mdlx *m_mdlx;
		class ModelView *m_modelView;
		Ogre::SceneNode *m_sceneNode;

		std::map<const class mdlx::Texture*, Ogre::TexturePtr> m_textures;
		std::map<const class mdlx::Material*, Ogre::MaterialPtr> m_materials;
		std::map<const class mdlx::Geoset*, Ogre::ManualObject* > m_geosets;
		Cameras m_cameras;
		CollisionShapes m_collisionShapes;

		std::map<const class mdlx::Node*, Ogre::Node*> m_nodes;
		std::map<const class mdlx::Bone*, Ogre::Bone*> m_bones;

		class GlobalSequence *m_globalSequence; /// Current global sequence which is played.

		// these members are required for dynamic team glow and color settings
		enum TeamColor m_teamColor;
		enum TeamColor m_teamGlow;
		std::list<Ogre::TexturePtr> m_teamColorTextures;
		std::list<Ogre::TexturePtr> m_teamGlowTextures;
};

inline QColor OgreMdlx::teamColor(enum TeamColor teamColor)
{
	switch (teamColor)
	{
		case Red:
			return QColor(Qt::red);
		
		case Blue:
			return QColor(Qt::blue);
			
		case Teal:
		case Purple:
			return QColor(); // FIXME
			
		case Yellow:
			return QColor(Qt::yellow);
			
		case Orange:
			return QColor(); // FIXME
			
		case Green:
			return QColor(Qt::green);
			
		case Pink:
		case Gray:
		case LightBlue:
		case DarkGreen:
		case Brown:
			return QColor(); // FIXME
			
		case Black:
			return QColor(Qt::black);
	}
	
	return QColor(Qt::white);
}

inline enum OgreMdlx::TeamColor OgreMdlx::teamColor(const QColor &color)
{
	if (color == Qt::red)
		return Red;
	else if (color == Qt::blue)
		return Blue;
	//case Teal:
	//case Purple:
	else if (color == Qt::yellow)
		return Yellow;
	//case Orange:
	else if (color == Qt::green)
		return Green;
	/*
	case Pink:
	case Gray:
	case LightBlue:
	case DarkGreen:
	case Brown:
	*/
	else if (color == Qt::black)
		return Black;
	
	return Red;
}

inline const class mdlx::Mdlx* OgreMdlx::mdlx() const
{
	return this->m_mdlx;
}

inline class ModelView* OgreMdlx::modelView() const
{
	return this->m_modelView;
}

inline const OgreMdlx::Cameras& OgreMdlx::cameras() const
{
	return this->m_cameras;
}

inline const OgreMdlx::CollisionShapes& OgreMdlx::collisionShapes() const
{
	return this->m_collisionShapes;
}

inline enum OgreMdlx::TeamColor OgreMdlx::teamColor() const
{
	return this->m_teamColor;
}

inline enum OgreMdlx::TeamColor OgreMdlx::teamGlow() const
{
	return this->m_teamGlow;
}

/// Global type cast function.
inline Ogre::Real ogreReal(mdlx::float32 value)
{
	return boost::numeric_cast<Ogre::Real>(value);
}

/// Global type cast function.
inline Ogre::Vector3 ogreVector3(const mdlx::VertexData &vertexData)
{
	return Ogre::Vector3(vertexData.x, vertexData.y, vertexData.z);
}

}

}

#endif
