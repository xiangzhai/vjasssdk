/***************************************************************************
 *   Copyright (C) 2011 by Tamino Dauth                                    *
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

#include "customobjects.hpp"
#include "../utilities.hpp"


namespace wc3lib
{

namespace map
{

Modification* CustomObjects::Unit::createModification() const
{
	return new CustomObjects::Modification(this->type());
}

std::streamsize CustomObjects::Modification::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = readData(istream);

	if (this->type() == CustomObjects::Doodads || this->type() == CustomObjects::Abilities || this->type() == CustomObjects::Upgrades)
	{
		wc3lib::read(istream, this->m_level, size);
		wc3lib::read(istream, this->m_data, size);
	}

	int32 end;
	wc3lib::read(istream, end, size);

	return size;
}

std::streamsize CustomObjects::Modification::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	std::streamsize size = writeData(ostream);

	if (this->type() == CustomObjects::Doodads || this->type() == CustomObjects::Abilities || this->type() == CustomObjects::Upgrades)
	{
		wc3lib::write(ostream, this->m_level, size);
		wc3lib::write(ostream, this->m_data, size);
	}

	int32 end = 0;
	wc3lib::write(ostream, end, size);

	return size;
}

CustomObjects::CustomObjects(Type type) : m_type(type)
{
}

const char* CustomObjects::fileName() const
{
	std::string name = "war3map.";

	switch (this->type())
	{
		case Units:
			return name.append("w3u");

		case Items:
			return name.append("w3t");

		case Destructables:
			return name.append("w3b");

		case Doodads:
			return name.append("w3d");

		case Abilities:
			return name.append("w3a");

		case Buffs:
			return name.append("w3h");

		case Upgrades:
			return name.append("w3q");
	}

	return name;
}

Unit* CustomObjects::createUnit() const
{
	return new CustomObjects::Unit(this->type());
}

}

}
