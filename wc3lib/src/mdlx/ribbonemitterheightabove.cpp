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

#include <cstdio>

#include "ribbonemitterheightabove.hpp"
#include "ribbonemitterheightsabove.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

RibbonEmitterHeightAbove::RibbonEmitterHeightAbove(class RibbonEmitterHeightsAbove *heights) : m_heights(heights)
{
}

RibbonEmitterHeightAbove::~RibbonEmitterHeightAbove()
{
}

void RibbonEmitterHeightAbove::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void RibbonEmitterHeightAbove::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 RibbonEmitterHeightAbove::readMdx(std::fstream &fstream) throw (class Exception)
{
	fstream.read(reinterpret_cast<char*>(&this->m_frame), sizeof(this->m_frame));
	long32 bytes = fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_state), sizeof(this->m_state)); //(0 or 1)
	bytes += fstream.gcount();
	
	if (this->m_state != 0 && this->m_state != 1)
	{
		char message[50];
		sprintf(message, _("Unknown state: %d. Should be 0 or 1."), this->m_state);
		
		throw Exception(message);
	}
	
	if (this->m_heights->lineType() > 1)
	{
		fstream.read(reinterpret_cast<char*>(&this->m_inTan), sizeof(this->m_inTan));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_outTan), sizeof(this->m_outTan));
		bytes += fstream.gcount();
	}
	
	return bytes;
}

long32 RibbonEmitterHeightAbove::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}