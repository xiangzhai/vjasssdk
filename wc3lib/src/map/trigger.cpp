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

#include "trigger.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace map
{

Trigger::Trigger(class Triggers *triggers) : m_trigger(triggers)
{
}

std::streamsize Trigger::read(std::istream &istream) throw (class Exception)
{
	std::streamsize size = 0;
	readString(istream, this->m_name, size);
	readString(istream, this->m_description, size);
	read<int32>(istream, this->m_isEnabled, size);
	read<int32>(istream, this->m_isCustomText, size);
	read<int32>(istream, this->m_isInitiallyOn, size);
	read<int32>(istream, this->m_unknown, size);
	int32 categoryIndex;
	read<int32>(istream, categoryIndex, size);
	this->m_triggerCategory = this->m_triggers->category(categoryIndex);
	int32 functions;
	read<int32>(istream, functions, size);

	for (int32 i = 0; i < functions; ++i)
	{
		class TriggerFunction *function = new TriggerFunction(this);
		this->m_functions.push_back(function);
		size += function->read(istream);
	}

	return size;
}

std::streamsize Trigger::write(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = 0;

	writeString(ostream, this->m_name, size);
	writeString(ostream, this->m_description, size);
	write<int32>(ostream, this->m_isEnabled, size);
	write<int32>(ostream, this->m_isCustomText, size);
	write<int32>(ostream, this->m_isInitiallyOn, size);
	write<int32>(ostream, this->m_unknown, size);
	write<int32>(ostream, this->m_category->index(), size);
	write<int32>(ostream, this->m_functions.size(), size);

	BOOST_FOREACH(const class TriggerFunction *function)
		size += function->write(ostream);

	return size;
}

}

}
