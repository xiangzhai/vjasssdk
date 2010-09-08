/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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

#include "bounds.hpp"

namespace wc3lib
{

namespace mdlx
{

class Sequence : public Bounds
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

		virtual std::streamsize readMdl(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(std::ostream &ostream) const throw (class Exception);
		/**
		* @return Returns read byte count.
		*/
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class Sequences *m_sequences;
		ascii m_name[0x50]; //(0x50 bytes)
		long32 m_intervalStart, m_intervalEnd;
		float32 m_moveSpeed;
		long32 m_noLooping; //(0:loop; 1:no loop)
		float32 m_rarity;
		long32 m_unknown0; //(0)

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

}

}

#endif
