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
#include "cameratranslations.hpp"
#include "camerarotationlengths.hpp"
#include "cameratargettranslations.hpp"
#include "../internationalisation.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

Camera::Camera(class Cameras *cameras) : GroupMdxBlockMember(cameras), m_translations(new CameraTranslations(this)), m_rotationLengths(new CameraRotationLengths(this)), m_targetTranslations(new CameraTargetTranslations(this))
{
}

Camera::~Camera()
{
	delete this->m_translations;
	delete this->m_rotationLengths;
	delete this->m_targetTranslations;
}

std::streamsize Camera::readMdl(istream &istream) throw (class Exception)
{
}

std::streamsize Camera::writeMdl(ostream &ostream) const throw (class Exception)
{
}

std::streamsize Camera::readMdx(istream &istream) throw (class Exception)
{
	long32 nbytesi;
	std::cout << "Camera 1" << std::endl;
	std::streamsize bytes = 0;
	wc3lib::read(istream, nbytesi, bytes);
	std::cout << "Camera 2" << std::endl;
	wc3lib::read(istream, this->m_name, bytes);
	std::cout << "Camera 3" << std::endl;
	wc3lib::read(istream, this->m_position, bytes);
	wc3lib::read(istream, this->m_fieldOfView, bytes);
	wc3lib::read(istream, this->m_farClip, bytes);
	wc3lib::read(istream, this->m_nearClip, bytes);
	wc3lib::read(istream, this->m_target, bytes);
	std::cout << "Camera 4" << std::endl;
	bytes += this->m_translations->readMdx(istream);
	std::cout << "Camera 5" << std::endl;
	bytes += this->m_rotationLengths->readMdx(istream);
	std::cout << "Camera 6" << std::endl;
	bytes += this->m_targetTranslations->readMdx(istream);
	std::cout << "Camera 7" << std::endl;
	//(BKCT) ?????????????????????????????????????????????????????????????????

	if (bytes != nbytesi)
		throw Exception(boost::str(boost::format(_("Camera: File byte count is not equal to real byte count.\nFile byte count: %1%.\nReal byte count: %2%.\n")) % nbytesi % bytes));

	return bytes;
}

std::streamsize Camera::writeMdx(ostream &ostream) const throw (class Exception)
{
	std::streampos position;
	skipByteCount<long32>(ostream, position);
	
	std::streamsize bytes = 0;
	wc3lib::write(ostream, this->m_name, bytes);
	wc3lib::write(ostream, this->m_position, bytes);
	wc3lib::write(ostream, this->m_fieldOfView, bytes);
	wc3lib::write(ostream, this->m_farClip, bytes);
	wc3lib::write(ostream, this->m_nearClip, bytes);
	wc3lib::write(ostream, this->m_target, bytes);
	bytes += this->m_translations->writeMdx(ostream);
	bytes += this->m_rotationLengths->writeMdx(ostream);
	bytes += this->m_targetTranslations->writeMdx(ostream);
	//(BKCT) ?????????????????????????????????????????????????????????????????
	
	long32 nbytesi = bytes;
	writeByteCount(ostream, nbytesi, position, bytes, true);

	return bytes;
}

}

}
