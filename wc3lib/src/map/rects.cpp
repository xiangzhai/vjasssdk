/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#include "rects.hpp"
#include "rect.hpp"
#include "../internationalisation.hpp"

namespace wc3lib::map
{

const int32 Rects::version = 5;

Rects::Rects(class W3m *w3m) : m_w3m(w3m)
{
}

std::streamsize Rects::read(std::istream &istream) throw (class Exception)
{
	std::streamsize size;
	read(istream, this->m_version, size);

	if (this->m_version != Rects::version)
		throw Exception(boost::format(_("Rects: Unknown version \"%1%\", expected \"%2%\".")) % this->m_version % Rects::version);

	int32 number;
	read(istream, number, size);

	for ( ; number >= 0; --number)
	{
		class Rect *rect = new Rect(this);
		size += rect->read(istream);
		this->m_rects.insert(std::make_pair(rect->index(), rect));
	}

	return size;
}

std::streamsize Rects::write(std::ostream &ostream) const throw (class Exception)
{
	if (this->m_version != Rects::version)
		throw Exception(boost::format(_("Rects: Unknown version \"%1%\", expected \"%2%\".")) % this->m_version % Rects::version);

	std::streamsize size;
	write(ostream, this->m_version, size);
	int32 number = this->m_rects.size();
	write(ostream, number, size);

	BOOST_FOREACH(const class Rect *rect, this->m_rects)
		size += rect->write(ostream);

	return size;
}

}
