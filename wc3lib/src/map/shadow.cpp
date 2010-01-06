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

#include "shadow.hpp"
#include "w3m.hpp"

namespace wc3lib
{
	
namespace map
{

Shadow::Shadow(class W3m *w3m) : m_w3m(w3m), m_data(new char8[Shadow::shadowPointsPerTileset * w3m->width() * w3m->height()])
{
}

Shadow::~Shadow()
{
	delete[] this->m_data;
}

std::streamsize Shadow::read(std::istream &istream) throw (class Exception)
{
	istream.read(this->m_data, sizeof(this->m_data));
	
	return istream.gcount();
}

std::streamsize Shadow::write(std::ostream &ostream) throw (class Exception)
{
	ostream.write(this->m_data, sizeof(this->m_data));
	
	return sizeof(this->m_data);
}
	
}

}
