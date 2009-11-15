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

#include <iostream> //debug

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

void Sequence::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 Sequence::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = 0;
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
	istream.read(reinterpret_cast<char*>(&this->m_boundsRadius), sizeof(this->m_boundsRadius));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_minExtX), sizeof(this->m_minExtX));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_minExtY), sizeof(this->m_minExtY));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_minExtZ), sizeof(this->m_minExtZ));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_maxExtX), sizeof(this->m_maxExtX));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_maxExtY), sizeof(this->m_maxExtY));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_maxExtZ), sizeof(this->m_maxExtZ));
	bytes += istream.gcount();
	std::cout << "Single sequence bytes " << bytes << std::endl;
	
	return bytes;
}

long32 Sequence::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = 0;
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
	ostream.write(reinterpret_cast<const char*>(&this->m_boundsRadius), sizeof(this->m_boundsRadius));
	bytes += sizeof(this->m_boundsRadius);
	ostream.write(reinterpret_cast<const char*>(&this->m_minExtX), sizeof(this->m_minExtX));
	bytes += sizeof(this->m_minExtX);
	ostream.write(reinterpret_cast<const char*>(&this->m_minExtY), sizeof(this->m_minExtY));
	bytes += sizeof(this->m_minExtY);
	ostream.write(reinterpret_cast<const char*>(&this->m_minExtZ), sizeof(this->m_minExtZ));
	bytes += sizeof(this->m_minExtZ);
	ostream.write(reinterpret_cast<const char*>(&this->m_maxExtX), sizeof(this->m_maxExtX));
	bytes += sizeof(this->m_maxExtX);
	ostream.write(reinterpret_cast<const char*>(&this->m_maxExtY), sizeof(this->m_maxExtY));
	bytes += sizeof(this->m_maxExtY);
	ostream.write(reinterpret_cast<const char*>(&this->m_maxExtZ), sizeof(this->m_maxExtZ));
	bytes += sizeof(this->m_maxExtZ);
	
	return bytes;
}

}

}
