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

#include "sequence.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

Sequence::Sequence(class Sequences *sequences) : GroupMdxBlockMember(sequences)
{
}

std::streamsize Sequence::readMdl(istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Sequence::writeMdl(ostream &ostream) const throw (class Exception)
{
	ostream
	<< "Anim " << this->name() << " {\n"
	<< "Interval { " << this->intervalStart() << ", " << this->intervalEnd() << " },\n"
	;

	if (this->noLooping() == 1)
		ostream << "NonLooping,\n";

	if (this->moveSpeed() != 0.0)
		ostream << "MoveSpeed " << this->moveSpeed() << ",\n";

	if (this->rarity() != 0.0)
		ostream << "Rarity " << this->rarity() << ",\n";

	Bounds::writeMdl(ostream);

	ostream << "}\n";

	return 0;
}

std::streamsize Sequence::readMdx(istream &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read(istream, m_name, size);
	wc3lib::read(istream, m_intervalStart, size);
	wc3lib::read(istream, m_intervalEnd, size);
	wc3lib::read(istream, m_moveSpeed, size);
	wc3lib::read(istream, m_noLooping, size);
	wc3lib::read(istream, m_rarity, size);
	wc3lib::read(istream, m_unknown0, size);
	size += Bounds::readMdx(istream);

	return size;
}

std::streamsize Sequence::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::write(ostream, m_name, size);
	wc3lib::write(ostream, m_intervalStart, size);
	wc3lib::write(ostream, m_intervalEnd, size);
	wc3lib::write(ostream, m_moveSpeed, size);
	wc3lib::write(ostream, m_noLooping, size);
	wc3lib::write(ostream, m_rarity, size);
	wc3lib::write(ostream, m_unknown0, size);
	size += Bounds::writeMdx(ostream);

	return size;
}

}

}
