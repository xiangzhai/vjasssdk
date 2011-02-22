/***************************************************************************
 *   Copyright (C) 2011 by Tamino Dauth                                    *
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

#include "mdlxanimatedproperties.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{
	
MdlxAnimatedProperties::MdlxAnimatedProperties(class Mdlx *mdlx, const byte mdxIdentifier[MdxBlock::mdxIdentifierSize], const string &mdlIdentifier, bool optional) : MdxBlock(mdxIdentifier, optional), MdlBlock(mdlIdentifier, optional), m_mdlx(mdlx)
{
}

MdlxAnimatedProperties::~MdlxAnimatedProperties()
{
	BOOST_FOREACH(class MdlxAnimatedPropertiy *property, this->m_properties)
		delete property;
}

std::streamsize MdlxAnimatedProperties::readMdl(istream &istream) throw (class Exception)
{
	/// \todo FIXME
	return 0;
}

std::streamsize MdlxAnimatedProperties::writeMdl(ostream &ostream) const throw (class Exception)
{
	// Tag <long_count> {
	std::streamsize size = MdlValueBlock::writeMdl(ostream);
	
	switch (lineType())
	{
		case DontInterpolate:
			writeMdlProperty(ostream, "DontInterp", size);
			
			break;
	
		case Linear:
			writeMdlProperty(ostream, "Linear", size);
			
			break;
	
		case Hermite:
			writeMdlProperty(ostream, "Hermite", size);
			
			break;
	
		case Bezier:
			writeMdlProperty(ostream, "Bezier", size);
			
			break;
	}
	
	if (hasGlobalSequence())
		writeMdlValueProperty(ostream, "GlobalSeqId", globalSequenceId(), size);
	
	BOOST_FOREACH(const class MdlxAnimatedProperty *property, properties())
		size += property->writeMdl(ostream);

	writeMdlBlockConclusion(ostream, size);
	
	return size;
}

std::streamsize MdlxAnimatedProperties::readMdx(std::istream &istream) throw (class Exception)
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
		class MdlxAnimatedPropertiy *property = createAnimatedProperty();
		size += property->readMdx(istream);
		this->m_properties.push_back(property);
	}

	return size;
}

std::streamsize MdlxAnimatedProperties::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = MdxBlock::writeMdx(ostream);

	// is optional!
	if (size == 0)
		return 0;

	wc3lib::write(ostream, static_cast<long32>(this->m_properties.size()), size);
	wc3lib::write(ostream, static_cast<long32>(this->m_lineType), size);
	wc3lib::write(ostream, this->m_globalSequenceId, size);

	BOOST_FOREACH(const class MdlxAnimatedPropertiy *property, this->properties())
		size += property->writeMdx(ostream);

	return size;
}

}

}
