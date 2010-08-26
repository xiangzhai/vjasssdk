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

#include "triggerfunction.hpp"
#include "triggerfunctionparameter.hpp"
#include "../utilities.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace map
{

TriggerFunction::TriggerFunction(class Trigger *trigger) : m_trigger(trigger), m_type(TriggerFunction::Event), m_name(), m_isEnabled(false), m_parameters()
{
}

std::streamsize TriggerFunction::read(std::istream &istream) throw (class Exception)
{
	std::streamsize size;
	int32 type;
	wc3lib::read(istream, type, size);
	this->m_type = static_cast<enum Type>(type);
	readString(istream, this->m_name, size);
	int32 isEnabled;
	wc3lib::read(istream, isEnabled, size);
	this->m_isEnabled = static_cast<bool>(isEnabled);

	/// @todo get trigger parameters, hardcoded

	return size;
}

std::streamsize TriggerFunction::write(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size;
	wc3lib::write<int32>(ostream, this->m_type, size);
	writeString(ostream, this->m_name, size);
	wc3lib::write<int32>(ostream, this->m_isEnabled, size);

	BOOST_FOREACH(const class TriggerFunctionParameter *parameter, this->m_parameters)
		size += parameter->write(ostream);

	return size;
}

}

}
