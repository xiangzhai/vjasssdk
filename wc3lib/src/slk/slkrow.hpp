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

#ifndef WC3LIB_SLK_SLKROW_HPP
#define WC3LIB_SLK_SLKROW_HPP

#include <map>

#include "slk.hpp"
#include "../format.hpp"

namespace wc3lib
{

namespace slk
{

class SlkRow : public Format<map::byte>
{
	public:
		SlkRow(Slk::IndexType row);

		std::streamsize read(std::basic_istream<map::byte> &istream) throw (class Exception);
		std::streamsize write(std::basic_ostream<map::byte> &ostream) const throw (class Exception);

	protected:
		Slk::IndexType m_row;
		std::map<class SlkColumn*, class SlkValue*> m_values;
};

}

}

#endif
