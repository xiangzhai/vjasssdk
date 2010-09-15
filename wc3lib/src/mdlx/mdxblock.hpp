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

#ifndef WC3LIB_MDLX_MDXBLOCK_HPP
#define WC3LIB_MDLX_MDXBLOCK_HPP

#include <iostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class MdxBlock
{
	public:
		static const std::size_t blockNameSize = 4;

		MdxBlock(const byte blockName[blockNameSize], bool optional = true);
		virtual ~MdxBlock();

		/**
		* @return Returns the block name with size MdxBlock::blockNameSize.
		*/
		const byte* blockName() const;
		/**
		* Some MDX blocks such as translations, scalings and rotations are optional and do only appear if they're not inherited by parent object.
		* @return Returns true if the MDX block is optional which means it doesn't appear in output stream.
		*/
		bool optional() const;
		bool exists() const;

		virtual std::streamsize readMdx(std::basic_istream<byte> &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::basic_ostream<byte> &ostream) const throw (class Exception);

		bool moveToBlockName(std::basic_iostream<byte> &iostream);

	protected:
		byte m_blockName[blockNameSize]; /// @todo byte or ascii?
		bool m_optional;
		bool m_exists;
};

inline const byte* MdxBlock::blockName() const
{
	return this->m_blockName;
}

inline bool MdxBlock::optional() const
{
	return this->m_optional;
}

inline bool MdxBlock::exists() const
{
	return this->m_exists;
}

}

}

#endif
