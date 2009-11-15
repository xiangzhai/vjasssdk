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

#include "mdxalphas.hpp"
#include "mdxalpha.hpp"

namespace wc3lib
{
	
namespace mdlx
{

MdxAlphas::MdxAlphas(byte blockName[4], bool optional) : MdxBlock(blockName, optional)
{
}

MdxAlphas::~MdxAlphas()
{
	for (std::list<class MdxAlpha*>::iterator iterator = this->m_alphas.begin(); iterator != this->m_alphas.end(); ++iterator)
		delete *iterator;
}
		
long32 MdxAlphas::readMdx(std::istream &istream) throw (class Exception)
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
		class MdxAlpha *alpha = this->createNewMember();
		bytes += alpha->readMdx(istream);
		this->m_alphas.push_back(alpha);
	}
	
	return bytes;
}

long32 MdxAlphas::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	long32 nunks = this->m_alphas.size();
	ostream.write(reinterpret_cast<const char*>(&nunks), sizeof(nunks));
	bytes += sizeof(nunks);
	ostream.write(reinterpret_cast<const char*>(&this->m_lineType), sizeof(this->m_lineType));
	bytes += sizeof(this->m_lineType);
	ostream.write(reinterpret_cast<const char*>(&this->m_globalSequenceId), sizeof(this->m_globalSequenceId));
	bytes += sizeof(this->m_globalSequenceId);
	
	for (std::list<class MdxAlpha*>::iterator iterator = this->m_alphas.begin(); iterator != this->m_alphas.end(); ++iterator)
		bytes += (*iterator)->writeMdx(ostream);
	
	return bytes;
}

class MdxAlpha* MdxAlphas::createNewMember()
{
	return new MdxAlpha(this);
}

}

}
