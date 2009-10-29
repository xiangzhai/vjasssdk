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

#include "translation1.hpp"
#include "translation1s.hpp"

namespace wc3lib
{

namespace mdlx
{
	
Translation1::Translation1(class Translation1s *translations) : m_translations(translations)
{
}

void Translation1::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Translation1::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Translation1::readMdx(std::fstream &fstream) throw (class Exception)
{
	fstream.read(reinterpret_cast<char*>(&this->m_frame), sizeof(this->m_frame));
	long32 bytes = fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_x), sizeof(this->m_x));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_y), sizeof(this->m_y));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_z), sizeof(this->m_z));
	bytes += fstream.gcount();
	
	if (this->m_translations->lineType() > 1)
	{
		fstream.read(reinterpret_cast<char*>(&this->m_inTanX), sizeof(this->m_inTanX));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_inTanY), sizeof(this->m_inTanY));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_inTanZ), sizeof(this->m_inTanZ));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_outTanX), sizeof(this->m_outTanX));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_outTanY), sizeof(this->m_outTanY));
		bytes += fstream.gcount();
		fstream.read(reinterpret_cast<char*>(&this->m_outTanZ), sizeof(this->m_outTanZ));
		bytes += fstream.gcount();
	}
	
	return bytes;
}

long32 Translation1::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
