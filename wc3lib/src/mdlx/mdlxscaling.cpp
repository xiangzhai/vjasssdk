/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
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

#include "mdlxscaling.hpp"
#include "mdlxscalings.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

MdlxScaling::MdlxScaling(class MdlxScalings *scalings) : m_scalings(scalings)
{
}

MdlxScaling::~MdlxScaling()
{
}

std::streamsize MdlxScaling::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize MdlxScaling::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize MdlxScaling::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read(istream, this->m_frame, size);
	wc3lib::read(istream, this->m_vertexData, size);

	if (this->m_scalings->lineType() > Linear)
		wc3lib::read(istream, this->m_interpolationData, size);

	return size;
}

std::streamsize MdlxScaling::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::write(ostream, this->m_frame, size);
	wc3lib::write(ostream, this->m_vertexData, size);

	if (this->m_scalings->lineType() > Linear)
		wc3lib::write(ostream, this->m_interpolationData, size);

	return size;
}

}

}
