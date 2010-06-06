/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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

#ifndef WC3LIB_MAP_CAMERA_HPP
#define WC3LIB_MAP_CAMERA_HPP

#include <istream>
#include <ostream>
#include <string>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib::map
{

class Cameras;

class Camera
{
	public:
		Camera(class Cameras *cameras);
		~Camera();

		std::streamsize read(std::istream &istream) throw (class Exception);
		std::streamsize write(std::ostream &ostream) const throw (class Exception);

		void setTargetX(float32 targetX);
		float32 targetX() const;
		void setTargetY(float32 targetY);
		float32 targetY() const;
		void setZOffset(float32 zOffset);
		float32 zOffset() const;
		void setRotation(float32 rotation);
		float32 rotation() const;
		void setAngleOfAttack(float32 angleOfAttack);
		float32 angleOfAttack() const;
		void setDistance(float32 distance);
		float32 distance() const;
		void setRoll(float32 roll);
		float32 roll() const;
		void setFieldOfView(float32 fieldOfView);
		float32 fieldOfView() const;
		void setFarZ(float32 farZ);
		float32 farZ() const;
		void setUnknown(float32 unknown);
		float32 unknown() const;
		void setName(const std::string &name);
		const std::string& name() const;

	protected:
		class Cameras *m_cameras;
		float32 m_targetX;
		float32 m_targetY;
		float32 m_zOffset;
		float32 m_rotation;
		float32 m_angleOfAttack;
		float32 m_distance;
		float32 m_roll;
		float32 m_fieldOfView;
		float32 m_farZ;
		float32 m_unknown; // 100
		std::string m_name;
};

}

#endif