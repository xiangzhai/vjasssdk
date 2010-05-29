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

#ifndef WC3LIB_MDLX_GROUPMDXBLOCK_HPP
#define WC3LIB_MDLX_GROUPMDXBLOCK_HPP

#include <list>

#include "mdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{

class GroupMdxBlockMember;

/**
* Some MDX blocks are structured like:
* byte[4]	MDX block name
* long32	group members count
* ...		group members list
* This class provides a simple abstraction layer for those MDX block classes.
*/
class GroupMdxBlock : public MdxBlock
{
	public:
		GroupMdxBlock(byte blockName[4], bool optional = true);
		~GroupMdxBlock();
		
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);
		
	protected:
		/**
		* This method should be overwritten in child class.
		* @return Returns a new allocated group member which will be added to list.
		*/
		virtual class GroupMdxBlockMember* createNewMember();
		
		/**
		* Provides access to all read members for child class.
		*/
		std::list<class GroupMdxBlockMember*> m_members;
};

}

}

#endif
