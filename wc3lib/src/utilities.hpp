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

#ifndef WC3LIB_UTILITIES_HPP
#define WC3LIB_UTILITIES_HPP

#include <cmath>
#include <fstream>
#include <sstream>

#include "internationalisation.hpp"

namespace wc3lib
{

/**
* Note that you can not use fstream >> operators to read values (e. g. longs or floats) from binary files.
* Thus this template function exists.
*/
template<typename T>
inline T readValue(std::fstream &fstream, bool byteSwap = false)
{
	char bytes[sizeof(T)];
	fstream.read(bytes, sizeof(T));
	T result = 0;
	
	// i has to be signed?!
	if (byteSwap)
	{
		for (int i = sizeof(T) - 1; i >= 0; --i)
			result |= (bytes[i] << (i << 3));
	}
	else
	{
		int j =  sizeof(T) - 1;
		
		for (int i = 0; i < sizeof(T); ++i, --j)
			result |= (bytes[i] << (j << 3));
	}
	
	return result;
}

template<typename T>
std::string sizeStringBinary(T size)
{
	std::string unit;
	T remainder;
	
	if (size >= pow(1024, 3))
	{
		remainder = size % T(pow(1024, 3));
		size /= pow(1024, 3);
		unit = _("Gi");
	}
	else if (size >= pow(1024, 2))
	{
		remainder = size % T(pow(1024, 2));
		size /= pow(1024, 2);
		unit = _("Mi");
	}
	else if (size >= 1024)
	{
		remainder = size % 1024;
		size /= 1024;
		unit = _("Ki");
	}
	else
	{
		remainder = 0;
		unit = _("B");
	}

	std::stringstream sstream;
	sstream << size;
	
	if (remainder != 0)
		sstream << _(".") << remainder;
	
	sstream << ' ' << unit;
	
	return sstream.str();
}

template<typename T>
std::string sizeStringDecimal(T size)
{
	std::string unit;
	T remainder;
	
	if (size >= pow(1000, 3))
	{
		remainder = size % T(pow(1000, 3));
		size /= pow(1000, 3);
		unit = _("G");
	}
	else if (size >= pow(1000, 2))
	{
		remainder = size % T(pow(1000, 2));
		size /= pow(1000, 2);
		unit = _("M");
	}
	else if (size >= 1000)
	{
		remainder = size % 1000;
		size /= 1000;
		unit = _("k");
	}
	else
	{
		remainder = 0;
		unit = _("B");
	}

	std::stringstream sstream;
	sstream << size;
	
	if (remainder != 0)
		sstream << _(".") << remainder;
	
	sstream << ' ' << unit;
	
	return sstream.str();
}

std::string boolString(bool value)
{
	if (value)
		return _("Yes");

	return _("No");
}

}

#endif
