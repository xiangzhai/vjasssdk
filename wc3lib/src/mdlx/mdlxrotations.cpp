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

#include <boost/foreach.hpp>

#include "mdlxrotations.hpp"
#include "mdlxrotation.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

MdlxRotations::MdlxRotations(class Mdlx *mdlx) : MdxBlock("KGRT"), m_mdlx(mdlx)
{
}

MdlxRotations::~MdlxRotations()
{
	BOOST_FOREACH(class MdlxRotation *rotation, this->m_rotations)
		delete rotation;
}

std::streamsize MdlxRotations::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize MdlxRotations::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize MdlxRotations::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);

	// is optional!
	if (size == 0)
		return 0;

	long32 number;
	wc3lib::read(istream, number, size);
	wc3lib::read(istream, *reinterpret_cast<long32*>(&this->m_lineType), size);
	wc3lib::read(istream, this->m_globalSequenceId, size);

	for ( ; number > 0; --number)
	{
		class MdlxRotation *rotation = new MdlxRotation(this);
		size += rotation->readMdx(istream);
		this->m_rotations.push_back(rotation);
	}

	return size;
}

std::streamsize MdlxRotations::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = MdxBlock::writeMdx(ostream);

	// is optional!
	if (size == 0)
		return 0;

	wc3lib::write(ostream, static_cast<long32>(this->m_rotations.size()), size);
	wc3lib::write(ostream, static_cast<long32>(this->m_lineType), size);
	wc3lib::write(ostream, this->m_globalSequenceId, size);

	BOOST_FOREACH(const class MdlxRotation *rotation, this->m_rotations)
		size += rotation->writeMdx(ostream);

	return size;
}

}

}
