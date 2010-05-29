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

#include "groupmdxblock.hpp"
#include "groupmdxblockmember.hpp"

namespace wc3lib
{
	
namespace mdlx
{

GroupMdxBlock::GroupMdxBlock(byte blockName[4], bool optional) : MdxBlock(blockName, optional)
{
}

GroupMdxBlock::~GroupMdxBlock()
{
	BOOST_FOREACH(class GroupMdxBlockMember *groupMdxBlockMember, this->m_members)
		delete groupMdxBlockMember;
}

std::streamsize GroupMdxBlock::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;

	long32 groupCount = 0;
	istream.read(reinterpret_cast<char*>(&groupCount), sizeof(groupCount));
	bytes += istream.gcount();
	
	for ( ; groupCount > 0; --groupCount)
	{
		class GroupMdxBlockMember *member = this->createNewMember();
		bytes += member->readMdx(istream);
		this->m_members.push_back(member);
	}
	
	return bytes;
}

std::streamsize GroupMdxBlock::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	if (!this->exists())
		return 0;
	
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	
	long32 groupCount = this->m_members.size();
	ostream.write(reinterpret_cast<const char*>(&groupCount), sizeof(groupCount));
	bytes += sizeof(groupCount);
	
	BOOST_FOREACH(const class GroupMdxBlockMember *groupMdxBlockMember, this->m_members)
		bytes += groupMdxBlockMember->writeMdx(ostream);
	
	return bytes;
}

class GroupMdxBlockMember* GroupMdxBlock::createNewMember()
{
	class GroupMdxBlockMember *member = new GroupMdxBlockMember(this);
	
	return member;
}

}

}
