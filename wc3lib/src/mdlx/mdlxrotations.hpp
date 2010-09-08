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

#ifndef WC3LIB_MDLX_MDLXROTATIONS_HPP
#define WC3LIB_MDLX_MDLXROTATIONS_HPP

#include <list>

#include "mdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{

/// Block name KGRT.
class MdlxRotations : public MdxBlock
{
	public:
		MdlxRotations(class Mdlx *mdlx);
		virtual ~MdlxRotations();

		class Mdlx* mdlx() const;
		enum LineType lineType() const;
		long32 globalSequenceId() const;
		bool hasGlobalSequence() const;
		const std::list<class MdlxRotation*>& mdlxRotations() const;

		virtual std::streamsize readMdl(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		enum LineType m_lineType; //(0:don't interp;1:linear;2:hermite;3:bezier)
		long32 m_globalSequenceId; // 0xFFFFFFFF if none
		std::list<class MdlxRotation*> m_rotations;
};

inline class Mdlx* MdlxRotations::mdlx() const
{
	return this->m_mdlx;
}

inline enum LineType MdlxRotations::lineType() const
{
	return this->m_lineType;
}

inline long32 MdlxRotations::globalSequenceId() const
{
	return this->m_globalSequenceId;
}

inline bool MdlxRotations::hasGlobalSequence() const
{
	return this->m_globalSequenceId != 0xFFFFFFFF;
}

inline const std::list<class MdlxRotation*>& MdlxRotations::mdlxRotations() const
{
	return this->m_rotations;
}

}

}

#endif

