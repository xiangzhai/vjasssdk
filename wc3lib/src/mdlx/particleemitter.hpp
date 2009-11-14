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

#include <iostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class ParticleEmitters;
class Translation1s;
class Rotation0s;
class Scaling0s;
class Visibility3s;

class ParticleEmitter
{
	public:
		enum Flags
		{
			EmitterUsesMdl = 0x23,
			EmitterUsesTga = 0x08
		};

		ParticleEmitter(class ParticleEmitters *particleEmitters);
		virtual ~ParticleEmitter();

		class ParticleEmitters* particleEmitters() const;
		const ascii* name() const;
		long32 objectId() const;
		long32 parent() const;
		long32 flags() const;
		class Translation1s* translations() const;
		class Rotation0s* rotations() const;
		class Scaling0s* scalings() const;
		float32 emissionRate() const;
		float32 gravity() const;
		float32 longitude() const;
		float32 latitidue() const;
		const ascii* modelPath() const;
		long32 unknown0() const;
		float32 lifeSpan() const;
		float32 initVelocity() const;
		class Visibility3s* visibilities() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

	protected:
		class ParticleEmitters *m_particleEmitters;
		//long32 nbytesi;
		//long32 nbytesikg; // inclusive bytecount including KGXXs
		ascii m_name[0x50]; //(0x50 bytes)
		long32 m_objectId;
		long32 m_parent; //(0xFFFFFFFF if none)
		long32 m_flags; //(bit20)	// +bit23(EmitterUsesMDL) +bit8(EmitterUsesTGA)
		class Translation1s *m_translations; //(KGTR)
		class Rotation0s *m_rotations; //(KGRT)
		class Scaling0s *m_scalings; //(KGSC)
		float32 m_emissionRate;
		float32 m_gravity;
		float32 m_longitude;
		float32 m_latitidue;
		ascii m_modelPath[0x100]; //(0x100 bytes)
		long32 m_unknown0; //(0)
		float32 m_lifeSpan;
		float32 m_initVelocity;
		class Visibility3s *m_visibilities; //(KPEV)
};

inline class ParticleEmitters* ParticleEmitter::particleEmitters() const
{
	return this->m_particleEmitters;
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

inline class Translation1s* ParticleEmitter::translations() const
{
	return this->m_translations;
}

inline class Rotation0s* ParticleEmitter::rotations() const
{
	return this->m_rotations;
}

inline class Scaling0s* ParticleEmitter::scalings() const
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

inline class Visibility3s* ParticleEmitter::visibilities() const
{
	return this->m_visibilities;
}

}

}

#endif
