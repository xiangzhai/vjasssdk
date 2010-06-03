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

#include "camera.hpp"
#include "../utilities.hpp"

namespace wc3lib::map
{

Camera::Camera(class Cameras *cameras) : m_cameras(cameras)
{
}

Camera::~Camera()
{

}

std::streamsize Camera::read(std::istream &istream) throw (class Exception)
{
	streamsize size = istream.gcount();
	read(istream, this->m_targetX, size);
	read(istream, this->m_targetY, size);
	read(istream, this->m_zOffset, size);
	read(istream, this->m_rotation, size);
	read(istream, this->m_angleOfAttack, size);
	read(istream, this->m_distance, size);
	read(istream, this->m_roll, size);
	read(istream, this->m_fieldOfView, size);
	read(istream, this->m_farZ, size);
	read(istream, this->m_unknown, size);
	readString(istream, this->m_name, size);

	return size;
}

std::streamsize Camera::write(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size;
	write(ostream, this->m_targetX, size);
	write(ostream, this->m_targetY, size);
	write(ostream, this->m_zOffset, size);
	write(ostream, this->m_rotation, size);
	write(ostream, this->m_angleOfAttack, size);
	write(ostream, this->m_distance, size);
	write(ostream, this->m_roll, size);
	write(ostream, this->m_fieldOfView, size);
	write(ostream, this->m_farZ, size);
	write(ostream, this->m_unknown, size);
	writeString(ostream, this->m_name, size);

	return size;
}

}
