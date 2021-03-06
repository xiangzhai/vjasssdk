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

#include <QtCore>
#include <QtGui>

#include <KMessageBox>
#include <KLocale>
#include <KUrl>
#include <KFileDialog> /// TEST

#include <OgreCodec.h>

#include "ogremdlx.hpp"
#include "modelview.hpp"
#include "editor.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace editor
{

void OgreMdlx::updateCamera(const class mdlx::Camera &camera, Ogre::Camera *ogreCamera)
{
	ogreCamera->setPosition(Ogre::Vector3(camera.position().x, camera.position().y, camera.position().z));
	ogreCamera->setFOVy(Ogre::Radian(camera.fieldOfView()));
	ogreCamera->setFarClipDistance(camera.farClip());
	ogreCamera->setNearClipDistance(camera.nearClip());
	ogreCamera->setDirection(Ogre::Vector3(camera.target().x, camera.target().y, camera.target().z));
}

OgreMdlx::OgreMdlx(const KUrl &url, const class mdlx::Mdlx &mdlx, class ModelView *modelView) : Resource(url, Resource::Model), m_mdlx(&mdlx), m_modelView(modelView), m_sceneNode(0), m_teamColor(Red), m_teamGlow(Red)
{
}

OgreMdlx::~OgreMdlx()
{
	if (m_sceneNode != 0)
	{
		this->m_sceneNode->detachAllObjects();
		delete m_sceneNode;
	}
	
	BOOST_FOREACH(CollisionShapes::reference value, m_collisionShapes)
		delete value.second;
}

void OgreMdlx::setTeamColor(enum OgreMdlx::TeamColor teamColor)
{
	this->m_teamColor = teamColor;

	BOOST_FOREACH(Ogre::TexturePtr tex, this->m_teamColorTextures)
	{
		try
		{
			tex->loadImage(this->modelView()->editor()->teamColorImage(this->teamColor()));
		}
		catch (class Exception &exception)
		{
			KMessageBox::error(this->modelView(), i18n("Texture loading error:\n%1", exception.what().c_str()));
		}
	}
}

void OgreMdlx::setTeamGlow(enum OgreMdlx::TeamColor teamGlow)
{
	this->m_teamGlow = teamGlow;

	BOOST_FOREACH(Ogre::TexturePtr tex, this->m_teamGlowTextures)
	{
		try
		{
			tex->loadImage(this->modelView()->editor()->teamGlowImage(this->teamGlow()));
		}
		catch (class Exception &exception)
		{
			KMessageBox::error(this->modelView(), i18n("Texture loading error:\n%1", exception.what().c_str()));
		}
	}
}

void OgreMdlx::refresh() throw (class Exception, class Ogre::Exception)
{
	this->mdlx()->textures()->members().size(); // TEST
	Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_BOREME); // TEST
	this->m_sceneNode = this->m_modelView->sceneManager()->getRootSceneNode()->createChildSceneNode(mdlx()->model()->name());

	//this->modelView()->camera()->setAutoTracking(true, this->m_sceneNode); // camera follows ogre mdlx automatically

	// create textures
	BOOST_FOREACH(const mdlx::Texture *texture, this->mdlx()->textures()->textures())
		this->m_textures[texture] = this->createTexture(*texture);

	// create materials
	BOOST_FOREACH(const mdlx::Material *material, this->mdlx()->materials()->materials())
		this->m_materials[material] = this->createMaterial(*material);

	// create geosets
	BOOST_FOREACH(const mdlx::Geoset *geoset, this->mdlx()->geosets()->geosets())
		this->m_geosets[geoset] = this->createGeoset(*geoset);

	// create cameras
	BOOST_FOREACH(const mdlx::Camera *camera, this->mdlx()->cameras()->cameras())
		this->m_cameras[camera] = this->createCamera(*camera);
		
	BOOST_FOREACH(const mdlx::CollisionShape *collisionShape, this->mdlx()->collisionShapes()->collisionShapes())
		this->m_collisionShapes[collisionShape] = this->createCollisionShape(*collisionShape);

	Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_NORMAL); // TEST
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

void OgreMdlx::saveAs(const KUrl &url) throw (class Exception)
{
	QFileInfo fileInfo(url.toLocalFile());
	
	if (fileInfo.exists())
	{
		if (!fileInfo.isWritable())
			throw Exception(boost::format(_("File \"%1%\" is not writable.")) % url.toLocalFile().toUtf8().constData());
		else if (KMessageBox::questionYesNo(0, i18n("Target file does already exists. Would you like to overwrite it?")) == KMessageBox::No)
			return;
	}
	
	QString extension(fileInfo.suffix().toLower());
	
	if (extension == "mdx" || extension == "mdl")
	{
		std::ios_base::openmode openmode = std::ios_base::out;
		bool isMdx;

		if (extension == "mdx")
		{
			isMdx = true;
			openmode |= std::ios_base::binary;
		}
		else
			isMdx = false;

		boost::filesystem::basic_ofstream<mdlx::byte> ofstream(url.path().toUtf8().constData(), openmode);
		
		if (!ofstream)
			throw Exception(boost::format(_("Error when opening file \"%1%\".")) % url.toLocalFile().toUtf8().constData());

		std::streamsize size;

		if (isMdx)
			size = mdlx()->writeMdx(ofstream);
		else
			size = mdlx()->writeMdl(ofstream);

		KMessageBox::information(0, i18n("Wrote %1 file \"%2\" successfully.\nSize: %3.", isMdx ? i18n("MDX") : i18n("MDL"), url.toLocalFile(), sizeStringBinary(size).c_str()));
	}
	else if (extension == "mesh")
	{
		Ogre::MeshSerializer *serializer = new Ogre::MeshSerializer();
	
		BOOST_FOREACH(Geosets::const_reference value, m_geosets)
		{
			Ogre::ManualObject *manualObject = value.second;
			std::ostringstream sstream;
			sstream << manualObject->getName();
			sstream << "Mesh";
			const Ogre::MeshPtr mesh = manualObject->convertToMesh(sstream.str());
			
			const QFileInfo info(url.toLocalFile().toUtf8());
			KUrl geosetUrl(info.absoluteDir().absolutePath());
			QString fileName(info.baseName() + sstream.str().c_str());
			fileName += '.' + info.completeSuffix();
			geosetUrl.addPath(fileName);
			qDebug() << "Geoset URL: " << geosetUrl.toLocalFile();
			serializer->exportMesh(mesh.getPointer(), geosetUrl.toLocalFile().toUtf8().constData());
		}
		
		delete serializer;
	}
	else
		throw Exception(boost::format(_("Format \"%1%\" is not supported.")) % extension.toUtf8().constData());
		
	
	
	/*
	TODO
	typedef std::pair<const mdlx::Geoset*, Ogre::ManualObject*> EntryType;


	BOOST_FOREACH(EntryType entry, this->m_geosets)
		;

	Ogre::MeshSerializer serializer = new MeshSerializer();
	*/


//	serializer.ExportMesh(Target.ConvertToMesh(MeshName), "Ucgen.mesh");
}

Ogre::Node* OgreMdlx::createNode(const class mdlx::Node &node)
{
	Ogre::Node *result = 0;

	if (node.type() & mdlx::Node::Helper)
		return 0;

	if (node.type() & mdlx::Node::Bone)
		return 0;

	if (node.type() & mdlx::Node::Light)
		return 0;

	if (node.type() & mdlx::Node::EventObject)
		return 0;

	if (node.type() & mdlx::Node::Attachment)
		return 0;

	if (node.type() & mdlx::Node::ParticleEmitter)
		return 0;

	if (node.type() & mdlx::Node::CollisionShape)
		return 0;

	if (node.type() & mdlx::Node::RibbonEmitter)
		return 0;

	return result;
}

QString OgreMdlx::namePrefix() const
{
	return this->m_mdlx->model()->name();
}

mdlx::long32 OgreMdlx::mdlxId(const mdlx::GroupMdxBlockMember &member, const mdlx::GroupMdxBlock *block) const
{
	mdlx::long32 id = 0;

	BOOST_FOREACH(const mdlx::GroupMdxBlockMember *mem, block->members())
	{
		qDebug() << "id " << id;
		if (mem == &member)
			break;

		++id;
	}
	
	return id;
}

bool OgreMdlx::useDirectoryUrl(KUrl &url, bool showMessage) const
{
	if (!QFileInfo(this->modelView()->editor()->findFile(url).toLocalFile()).exists())
	{
		KUrl newUrl = this->url().directory();
		newUrl.addPath(QFileInfo(url.toLocalFile()).filePath());
		
		if (showMessage)
			KMessageBox::information(this->modelView(), i18n("No valid texture resource was found at \"%1\". Trying directory URL \"%2\".", url.toLocalFile(), newUrl.toLocalFile()));
		
		url = newUrl;
		
		return true;
	}
	
	return false;
}

Ogre::TexturePtr OgreMdlx::createTexture(const class mdlx::Texture &texture) throw (class Exception)
{
	//texture.parent()->members().size();
	//qDebug() << "First address " << this->mdlx()->textures() << " and second address " << texture.parent();
	//abort();
	mdlx::long32 id = mdlxId(texture, this->mdlx()->textures());

	Ogre::TexturePtr tex =
	this->m_modelView->root()->getTextureManager()->create((boost::format("%1%.Texture%2%") % namePrefix().toUtf8().constData() % id).str().c_str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
	QString texturePath(texture.texturePath());
	// converts \ to / on Unix systems
#ifdef Q_OS_UNIX
	texturePath.replace('\\', '/');
#elif defined Q_OS_WIN32
	texturePath.replace('/', '\\');
#else
#warning Unsupported OS?
#endif
	KUrl url(texturePath);

	qDebug() << "Texture with path " << texturePath << " and replaceable id " << texture.replaceableId();
	
	// use given texture path
	if (texture.replaceableId() == mdlx::None)
		useDirectoryUrl(url); // uses directory URL if global doesn't exist
	
	// replace with replaceable id -> replaceable textures should be cached by editor, therefore you have to check if you have to use a local file
	else
	{
		url = "ReplaceableTextures";

		switch (texture.replaceableId())
		{
			case mdlx::TeamColor:
			{
				url = this->modelView()->editor()->teamColorUrl(this->teamColor());
				this->m_teamColorTextures.push_back(tex);
			
				// use global image file and finish if there is one otherwise load local file
				if (!useDirectoryUrl(url))
				{
					// ReplaceableTextures\\TeamColor\\TeamColor0x.blp
					try
					{
						tex->loadImage(this->modelView()->editor()->teamColorImage(this->teamColor()));
					}
					catch (class Exception &exception)
					{
						KMessageBox::error(this->modelView(), i18n("Texture loading error:\n%1", exception.what().c_str()));
					}
					
					return tex;
				}
				
				break;
			}

			case mdlx::TeamGlow:
			{
				this->m_teamGlowTextures.push_back(tex);
				url = this->modelView()->editor()->teamGlowUrl(this->teamGlow());
				
				// use global image file and finish if there is one otherwise load local file
				if (!useDirectoryUrl(url))
				{
					// ReplaceableTextures\\TeamGlow\\TeamGlow0x.blp
					try
					{
						tex->loadImage(this->modelView()->editor()->teamGlowImage(this->teamGlow()));
					}
					catch (class Exception &exception)
					{
						KMessageBox::error(this->modelView(), i18n("Texture loading error:\n%1", exception.what().c_str()));
					}

					return tex; // we finished!
				}

				qDebug() << "TEAM GLOW!!!";
				
				break;
			}

			case mdlx::Cliff:
				url.addPath("Cliff/Cliff0.blp");

				break;

			case mdlx::LordaeronTree:
				url.addPath("LordaeronTree/LordaeronSummerTree.blp");

				break;

			case mdlx::AshenvaleTree:
				url.addPath("AshenvaleTree/AshenTree.blp");

				break;

			case mdlx::BarrensTree:
				url.addPath("BarrensTree/BarrensTree.blp");

				break;

			case mdlx::NorthrendTree:
				url.addPath("NorthrendTree/NorthTree.blp");

				break;

			case mdlx::MushroomTree:
				url.addPath("Mushroom/MushroomTree.blp");

				break;
		}
	}

	Ogre::Image *image = 0;

	try
	{
		image = this->modelView()->editor()->blpToOgre(url);
	}
	catch (class Exception &exception)
	{
		KMessageBox::error(this->modelView(), i18n("Texture loading error:\n%1", exception.what().c_str()));
	}

	if (image != 0)
	{
		tex->loadImage(*image);

		/// \todo Apply texture properties:
		switch (texture.wrapping())
		{
			case mdlx::Texture::WrapWidth:
				KMessageBox::error(this->modelView(), i18n("Unsupported texture wrapping type:\nWrapWidth."));

				break;

			case mdlx::Texture::WrapHeight:
				KMessageBox::error(this->modelView(), i18n("Unsupported texture wrapping type:\nWrapHeight."));

				break;

			case mdlx::Texture::Both:
				KMessageBox::error(this->modelView(), i18n("Unsupported texture wrapping type:\nBoth."));

				break;
		}

		delete image;
		image = 0;
	}

	return tex;
}

Ogre::MaterialPtr OgreMdlx::createMaterial(const class mdlx::Material &material) throw (class Exception)
{
	mdlx::long32 id = mdlxId(material, material.parent());

	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create((boost::format("%1%.Material%2%") % namePrefix().toUtf8().constData() % id).str().c_str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); // material->mdlx()->model()->name()

	// properties
	/*
	ConstantColor,
	SortPrimsFarZ,
	FullResolution,
	PriorityPlane <long>,
	*/
	if (material.renderMode() & mdlx::Material::ConstantColor)
		;
			
	if (material.renderMode() & mdlx::Material::SortPrimitivesFarZ)
		;
			
	if (material.renderMode() & mdlx::Material::SortPrimitivesNearZ)
		;
	
	if (material.renderMode() & mdlx::Material::FullResolution)
		;
	
	//material->prio
	//mat->set

	// layers do have texture id!!!
	// use Ogre::Technique?

	mat->removeAllTechniques(); // there's always one default technique

	BOOST_FOREACH(const class mdlx::Layer *layer, material.layers()->layers())
	{
		Ogre::Technique *technique = mat->createTechnique();
		technique->removeAllPasses(); // there shouldn't be any default pass, anyway, we want to make sure
		Ogre::Pass *pass = technique->createPass();
		//pass->setFog (bool overrideScene, FogMode mode=FOG_NON
		// TEST
		Ogre::Material::TechniqueIterator iterator = mat->getTechniqueIterator();
		int i = 0;
		while (iterator.hasMoreElements())
		{
			iterator.getNext();
			++i;
		}

		qDebug() << "We have " << i << " techniques.";
		// TEST END

		Ogre::TextureUnitState *textureUnitState = pass->createTextureUnitState((boost::format("%1%.Texture%2%") % namePrefix().toUtf8().constData() % layer->textureId()).str().c_str(), layer->coordinatesId());

		//textureUnitState->setTextureFiltering();
		//textureUnitState->setAlphaOperation(Ogre::LBX_MODULATE_X4);
		//float32	alpha() const; TODO set alpha!
		technique->setLightingEnabled(false); // default value

		switch (layer->filterMode())
		{
			case mdlx::Layer::Transparent:
				textureUnitState->setIsAlpha(true);

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

		if (layer->shading() == mdlx::Layer::Unshaded)
		{
		}
		else
		{
			if (layer->shading() & mdlx::Layer::SphereEnvironmentMap)
				qDebug() << "Material: shading type \"SphereEnvironmentMap\" is not supported.";

			if (layer->shading() & mdlx::Layer::Unknown0)
				qDebug() << "Material: shading type \"Unknown0\" is not supported.";;

			if (layer->shading() & mdlx::Layer::Unknown1)
				qDebug() << "Material: shading type \"Unknown1\" is not supported.";

			if (layer->shading() & mdlx::Layer::TwoSided)
				technique->setCullingMode(Ogre::CULL_NONE);

			if (layer->shading() & mdlx::Layer::Unfogged)
				qDebug() << "Material: shading type \"Unfogged\" is not supported.";;

			if (layer->shading() & mdlx::Layer::NoDepthTest)
				qDebug() << "Material: shading type \"NoDepthTest\" is not supported.";;

			if (layer->shading() & mdlx::Layer::NoDepthSet)
				qDebug() << "Material: shading type \"NoDepthSet\" is not supported.";;
		}

		/*
		Texture animation stuff
		BOOST_FOREACH(const class mdlx::TextureId *textureId, layer->textureIds())
		{
			//textureId->frame()
		}
		*/
	}

	// TEST BLOCK
	/*
	static bool test = false;

	if (!test)
	{
		qDebug() << "Creating plane!!!!!!!!!!!!!!!!!!!!!!!";
		Ogre::Plane plane;
		plane.normal = Ogre::Vector3::UNIT_Y;
		plane.d = 0;
		this->m_modelView->root()->getMeshManager()->createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 256.0f, 256.0f, 10, 10, true, 1, 50.0f, 50.0f, Ogre::Vector3::UNIT_Z);
		Ogre::Entity* planeEnt = this->m_modelView->sceneManager()->createEntity("plane", "floor");
		planeEnt->setMaterialName(boost::str(boost::format("%1%.Material0") % namePrefix().toUtf8().constData()).c_str());
		planeEnt->setCastShadows(false);
		this->m_sceneNode->attachObject(planeEnt);
		test = true;
	}
	*/
	// END TEST

	return mat;
}

Ogre::ManualObject* OgreMdlx::createGeoset(const class mdlx::Geoset &geoset) throw (class Exception)
{
	qDebug() << "Creating geoset";
	mdlx::long32 id = mdlxId(geoset, geoset.parent());

	Ogre::ManualObject *object = this->modelView()->sceneManager()->createManualObject((boost::format("%1%.Geoset%2%") % namePrefix().toUtf8().constData() % id).str().c_str());
	//object->setKeepDeclarationOrder(true);
	qDebug() << "Creating geoset";

	// get corresponding material by id
	const class mdlx::Material *material = 0;
	mdlx::long32 materialId = 0;

	BOOST_FOREACH(material, this->m_mdlx->materials()->materials())
	{
		if (materialId == geoset.materialId())
			break;

		++materialId;
	}

	if (materialId != geoset.materialId())
		throw Exception(boost::format(_("Missing material %1%")) % id);
	
	
	object->setRenderQueueGroupAndPriority(object->getRenderQueueGroup(), boost::numeric_cast<Ogre::ushort>(material->priorityPlane())); // set priority by corresponding material -> TODO should be applied for material?

	// increase processor performance
	object->estimateVertexCount(geoset.vertices()->vertices().size());
	object->estimateIndexCount(geoset.vertices()->vertices().size());
	object->begin(this->m_materials[material]->getName(), Ogre::RenderOperation::OT_TRIANGLE_LIST);
	// build vertices
	std::list<class mdlx::Vertex*>::const_iterator vertexIterator = geoset.vertices()->vertices().begin();
	std::list<class mdlx::Normal*>::const_iterator normalIterator = geoset.normals()->normals().begin();
	std::list<class mdlx::TextureVertex*>::const_iterator textureVertexIterator = geoset.textureVertices()->textureVertices().begin();
	Ogre::uint32 index = 0;
	//qDebug() << "TVertices " << geoset.textureVertices()->textureVertices().size() << "{";
	//qDebug() << "Normals " << geoset.normals()->normals().size() << "(";
	//qDebug() << "Vertices " << geoset.vertices()->vertices().size() << "(";
	
	while (vertexIterator != geoset.vertices()->vertices().end())
	{
		//qDebug() << "Adding vertex (" << (*vertexIterator)->vertexData().x << "|" << (*vertexIterator)->vertexData().y << "|" << (*vertexIterator)->vertexData().z << ")";
		//qDebug() << "Adding normal (" << (*normalIterator)->vertexData().x << "|" << (*normalIterator)->vertexData().y << "|" << (*normalIterator)->vertexData().z << ")";
		//qDebug() << "Adding texture coordinates (" << (*textureVertexIterator)->x() << "|" << (*textureVertexIterator)->y() << ")";
		object->position((*vertexIterator)->vertexData().x, (*vertexIterator)->vertexData().y, (*vertexIterator)->vertexData().z);
		object->normal((*normalIterator)->vertexData().x, (*normalIterator)->vertexData().y, (*normalIterator)->vertexData().z);
		object->textureCoord((*textureVertexIterator)->x(), (*textureVertexIterator)->y());
		//object->colour(1.0 - this->modelView()->viewPort()->getBackgroundColour().r, 1.0 - this->modelView()->viewPort()->getBackgroundColour().g, 1.0 - this->modelView()->viewPort()->getBackgroundColour().b, 1.0 - this->modelView()->viewPort()->getBackgroundColour().a);
		//object->index(index);
		
		//qDebug() << "{ " << (*textureVertexIterator)->x() << ", " << (*textureVertexIterator)->y() << " },";
		//qDebug() << "{ " << (*normalIterator)->vertexData().x << ", " << (*normalIterator)->vertexData().y << ", " << (*normalIterator)->vertexData().z << " },";
		//qDebug() << "{ " << (*vertexIterator)->vertexData().x << ", " << (*vertexIterator)->vertexData().y << ", " << (*vertexIterator)->vertexData().z << " },";
		qDebug() << "Index " << index;

		++vertexIterator;
		++normalIterator;
		++textureVertexIterator;
		++index;
	}

	qDebug() << "Built " << index << " vertices.";
	
	if (geoset.primitiveTypes()->primitiveTypes().size() != geoset.primitiveSizes()->primitiveSizes().size())
		throw Exception((boost::format(_("Primitives error: Types size (%1%) and sizes size (%2%) are not equal.")) % geoset.primitiveTypes()->primitiveTypes().size()) % geoset.primitiveSizes()->primitiveSizes().size());

	// build primitives
	std::list<class mdlx::PrimitiveType*>::const_iterator pTypeIterator = geoset.primitiveTypes()->primitiveTypes().begin();
	std::list<class mdlx::PrimitiveSize*>::const_iterator pSizeIterator = geoset.primitiveSizes()->primitiveSizes().begin();
	std::list<class mdlx::PrimitiveVertex*>::const_iterator pVertexIterator = geoset.primitiveVertices()->primitiveVertices().begin();

	qDebug() << "Number of primitives " << geoset.primitiveSizes()->primitiveSizes().size();
	qDebug() << "Number of primitive types " << geoset.primitiveTypes()->primitiveTypes().size();
	qDebug() << "Faces " << geoset.primitiveSizes()->primitiveSizes().size() << " " << geoset.primitiveVertices()->primitiveVertices().size() << "{\nTriangles {";
	QString result("{ ");
	
	while (pTypeIterator != geoset.primitiveTypes()->primitiveTypes().end())
	{
		if ((*pTypeIterator)->type() == mdlx::PrimitiveType::Triangles)
		{
			
			//qDebug() << (*pSizeIterator)->value() << " triangles.";

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

				result += QString("%1, %2, %3, ").arg(indices[0]).arg(indices[1]).arg(indices[2]);
				//qDebug() << "Building triangle with vertex indices (" << indices[0] << "|" << indices[1] << "|" << indices[2] << ")";
				object->triangle(indices[0], indices[1], indices[2]);
				//qDebug() << "Building triangle";
			}
		}
		else
		{
			KMessageBox::error(this->modelView(), i18n("Unsupported primitive type:\n%1", (*pTypeIterator)->type()));

			/// \todo build other primitives (other than triangles)
			for (mdlx::long32 i = 0; i < (*pSizeIterator)->value(); ++i)
				++pVertexIterator; /// \todo triangles have 3 vertices, how much?
		}

		++pTypeIterator;
		++pSizeIterator;
	}

	result += "}\n}";
	qDebug() << result.toUtf8().constData();
	object->end();

	// set bounds
	/// \todo Set Bounding radius (hit test?)
	//object->setBoundingSphereRadius(geoset->boundsRadius());
	/*object->setBoundingBox(Ogre::AxisAlignedBox(
		geoset.minimumExtent().x,
		geoset.minimumExtent().y,
		geoset.minimumExtent().z,
		geoset.maximumExtent().x,
		geoset.maximumExtent().y,
		geoset.maximumExtent().z
		)
	);*/

	this->m_sceneNode->attachObject(object);

	return object;
}

Ogre::Camera* OgreMdlx::createCamera(const class mdlx::Camera &camera) throw (class Exception)
{
	Ogre::Camera *ogreCamera = this->modelView()->sceneManager()->createCamera((boost::format("%1%.Camera.%2%") % namePrefix().toUtf8().constData() % camera.name()).str().c_str());

	updateCamera(camera, ogreCamera);

	this->m_sceneNode->attachObject(ogreCamera);

	return ogreCamera;
}

OgreMdlx::CollisionShape* OgreMdlx::createCollisionShape(const class mdlx::CollisionShape &collisionShape) throw (class Exception)
{
	CollisionShape *cs = new CollisionShape();
	cs->shape = collisionShape.shape();
	
	if (collisionShape.shape() == mdlx::CollisionShape::Box)
		cs->box = new Ogre::AxisAlignedBox(ogreVector3(collisionShape.vertexData()), ogreVector3(collisionShape.boxVertexData()));
	// sphere
	else
		cs->sphere = new Ogre::Sphere(ogreVector3(collisionShape.vertexData()), ogreReal(collisionShape.boundsRadius()));
	
	return cs;
}

std::map<const class mdlx::Node*, Ogre::Node*> OgreMdlx::setupInheritance(const std::list<const class mdlx::Node*> &initialNodes)
{
	std::list<const class mdlx::Node*> resultingNodes(initialNodes);
	std::map<const class mdlx::Node*, Ogre::Node*> nodes;

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
