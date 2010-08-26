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

#ifndef WC3LIB_MAP_TRIGGERFUNCTIONPARAMETER_HPP
#define WC3LIB_MAP_TRIGGERFUNCTIONPARAMETER_HPP

#include <string>

#include "platform.hpp"
#include "../format.hpp"

namespace wc3lib
{

namespace map
{

/**
* @see TriggerFunctionParameterEx
*/
class TriggerFunctionParameter : public Format
{
	public:
		enum Type
		{
			Preset,
			Variable,
			Function
		};

		TriggerFunctionParameter(class TriggerFunction *function);

		virtual std::streamsize read(std::istream &istream) throw (class Exception);
		virtual std::streamsize write(std::ostream &ostream) const throw (class Exception);

	protected:
		class TriggerFunction *m_function;
		enum Type m_type;
		std::string m_value;
		int32 m_unknown0;
		int32 m_unknown1;
};

}

}

#endif
