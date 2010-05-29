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

#include <boost/format.hpp>

#include "camera.hpp"
#include "cameras.hpp"
#include "cameratranslations.hpp"
#include "camerarotationlengths.hpp"
#include "cameratargettranslations.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Camera::Camera(class Cameras *cameras) : m_cameras(cameras), m_translations(new CameraTranslations(this)), m_rotationLengths(new CameraRotationLengths(this)), m_targetTranslations(new CameraTargetTranslations(this))
{
}

Camera::~Camera()
{
	delete this->m_translations;
	delete this->m_rotationLengths;
	delete this->m_targetTranslations;
}

void Camera::readMdl(std::istream &istream) throw (class Exception)
{
}

void Camera::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize Camera::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytesi;
	istream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	std::streamsize bytes = istream.gcount();
	istream.read(this->m_name, sizeof(this->m_name));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_positionX), sizeof(this->m_positionX));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_positionY), sizeof(this->m_positionY));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_positionZ), sizeof(this->m_positionZ));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_fieldOfView), sizeof(this->m_fieldOfView));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_farClip), sizeof(this->m_farClip));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_nearClip), sizeof(this->m_nearClip));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_targetX), sizeof(this->m_targetX));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_targetY), sizeof(this->m_targetY));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_targetZ), sizeof(this->m_targetZ));
	bytes += istream.gcount();
	bytes += this->m_translations->readMdx(istream);
	bytes += this->m_rotationLengths->readMdx(istream);
	bytes += this->m_targetTranslations->readMdx(istream);
	//(BKCT) ?????????????????????????????????????????????????????????????????
	
	if (bytes != nbytesi)	
		throw Exception(boost::str(boost::format(_("Camera: File byte count is not equal to real byte count.\nFile byte count: %1%.\nReal byte count: %2%.\n")) % nbytesi % bytes));
	
	return bytes;
}

std::streamsize Camera::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
