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

#include "bones.hpp"
#include "bone.hpp"
#include "../internationalisation.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

Bones::Bones(class Mdlx *mdlx) : MdxBlock("BONE"), m_mdlx(mdlx)
{
}

Bones::~Bones()
{
	BOOST_FOREACH(class Bone *bone, this->m_bones)
		delete bone;
}

std::streamsize Bones::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Bones::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize Bones::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);

	if (size == 0)
		return 0;

	long32 nbytes = 0;
	wc3lib::read(istream, nbytes, size);
	std::cout << "Bones bytes: " << nbytes << std::endl;

	if (nbytes <= 0)
		throw Exception(boost::str(boost::format(_("Bones: Byte count error, %1% bytes.\n")) % nbytes));

	std::size_t i = 0;

	while (nbytes > 0)
	{
		std::cout << "Bone " << i + 1 << " nbytes: " << nbytes << std::endl;
		class Bone *bone = new Bone(this);
		std::streamsize readSize = bone->readMdx(istream);
		size += readSize;
		nbytes -= boost::numeric_cast<long32>(readSize);
		this->m_bones.push_back(bone);
		++i;
		bone->print(std::cout);
	}

	return size;
}

std::streamsize Bones::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
