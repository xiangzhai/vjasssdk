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

#ifndef WC3LIB_MAP_VARIABLE_HPP
#define WC3LIB_MAP_VARIABLE_HPP

#include <istream>
#include <ostream>
#include <string>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace map
{

/**
* @see VariableEx
*/
class Variable
{
	public:
		Variable(class Triggers *triggers);

		std::streamsize read(std::istream &istream) throw (class Exception);
		std::streamsize write(std::ostream &ostream) const throw (class Exception);

	protected:
		std::string m_name;
		std::string m_type;
		int32 m_number;
		bool m_isArray;
		bool m_isInitialized;
		std::string m_initialValue;
};

}

}

#endif
