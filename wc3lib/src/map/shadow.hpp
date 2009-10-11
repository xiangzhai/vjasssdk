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

#ifndef WC3LIB_MAP_SHADOW_HPP
#define WC3LIB_MAP_SHADOW_HPP

#include <fstream>

#include "../exception.hpp"

namespace wc3lib
{

namespace map
{

class W3m;

/**
* Each byte set the shadow status of 1/16 of a tileset.
* It means that each tileset is divided in 16 parts (4*4).
*/
class Shadow
{
	public:
		Shadow(class W3m *w3m);

		void read(std::fstream &fstream) throw (class Exception);
		void write(std::fstream &fstream) throw (class Exception);

		bool containsShadow(int x, int y, int point);

	private:
		class W3m *m_w3m;

		/**
		1byte can have 2 values:
		00h = no shadow
		FFh = shadow
		*/
		char *m_data;
};

inline bool Shadow::containsShadow(int x, int y, int point)
{
	return (this->m_data[x * y][point] == 0xFFh);
}

}

}

#endif