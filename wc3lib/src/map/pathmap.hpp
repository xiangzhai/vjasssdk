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

#ifndef WC3LIB_MAP_PATHMAP_HPP
#define WC3LIB_MAP_PATHMAP_HPP

#include <istream>
#include <ostream>
#include <map>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace map
{
	
class W3m;

class Pathmap
{
	public:
		enum Type
		{
			Walk = 0x02,
			Fly = 0x04,
			Build = 0x08,
			Blight = 0x20,
			Water = 0x40,
			Unknown = 0x80
		};
		
		Pathmap(class W3m *w3m);
		~Pathmap();
		
		std::streamsize read(std::istream &istream) throw (class Exception);
		std::streamsize write(std::istream &istream) throw (class Exception);
		
		int32 fileVersion() const;
		int32 width() const;
		int32 height() const;
		enum Type type(const class Position &position) const throw (class Exception);
		
		static const char8 identifier[4];

	protected:
		class W3m *m_w3m;
		int32 m_fileVersion;
		int32 m_width;
		int32 m_height;
		std::map<class Position, enum Type> m_data;
};

inline int32 Pathmap::fileVersion() const
{
	return this->m_fileVersion;
}

inline int32 Pathmap::width() const
{
	return this->m_width;
}

inline int32 Pathmap::height() const
{
	return this->m_height;
}

inline enum Pathmap::Type Pathmap::type(const class Position &position) const throw (class Exception)
{
	std::map<class Position, enum Type>::const_iterator iterator = this->m_data.find(position);

	if (iterator == this->m_data.end())
		throw Exception();
	
	return iterator->second;
}

}

}

#endif
