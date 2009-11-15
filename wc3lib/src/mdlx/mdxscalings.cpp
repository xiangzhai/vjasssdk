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

#include "mdxscalings.hpp"
#include "mdxscaling.hpp"

namespace wc3lib
{
	
namespace mdlx
{

MdxScalings::MdxScalings(byte blockName[4], bool optional) : MdxBlock(blockName, optional)
{
}

MdxScalings::~MdxScalings()
{
	for (std::list<class MdxScaling*>::iterator iterator = this->m_scalings.begin(); iterator != this->m_scalings.end(); ++iterator)
		delete *iterator;
}
		
long32 MdxScalings::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nunks;
	istream.read(reinterpret_cast<char*>(&nunks), sizeof(nunks));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_lineType), sizeof(this->m_lineType));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_globalSequenceId), sizeof(this->m_globalSequenceId));
	bytes += istream.gcount();

	for ( ; nunks > 0; --nunks)
	{
		class MdxScaling *scaling = this->createNewMember();
		bytes += scaling->readMdx(istream);
		this->m_scalings.push_back(scaling);
	}
	
	return bytes;
}

long32 MdxScalings::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	long32 nunks = this->m_scalings.size();
	ostream.write(reinterpret_cast<const char*>(&nunks), sizeof(nunks));
	bytes += sizeof(nunks);
	ostream.write(reinterpret_cast<const char*>(&this->m_lineType), sizeof(this->m_lineType));
	bytes += sizeof(this->m_lineType);
	ostream.write(reinterpret_cast<const char*>(&this->m_globalSequenceId), sizeof(this->m_globalSequenceId));
	bytes += sizeof(this->m_globalSequenceId);
	
	for (std::list<class MdxScaling*>::iterator iterator = this->m_scalings.begin(); iterator != this->m_scalings.end(); ++iterator)
		bytes += (*iterator)->writeMdx(ostream);
	
	return bytes;
}

class MdxScaling* MdxScalings::createNewMember()
{
	return new MdxScaling(this);
}

}

}
