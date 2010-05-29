/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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
#include <boost/tokenizer.hpp>

#include "bounds.hpp"

namespace wc3lib
{

namespace mdlx
{

Bounds::Bounds()
{
}

Bounds::~Bounds()
{
}

std::streamsize Bounds::readMdx(std::istream &istream) throw (class Exception)
{
	istream.read(reinterpret_cast<char*>(&this->m_boundsRadius), sizeof(this->m_boundsRadius));
	std::streamsize size = istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_minimumExtent), sizeof(this->m_minimumExtent));
	size += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_maximumExtent), sizeof(this->m_maximumExtent));
	size += istream.gcount();

	return size;
}

std::streamsize Bounds::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	ostream.write(reinterpret_cast<const char*>(&this->m_boundsRadius), sizeof(this->m_boundsRadius));
	std::streamsize size = sizeof(this->m_boundsRadius);
	ostream.write(reinterpret_cast<const char*>(&this->m_minimumExtent), sizeof(this->m_minimumExtent));
	size += sizeof(this->m_minimumExtent);
	ostream.write(reinterpret_cast<const char*>(&this->m_maximumExtent), sizeof(this->m_maximumExtent));
	size += sizeof(this->m_maximumExtent);

	return size;
}

// MinimumExtent, MaximumExtent and BoundsRadius only appear when their values are not 0.0.
void Bounds::readMdl(std::istream &istream) throw (class Exception)
{
	/// @todo If values do not exist they're always 0.0.
	//boost::tokenizer<> tokenizer(istream);
	/*
	if ((*iterator)->minExtX() != 0.0 || (*iterator)->minExtY() != 0.0 || (*iterator)->minExtZ() != 0.0)
		ostream << "\t\tMinimumExtent { " << (*iterator)->minExtX() << ", " << (*iterator)->minExtY() << ", " << (*iterator)->minExtZ() << " },\n";

	if ((*iterator)->maxExtX() != 0.0 || (*iterator)->maxExtY() != 0.0 || (*iterator)->maxExtZ() != 0.0)
		ostream << "\t\tMaxmimumExtent { " << (*iterator)->maxExtX() << ", " << (*iterator)->maxExtY() << ", " << (*iterator)->maxExtZ() << " },\n";

	if ((*iterator)->boundsRadius() != 0.0)
		ostream << "\t\tBoundsRadius " << (*iterator)->boundsRadius() << ",\n";
	*/
}

// MinimumExtent, MaximumExtent and BoundsRadius only appear when their values are not 0.0.
void Bounds::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	if (this->minimumExtent().x != 0.0 || this->minimumExtent().y != 0.0 || this->minimumExtent().z != 0.0)
		ostream
		<< "MinimumExtent { "
		<< this->minimumExtent().x << ", "
		<< this->minimumExtent().y << ", "
		<< this->minimumExtent().z
		<< "}, " << std::endl;

	if (this->maximumExtent().x != 0.0 || this->maximumExtent().y != 0.0 || this->maximumExtent().z != 0.0)
		ostream
		<< "MaximumExtent { "
		<< this->maximumExtent().x << ", "
		<< this->maximumExtent().y << ", "
		<< this->maximumExtent().z
		<< "}, " << std::endl;

	if (this->boundsRadius() != 0.0)
		ostream << "BoundsRadius " << this->boundsRadius() << ", " << std::endl;
}

}

}
