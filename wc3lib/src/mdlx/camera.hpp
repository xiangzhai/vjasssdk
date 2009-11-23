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

#include <iostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Cameras;
class Translation0s;
class CameraRotationLengths;
class Translation3s;

//not a child of class Object!
class Camera
{
	public:
		Camera(class Cameras *cameras);
		virtual ~Camera();
		
		class Cameras* cameras() const;
		
		const ascii* name() const;
		float32 positionX() const;
		float32 positionY() const;
		float32 positionZ() const;
		float32 fieldOfView() const;
		float32 farClip() const;
		float32 nearClip() const;
		float32	targetX() const;
		float32 targetY() const;
		float32 targetZ() const;
		class Translation0s* targetTranslations() const;
		class CameraRotationLengths* rotationLengths() const;
		class Translation3s* translations() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

	protected:
		class Cameras *m_cameras;
		//long nbytesi;
		ascii m_name[0x50]; //(0x50)
		float32 m_positionX, m_positionY, m_positionZ;
		float32 m_fieldOfView;
		float32 m_farClip;
		float32 m_nearClip;
		float32	m_targetX, m_targetY, m_targetZ;
		class Translation0s *m_targetTranslations; //(KCTR)
		class CameraRotationLengths *m_rotationLengths; //(KCRL)
		class Translation3s *m_translations; //(KTTR)
		//(BKCT) ?????????????????????????????????????????????????????????????????
};

inline class Cameras* Camera::cameras() const
{
	return this->m_cameras;
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

inline float32 Camera::targetX() const
{
	return this->m_targetX;
}

inline float32 Camera::targetY() const
{
	return this->m_targetY;
}

inline float32 Camera::targetZ() const
{
	return this->m_targetZ;
}

inline class Translation0s* Camera::targetTranslations() const
{
	return this->m_targetTranslations;
}

inline class CameraRotationLengths* Camera::rotationLengths() const
{
	return this->m_rotationLengths;
}

inline class Translation3s* Camera::translations() const
{
	return this->m_translations;
}

}

}

#endif
