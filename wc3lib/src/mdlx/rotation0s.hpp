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

#ifndef WC3LIB_MDLX_ROTATION0S_HPP
#define WC3LIB_MDLX_ROTATION0S_HPP

#include <fstream>
#include <list>

#include "mdxblock.hpp"
#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;
class Rotation0;

//KGRT
class Rotation0s : public MdxBlock
{
	public:
		enum LineType
		{
			DontInterp = 0,
			Linear = 1,
			Hermite = 2,
			Bezier = 3
		};

		Rotation0s(class Mdlx *mdlx);
		virtual ~Rotation0s();

		class Mdlx* mdlx() const;
		long32 lineType() const;
		long32 globalSequenceId() const;
		std::list<class Rotation0*> rotations() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		long32 m_lineType; //(0:don't interp;1:linear;2:hermite;3:bezier)
		long32 m_globalSequenceId; // 0xFFFFFFFF if none
		std::list<class Rotation0*> m_rotations;
};

inline class Mdlx* Rotation0s::mdlx() const
{
	return this->m_mdlx;
}

inline long32 Rotation0s::lineType() const
{
	return this->m_lineType;
}

inline long32 Rotation0s::globalSequenceId() const
{
	return this->m_globalSequenceId;
}

inline std::list<class Rotation0*> Rotation0s::rotations() const
{
	return this->m_rotations;
}

}

}

#endif

