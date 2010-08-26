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

#ifndef WC3LIB_MAP_RECTS_HPP
#define WC3LIB_MAP_RECTS_HPP

#include <map>

#include "platform.hpp"
#include "../format.hpp"

namespace wc3lib
{

namespace map
{

class W3m;

/**
* "war3map.w3r" file usually contains rects of map.
*/
class Rects : public Format
{
	public:
		typedef std::pair<int32, class Rect*> RectType;

		static const int32 version;

		Rects(class W3m *w3m);
		std::streamsize read(std::istream &istream) throw (class Exception);
		std::streamsize write(std::ostream &ostream) const throw (class Exception);

	protected:
		class W3m *m_w3m;
		int32 m_version;
		std::map<int32, class Rect*> m_rects;
};

}

}

#endif
