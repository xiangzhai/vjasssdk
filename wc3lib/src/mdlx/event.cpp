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
#include "../utilities.hpp"

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

std::streamsize Event::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Event::writeMdl(std::ostream &ostream) const throw (Exception)
{
	return 0;
}

std::streamsize Event::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = Object::readMdx(istream);
	size += this->m_tracks->readMdx(istream);

	return size;
}

std::streamsize Event::writeMdx(std::ostream &ostream) const throw (Exception)
{
	std::streamsize size = Object::writeMdx(ostream);
	size += this->m_tracks->writeMdx(ostream);

	return size;
}

}

}
