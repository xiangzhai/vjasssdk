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

namespace wc3lib
{

namespace map
{

/**
 * \todo Add derived class TriggersX.
 * \sa TriggersX
 */
class Triggers : public Format
{
	public:
		typedef std::pair<int32, class TriggerCategory*> CategoryType;

		Triggers(class W3m *w3m);

		virtual std::streamsize read(InputStream &istream) throw (class Exception);
		virtual std::streamsize write(OutputStream &ostream) const throw (class Exception);
		
		virtual int32 fileId() const;
		virtual const char8* fileName() const;
		virtual int32 latestFileVersion() const;
		
		virtual int32 version() const;

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

inline int32 Triggers::fileId() const
{
	return (int32)"WTG!";
}

inline const char8* Triggers::fileName() const
{
	return "war3map.wtg";
}

inline int32 Triggers::latestFileVersion() const
{
	return 4;
}

inline int32 Triggers::version() const
{
	return m_version;
}

}

}

#endif
