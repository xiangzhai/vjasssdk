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

#ifndef WC3LIB_MDLX_AMBINTENSITIES_HPP
#define WC3LIB_MDLX_AMBINTENSITIES_HPP

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
class AmbIntensity;

//KLBI	// [AmbIntensity]:KMTA;
class AmbIntensities : public MdxBlock
{
	public:
		enum LineType
		{
			DontInterp = 0,
			Linear = 1,
			Hermite = 2,
			Bezier = 3
		};

		AmbIntensities(class Mdlx *mdlx);
		virtual ~AmbIntensities();

		class Mdlx* mdlx() const;
		long32 lineType() const;
		long32 globalSequenceId() const;
		std::list<class AmbIntensity*> intensities() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		long32 m_lineType; //(0:don't interp;1:linear;2:hermite;3:bezier)
		long32 m_globalSequenceId; // 0xFFFFFFFF if none
		std::list<class AmbIntensity*> m_intensities;
};

inline class Mdlx* AmbIntensities::mdlx() const
{
	return this->m_mdlx;
}

inline long32 AmbIntensities::lineType() const
{
	return this->m_lineType;
}

inline long32 AmbIntensities::globalSequenceId() const
{
	return this->m_globalSequenceId;
}

inline std::list<class AmbIntensity*> AmbIntensities::intensities() const
{
	return this->m_intensities;
}

}

}

#endif
