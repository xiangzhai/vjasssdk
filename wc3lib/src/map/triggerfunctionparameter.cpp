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

#include "triggerfunctionparameer.hpp"
#include "utilities.hpp"

namespace wc3lib
{

namespace map
{

TriggerFunctionParameter::TriggerFunctionParameter(class TriggerFunction *function) : m_function(function), m_type(TriggerFunctionParameter::Preset),  m_value(), m_unknown0(0), m_unknown1(0)
{
}


std::streamsize TriggerFunctionParameter::read(std::istream &istream) throw (class Exception)
{
	std::streamsize size;
	read<int32>(istream, this->m_type, size);
	readString(istream, this->m_value, size);
	read<int32>(istream, this->m_unknown0, size);
	read<int32>(istream, this->m_unknown1, size);

	return size;
}


std::streamsize TriggerFunctionParameter::write(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size;
	write<int32>(ostream, this->m_type, size);
	writeString(ostream, this->m_value, size);
	write<int32>(ostream, this->m_unknown0, size);
	write<int32>(ostream, this->m_unknown1, size);

	return size;
}

}

}
