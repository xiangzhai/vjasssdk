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

#ifndef WC3LIB_MDLX_AMBIENTCOLORS_HPP
#define WC3LIB_MDLX_AMBIENTCOLORS_HPP

#include <fstream>
#include <list>

#include "mdxblock.hpp"
#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Light;
class AmbientColor;

//KLBC
class AmbientColors : public MdxBlock
{
	public:
		enum LineType
		{
			DontInterp = 0,
			Linear = 1,
			Hermite = 2,
			Bezier = 3
		};

		AmbientColors(class Light *light);
		virtual ~AmbientColors();

		class Light* light() const;
		long32 lineType() const;
		long32 globalSequenceId() const;
		std::list<class AmbientColor*> colors() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual long32 readMdx(std::fstream &fstream) throw (class Exception);
		virtual long32 writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Light *m_light;
		long32 m_lineType; //(0:don't interp;1:linear;2:hermite;3:bezier)
		long32 m_globalSequenceId; // 0xFFFFFFFF if none
		std::list<class AmbientColor*> m_colors;
};

inline class Light* AmbientColors::light() const
{
	return this->m_light;
}

inline long32 AmbientColors::lineType() const
{
	return this->m_lineType;
}

inline long32 AmbientColors::globalSequenceId() const
{
	return this->m_globalSequenceId;
}

inline std::list<class AmbientColor*> AmbientColors::colors() const
{
	return this->m_colors;
}

}

}

#endif
