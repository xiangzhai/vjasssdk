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

#include <boost/foreach.hpp>

#include "shadow.hpp"
#include "w3m.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace map
{

const int32 Shadow::shadowPointsPerTileset = 16;
const char* Shadow::fileName = "war3map.shd";

Shadow::Shadow(class W3m *w3m) : m_w3m(w3m)
{
}

Shadow::~Shadow()
{
}

std::streamsize Shadow::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = 0;

	for (int32 width = 0; width < this->m_w3m->width(); ++width)
	{
		for (int32 height = 0; height < this->m_w3m->height(); ++height)
		{
			for (int32 point = 0; point < Shadow::shadowPointsPerTileset; ++point)
			{
				char8 data;
				wc3lib::read(istream, data, size);
				this->m_data.insert(std::make_pair(Key(width, height, point), static_cast<enum Shadow::Type>(data)));
			}
		}
	}

	return size;
}

std::streamsize Shadow::write(std::basic_ostream<byte> &ostream) throw (class Exception)
{
	std::streamsize size = 0;
	typedef std::pair<class Key, enum Type> valueType;

	// Should be sorted correctly (operator< of class Key)
	BOOST_FOREACH(valueType value, this->m_data)
	{
		char8 data = static_cast<char8>(value.second);
		wc3lib::write(ostream, data, size);
	}

	return size;
}

}

}
