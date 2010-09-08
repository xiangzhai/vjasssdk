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

//#include <cstring>

#include <boost/tokenizer.hpp>

#include "bone.hpp"
#include "bones.hpp"
#include "../exception.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

Bone::Bone(class Bones *bones) : Object(bones->mdlx())
{
}

Bone::~Bone()
{
}

std::streamsize Bone::readMdl(std::istream &istream) throw (class Exception)
{
	std::string line;
	std::getline(istream, line);
	boost::tokenizer<> tokenizer(line);
	boost::tokenizer<>::iterator iterator = tokenizer.begin();
	/// @todo FIXME

	return 0;
}

std::streamsize Bone::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	/// @todo FIXME
	return 0;
}

std::streamsize Bone::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = Object::readMdx(istream);
	wc3lib::read(istream, this->m_geosetId, size);
	wc3lib::read(istream, this->m_geosetAnimationId, size);

	return size;
}

std::streamsize Bone::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = Object::writeMdx(ostream);
	wc3lib::write(ostream, this->m_geosetId, size);
	wc3lib::write(ostream, this->m_geosetAnimationId, size);

	return size;
}

}

}
