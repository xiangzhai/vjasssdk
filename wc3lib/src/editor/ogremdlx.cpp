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

#include <cstring>

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/cast.hpp>
#include <boost/filesystem.hpp>

#include <QtGui>

#include <kmessagebox.h>
#include <klocale.h>
#include <kurl.h>
#include <kfiledialog.h> /// TEST

#include <OgreCodec.h>

#include "ogremdlx.hpp"
#include "modelview.hpp"
#include "editor.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace editor
{

OgreMdlx::OgreMdlx(const KUrl &url, const class Mdlx &mdlx, class ModelView *modelView) : Resource(url, Resource::Model), m_mdlx(&mdlx), m_modelView(modelView)
{
}

void OgreMdlx::refresh() throw (class Exception)
{
	Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_BOREME);
	Ogre::SceneManager &sceneManager = *this->m_modelView->sceneManager();
	// create textures
	BOOST_FOREACH(const mdlx::Texture *texture, this->m_mdlx->textures()->textures())
		this->m_textures[texture] = this->createTexture(*texture);

	// create materials
	BOOST_FOREACH(const mdlx::Material *material, this->m_mdlx->materials()->materials())
		this->m_materials[material] = this->createMaterial(*material);

	// create geosets
	BOOST_FOREACH(const mdlx::Geoset *geoset, this->m_mdlx->geosets()->geosets())
		this->m_geosets[geoset] = this->createGeoset(*geoset);

	Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_NORMAL);
	// get new objects
	/*
	std::list<const class Node*> nodes = this->m_mdlx->nodes();

	BOOST_FOREACH(const class Node *node, nodes)
	{
		if (this->m_nodes.find(node) != this->m_nodes.end())
			nodes.remove(node);
	}
	*/

	// setup nodes of new objects (inheritance)
	/*
	std::map<const class Node*, Ogre::Node*> newNodes(this->setupInheritance(nodes));

	BOOST_FOREACH(NodePairType nodePair, newNodes)
		this->m_nodes.insert(nodePair);
	*/


	// refresh model
	/*
	class Ogre::MeshManager *meshManager = Ogre::MeshManager::getSingletonPtr();

	if (this->m_mesh.isNull())
		this->m_mesh = meshManager->createManual(this->m_mdlx->model()->name(), "MDLX");

	//else if (this->m_mesh->
	/// @todo bounds radius, minimum and maximum extent are for Warcraft rendering only?
	this->m_mesh->_setBoundingSphereRadius(this->m_mdlx->model()->boundsRadius());
	this->m_mesh->_setBounds(Ogre::AxisAlignedBox(
		this->m_mdlx->model()->minimumExtent().x,
		this->m_mdlx->model()->minimumExtent().y,
		this->m_mdlx->model()->minimumExtent().z,
		this->m_mdlx->model()->maximumExtent().x,
		this->m_mdlx->model()->maximumExtent().y,
		this->m_mdlx->model()->maximumExtent().z
		)
	);
	*/

	// refresh sequences

	//* 	createAnimation (const String &name, Real length)


	// refresh global sequences
	// Global sequences are played for all sequences!
	/*
	std::size_t i = 0;

	BOOST_FOREACH(const class GlobalSequence *globalSequence, this->m_mdlx->globalSequences()->globalSequences())
	{
		if (this->m_nodes.find(static_cast<const class Object*>(globalSequence)) == this->m_nodes.end())
			this->m_nodes[globalSequence] = this->m_mesh->createAnimation(boost::str(boost::format(_("Global Sequence - %1%")) % i), globalSequence->duration());
	}
	*/

	// refresh geosets
	/// ...
	// std::size_t i = 0;

	//BOOST_FOREACH(const class Geoset *geoset, this->m_mdlx->geosets()->geosets())
	//{
	//	const class Geoset &rGeoset = *geoset;
	//	Ogre::SceneNode *node = sceneManager.createSceneNode();
	//	node->attachObject(this->createGeoset(rGeoset));
	//}
		/*
		std::map<const class Geoset*, Ogre::MeshPtr>::iterator iterator = this->m_geosets.find(geoset);
		Ogre::MeshPtr geosetMesh;
		std::string name;

		if (iterator == this->m_geosets.end())
		{
			name = boost::str(boost::format(_("Geoset - %1%")) % i);
			geosetMesh = meshManager->createManual(boost::str(boost::format(_("%1% - %2%")) % this->m_mdlx->model()->name() % name), "MDLX");
			this->m_geosets[geoset] = geosetMesh;
		}
		else
		{
			geosetMesh = iterator->second;
			name = geosetMesh->getName();
		}

		geosetMesh->_setBoundingSphereRadius(geoset->boundsRadius());
		geosetMesh->_setBounds(Ogre::AxisAlignedBox(
			geoset->minimumExtent().x,
			geoset->minimumExtent().y,
			geoset->minimumExtent().z,
			geoset->maximumExtent().x,
			geoset->maximumExtent().y,
			geoset->maximumExtent().z
			)
		);

		//Ogre::SubMesh *subMesh = this->m_mesh->createSubMesh(name.c_str());
		std::string verticesPoseName = boost::str(boost::format(_("%1% - Vertices Pose")) % name);
		class Ogre::Pose *verticesPose = geosetMesh->createPose(i + 1, verticesPoseName.c_str());
		std::size_t j = 0;

		BOOST_FOREACH(const class Vertex *vertex, geoset->vertices()->vertices())
		{
			verticesPose->addVertex(j, Ogre::Vector3(vertex->vertexData().x, vertex->vertexData().y, vertex->vertexData().z));
			++j;
		}

		std::string normalsPoseName = boost::str(boost::format(_("%1% - Normals Pose")) % name);
		class Ogre::Pose *normalsPose = geosetMesh->createPose(i + 1, normalsPoseName.c_str());
		j = 0;

		BOOST_FOREACH(const class Normal *normal, geoset->normals()->normals())
		{
			normalsPose->addVertex(j, Ogre::Vector3(normal->vertexData().x, normal->vertexData().y, normal->vertexData().z));
			++j;
		}

		// primitives data
		// vertex group data

		//subMesh->vertexData()
		//subMesh->vertexData()void 	setMaterialName (const String &matName)
		*/
		/*
		std::list<const class Bone*> geosetBones;

		// Bones
		BOOST_FOREACH(const class Bone *bone, this->m_mdlx->bones()->bones())
		{
			if (geoset == this->m_mdlx->boneGeoset(*bone))
			{
				geosetBones.push_back(bone);

				Ogre::Node *node = this->m_nodes[bone];

				// hasn't already been created by parent
				if (!bone->hasParent() || this->m_bones.find(bone) != this->m_bones.end())
				{
					this->m_bones[bone] = geosetMesh->getSkeleton()->createBone(bone->name());
				}
				// else, has already been created

				Ogre::Bone *ogreBone = this->m_bones[bone];

				std::list<const class Node*> children = this->m_mdlx->children(*bone);

				BOOST_FOREACH(const class Node *node, children)
				{
					const class Bone *childBone = static_cast<const class Bone*>(node);
					Ogre::Bone *ogreChildBone = geosetMesh->getSkeleton()->createBone(childBone->name());
					ogreBone->addChild(ogreChildBone);
				}
			}
		}
		*/

	//}

	// apply object inhertance modifiers with pivot points
	/// @todo Consider pivot points and the fact that this function is called for refreshment not initial placement.
	/*
	BOOST_FOREACH(NodePairType nodePair, this->m_nodes)
	{
		const class Node *node = nodePair.first;
		Ogre::Node *node = nodePair.second;
		long32 currentSequenceId = 0; /// @todo Set by animation?
		long32 currentFrame = 0; /// @todo Set by animation?

		// If the object has pivot point it uses its own orientation and not the parent's one.
		const class PivotPoint *pivotPoint = this->m_mdlx->objectPivotPoint(*object);

		// Ogre: Note that rotations are oriented around the node's origin.
		if (pivotPoint == 0)
		{
			node->setInheritOrientation(true);
		}
		else
		{
			node->setInheritOrientation(false);
			node->setOrientation(0.0, /// @todo Real w???, aren't same as pivot points?
				pivotPoint->x(),
				pivotPoint->y(),
				pivotPoint->z()
				);
		}

		if (object->hasParent())
		{
			if (object->inheritsTranslation() && (!object->translations()->hasGlobalSequence() || object->translations()->globalSequenceId() == currentSequenceId))
			{
				BOOST_FOREACH(const class MdlxTranslation *translation, object->translations()->mdlxTranslations())
				{
					if (translation->frame() == currentFrame)
					{
						switch (object->translations()->lineType())
						{
							case DontInterpolate:
								node->translate(translation->vertexData().x, translation->vertexData().y, translation->vertexData().z, Ogre::Node::TS_PARENT);

								break;

							case Linear:
								/// @todo FIXME

								break;

							case Hermite:
								/// @todo FIXME

								break;

							case Bezier:
								/// @todo FIXME

								break;
						}
					}
				}
			}

			if (object->inheritsRotation() && (!object->rotations()->hasGlobalSequence() || object->rotations()->globalSequenceId() == currentSequenceId))
			{
				BOOST_FOREACH(const class MdlxRotation *rotation, object->rotations()->mdlxRotations())
				{
					if (rotation->frame() == currentFrame)
					{
						switch (object->rotations()->lineType())
						{
							case DontInterpolate:
								node->rotate(Ogre::Quaternion(rotation->quaternionData().a, rotation->quaternionData().b, rotation->quaternionData().c, rotation->quaternionData().d), Ogre::Node::TS_PARENT);

								break;

							case Linear:
								/// @todo FIXME

								break;

							case Hermite:
								/// @todo FIXME

								break;

							case Bezier:
								/// @todo FIXME

								break;
						}
					}
				}
			}

			if (object->inheritsScaling() && (!object->scalings()->hasGlobalSequence() || object->scalings()->globalSequenceId() == currentSequenceId))
			{
				node->setInheritScale(true);

				BOOST_FOREACH(const class MdlxScaling *scaling, object->scalings()->mdlxScalings())
				{
					if (scaling->frame() == currentFrame)
					{
						switch (object->scalings()->lineType())
						{
							case DontInterpolate:
								node->scale(Ogre::Vector3(scaling->vertexData().x, scaling->vertexData().y, scaling->vertexData().z));

								break;

							case Linear:
								/// @todo FIXME

								break;

							case Hermite:
								/// @todo FIXME

								break;

							case Bezier:
								/// @todo FIXME

								break;
						}
					}
				}
			}
		}
	}
	*/
}

Ogre::Node* OgreMdlx::createNode(const class Node &node)
{
	Ogre::Node *result = 0;

	if (node.type() & Node::Helper)
		return 0;

	if (node.type() & Node::Bone)
		return 0;

	if (node.type() & Node::Light)
		return 0;

	if (node.type() & Node::EventObject)
		return 0;

	if (node.type() & Node::Attachment)
		return 0;

	if (node.type() & Node::ParticleEmitter)
		return 0;

	if (node.type() & Node::CollisionShape)
		return 0;

	if (node.type() & Node::RibbonEmitter)
		return 0;

	return result;
}

namespace
{

/// @todo Use KUrl and IO slaves (MPQ protocol)
/**
* Opens external image file with the given URL and reads it by using Qt which allows you to read BLP files, too and converts it into an OGRE image.
* @note OGRE has to be compiled and linked with FreeImage (PNG support).
*/
Ogre::Image* blpToOgre(const KUrl &url) throw (class Exception)
{
	QFile file(url.toLocalFile());

	if (!file.open(QIODevice::ReadOnly))
		throw Exception(boost::format(_("Unable to open texture image \"%1%\".")) % url.toLocalFile().toAscii().data());

	QImage qImage;

	if (!qImage.load(&file, 0))
		throw Exception(boost::format(_("Unable to load texture image \"%1%\".")) % url.toLocalFile().toAscii().data());

	file.close();
	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	qImage.save(&buffer, "PNG");

	// TEST (writing buffer on disk)
	/*
	KUrl fileUrl = KFileDialog::getSaveUrl(KUrl(""), i18n("*.png|PNG"));

	if (!fileUrl.isEmpty() && fileUrl.isLocalFile())
	{
		QFile newFile(fileUrl.toLocalFile());
		newFile.open(QIODevice::WriteOnly);
		newFile.write(ba.data(), ba.size());
		newFile.close();

		KMessageBox::information(0, i18n("Hat geklappt!"));
	}
	*/
	// END TEST
	// bis hierher klappt alles, die PNG-Bilder sehen korrekt aus, wenn sie auf die Festplatte geschrieben werden

	// load for OGRE
	//void *memory[ba.size()];
	//memcpy(memory, (const void*)(ba), ba.size());
	Ogre::DataStreamPtr dsPtr(new Ogre::MemoryDataStream(static_cast<void*>(ba.data()), ba.size()));
	//ba.clear();
	//dsPtr.bind(dynamic_cast<Ogre::DataStream*>(ms));
	Ogre::Image *image = new Ogre::Image();

	try
	{
		image->load(dsPtr);
	}
	catch (Ogre::Exception &exception)
	{
		throw Exception(boost::format(_("Unable to open texture image \"%1%\".\nOGRE error: \"%2%\"")) % url.toLocalFile().toAscii().data() % exception.what());
	}

	return image;
}

}

Ogre::TexturePtr OgreMdlx::createTexture(const class mdlx::Texture &texture) throw (class Exception)
{
	Ogre::Image *image = 0;
	mdlx::long32 id = 0;

	BOOST_FOREACH(const class mdlx::Texture *tex, this->m_mdlx->textures()->textures())
	{
		if (tex == &texture)
			break;

		++id;
	}

	Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().create((boost::format("Texture%1%") % id).str().c_str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	if (texture.replaceableId() == mdlx::None)
	{
		// if image could not be loaded continue with empty texture
		try
		{
			KUrl url = this->modelView()->editor()->findFile(KUrl(texture.texturePath()));

			// if no editor resource has been found file directory will be tried
			if (!url.isValid())
			{
				url = this->url().directory();
				url.addPath(texture.texturePath());
			}

			image = blpToOgre(url);
		}
		catch (class Exception &exception)
		{
			KMessageBox::error(0, i18n("Texture loading error:\n%1", exception.what().c_str()));
		}

		if (image != 0)
		{
			tex->loadImage(*image);

			/// @todo Apply texture properties:
			/*
			enum Wrapping
			{
				WrapWidth = 1,
				WrapHeight = 2,
				Both = 3
			};
			*/
			// enum ReplaceableId replaceableId() const;
			// Use m_teamColor and m_teamGlowColor for color values!
			/*
			enum ReplaceableId
			{
				None = 0,
				TeamColor = 1,
				TeamGlow = 2,
				Cliff = 11,
				LordaeronTree = 31,
				AshenvaleTree = 32,
				BarrensTree = 33,
				NorthrendTree = 34,
				MushroomTree = 35
			};
			*/

			delete image;
			image = 0;
		}
	}
	// replace with replaceable id
	/// @todo Load corresponding replacebale textures (from MPQ archives)
	else
	{

	}

	return tex;
}

Ogre::MaterialPtr OgreMdlx::createMaterial(const class mdlx::Material &material) throw (class Exception)
{
	mdlx::long32 id = 0;

	BOOST_FOREACH(const class mdlx::Material *mat, this->m_mdlx->materials()->materials())
	{
		if (mat == &material)
			break;

		++id;
	}

	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create((boost::format("Material%1%") % id).str().c_str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); // material->mdlx()->model()->name()

	// properties
	/*
	ConstantColor,
	SortPrimsFarZ,
	FullResolution,
	PriorityPlane <long>,
	*/

	// layers do have texture id!!!
	// use Ogre::Technique?

	//return 0;

	BOOST_FOREACH(const class mdlx::Layer *layer, material.layers()->layers())
	{
		Ogre::Technique *technique = mat->createTechnique();
		Ogre::Pass *pass = technique->createPass();
		Ogre::TextureUnitState *textureUnitState = pass->createTextureUnitState((boost::format("Texture%1%") % layer->textureId()).str().c_str());
		//textureUnitState->setTextureFiltering();
		//textureUnitState->setAlphaOperation(Ogre::LBX_MODULATE_X4);
		textureUnitState->setTextureCoordSet(layer->coordinatesId());

		switch (layer->filterMode())
		{
			case mdlx::Layer::Transparent:
				break;

			case mdlx::Layer::Blend:
				textureUnitState->setColourOperation(Ogre::LBO_ALPHA_BLEND);

				break;

			case mdlx::Layer::Additive:
				textureUnitState->setColourOperation(Ogre::LBO_ADD);

				break;

			case mdlx::Layer::AddAlpha:
				break;

			case mdlx::Layer::Modulate:
				textureUnitState->setColourOperation(Ogre::LBO_MODULATE);

				break;

			case mdlx::Layer::Modulate2x:
				break;
		}

		/*
		enum FilterMode
		{
			None = 0,
			Transparent = 1,
			Blend = 2,
			Additive = 3,
			AddAlpha = 4,
			Modulate = 5,
			Modulate2x = 6
		};

		enum Shading
		{
			Unshaded = 1,
			SphereEnvironmentMap = 2,
			Unknown0 = 4,
			Unknown1 = 8,
			TwoSided = 16,
			Unfogged = 32,
			NoDepthTest = 64,
			NoDepthSet = 128
		};

		Non-animation properties of layer:
		enum FilterMode filterMode() const;
		enum Shading shading() const;
		long32 textureId() const;
		long32 coordinatesId() const;
		float32	alpha() const;
		*/

		/*
		Texture animation stuff
		BOOST_FOREACH(const class mdlx::TextureId *textureId, layer->textureIds())
		{
			//textureId->frame()
		}
		*/
	}

	return mat;
}

Ogre::ManualObject* OgreMdlx::createGeoset(const class mdlx::Geoset &geoset) throw (class Exception)
{
	Ogre::ManualObject *object = new Ogre::ManualObject("geoset");
	qDebug() << "Creating geoset";

	// set bounds
	/// @todo Set Bounding radius
	//object->setBoundingSphereRadius(geoset->boundsRadius());
	object->setBoundingBox(Ogre::AxisAlignedBox(
		geoset.minimumExtent().x,
		geoset.minimumExtent().y,
		geoset.minimumExtent().z,
		geoset.maximumExtent().x,
		geoset.maximumExtent().y,
		geoset.maximumExtent().z
		)
	);

	// get material
	const class mdlx::Material *material = 0;
	mdlx::long32 id = 0;

	BOOST_FOREACH(material, this->m_mdlx->materials()->materials())
	{
		if (id == geoset.materialId())
			break;

		++id;
	}

	if (id != geoset.materialId())
		throw Exception(boost::format(_("Missing material %1%")) % id);

	// get material
	//this->m_materials[material]

	// increase processor efficiency
	object->estimateVertexCount(geoset.vertices()->vertices().size());
	object->estimateIndexCount(geoset.vertices()->vertices().size());
	object->begin(this->m_materials[material]->getName(), Ogre::RenderOperation::OT_TRIANGLE_LIST);

	// build vertices
	std::list<class mdlx::Vertex*>::const_iterator vertexIterator = geoset.vertices()->vertices().begin();
	std::list<class mdlx::Normal*>::const_iterator normalIterator = geoset.normals()->normals().begin();
	std::list<class mdlx::TextureVertex*>::const_iterator textureVertexIterator = geoset.textureVertices()->textureVertices().begin();
	Ogre::uint32 index = 0;

	while (vertexIterator != geoset.vertices()->vertices().end())
	{
		object->position((*vertexIterator)->vertexData().x, (*vertexIterator)->vertexData().y, (*vertexIterator)->vertexData().z);
		object->normal((*normalIterator)->vertexData().x, (*normalIterator)->vertexData().y, (*normalIterator)->vertexData().z);
		object->textureCoord((*textureVertexIterator)->x(), (*textureVertexIterator)->y());
		object->index(index);

		++vertexIterator;
		++normalIterator;
		++textureVertexIterator;
		++index;
	}

	qDebug() << "Built " << index << " vertices.";

	// build primitives
	std::list<class mdlx::PrimitiveType*>::const_iterator pTypeIterator = geoset.primitiveTypes()->primitiveTypes().begin();
	std::list<class mdlx::PrimitiveSize*>::const_iterator pSizeIterator = geoset.primitiveSizes()->primitiveSizes().begin();
	std::list<class mdlx::PrimitiveVertex*>::const_iterator pVertexIterator = geoset.primitiveVertices()->primitiveVertices().begin();

	while (pTypeIterator != geoset.primitiveTypes()->primitiveTypes().end())
	{
		if ((*pTypeIterator)->type() == mdlx::PrimitiveType::Triangles)
		{
			for (mdlx::long32 i = 0; i < (*pSizeIterator)->value(); i += 3)
			{
				Ogre::uint32 indices[3];
				//qDebug() << "Triangle with size " << 3;

				for (mdlx::long32 size = 0; size < 3; ++size)
				{
					mdlx::short16 vertexIndex = (*pVertexIterator)->value();
					indices[size] = boost::numeric_cast<Ogre::uint32>(vertexIndex);
					++pVertexIterator;
				}

				object->triangle(indices[0], indices[1], indices[2]);
				//qDebug() << "Building triangle";
			}
		}
		else
		{
			/// @todo build other primitives (other than triangles)
			for (mdlx::long32 i = 0; i < (*pSizeIterator)->value(); ++i)
				++pVertexIterator; /// @todo triangles have 3 vertices, how much?
		}

		++pTypeIterator;
		++pSizeIterator;
	}

	object->end();

	return object;
}

std::map<const class Node*, Ogre::Node*> OgreMdlx::setupInheritance(const std::list<const class Node*> &initialNodes)
{
	std::list<const class Node*> resultingNodes(initialNodes);
	std::map<const class Node*, Ogre::Node*> nodes;

	/*
	BOOST_FOREACH(const class Node *node, resultingNodes)
	{
		// is parent
		if (!node->hasParent())
		{
			nodes[node] = new Ogre::Node(node->name());
			resultingNodes.remove(node);

			continue;
		}

		bool contained = false;
		const class Node *parent = this->m_mdlx->objectNode(*node);

		BOOST_FOREACH(const class Node *node, resultingNodes)
		{
			if (node == parent)
			{
				contained = true;

				break;
			}
		}

		// create as child, parent is already in map!
		if (!contained)
		{
			nodes[node] = nodes[parent]->createChild(node->name());
			resultingNodes.remove(node);
		}
	}
	*/

	return nodes;
}

}

}
