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

#include "sequence.hpp"
#include "sequences.hpp"

namespace wc3lib
{

namespace mdlx
{

Sequence::Sequence(class Sequences *sequences) : m_sequences(sequences)
{
}

Sequence::~Sequence()
{
}

void Sequence::readMdl(std::istream &istream) throw (class Exception)
{
}

void Sequence::writeMdl(std::ostream &ostream) const throw (class Exception)
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
}

std::streamsize Sequence::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = 0;
	istream.read(this->m_name, sizeof(this->m_name));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_intervalStart), sizeof(this->m_intervalStart));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_intervalEnd), sizeof(this->m_intervalEnd));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_moveSpeed), sizeof(this->m_moveSpeed));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_noLooping), sizeof(this->m_noLooping));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_rarity), sizeof(this->m_rarity));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_unknown0), sizeof(this->m_unknown0));
	bytes += istream.gcount();
	bytes += Bounds::readMdx(istream);
	
	return bytes;
}

std::streamsize Sequence::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = 0;
	ostream.write(this->m_name, sizeof(this->m_name));
	bytes += sizeof(this->m_name);
	ostream.write(reinterpret_cast<const char*>(&this->m_intervalStart), sizeof(this->m_intervalStart));
	bytes += sizeof(this->m_intervalStart);
	ostream.write(reinterpret_cast<const char*>(&this->m_intervalEnd), sizeof(this->m_intervalEnd));
	bytes += sizeof(this->m_intervalEnd);
	ostream.write(reinterpret_cast<const char*>(&this->m_moveSpeed), sizeof(this->m_moveSpeed));
	bytes += sizeof(this->m_moveSpeed);
	ostream.write(reinterpret_cast<const char*>(&this->m_noLooping), sizeof(this->m_noLooping));
	bytes += sizeof(this->m_noLooping);
	ostream.write(reinterpret_cast<const char*>(&this->m_rarity), sizeof(this->m_rarity));
	bytes += sizeof(this->m_rarity);
	ostream.write(reinterpret_cast<const char*>(&this->m_unknown0), sizeof(this->m_unknown0));
	bytes += sizeof(this->m_unknown0);
	bytes += Bounds::writeMdx(ostream);
	
	return bytes;
}

}

}
