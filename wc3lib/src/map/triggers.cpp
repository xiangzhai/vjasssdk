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

#include "triggers.hpp"
#include "../utilities.hpp"
#include "../internationalisation.hpp"
#include "w3m.hpp"
#include "triggercategory.hpp"
#include "variable.hpp"
#include "trigger.hpp"

namespace wc3lib
{

namespace map
{

const int32 Triggers::version = 4;
const char* Triggers::fileName = "war3map.wtg";

Triggers::Triggers(class W3m *w3m) : m_w3m(w3m)
{
}

std::streamsize Triggers::read(std::istream &istream) throw (class Exception)
{
	std::streamsize size;
	wc3lib::read<int32>(istream, this->m_version, size);

	if (this->m_version != Triggers::version)
		throw Exception(boost::format(_("Triggers: Version %1% is not supported (version %2% only).")) % this->m_version % Triggers::version);

	int32 number;
	wc3lib::read<int32>(istream, number, size);

	for (int32 i = 0; i < number; ++i)
	{
		class TriggerCategory *category = new TriggerCategory(this);
		size += category->read(istream);
		this->m_categories.insert(std::make_pair(category->index(), category));
	}

	wc3lib::read<int32>(istream, this->m_unknown0, size);
	wc3lib::read<int32>(istream, number, size);

	for (int32 i = 0; i < number; ++i)
	{
		class Variable *variable = new Variable(this);
		this->m_variables.push_back(variable);
		size += variable->read(istream);
	}

	wc3lib::read<int32>(istream, number, size);

	for (int32 i = 0; i < number; ++i)
	{
		class Trigger *trigger = new Trigger(this);
		this->m_triggers.push_back(trigger);
		size += trigger->read(istream);
	}

	return size;
}

std::streamsize Triggers::write(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size;
	wc3lib::write<int32>(ostream, this->m_version, size);
	wc3lib::write<int32>(ostream, this->m_categories.size(), size);

	BOOST_FOREACH(const CategoryType category, this->m_categories)
		size += category.second->write(ostream);

	wc3lib::write<int32>(ostream, this->m_unknown0, size);
	wc3lib::write<int32>(ostream, this->m_variables.size(), size);

	BOOST_FOREACH(const class Variable *variable, this->m_variables)
		size += variable->write(ostream);

	wc3lib::write<int32>(ostream, this->m_triggers.size(), size);

	BOOST_FOREACH(const class Trigger *trigger, this->m_triggers)
		size += trigger->write(ostream);

	return size;
}

class TriggerCategory* Triggers::category(int32 index)
{
	std::map<int32, class TriggerCategory*>::iterator iterator = this->m_categories.find(index);

	if (iterator == this->m_categories.end())
		return 0;

	return iterator->second;
}

}

}
