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

#ifndef WC3LIB_MDLX_OBJECT_HPP
#define WC3LIB_MDLX_OBJECT_HPP

#include <fstream>

#include "../exception.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;
class Translations;
class Rotations;
class Scalings;
class Visibility0s;

class Object
{
	public:
		/// @todo long32
		enum Type
		{
			Helper = 0,
			DontInheritTranslation = 1,
			DontInheritRotation = 2,
			DontInheritScaling = 4,
			Billboarded = 8,
			BillboardedLockX = 16,
			BillboardedLockY = 32,
			BillboardedLockZ = 64,
			CameraAnchored = 128,
			Bone = 256,
			Light = 512,
			EventObject = 1024,
			Attachment = 2048,
			ParticleEmitter = 4096,
			CollisionShape = 8192,
			RibbonEmitter = 16384,
			Unshaded = 32768,
			EmitterUsesMdl = 32768,
			SortPrimitivesFarZ = 65536,
			EmitterUsesTga = 65536,
			LineEmitter = 131072,
			Unfogged = 262144,
			ModelSpace = 524288,
			XyQuad = 1048576
		};

		Object(class Mdlx *mdlx);
		virtual ~Object();

		class Mdlx* mdlx() const;
		void setName(ascii name[0x50]);
		const ascii* name() const;
		void setObjectId(long32 objectId);
		long32 objectId() const;
		void setParent(long32 parent);
		long32 parent() const;
		void setType(long32 type);
		long32 type() const;
		void setTranslations(class Translations *translations);
		class Translations* translations() const;
		void setRotations(class Rotations *rotations);
		class Rotations* rotations() const;
		void setScalings(class Scalings *scalings);
		class Scalings* scalings() const;
		void setVisibilties(class Visibility0s *visibilities);
		class Visibility0s* visibilities() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		ascii m_name[0x50];
		long32 m_objectId;
		long32 m_parent;
		long32 m_type; //use enum
		class Translations *m_translations;
		class Rotations *m_rotations;
		class Scalings *m_scalings;
		class Visibility0s *m_visibilities;
};

inline class Mdlx* Object::mdlx() const
{
	return this->m_mdlx;
}

inline void Object::setName(ascii name[0x50])
{
	for (int i = 0; i < 0x50; ++i)
		this->m_name[i] = name[i];
}


inline const ascii* Object::name() const
{
	return this->m_name;
}


inline void Object::setObjectId(long32 objectId)
{
	this->m_objectId = objectId;
}

inline long32 Object::objectId() const
{
	return this->m_objectId;
}

inline void Object::setParent(long32 parent)
{
	this->m_parent = parent;
}

inline long32 Object::parent() const
{
	return this->m_parent;
}

inline void Object::setType(long32 type)
{
	this->m_type = type;
}

inline long32 Object::type() const
{
	return this->m_type;
}

inline void Object::setTranslations(class Translations *translations)
{
	this->m_translations = translations;
}

inline class Translations* Object::translations() const
{
	return this->m_translations;
}

inline void Object::setRotations(class Rotations *rotations)
{
	this->m_rotations = rotations;
}

inline class Rotations* Object::rotations() const
{
	return this->m_rotations;
}

inline void Object::setScalings(class Scalings *scalings)
{
	this->m_scalings = scalings;
}

inline class Scalings* Object::scalings() const
{
	return this->m_scalings;
}

inline void Object::setVisibilties(class Visibility0s *visibilities)
{
	this->m_visibilities = visibilities;
}

inline class Visibility0s* Object::visibilities() const
{
	return this->m_visibilities;
}

}

}

#endif
