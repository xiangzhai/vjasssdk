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

#ifndef WC3LIB_MDLX_PARTICLEEMITTER2_HPP
#define WC3LIB_MDLX_PARTICLEEMITTER2_HPP

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
class SegmentColors;
class Speeds;
class Latitudes;
class EmissionRates;
class Visibility2s;
class Lengths;
class Widths;

class ParticleEmitter2
{
	public:
		enum Flags
		{
			DontInheritRotation = 0x26,
			Unshaded = 0x23,
			XYQuad = 0x12,
			Unfogged = 0x10,
			ModelSpace = 0x11,
			LineEmitter = 0x09,
			SortPrimsFarZ = 0x08
		};

		enum FilterMode
		{
			Blend = 0,
			Additive = 1,
			Modulate = 2,
			AlphaKey = 4
		};

		enum Flag2
		{
			Head = 0,
			Tail = 1,
			Both = 2
		};

		ParticleEmitter2(class Mdlx *mdlx);
		virtual ~ParticleEmitter2();

		class Mdlx* mdlx() const;
		const ascii* name() const;
		long32 objectId() const;
		long32 parent() const;
		long32 flags() const;
		class Translations* translations() const;
		class Rotations* rotations() const;
		class Scalings* scalings() const;
		float32 speed() const;
		float32 variation() const;
		float32 latitidue() const;
		float32 gravity() const;
		float32 lifespan() const;
		float32 emissionRate() const;
		float32 length() const;
		float32 width() const;
		long32 filterMode() const;
		long32 rows() const;
		long32 columns() const;
		long32 flag2() const;
		float32 tailLength() const;
		float32 time() const;
		class SegmentColors* segmentColors() const;
		byte alpha1() const;
		byte alpha2() const;
		byte alpha3() const;
		float32 scalingX() const;
		float32 scalingY() const;
		float32 scalingZ() const;
		long32 lifeSpanUvAnim1() const;
		long32 lifeSpanUvAnim2() const;
		long32 lifeSpanUvAnim3() const;
		long32 decayUvAnim1() const;
		long32 decayUvAnim2() const;
		long32 decayUvAnim3() const;
		long32 tailUvAnim1() const;
		long32 tailUvAnim2() const;
		long32 tailUvAnim3() const;
		long32 tailDecayUvAnim1() const;
		long32 tailDecayUvAnim2() const;
		long32 tailDecayUvAnim3() const;
		long32 textureId() const;
		long32 squirt() const;
		long32 priorityPlane() const;
		long32 replaceableId() const;
		class Speeds* speeds() const;
		class Latitudes* latitudes() const;
		class EmissionRates* emissionRates() const;
		class Visibility2s* visibilities() const;
		class Lengths* lengths() const;
		class Widths* widths() const;

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
		long32 m_flags; //(bit20)	// +bit26(DontInherit Rotation)
		class Translations *m_translations; //(KGTR) // +bit23(Unshaded)	+bit10(Unfogged)
		class Rotations *m_rotations; //(KGRT) // +bit12(XYQuad)	+bit9(LineEmitter)
		class Scalings *m_scalings; //(KGSC) // +bit11(ModelSpace)	+bit8(SortPrimsFarZ)
		float32 m_speed;
		float32 m_variation;
		float32 m_latitidue;
		float32 m_gravity;
		float32 m_lifespan;
		float32 m_emissionRate;
		float32 m_length;
		float32 m_width;
		long32 m_filterMode; //(0:Blend;1:Additive;2:Modulate;4:AlphaKey)
		long32 m_rows;
		long32 m_columns;
		long32 m_flag2; //(0:Head;1:Tail;2:Both)
		float32 m_tailLength;
		float32 m_time;
		class SegmentColors *m_segmentColors;
		byte m_alpha1, m_alpha2, m_alpha3; //byte
		float32 m_scalingX, m_scalingY, m_scalingZ;
		long32 m_lifeSpanUvAnim1, m_lifeSpanUvAnim2, m_lifeSpanUvAnim3;
		long32 m_decayUvAnim1, m_decayUvAnim2, m_decayUvAnim3;
		long32 m_tailUvAnim1, m_tailUvAnim2, m_tailUvAnim3;
		long32 m_tailDecayUvAnim1, m_tailDecayUvAnim2, m_tailDecayUvAnim3;
		long32 m_textureId;
		long32 m_squirt; //(1:Squirt)
		long32 m_priorityPlane;
		long32 m_replaceableId;
		class Speeds *m_speeds; //(KP2S)
		class Latitudes *m_latitudes; //(KP2L)
		class EmissionRates *m_emissionRates; //(KP2E)
		class Visibility2s *m_visibilities; //(KP2V)
		class Lengths *m_lengths; //(KP2N)
		class Widths *m_widths; //(KP2W)
};

inline class Mdlx* ParticleEmitter2::mdlx() const
{
	return this->m_mdlx;
}

inline const ascii* ParticleEmitter2::name() const
{
	return this->m_name;
}

inline long32 ParticleEmitter2::objectId() const
{
	return this->m_objectId;
}

inline long32 ParticleEmitter2::parent() const
{
	return this->m_parent;
}

inline long32 ParticleEmitter2::flags() const
{
	return this->m_flags;
}

inline class Translations* ParticleEmitter2::translations() const
{
	return this->m_translations;
}

inline class Rotations* ParticleEmitter2::rotations() const
{
	return this->m_rotations;
}

inline class Scalings* ParticleEmitter2::scalings() const
{
	return this->m_scalings;
}

inline float32 ParticleEmitter2::speed() const
{
	return this->m_speed;
}

inline float32 ParticleEmitter2::variation() const
{
	return this->m_variation;
}

inline float32 ParticleEmitter2::latitidue() const
{
	return this->m_latitidue;
}

inline float32 ParticleEmitter2::gravity() const
{
	return this->m_gravity;
}

inline float32 ParticleEmitter2::lifespan() const
{
	return this->m_lifespan;
}

inline float32 ParticleEmitter2::emissionRate() const
{
	return this->m_emissionRate;
}

inline float32 ParticleEmitter2::length() const
{
	return this->m_length;
}

inline float32 ParticleEmitter2::width() const
{
	return this->m_width;
}

inline long32 ParticleEmitter2::filterMode() const
{
	return this->m_filterMode;
}

inline long32 ParticleEmitter2::rows() const
{
	return this->m_rows;
}

inline long32 ParticleEmitter2::columns() const
{
	return this->m_columns;
}

inline long32 ParticleEmitter2::flag2() const
{
	return this->m_flag2;
}

inline float32 ParticleEmitter2::tailLength() const
{
	return this->m_tailLength;
}

inline float32 ParticleEmitter2::time() const
{
	return this->m_time;
}

inline class SegmentColors* ParticleEmitter2::segmentColors() const
{
	return this->m_segmentColors;
}

inline byte ParticleEmitter2::alpha1() const
{
	return this->m_alpha1;
}

inline byte ParticleEmitter2::alpha2() const
{
	return this->m_alpha2;
}

inline byte ParticleEmitter2::alpha3() const
{
	return this->m_alpha3;
}

inline float32 ParticleEmitter2::scalingX() const
{
	return this->m_scalingX;
}

inline float32 ParticleEmitter2::scalingY() const
{
	return this->m_scalingY;
}

inline float32 ParticleEmitter2::scalingZ() const
{
	return this->m_scalingZ;
}

inline long32 ParticleEmitter2::lifeSpanUvAnim1() const
{
	return this->m_lifeSpanUvAnim1;
}

inline long32 ParticleEmitter2::lifeSpanUvAnim2() const
{
	return this->m_lifeSpanUvAnim2;
}

inline long32 ParticleEmitter2::lifeSpanUvAnim3() const
{
	return this->m_lifeSpanUvAnim3;
}

inline long32 ParticleEmitter2::decayUvAnim1() const
{
	return this->m_decayUvAnim1;
}

inline long32 ParticleEmitter2::decayUvAnim2() const
{
	return this->m_decayUvAnim2;
}

inline long32 ParticleEmitter2::decayUvAnim3() const
{
	return this->m_decayUvAnim3;
}

inline long32 ParticleEmitter2::tailUvAnim1() const
{
	return this->m_tailUvAnim1;
}

inline long32 ParticleEmitter2::tailUvAnim2() const
{
	return this->m_tailUvAnim2;
}

inline long32 ParticleEmitter2::tailUvAnim3() const
{
	return this->m_tailUvAnim3;
}

inline long32 ParticleEmitter2::tailDecayUvAnim1() const
{
	return this->m_tailDecayUvAnim1;
}

inline long32 ParticleEmitter2::tailDecayUvAnim2() const
{
	return this->m_tailDecayUvAnim2;
}

inline long32 ParticleEmitter2::tailDecayUvAnim3() const
{
	return this->m_tailDecayUvAnim3;
}

inline long32 ParticleEmitter2::textureId() const
{
	return this->m_textureId;
}

inline long32 ParticleEmitter2::squirt() const
{
	return this->m_squirt;
}

inline long32 ParticleEmitter2::priorityPlane() const
{
	return this->m_priorityPlane;
}

inline long32 ParticleEmitter2::replaceableId() const
{
	return this->m_replaceableId;
}

inline class Speeds* ParticleEmitter2::speeds() const
{
	return this->m_speeds;
}

inline class Latitudes* ParticleEmitter2::latitudes() const
{
	return this->m_latitudes;
}

inline class EmissionRates* ParticleEmitter2::emissionRates() const
{
	return this->m_emissionRates;
}

inline class Visibility2s* ParticleEmitter2::visibilities() const
{
	return this->m_visibilities;
}

inline class Lengths* ParticleEmitter2::lengths() const
{
	return this->m_lengths;
}

inline class Widths* ParticleEmitter2::widths() const
{
	return this->m_widths;
}

}

}

#endif
