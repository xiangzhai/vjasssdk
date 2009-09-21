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

#ifndef WC3LIB_MDLX_CAMERA_HPP
#define WC3LIB_MDLX_CAMERA_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;

//not a child of class Object!
class Camera
{
	public:
		Camera(class Mdlx *mdlx);
		virtual ~Camera();
		class Mdlx* mdlx() const;
		const ascii* name() const;
		float32 positionX() const;
		float32 positionY() const;
		float32 positionZ() const;
		float32 fieldOfView() const;
		float32 farClip() const;
		float32 nearClip() const;
		class Target* target() const;
		class Rotation* rotation() const;
		class Translation* translation() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		//long nbytesi;
		ascii m_name[0x50]; //(0x50)
		float32 m_positionX, m_positionY, m_positionZ;
		float32 m_fieldOfView;
		float32 m_farClip;
		float32 m_nearClip;
		class Target *m_target;
		class Rotation *m_rotation; //(KCRL)
		class Translation *m_translation; //(KTTR)
		//(BKCT) ?????????????????????????????????????????????????????????????????
};

class Mdlx* Camera::mdlx() const
{
	return this->m_mdlx;
}

inline const ascii* Camera::name() const
{
	return this->m_name;
}

inline float32 Camera::positionX() const
{
	return this->m_positionX;
}

inline float32 Camera::positionY() const
{
	return this->m_positionY;
}

inline float32 Camera::positionZ() const
{
	return this->m_positionZ;
}

inline float32 Camera::fieldOfView() const
{
	return this->m_fieldOfView;
}

inline float32 Camera::farClip() const
{
	return this->m_farClip;
}

inline float32 Camera::nearClip() const
{
	return this->m_nearClip;
}

inline class Target* Camera::target() const
{
	return this->m_target;
}

inline class Rotation* Camera::rotation() const
{
	return this->m_rotation;
}

inline class Translation* Camera::translation() const
{
	return this->m_translation;
}

}

}

#endif
