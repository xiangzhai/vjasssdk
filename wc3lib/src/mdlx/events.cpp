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

#include "events.hpp"
#include "event.hpp"

namespace wc3lib
{

namespace mdlx
{

Events::Events(class Mdlx *mdlx) : MdxBlock("EVTS"), m_mdlx(mdlx)
{
}

Events::~Events()
{
	for (std::list<class Event*>::iterator iterator = this->m_events.begin(); iterator != this->m_events.end(); ++iterator)
		delete *iterator;
}

void Events::readMdl(std::istream &istream) throw (class Exception)
{
}

void Events::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 Events::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();
	
	while (nbytes > 0)
	{
		class Event *event = new Event(this);
		long32 readBytes = event->readMdx(istream);
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_events.push_back(event);
	}
	
	return bytes;
}

long32 Events::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	return 0;
}

}

}
