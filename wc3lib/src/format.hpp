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

#ifndef WC3LIB_FORMAT_HPP
#define WC3LIB_FORMAT_HPP

#include <istream>
#include <ostream>

#include "exception.hpp"

namespace wc3lib
{

/// @brief Abstract class for formats.
template<typename _CharT>
class Format
{
	public:
		typedef _CharT CharType;

		virtual std::streamsize read(std::basic_istream<_CharT> &istream) throw (class Exception) = 0;
		virtual std::streamsize write(std::basic_ostream<_CharT> &ostream) const throw (class Exception) = 0;

		class Format& operator<<(std::basic_istream<_CharT> &istream) throw (class Exception);
		const class Format& operator>>(std::basic_ostream<_CharT> &ostream) const throw (class Exception);
};

template<typename _CharT>
inline class Format<_CharT>& Format<_CharT>::operator<<(std::basic_istream<_CharT> &istream) throw (class Exception)
{
	this->read(istream);

	return *this;
}

template<typename _CharT>
inline const class Format<_CharT>& Format<_CharT>::operator>>(std::basic_ostream<_CharT> &ostream) const throw (class Exception)
{
	this->write(ostream);

	return *this;
}

template<typename _CharT>
inline std::basic_istream<_CharT>& operator>>(std::basic_istream<_CharT> &istream, class Format<_CharT> &format) throw (class Exception)
{
	format.read(istream);

	return istream;
}

template<typename _CharT>
inline std::basic_ostream<_CharT>& operator<<(std::basic_ostream<_CharT> &ostream, const class Format<_CharT> &format) throw (class Exception)
{
	format.write(ostream);

	return ostream;
}

}

#endif
