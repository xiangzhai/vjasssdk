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

#include "groupmdxblock.hpp"
#include "groupmdxblockmember.hpp"
#include "../utilities.hpp"

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

std::streamsize GroupMdxBlock::readMdx(istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);

	if (size == 0)
		return 0;

	long32 groupCount = 0;
	wc3lib::read(istream, groupCount, size);

	for ( ; groupCount > 0; --groupCount)
	{
		class GroupMdxBlockMember *member = this->createNewMember();
		size += member->readMdx(istream);
		this->m_members.push_back(member);
	}

	return size;
}

std::streamsize GroupMdxBlock::writeMdx(ostream &ostream) const throw (class Exception)
{
	if (!this->exists())
		return 0;

	std::streamsize size = MdxBlock::writeMdx(ostream);

	long32 groupCount = this->m_members.size();
	wc3lib::write(ostream, groupCount, size);

	BOOST_FOREACH(const class GroupMdxBlockMember *groupMdxBlockMember, this->m_members)
		size += groupMdxBlockMember->writeMdx(ostream);

	return size;
}

class GroupMdxBlockMember* GroupMdxBlock::createNewMember()
{
	class GroupMdxBlockMember *member = new GroupMdxBlockMember(this);

	return member;
}

}

}
