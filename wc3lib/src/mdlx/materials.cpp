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
#include <boost/foreach.hpp>

#include "materials.hpp"
#include "material.hpp"
#include "../internationalisation.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

Materials::Materials(class Mdlx *mdlx) : MdxBlock("MTLS", true), m_mdlx(mdlx)
{
}

Materials::~Materials()
{
}

std::streamsize Materials::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Materials::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize Materials::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);

	if (size == 0)
		return 0;

	long32 nbytes = 0; //nbytes
	wc3lib::read(istream, nbytes, size);

	while (nbytes > 0)
	{
		class Material *material = new Material(this);
		std::streamsize readSize = material->readMdx(istream);

		if (readSize == 0)
			throw Exception(_("Materials: 0 byte material"));

		nbytes -= boost::numeric_cast<long32>(readSize);
		size += readSize;
		this->m_materials.push_back(material);
	}

	return size;
}

std::streamsize Materials::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = MdxBlock::writeMdx(ostream);
	std::streampos position = ostream.tellp();
	ostream.seekp(sizeof(long32), std::ios_base::cur);

	BOOST_FOREACH(const class Material *material, this->m_materials)
		size += material->writeMdx(ostream);

	std::streampos currentPosition = ostream.tellp();
	ostream.seekp(position);
	wc3lib::write(ostream, *reinterpret_cast<const long32*>(&size), size);
	ostream.seekp(currentPosition);

	return size;
}

}

}
