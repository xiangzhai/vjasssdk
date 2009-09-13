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
class Translation;
class Rotation;
class Scaling;
class Visibility;

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
		ascii[0x50] name() const;
		void setObjectId(long32 objectId);
		long32 objectId() const;
		void setParent(long32 parent);
		long32 parent() const;
		void setType(long32 type);
		long32 type() const;
		void setTranslation(class Translation *translation);
		class Translation* translation() const;
		void setRotation(class Rotation *rotation);
		class Rotation* rotation() const;
		void setScaling(class Scaling *scaling);
		class Scaling* scaling() const;
		void setVisibilty(class Visibility *visibility);
		class Visibility* visibility() const;

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
		class Translation *m_translation;
		class Rotation *m_rotation;
		class Scaling *m_scaling;
		class Visibility *m_visibility;
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


inline ascii[0x50] Object::name() const
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

inline void Object::setTranslation(class Translation *translation)
{
	this->m_translation = translation;
}

inline class Translation* Object::translation() const
{
	return this->m_translation;
}

inline void Object::setRotation(class Rotation *rotation)
{
	this->m_rotation = rotation;
}

inline class Rotation* Object::rotation() const
{
	return this->m_rotation;
}

inline void Object::setScaling(class Scaling *scaling)
{
	this->m_scaling = scaling;
}

inline class Scaling* Object::scaling() const
{
	return this->m_scaling;
}

inline void Object::setVisibilty(class Visibility *visibility)
{
	this->m_visibility = visibility;
}

inline class Visibility* Object::visibility() const
{
	return this->visibility;
}

}

}

#endif
