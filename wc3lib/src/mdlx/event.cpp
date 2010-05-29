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

#include "event.hpp"
#include "events.hpp"
#include "eventtracks.hpp"

namespace wc3lib
{

namespace mdlx
{

Event::Event(class Events *events) : Object(events->mdlx()), m_events(events), m_tracks(new EventTracks(this))
{
}

Event::~Event()
{
	delete this->m_tracks;
}

void Event::readMdl(std::istream &istream) throw (class Exception)
{
}

void Event::writeMdl(std::ostream &ostream) const throw (Exception)
{
}

std::streamsize Event::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = Object::readMdx(istream);
	bytes += this->m_tracks->readMdx(istream);
	long32 ntrks = 0; // usually (1)
	istream.read(reinterpret_cast<char*>(&ntrks), sizeof(ntrks));
	bytes += istream.gcount();
	long32 tmpValue; //0xFFFFFFFF, 8 byte value
	istream.read(reinterpret_cast<char*>(&tmpValue), sizeof(tmpValue));
	
	for ( ; ntrks > 0; --ntrks)
	{
		long32 frame;
		istream.read(reinterpret_cast<char*>(&frame), sizeof(frame));
		bytes += istream.gcount();
		this->m_frames.push_back(frame);
	}
	
	return bytes;
}

std::streamsize Event::writeMdx(std::ostream &ostream) const throw (Exception)
{
	return 0;
}

}

}
