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

#include "mdlxrotation.hpp"
#include "mdlxrotations.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

MdlxRotation::MdlxRotation(class MdlxRotations *rotations) : m_rotations(rotations)
{
}

MdlxRotation::~MdlxRotation()
{
}

std::streamsize MdlxRotation::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize MdlxRotation::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize MdlxRotation::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read(istream, this->m_frame, size);
	wc3lib::read(istream, this->m_quaternionData, size);

	if (this->m_rotations->lineType() > Linear)
		wc3lib::read(istream, this->m_interpolationData, size);

	return size;
}

std::streamsize MdlxRotation::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::write(ostream, this->m_frame, size);
	wc3lib::write(ostream, this->m_quaternionData, size);

	if (this->m_rotations->lineType() > Linear)
		wc3lib::write(ostream, this->m_interpolationData, size);

	return size;
}

}

}