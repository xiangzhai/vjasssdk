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

#ifndef WC3LIB_MDLX_PARTICLEEMITTER_HPP
#define WC3LIB_MDLX_PARTICLEEMITTER_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;
class Translations;
class Rotations;
class Scalings;
class Visibilities;

class ParticleEmitter
{
	public:
		enum Flags
		{
			EmitterUsesMdl = 0x23,
			EmitterUsesTga = 0x08
		};

		ParticleEmitter(class Mdlx *mdlx);
		virtual ~ParticleEmitter();

		class Mdlx* mdlx() const;
		const ascii* name() const;
		long32 objectId() const;
		long32 parent() const;
		long32 flags() const;
		class Translations* translations() const;
		class Rotations* rotations() const;
		class Scalings* scalings() const;
		float32 emissionRate() const;
		float32 gravity() const;
		float32 longitude() const;
		float32 latitidue() const;
		const ascii* modelPath() const;
		long32 unknown0() const;
		float32 lifeSpan() const;
		float32 initVelocity() const;
		class Visibilities* visibilities() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		//long32 nbytesi;
		//long32 nbytesikg; // inclusive bytecount including KGXXs
		ascii m_name[0x50]; //(0x50 bytes)
		long32 m_objectId;
		long32 m_parent; //(0xFFFFFFFF if none)
		long32 m_flags; //(bit20)	// +bit23(EmitterUsesMDL) +bit8(EmitterUsesTGA)
		class Translations *m_translations; //(KGTR)
		class Rotations *m_rotations; //(KGRT)
		class Scalings *m_scalings; //(KGSC)
		float32 m_emissionRate;
		float32 m_gravity;
		float32 m_longitude;
		float32 m_latitidue;
		ascii m_modelPath[0x100]; //(0x100 bytes)
		long32 m_unknown0; //(0)
		float32 m_lifeSpan;
		float32 m_initVelocity;
		class Visibilities *m_visibilities; //(KPEV)
};

inline class Mdlx* ParticleEmitter::mdlx() const
{
	return this->m_mdlx;
}

inline const ascii* ParticleEmitter::name() const
{
	return this->m_name;
}

inline long32 ParticleEmitter::objectId() const
{
	return this->m_objectId;
}

inline long32 ParticleEmitter::parent() const
{
	return this->m_parent;
}

inline long32 ParticleEmitter::flags() const
{
	return this->m_flags;
}

inline class Translations* ParticleEmitter::translations() const
{
	return this->m_translations;
}

inline class Rotations* ParticleEmitter::rotations() const
{
	return this->m_rotations;
}

inline class Scalings* ParticleEmitter::scalings() const
{
	return this->m_scalings;
}

inline float32 ParticleEmitter::emissionRate() const
{
	return this->m_emissionRate;
}

inline float32 ParticleEmitter::gravity() const
{
	return this->m_gravity;
}

inline float32 ParticleEmitter::longitude() const
{
	return this->m_longitude;
}

inline float32 ParticleEmitter::latitidue() const
{
	return this->m_latitidue;
}

inline const ascii* ParticleEmitter::modelPath() const
{
	return this->m_modelPath;
}

inline long32 ParticleEmitter::unknown0() const
{
	return this->m_unknown0;
}

inline float32 ParticleEmitter::lifeSpan() const
{
	return this->m_lifeSpan;
}

inline float32 ParticleEmitter::initVelocity() const
{
	return this->m_initVelocity;
}

inline class Visibilities* ParticleEmitter::visibilities() const
{
	return this->m_visibilities;
}

}

}

#endif
