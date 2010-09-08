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

#include <boost/foreach.hpp>
#include <boost/cast.hpp>

#include "events.hpp"
#include "event.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

Events::Events(class Mdlx *mdlx) : MdxBlock("EVTS"), m_mdlx(mdlx)
{
}

Events::~Events()
{
	BOOST_FOREACH(class Event *event, this->m_events)
		delete event;
}

std::streamsize Events::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Events::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize Events::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);

	if (size == 0)
		return 0;

	long32 nbytes;
	wc3lib::read(istream, nbytes, size);

	while (nbytes > 0)
	{
		class Event *event = new Event(this);
		std::streamsize readSize = event->readMdx(istream);
		nbytes -= boost::numeric_cast<long32>(readSize);
		size += readSize;
		this->m_events.push_back(event);
	}

	return size;
}

std::streamsize Events::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = MdxBlock::writeMdx(ostream);

	if (bytes == 0)
		return 0;

	return bytes;
}

}

}
