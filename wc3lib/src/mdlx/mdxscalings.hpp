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

#ifndef WC3LIB_MDLX_MDXSCALINGS_HPP
#define WC3LIB_MDLX_MDXSCALINGS_HPP

#include <list>

#include "mdxblock.hpp"

namespace wc3lib
{
	
namespace mdlx
{

class MdxScaling;

class MdxScalings : public MdxBlock
{
	public:
		enum LineType
		{
			DontInterp = 0,
			Linear = 1,
			Hermite = 2,
			Bezier = 3
		};
	
		MdxScalings(byte blockName[4], bool optional = true);
		virtual ~MdxScalings();
		
		long32 lineType() const;
		
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);
		
	protected:
		/**
		* This method should be overwritten in child class.
		* @return Returns a new allocated group member which will be added to list.
		*/
		virtual class MdxScaling* createNewMember();
		
		long32 m_lineType; //(0:don't interp;1:linear;2:hermite;3:bezier)
		long32 m_globalSequenceId; // 0xFFFFFFFF if none
		std::list<class MdxScaling*> m_scalings;
};

inline long32 MdxScalings::lineType() const
{
	return this->m_lineType;
}

}

}

#endif
