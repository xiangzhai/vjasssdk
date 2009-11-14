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

#include "eventtracks.hpp"
#include "eventtrack.hpp"

namespace wc3lib
{

namespace mdlx
{

EventTracks::EventTracks(class Event *event) : MdxBlock("KEVT"), m_event(event)
{
}

EventTracks::~EventTracks()
{
	for (std::list<class EventTrack*>::iterator iterator = this->m_tracks.begin(); iterator != this->m_tracks.end(); ++iterator)
		delete *iterator;
}

void EventTracks::readMdl(std::istream &istream) throw (class Exception)
{
}

void EventTracks::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 EventTracks::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	/// @todo Unspecified
	
	return bytes;
}

long32 EventTracks::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	return 0;
}

}

}
