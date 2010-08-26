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

#ifndef WC3LIB_MAP_TRIGGERFUNCTION_HPP
#define WC3LIB_MAP_TRIGGERFUNCTION_HPP

#include <string>
#include <list>

#include "../format.hpp"

namespace wc3lib
{

namespace map
{

/**
* @see TriggerFunctionEx
*/
class TriggerFunction : public Format
{
	public:
		enum Type
		{
			Event,
			Condition,
			Action
		};

		TriggerFunction(class Trigger *trigger);

		virtual std::streamsize read(std::istream &istream) throw (class Exception);
		virtual std::streamsize write(std::ostream &ostream) const throw (class Exception);


	protected:
		class Trigger *m_trigger;
		enum Type m_type;
		std::string m_name;
		bool m_isEnabled;
		std::list<class TriggerFunctionParameter*> m_parameters;
};

}

}

#endif
