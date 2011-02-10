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

#include <boost/foreach.hpp>

#include "customunits.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace map
{

CustomUnits::Unit::Unit()
{
}

CustomUnits::Unit::~Unit()
{
	BOOST_FOREACH(Modification *modification, m_modifications)
		delete modification;
}

std::streamsize CustomUnits::Unit::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read(istream, this->m_originalId, size);
	wc3lib::read(istream, this->m_customId, size);
	int32 modifications;
	wc3lib::read(istream, modifications, size);

	for ( ; modifications > 0; --modifications)
	{
		Modification *modification = createModification();
		size += modification->read(istream);
		this->m_modifications.push_back(modification);
	}

	return size;
}

std::streamsize CustomUnits::Unit::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::write(ostream, this->m_originalId, size);
	wc3lib::write(ostream, this->m_customId, size);
	wc3lib::write<int32>(ostream, this->m_modifications.size(), size);

	BOOST_FOREACH(Modification *modification, this->m_modifications)
		size += modification->write(ostream);

	return size;
}

Modification* CustomUnits::Unit::createModification() const
{
	return new Modification();
}

CustomUnits::Modification::Modification()
{
}

CustomUnits::Modification::~Modification()
{
}

std::streamsize CustomUnits::Modification::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = readData();
	int32 end;
	wc3lib::read(istream, end, size); // usually 0

	return size;
}

std::streamsize CustomUnits::Modification::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	std::streamsize size = writeData();
	int32 end = 0;
	wc3lib::write(ostream, end, size);

	return size;
}

std::streamsize CustomUnits::Modification::readData(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read(istream, this->m_id, size);
	wc3lib::read<int>(istream, this->m_type, size);

	switch (this->m_type)
	{
		case Integer:
			wc3lib::read(istream, this->m_value.Integer, size);

			break;

		case Real:
			wc3lib::read(istream, this->m_value.Real, size);

			break;

		// TODO FINISH
	}

	return size;
}

std::streamsize CustomUnits::Modification::writeData(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::write(ostream, this->m_id, size);
	wc3lib::write<int>(ostream, this->m_type, size);

	switch (this->m_type)
	{
		case Integer:
			wc3lib::write(ostream, this->m_value.Integer, size);

			break;

		case Real:
			wc3lib::write(ostream, this->m_value.Real, size);

			break;

		// TODO FINISH
	}

	return size;
}

CustomUnits::CustomUnits()
{
}

CustomUnits::~CustomUnits()
{
	BOOST_FOREACH(Unit *unit, this->m_originalTable)
		delete unit;

	BOOST_FOREACH(Unit *unit, this->m_customTable)
		delete unit;
}

std::streamsize CustomUnits::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read(istream, this->m_version, size);

	if (this->m_version != latestFileVersion())
		throw Exception(boost::format(_("Custom Units: Unknown version \"%1%\", expected \"%2%\".")) % this->m_version % latestFileVersion());

	int32 originalUnits;
	wc3lib::read(istream, originalUnits, size);

	for ( ; originalUnits > 0; --originalUnits)
	{
		Unit *unit = createUnit();
		size += unit->read(istream);
		this->m_originalTable.push_back(unit);
	}

	int32 customUnits;
	wc3lib::read(istream, customUnits, size);

	for ( ; customUnits > 0; --customUnits)
	{
		Unit *unit = createUnit();
		size += unit->read(istream);
		this->m_customTable.push_back(unit);
	}

	return size;
}

std::streamsize CustomUnits::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	if (this->m_version != latestFileVersion())
		throw Exception(boost::format(_("Custom Units: Unknown version \"%1%\", expected \"%2%\".")) % this->m_version % latestFileVersion());

	std::streamsize size = 0;
	wc3lib::write(ostream, this->m_version, size);
	wc3lib::write<int32>(ostream, this->m_originalTable.size(), size);

	BOOST_FOREACH(Unit *unit, this->m_originalTable)
		size += unit->write(ostream);

	wc3lib::write<int32>(ostream, this->m_customTable.size(), size);

	BOOST_FOREACH(Unit *unit, this->m_customTable)
		size += unit->read(ostream);

	return size;
}

Unit* CustomUnits::createUnit() const
{
	return new Unit();
}

}

}
