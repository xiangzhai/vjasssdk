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

#ifndef WC3LIB_MDLX_SEQUENCE_HPP
#define WC3LIB_MDLX_SEQUENCE_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Sequences;

class Sequence
{
	public:
		Sequence(class Sequences *sequences);
		virtual ~Sequence();

		class Sequences* sequences() const;
		const ascii* name() const;
		long32 intervalStart() const;
		long32 intervalEnd() const;
		float32 moveSpeed() const;
		long32 noLooping() const;
		float32 rarity() const;
		long32 unknown0() const;
		float32 boundsRadius() const;
		float32 minExtX() const;
		float32 minExtY() const;
		float32 minExtZ() const;
		float32 maxExtX() const;
		float32 maxExtY() const;
		float32 maxExtZ() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		/**
		* @return Returns read byte count.
		*/
		virtual long32 readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Sequences *m_sequences;
		ascii m_name[0x50]; //(0x50 bytes)
		long32 m_intervalStart, m_intervalEnd;
		float32 m_moveSpeed;
		long32 m_noLooping; //(0:loop; 1:no loop)
		float32 m_rarity;
		long32 m_unknown0; //(0)
		float32 m_boundsRadius;
		float32 m_minExtX, m_minExtY, m_minExtZ;
		float32 m_maxExtX, m_maxExtY, m_maxExtZ;

};

inline class Sequences* Sequence::sequences() const
{
	return this->m_sequences;
}

inline const ascii* Sequence::name() const
{
	return this->m_name;
}

inline long32 Sequence::intervalStart() const
{
	return this->m_intervalStart;
}

inline long32 Sequence::intervalEnd() const
{
	return this->m_intervalEnd;
}

inline float32 Sequence::moveSpeed() const
{
	return this->m_moveSpeed;
}

inline long32 Sequence::noLooping() const
{
	return this->m_noLooping;
}

inline float32 Sequence::rarity() const
{
	return this->m_rarity;
}

inline long32 Sequence::unknown0() const
{
	return this->m_unknown0;
}

inline float32 Sequence::boundsRadius() const
{
	return this->m_boundsRadius;
}

inline float32 Sequence::minExtX() const
{
	return this->m_minExtX;
}

inline float32 Sequence::minExtY() const
{
	return this->m_minExtY;
}

inline float32 Sequence::minExtZ() const
{
	return this->m_minExtZ;
}

inline float32 Sequence::maxExtX() const
{
	return this->m_maxExtX;
}

inline float32 Sequence::maxExtY() const
{
	return this->m_maxExtY;
}

inline float32 Sequence::maxExtZ() const
{
	return this->m_maxExtZ;
}

}

}

#endif
