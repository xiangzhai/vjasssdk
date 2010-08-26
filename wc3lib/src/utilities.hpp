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
#include <cstring>
#include <sstream>
#include <iostream>

#include "exception.hpp"
#include "internationalisation.hpp"

namespace wc3lib
{

/**
* Note that you can not use istream >> operators to read values (e. g. longs or floats) from binary files.
* Thus this template function exists.
* @deprecated This function should not be used anymore since std::istream.read reads unformatted data and there is a couple of utility functions which provide additionally read funcitonality.
*/
template<typename T>
inline T readValue(std::istream &istream, bool byteSwap = false)
{
	char bytes[sizeof(T)];
	istream.read(bytes, sizeof(T));
	T result = 0;

	// i has to be signed?!
	if (byteSwap)
	{
		for (std::size_t i = sizeof(T) - 1; i >= 0; --i)
			result |= (bytes[i] << (i << 3));
	}
	else
	{
		std::size_t j =  sizeof(T) - 1;

		for (std::size_t i = 0; i < sizeof(T); ++i, --j)
			result |= (bytes[i] << (j << 3));
	}

	return result;
}

template<typename T>
inline std::istream& read(std::istream &istream, T &value, std::streamsize &sizeCounter)
{
	istream.read(reinterpret_cast<char*>(&value), sizeof(value));

	if (!istream)
		throw Exception(_("Input stream error."));

	sizeCounter += istream.gcount();

	return istream;
}

template<typename T>
inline std::istream& readArray(std::istream &istream, T value, std::size_t valueSize, std::streamsize &sizeCounter)
{
	istream.read(value, valueSize);

	if (!istream)
		throw Exception(_("Input stream error."));

	sizeCounter += istream.gcount();

	return istream;
}

inline std::istream& readCString(std::istream &istream, char *value, std::streamsize &sizeCounter)
{
	// get 0 terminating character, get name
	std::streampos position = istream.tellg();
	char character;
	istream.get(character);
	std::size_t i = position;

	while (character != '\0')
	{
		++i;
		istream.get(character);
	}

	std::size_t length = i - position;
	istream.seekg(position, std::ios_base::cur);
	value = new char[length];
	readArray(istream, value, length, sizeCounter);

	return istream;
}

inline std::istream& readString(std::istream &istream, std::string &value, std::streamsize &sizeCounter)
{
	char *cString = 0;
	readCString(istream, cString, sizeCounter);
	value = cString;

	return istream;
}

template<typename T>
inline std::ostream& write(std::ostream &ostream, const T &value, std::streamsize &sizeCounter)
{
	ostream.write(reinterpret_cast<const char*>(&value), sizeof(value));

	if (!ostream)
		throw Exception(_("Output stream error."));

	sizeCounter += sizeof(value);

	return ostream;
}

template<typename T>
inline std::ostream& writeArray(std::ostream &ostream, const T value, std::size_t valueSize, std::streamsize &sizeCounter)
{
	ostream.write(value, valueSize);

	if (!ostream)
		throw Exception(_("Output stream error."));

	sizeCounter += valueSize;

	return ostream;
}

inline std::ostream& writeCString(std::ostream &ostream, const char *value, std::streamsize &sizeCounter)
{
	ostream.write(value, strlen(value) + 1); // write 0 terminating character
	sizeCounter += strlen(value) + 1;

	return ostream;
}

inline std::ostream& writeString(std::ostream &ostream, const std::string &value, std::streamsize &sizeCounter)
{
	writeCString(ostream, value.data(), sizeCounter);

	return ostream;
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

inline std::string boolString(bool value)
{
	if (value)
		return _("Yes");

	return _("No");
}

inline bool expectInput()
{
	std::string input;
	std::cin >> input;

	while (input != _("y") && input != _("n"))
	{
		std::cout << _("Wrong input. Expecting new input (y/n):") << std::endl;
		std::cin >> input;
	}

	if (input == _("y"))
		return true;

	return false;
}

}

#endif
