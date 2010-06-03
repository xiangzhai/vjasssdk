/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#include "rect.hpp"
#include "../utilities.hpp"

namespace wc3lib::map
{

Rect::Rect(class Rects *rects) : m_rects(rects)
{
}

std::streamsize Rect::read(std::istream &istream) throw (class Exception)
{
	std::streamsize size;
	read(istream, this->m_left, size);
	read(istream, this->m_right, size);
	read(istream, this->m_bottom, size);
	read(istream, this->m_top, size);
	readString(istream, this->m_name, size);
	read(istream, this->m_index, size);
	read(istream, this->m_weatherEffectId, size);
	readString(istream, this->m_soundName, size);
	read(istream, this->m_color, size);

	return size;
}

std::streamsize Rect::write(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size;
	write(ostream, this->m_left, size);
	write(ostream, this->m_right, size);
	write(ostream, this->m_bottom, size);
	write(ostream, this->m_top, size);
	writeString(ostream, this->m_name, size);
	write(ostream, this->m_index, size);
	write(ostream, this->m_weatherEffectId, size);
	writeString(ostream, this->m_soundName, size);
	write(ostream, this->m_color, size);

	return size;
}

}
