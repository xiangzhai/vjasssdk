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

#ifndef WC3LIB_MAP_TRIGGERS_HPP
#define WC3LIB_MAP_TRIGGERS_HPP

#include <map>
#include <list>

#include "platform.hpp"
#include "../format.hpp"

namespace wc3lib
{

namespace map
{

/**
* @see TriggersEx
*/
class Triggers : public Format
{
	public:
		typedef std::pair<int32, class TriggerCategory*> CategoryType;

		static const int32 version;
		static const char* fileName;

		Triggers(class W3m *w3m);

		virtual std::streamsize read(std::istream &istream) throw (class Exception);
		virtual std::streamsize write(std::ostream &ostream) const throw (class Exception);

	protected:
		friend class Trigger;

		class TriggerCategory* category(int32 index);

		class W3m *m_w3m;
		int32 m_version;
		int32 m_unknown0;
		std::map<int32, class TriggerCategory*> m_categories;
		std::list<class Variable*> m_variables;
		std::list<class Trigger*> m_triggers;
};

}

}

#endif
