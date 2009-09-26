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

#ifndef WC3LIB_MDLX_MDLX_HPP
#define WC3LIB_MDLX_MDLX_HPP

#include <fstream>

#include "mdxblock.hpp"
#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Version;
class Model;
class Sequences;
class GlobalSequences;
class Materials;
class Textures;
class TextureAnimations;
class Geosets;
class GeosetAnimations;
class Bones;
class Lights;
class Helpers;
class Attachments;
class PivotPoints;
class ParticleEmitters;
class ParticleEmitter2s;
class RibbonEmitters;
class Cameras;
class Events;
class CollisionShapes;

/**
* long/float size: 32-bit
* @todo OBJ == ?!
* @todo KATV == float?!
*/
class Mdlx : public MdxBlock
{
	public:
		/*
		struct Bones
		{
			//long nbytes;
			std::list<struct Bone*> bones; //[nbons]
		};

		struct PrimitiveVertices
		{
			//long	ntris;
			std::list<short32> triangles; //[ntris];
		};

		struct VertexGroups
		{
			//long	nvgrps;
			std::list<byte>	vertexGroups; //[nvgrps];
		};

		struct GeosetAnimations
		{
			//long	nbytes;
			std::list<struct GeosetAnimation*> geosetAnimations;
		};

		struct MatriceGroupCounts
		{
			//--MTGC // [Groups]
			//long	nmtrcs; //   GroupCount is an array of lengths
			std::list<long32> groupCounts; //[nmtrcs]; //   for each group.
		};

		struct Vertices
		{
			//long	nvrts;
			std::list<struct Vertice*> vertices; //[nvrts]
		};

		struct Normals
		{
			std::list<struct Normal*> normals; //[nnrms];
		};

		//--UVBS // [TVertices] (nonexistant in v1300)
		struct TextureVertices
		{
			std::list<struct Tvertice*> tvertices;
		};

		//KGRT						// [Rotation]
		struct Rotations
		{
			//Rotations
			//long nunks;
			long32 lineType; //(0:don't interp;1:linear;2:hermite;3:bezier)
			long32 globalSequenceId; // 0xFFFFFFFF if none
			std::list<struct Rotation*> m_rotations; //[nunks]
		};
		*/

		Mdlx();
		~Mdlx();

		class Version* version() const;
		class Model* model() const;
		class Sequences* sequences() const;
		class GlobalSequences* globalSequences() const;
		class Materials* materials() const;
		class Textures* textures() const;
		class TextureAnimations* textureAnimations() const;
		class Geosets* geosets() const;
		class GeosetAnimations* geosetAnimations() const;
		class Bones* bones() const;
		class Lights* lights() const;
		class Helpers* helpers() const;
		class Attachments* attachments() const;
		class PivotPoints* pivotPoints() const;
		class ParticleEmitters* particleEmitters() const;
		class ParticleEmitter2s* particleEmitter2s() const;
		class RibbonEmitters* ribbonEmitters() const;
		class Cameras* cameras() const;
		class Events* events() const;
		class CollisionShapes* collisionShapes() const;

		void readMdl(std::fstream &fstream) throw (class Exception);
		void writeMdl(std::fstream &fstream) throw (class Exception);
		void readMdx(std::fstream &fstream) throw (class Exception);
		void writeMdx(std::fstream &fstream) throw (class Exception);
		void readBlend(std::fstream &fstream) throw (class Exception);
		void writeBlend(std::fstream &fstream) throw (class Exception);
		void read3ds(std::fstream &fstream) throw (class Exception);
		void write3ds(std::fstream &fstream) throw (class Exception);

	protected:
		class Version *m_version; //VERS
		class Model *m_model; //MODL
		class Sequences *m_sequences; //SEQS
		class GlobalSequences *m_globalSequences; //GLBS
		class Materials *m_materials; //MTLS
			//class Layers *m_layers; //LAYS
			//class Alphas *m_alphas; //KMTA
			//class  TextureIds *m_textureIds; //KMTF
		class Textures *m_textures; //TEXS
		class TextureAnimations *m_textureAnimations; //TXAN
			//class Translations *m_translations; //KTAT
			//class Rotations *m_rotations; //KGRT
			//class Scalings *m_scalings; //KGSC
		class Geosets *m_geosets;
		class GeosetAnimations *m_geosetAnimations;
		class Bones *m_bones;
		class Lights *m_lights; //LITE
		class Helpers *m_helpers;
		class Attachments *m_attachments;
		class PivotPoints *m_pivotPoints; //PIVT
		class ParticleEmitters *m_particleEmitters; //PREM
		class ParticleEmitter2s *m_particleEmitter2s; //PRE2
		class RibbonEmitters *m_ribbonEmitters; //RIBB
		class Cameras *m_cameras;
		class Events *m_events;
		class CollisionShapes *m_collisionShapes;

		/*
		class Visibilities0 *m_visibilities0; //KATV
		class Visibilities1 *m_visibilities1; //KLAV
		class Visibilities2 *m_visibilities2; //KP2V
		class Visibilities3 *m_visibilities3; //KPEV
		class Visibilities4 *m_visibilities4; //KRVS
		KGAO	// [Alpha]:	  KMTA;
		KLAI	// [Intensity]:   KMTA;
		KLBI	// [AmbIntensity]:KMTA;
		KMTF	// [TextureID]:	  KMTA; -> state is long value not float
		KP2E	// [EmissnRate]:  KMTA;
		KP2L	// [Latitude]: 	  KMTA;
		KP2N	// [Length]: 	  KMTA;
		KP2S	// [Speed]:	  KMTA;
		KP2W	// [Width]: 	  KMTA;
		KRHA	// [HeightAbove]: KMTA;
		KRHB	// [HeightBelow]: KMTA;
		KCRL 	// [Rotation]:	  KMTA;
		KGAC	// [Color]:	  KGSC;
		KLAC	// [Color]:	  KGSC;
		KLBC	// [AmbColor]:	  KGSC;
		KCTR 	// [Translation]: KGSC;
		KGTR 	// [Translation]: KGSC;
		KTAT	// [Translation]: KGSC;
		KTAS	// [Scaling]: 	  KGSC;
		KTAR	// [Rotation]:	  KGSC;
		KTTR	// [Translation]: KGSC;
		*/
};

inline class Version* Mdlx::version() const
{
	return this->m_version;
}

inline class Model* Mdlx::model() const
{
	return this->m_model;
}

inline class Sequences* Mdlx::sequences() const
{
	return this->m_sequences;
}

inline class GlobalSequences* Mdlx::globalSequences() const
{
	return this->m_globalSequences;
}

inline class Materials* Mdlx::materials() const
{
	return this->m_materials;
}

inline class Textures* Mdlx::textures() const
{
	return this->m_textures;
}

inline class TextureAnimations* Mdlx::textureAnimations() const
{
	return this->m_textureAnimations;
}

inline class Geosets* Mdlx::geosets() const
{
	return this->m_geosets;
}

inline class GeosetAnimations* Mdlx::geosetAnimations() const
{
	return this->m_geosetAnimations;
}

inline class Bones* Mdlx::bones() const
{
	return this->m_bones;
}

inline class Lights* Mdlx::lights() const
{
	return this->m_lights;
}

inline class Helpers* Mdlx::helpers() const
{
	return this->m_helpers;
}

inline class Attachments* Mdlx::attachments() const
{
	return this->m_attachments;
}

inline class PivotPoints* Mdlx::pivotPoints() const
{
	return this->m_pivotPoints;
}

inline class ParticleEmitters* Mdlx::particleEmitters() const
{
	return this->m_particleEmitters;
}

inline class ParticleEmitter2s* Mdlx::particleEmitter2s() const
{
	return this->m_particleEmitter2s;
}

inline class RibbonEmitters* Mdlx::ribbonEmitters() const
{
	return this->m_ribbonEmitters;
}

inline class Cameras* Mdlx::cameras() const
{
	return this->m_cameras;
}

inline class Events* Mdlx::events() const
{
	return this->m_events;
}

inline class CollisionShapes* Mdlx::collisionShapes() const
{
	return this->m_collisionShapes;
}

}

}

#endif
