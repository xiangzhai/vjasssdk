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

#include "../exception.hpp"

namespace wc3lib
{

namespace map
{
	
class W3m;

class Pathmap
{
	public:
		enum Data
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
		enum Data data(std::size_t width, size_t height) const throw (class Exception);
		
		static const char8 identifier[4];

	protected:
		class W3m *m_w3m;
		int32 m_fileVersion;
		int32 m_width;
		int32 m_height;
		enum Data *m_data;
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

inline enum Pathmap::Data Pathmap::data(std::size_t width, size_t height) const throw (class Exception)
{
	if (this->m_data == 0 || width >= this->m_width || height >= this->m_height)
		throw Exception();
	
	return this->m_data[width][height];
}

}

}

#endif
