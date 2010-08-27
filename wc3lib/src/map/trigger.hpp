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

#ifndef WC3LIB_MAP_TRIGGER_HPP
#define WC3LIB_MAP_TRIGGER_HPP

#include <string>
#include <list>

#include "platform.hpp"
#include "../format.hpp"

namespace wc3lib
{

namespace map
{

/**
* Definition of a Warcraft 3 trigger. For Warcraft 3 The Frozen Trigger use TriggerEx.
* @see TriggerEx
*/
class Trigger : public Format
{
	public:
		Trigger(class Triggers *triggers);

		virtual std::streamsize read(std::istream &istream) throw (class Exception);
		virtual std::streamsize write(std::ostream &ostream) const throw (class Exception);

	protected:
		class Triggers *m_triggers;
		std::string m_name;
		std::string m_description;
		bool m_isEnabled;
		bool m_isCustomText;
		bool m_isInitiallyOn;
		int32 m_unknown;
		class TriggerCategory *m_category;
		std::list<class TriggerFunction*> m_functions;
};

}

}

#endif