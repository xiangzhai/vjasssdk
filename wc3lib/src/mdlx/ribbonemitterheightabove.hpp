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

#ifndef WC3LIB_MDLX_RIBBONEMITTERHEIGHTABOVE_HPP
#define WC3LIB_MDLX_RIBBONEMITTERHEIGHTABOVE_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class RibbonEmitterHeightsAbove;

class RibbonEmitterHeightAbove
{
	public:
		RibbonEmitterHeightAbove(class RibbonEmitterHeightsAbove *heights);
		virtual ~RibbonEmitterHeightAbove();

		class RibbonEmitterHeightsAbove* heights() const;
		long32 frame() const;
		float32 state() const;
		float32 inTan() const;
		float32 outTan() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual long32 readMdx(std::fstream &fstream) throw (class Exception);
		virtual long32 writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class RibbonEmitterHeightsAbove *m_heights;
		long32 m_frame;
		float32 m_state; //(0 or 1)
		//if (LineType > 1) {
		float32 m_inTan;
		float32 m_outTan;
		//}
};

inline class RibbonEmitterHeightsAbove* RibbonEmitterHeightAbove::heights() const
{
	return this->m_heights;
}

inline long32 RibbonEmitterHeightAbove::frame() const
{
	return this->m_frame;
}

inline float32 RibbonEmitterHeightAbove::state() const
{
	return this->m_state;
}

inline float32 RibbonEmitterHeightAbove::inTan() const
{
	return this->m_inTan;
}

inline float32 RibbonEmitterHeightAbove::outTan() const
{
	return this->m_outTan;
}

}

}

#endif
