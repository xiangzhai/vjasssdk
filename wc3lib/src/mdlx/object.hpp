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

#include <iostream>
#include <cstring>

#include "../exception.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace mdlx
{

class Translation1s;
class Rotation0s;
class Scaling0s;
class Visibility0s;

/// No MDX block!
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
		void setTranslations(class Translation1s *translations);
		class Translation1s* translations() const;
		void setRotations(class Rotation0s *rotations);
		class Rotation0s* rotations() const;
		void setScalings(class Scaling0s *scalings);
		class Scaling0s* scalings() const;
		void setVisibilties(class Visibility0s *visibilities);
		class Visibility0s* visibilities() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		ascii m_name[0x50];
		long32 m_objectId;
		long32 m_parent;
		long32 m_type; //use enum
		class Translation1s *m_translations; //(KGTR)
		class Rotation0s *m_rotations; //(KGRT)
		class Scaling0s *m_scalings; //(KGSC)
		class Visibility0s *m_visibilities; //(KATV)
};

inline class Mdlx* Object::mdlx() const
{
	return this->m_mdlx;
}

inline void Object::setName(ascii name[0x50])
{
	memcpy(this->m_name, name, sizeof(name));
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

inline void Object::setTranslations(class Translation1s *translations)
{
	this->m_translations = translations;
}

inline class Translation1s* Object::translations() const
{
	return this->m_translations;
}

inline void Object::setRotations(class Rotation0s *rotations)
{
	this->m_rotations = rotations;
}

inline class Rotation0s* Object::rotations() const
{
	return this->m_rotations;
}

inline void Object::setScalings(class Scaling0s *scalings)
{
	this->m_scalings = scalings;
}

inline class Scaling0s* Object::scalings() const
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
