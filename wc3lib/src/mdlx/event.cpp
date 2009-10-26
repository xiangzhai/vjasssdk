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
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Event::Event(class Events *events) : Object(events->mdlx()), m_events(events)
{
}

Event::~Event()
{
}

void Event::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Event::writeMdl(std::fstream &fstream) throw (Exception)
{
}

long32 Event::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = Object::readMdx(fstream);
	ascii tag[5];
	tag[4] = '\0';
	fstream.read(reinterpret_cast<char*>(tag), sizeof(tag));
	bytes += fstream.gcount();
	
	static const char *expectedTag = "KEVT";
	
	if (memcmp(tag, expectedTag, 4) != 0)
	{
		char message[50];
		sprintf(message, _("Event: Expected %s tag, got %s.\n"), expectedTag, tag);
		
		throw Exception(message);
	}

	long32 ntrks = 0; // usually (1)
	fstream.read(reinterpret_cast<char*>(&ntrks), sizeof(ntrks));
	bytes += fstream.gcount();
	long32 tmpValue; //0xFFFFFFFF, 8 byte value
	fstream.read(reinterpret_cast<char*>(&tmpValue), sizeof(tmpValue));
	
	for ( ; ntrks > 0; --ntrks)
	{
		long32 frame;
		fstream.read(reinterpret_cast<char*>(&frame), sizeof(frame));
		bytes += fstream.gcount();
		this->m_frames.push_back(frame);
	}
	
	return bytes;
}

long32 Event::writeMdx(std::fstream &fstream) throw (Exception)
{
	return 0;
}

}

}
