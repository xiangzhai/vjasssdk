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

#include "mdlxscalings.hpp"
#include "mdlxscaling.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

MdlxScalings::MdlxScalings(class Mdlx *mdlx) : MdxBlock("KGSC", true), m_mdlx(mdlx)
{
}

MdlxScalings::~MdlxScalings()
{
	BOOST_FOREACH(class MdlxScaling *scaling, this->m_scalings)
		delete scaling;
}

std::streamsize MdlxScalings::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize MdlxScalings::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize MdlxScalings::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);

	// is optional!
	if (size == 0)
		return 0;

	long32 number;
	wc3lib::read(istream, number, size);
	long32 lineType;
	wc3lib::read(istream, lineType, size);
	this->m_lineType = static_cast<enum LineType>(lineType);
	wc3lib::read(istream, this->m_globalSequenceId, size);

	for ( ; number > 0; --number)
	{
		class MdlxScaling *scaling = this->createNewMember();
		size += scaling->readMdx(istream);
		this->m_scalings.push_back(scaling);
	}

	return size;
}

std::streamsize MdlxScalings::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = MdxBlock::writeMdx(ostream);

	// is optional!
	if (size == 0)
		return 0;

	long32 number = this->m_scalings.size();
	wc3lib::write(ostream, number, size);
	wc3lib::write(ostream, static_cast<long32>(this->m_lineType), size);
	wc3lib::write(ostream, this->m_globalSequenceId, size);

	BOOST_FOREACH(const class MdlxScaling *scaling, this->m_scalings)
		size += scaling->writeMdx(ostream);

	return size;
}

MdlxScalings::MdlxScalings(class Mdlx *mdlx, byte blockName[4], bool optional) : MdxBlock(blockName, optional), m_mdlx(mdlx)
{
}

class MdlxScaling* MdlxScalings::createNewMember()
{
	return new MdlxScaling(this);
}

}

}
