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

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include "ogremdlx.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

OgreMdlx::OgreMdlx(const class Mdlx &mdlx) : m_mdlx(&mdlx), m_mesh(0)
{
}

void OgreMdlx::refresh() throw (class Exception)
{
	// get new objects
	std::list<const class Node*> nodes = this->m_mdlx->nodes();

	BOOST_FOREACH(const class Node *node, nodes)
	{
		if (this->m_nodes.find(node) != this->m_nodes.end())
			nodes.remove(node);
	}

	// setup nodes of new objects (inheritance)
	/*
	std::map<const class Node*, Ogre::Node*> newNodes(this->setupInheritance(nodes));

	BOOST_FOREACH(NodePairType nodePair, newNodes)
		this->m_nodes.insert(nodePair);
	*/


	// refresh model
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
	std::size_t i = 0;

	BOOST_FOREACH(const class Geoset *geoset, this->m_mdlx->geosets()->geosets())
	{
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

	}

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
