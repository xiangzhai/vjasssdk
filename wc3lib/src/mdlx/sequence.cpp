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

void Sequence::readMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Sequence::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = 0;
	fstream.read(this->m_name, sizeof(this->m_name));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_intervalStart), sizeof(this->m_intervalStart));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_intervalEnd), sizeof(this->m_intervalEnd));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_moveSpeed), sizeof(this->m_moveSpeed));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_noLooping), sizeof(this->m_noLooping));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_rarity), sizeof(this->m_rarity));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_unknown0), sizeof(this->m_unknown0));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_boundsRadius), sizeof(this->m_boundsRadius));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_minExtX), sizeof(this->m_minExtX));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_minExtY), sizeof(this->m_minExtY));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_minExtZ), sizeof(this->m_minExtZ));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_maxExtX), sizeof(this->m_maxExtX));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_maxExtY), sizeof(this->m_maxExtY));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_maxExtZ), sizeof(this->m_maxExtZ));
	bytes += fstream.gcount();
	
	return bytes;
}

void Sequence::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

void Sequence::writeMdx(std::fstream &fstream) throw (class Exception)
{
}

}

}