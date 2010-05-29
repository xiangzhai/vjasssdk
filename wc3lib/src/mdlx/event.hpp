/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.de                                                      *
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

#ifndef WC3LIB_MDLX_EVENT_HPP
#define WC3LIB_MDLX_EVENT_HPP

#include <list>

#include "object.hpp"

namespace wc3lib
{

namespace mdlx
{
	
class Events;
class EventTracks;

class Event : public Object
{
	public:
		Event(class Events *events);
		virtual ~Event();

		class Events* events() const;
		class EventTracks* tracks() const;
		const std::list<long32>& frames() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class Events *m_events;
		//OBJ
		//ascii *bla; //ASCII "KEVT" // Actually a separate object
		//long32 ntrks; // usually (1)
		//0xFFFFFFFF!!!
		class EventTracks *m_tracks; //KEVT
		std::list<long32> m_frames;//[ntrks];
};

inline class Events* Event::events() const
{
	return this->m_events;
}

inline class EventTracks* Event::tracks() const
{
	return this->m_tracks;
}

inline const std::list<long32>& Event::frames() const
{
	return this->m_frames;
}

}

}

#endif
