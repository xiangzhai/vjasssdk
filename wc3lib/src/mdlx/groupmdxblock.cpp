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
	for (std::list<class GroupMdxBlockMember*>::iterator iterator = this->m_members.begin(); iterator != this->m_members.end(); ++iterator)
		delete *iterator;
}

long32 GroupMdxBlock::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	std::cout << "Block name " << this->blockName() << std::endl;
	long32 groupCount = 0;
	istream.read(reinterpret_cast<char*>(&groupCount), sizeof(groupCount));
	bytes += istream.gcount();
	std::cout << "Group count " << groupCount << std::endl;
	
	for ( ; groupCount > 0; --groupCount)
	{
		class GroupMdxBlockMember *member = this->createNewMember();
		bytes += member->readMdx(istream);
		this->m_members.push_back(member);
	}
	
	std::cout << "Group bytes " << bytes << std::endl;
	
	return bytes;
}

long32 GroupMdxBlock::writeMdx(std::ostream &ostream) throw (class Exception)
{
	if (!this->exists())
		return 0;
	
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	long32 groupCount = this->m_members.size();
	ostream.write(reinterpret_cast<const char*>(&groupCount), sizeof(groupCount));
	bytes += sizeof(groupCount);
	std::cout << "Group count " << groupCount << std::endl;
	
	for (std::list<class GroupMdxBlockMember*>::iterator iterator = this->m_members.begin(); iterator != this->m_members.end(); ++iterator)
	{
		bytes += (*iterator)->writeMdx(ostream);
	}
	
	std::cout << "Group bytes " << bytes << std::endl;
	
	return bytes;
}

class GroupMdxBlockMember* GroupMdxBlock::createNewMember()
{
	class GroupMdxBlockMember *member = new GroupMdxBlockMember(this);
	
	return member;
}

}

}
