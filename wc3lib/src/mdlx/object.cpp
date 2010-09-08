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

#include <boost/cast.hpp>

#include "object.hpp"
#include "mdlxtranslations.hpp"
#include "mdlxrotations.hpp"
#include "mdlxscalings.hpp"
#include "attachmentvisibilities.hpp"
#include "mdlx.hpp"
#include "../internationalisation.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

Object::Object(class Mdlx *mdlx) : Node(mdlx), m_visibilities(new AttachmentVisibilities(mdlx))
{
}

Object::~Object()
{
	delete this->m_visibilities;
}

std::streamsize Object::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Object::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize Object::readMdx(std::istream &istream) throw (class Exception)
{
	long32 sizeCount;
	std::streamsize size;
	wc3lib::read(istream, sizeCount, size);
	size += Node::readMdx(istream);
	size += this->m_visibilities->readMdx(istream);
	/*
	if (size != sizeCount)
	{
		std::cout << "test 1 and bytes are " << bytes << std::endl;
		char message[50];
		sprintf(message, _("Object: File byte count is not equal to real byte count.\nFile byte count: %d.\nReal byte count: %d.\n"), nbytesi, bytes);
		std::cout << "test 2" << std::endl;

		throw Exception(message);
	}
	*/

	return size;
}

std::streamsize Object::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streampos position = ostream.tellp();
	ostream.seekp(sizeof(long32), std::ios_base::cur); // skip byte count, write at the end
	std::streamsize size = Node::writeMdx(ostream);
	size += this->m_visibilities->writeMdx(ostream);

	std::streampos endPosition = ostream.tellp();
	ostream.seekp(position); // jump to beginning
	wc3lib::write(ostream, boost::numeric_cast<long32>(size), size);
	ostream.seekp(endPosition); // jump back

	return size;
}

}

}
