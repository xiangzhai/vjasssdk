/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
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

#include <boost/foreach.hpp>
#include <boost/cast.hpp>

#include "eventtracks.hpp"
#include "eventtrack.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

EventTracks::EventTracks(class Event *event) : MdxBlock("KEVT"), m_event(event)
{
}

EventTracks::~EventTracks()
{
	BOOST_FOREACH(class EventTrack *track, this->m_tracks)
		delete track;
}

std::streamsize EventTracks::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize EventTracks::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize EventTracks::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);
	long32 number;
	wc3lib::read(istream, number, size);
	wc3lib::read(istream, this->m_globalSequenceId, size);

	for ( ; number > 0; --number)
	{
		class EventTrack *track = new EventTrack(this);
		size += track->readMdx(istream);
		this->m_tracks.push_back(track);
	}

	return size;
}

std::streamsize EventTracks::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = MdxBlock::writeMdx(ostream);
	long32 number = boost::numeric_cast<long32>(this->m_tracks.size());
	wc3lib::write(ostream, number, size);
	wc3lib::write(ostream, this->m_globalSequenceId, size);

	BOOST_FOREACH(const class EventTrack *track, this->m_tracks)
		size += track->writeMdx(ostream);

	return size;
}

}

}
