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

#include "scaling0s.hpp"
#include "scaling0.hpp"

namespace wc3lib
{

namespace mdlx
{

Scaling0s::Scaling0s(class Mdlx *mdlx) : MdxBlock("KGSC"), m_mdlx(mdlx)
{
}

Scaling0s::~Scaling0s()
{
	for (std::list<class Scaling0>::iterator iterator = this->m_scalings.begin(); iterator != this->m_scalings.end(); ++iterator)
		delete *iterator;
}

void Scaling0s::readMdl(std::istream &istream) throw (class Exception)
{
}

void Scaling0s::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 Scaling0s::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nunks = 0;
	istream.read(reinterpret_cast<char*>(&nunks), sizeof(nunks));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_lineType), sizeof(this->m_lineType));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_globalSequenceId), sizeof(this->m_globalSequenceId));
	bytes += istream.gcount();
	
	for ( ; nunks > 0; --nunks)
	{
		class Scaling0 *scaling = new Scaling0(this);
		bytes += scaling->readMdx(istream);
		this->m_scalings.push_back(scaling);
	}
	
	return bytes;
}

long32 Scaling0s::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	return bytes;
}

}

}