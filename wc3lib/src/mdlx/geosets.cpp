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

#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/cast.hpp>

#include "geosets.hpp"
#include "geoset.hpp"
#include "../internationalisation.hpp"
#include "../utilities.hpp"

#include "../mdlx.hpp" /// TEST!!!!!!!!!!!

namespace wc3lib
{

namespace mdlx
{

Geosets::Geosets(class Mdlx *mdlx) : MdxBlock("GEOS"), m_mdlx(mdlx)
{
}

Geosets::~Geosets()
{
	BOOST_FOREACH(class Geoset *geoset, this->m_geosets)
		delete geoset;
}

std::streamsize Geosets::readMdl(istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Geosets::writeMdl(ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize Geosets::readMdx(istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);

	if (size == 0)
		return 0;

	long32 nbytes = 0; //nbytes
	wc3lib::read(istream, nbytes, size);
	std::cout << "Expecting " << nbytes << " geosets bytes." << std::endl;

	if (nbytes <= 0)
		throw Exception(_("Geosets: 0 byte geosets.\n"));

	while (nbytes > 0)
	{
		std::cout << "READING GEOSET!" << std::endl;
		class Geoset *geoset = new Geoset(this);
		std::streamsize readSize = geoset->readMdx(istream);
		nbytes -= boost::numeric_cast<long32>(readSize);
		size += readSize;
		this->m_geosets.push_back(geoset);

		std::cout << "GEOSET INFORMATION: " << std::endl
		<< "Vertices: " << geoset->vertices()->vertices().size() << std::endl
		<< "Normals: " << geoset->normals()->normals().size() << std::endl
		<< "Primitive types: " << geoset->primitiveTypes()->primitiveTypes().size() << std::endl
		<< "Primitive sizes: " << geoset->primitiveSizes()->primitiveSizes().size() << std::endl
		<< "Primitive vertices: " << geoset->primitiveVertices()->primitiveVertices().size() << std::endl
		<< "Group vertices: " << geoset->groupVertices()->groupVertices().size() << std::endl
		<< "Matrix group counts: " << geoset->matrixGroupCounts()->matrixGroupCounts().size() << std::endl
		<< "Matrices: " << geoset->matrices()->matrices().size() << std::endl
		<< "Ganimations: " << geoset->ganimations().size() << std::endl
		<< "Texture patches: " << geoset->texturePatches()->number() << std::endl
		<< "Texture vertices: " << geoset->textureVertices()->textureVertices().size() << std::endl
		;
	}

	return size;
}

std::streamsize Geosets::writeMdx(ostream &ostream) const throw (class Exception)
{
	std::streamsize size = MdxBlock::writeMdx(ostream);

	if (size == 0)
		return 0;

	std::streampos position;
	skipByteCount<long32>(ostream, position);

	BOOST_FOREACH(const class Geoset *geoset, geosets())
		size += geoset->writeMdx(ostream);

	
	writeByteCount(ostream, static_cast<long32&>(size), position, size);
	
	return size;
}

}

}
