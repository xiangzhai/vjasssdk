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
		Mdlx();
		virtual ~Mdlx();

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

		void readMdl(std::istream &istream) throw (class Exception);
		void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

#ifdef BLEND
		void readBlend(const std::string &filePath) throw (class Exception);
		void writeBlend(std::istream &istream) throw (class Exception);
#endif

#ifdef MAX
		void read3ds(std::istream &istream) throw (class Exception);
		void write3ds(std::ostream &ostream) throw (class Exception);
#endif

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
