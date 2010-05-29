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

#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include "bones.hpp"
#include "bone.hpp"
#include "../internationalisation.hpp"

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

void Bones::readMdl(std::istream &istream) throw (class Exception)
{
}

void Bones::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize Bones::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes = 0;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	
	if (nbytes <= 0)
		throw Exception(boost::str(boost::format(_("Bones: Byte count error, %1% bytes.\n")) % nbytes));
	
	while (nbytes > 0)
	{
		class Bone *bone = new Bone(this);
		long32 readBytes = bone->readMdx(istream);
		bytes += readBytes;
		nbytes -= readBytes;
		this->m_bones.push_back(bone);
	}
	
	return bytes;
}

std::streamsize Bones::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
