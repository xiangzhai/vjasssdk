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

#ifndef WC3LIB_SLK_SLK_HPP
#define WC3LIB_SLK_SLK_HPP

#include <istream>
#include <ostream>
#include <list>

#include "../exception.hpp"

namespace wc3lib
{

namespace slk
{

class SlkEntry;

/**
* Default class for SLK files like CliffTypes.slk.
* @todo Should use a Bison file which defines the possible syntax for these files.
*/
class Slk
{
	public:
		Slk();
		~Slk();

		std::streamsize read(std::istream &istream) throw (class Exception);
		std::streamsize write(std::ostream &ostream) const throw (class Exception);

	protected:
		virtual class SlkEntry* slkEntry(std::size_t column) = 0;

		std::size_t m_lines;
		std::size_t m_columns;
		std::list<class SlkEntry*> m_entries;
};

}

}

#endif
