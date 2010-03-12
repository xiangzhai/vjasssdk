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

#include <boost/format.hpp>

#include "ribbonemitterheightbelow.hpp"
#include "ribbonemitterheightsbelow.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

RibbonEmitterHeightBelow::RibbonEmitterHeightBelow(class RibbonEmitterHeightsBelow *heights) : m_heights(heights)
{
}

RibbonEmitterHeightBelow::~RibbonEmitterHeightBelow()
{
}

void RibbonEmitterHeightBelow::readMdl(std::istream &istream) throw (class Exception)
{
}

void RibbonEmitterHeightBelow::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 RibbonEmitterHeightBelow::readMdx(std::istream &istream) throw (class Exception)
{
	istream.read(reinterpret_cast<char*>(&this->m_frame), sizeof(this->m_frame));
	long32 bytes = istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_state), sizeof(this->m_state)); //(0 or 1)
	bytes += istream.gcount();
	
	if (this->m_state != 0 && this->m_state != 1)
		throw Exception(boost::str(boost::format(_("Unknown state: %1%. Should be 0 or 1.")) % this->m_state));
	
	if (this->m_heights->lineType() > 1)
	{
		istream.read(reinterpret_cast<char*>(&this->m_inTan), sizeof(this->m_inTan));
		bytes += istream.gcount();
		istream.read(reinterpret_cast<char*>(&this->m_outTan), sizeof(this->m_outTan));
		bytes += istream.gcount();
	}
	
	return bytes;
}

long32 RibbonEmitterHeightBelow::writeMdx(std::ostream &ostream) throw (class Exception)
{
	return 0;
}

}

}
