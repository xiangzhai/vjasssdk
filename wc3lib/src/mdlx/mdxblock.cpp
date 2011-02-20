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

#include <iostream>
#include <cstring>

#include <boost/format.hpp>

#include "mdxblock.hpp"
#include "../utilities.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

MdxBlock::MdxBlock(const byte mdxIdentifier[MdxBlock::mdxIdentifierSize], bool optional) : m_optional(optional), m_exists(false)
{
	memcpy(this->m_mdxIdentifier, mdxIdentifier, sizeof(mdxIdentifier));
}

MdxBlock::~MdxBlock()
{
}

/// @todo Consider optional like in Python script.
std::streamsize MdxBlock::readMdx(istream &istream) throw (class Exception)
{
	std::streamsize size = 0;
	byte identifier[sizeof(mdxIdentifier())];
	istream::pos_type position = istream.tellg();
	wc3lib::read(istream, identifier[0], size, sizeof(identifier));

	if (memcmp(identifier, mdxIdentifier(), sizeof(mdxIdentifier())) != 0)
	{
		if (this->optional())
		{
			istream.seekg(position);
			std::cout << boost::format(_("Block %1% is optional and doesn't exist.\nIt is not equal to identifier \"%2%\".")) % mdxIdentifier() % identifier << std::endl;

			return 0;
		}
		else
			throw Exception(boost::str(boost::format(_("Unexptected identifier \"%s\". Missing \"%s\" block name.")) % identifier % mdxIdentifier()));
	}

	this->m_exists = true;
	std::cout << boost::format(_("Block: %1%")) % mdxIdentifier() << std::endl;

	return bytes;
}

std::streamsize MdxBlock::writeMdx(ostream &ostream) const throw (class Exception)
{
	if (!this->exists())
		return 0;

	std::streamsize size = 0;
	wc3lib::write(ostream, mdxIdentifier()[0], size, MdxBlock::mdxIdentifierSize);

	return size;
}

bool MdxBlock::moveToBlockName(istream &istream)
{
	byte readBlockName[MdxBlock::mdxIdentifierSize];

	while (istream)
	{
		istream.read(readBlockName, sizeof(readBlockName));

		if (memcmp(readBlockName, mdxIdentifier(), sizeof(mdxIdentifier())) == 0)
			return true;
	}

	return true;
}

}

}
