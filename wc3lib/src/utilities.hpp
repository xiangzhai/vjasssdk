/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
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
inline std::istream& read(std::istream &istream, T &value, std::streamsize &sizeCounter, std::size_t size = sizeof(T))
{
	istream.read(reinterpret_cast<char*>(&value), size);

	if (!istream)
		throw Exception(_("Input stream error."));

	if (istream.gcount() != size)
		throw Exception(boost::format(_("Input stream read size %1% is not equal to expected size %2%.")) % istream.gcount() % size);

	sizeCounter += istream.gcount();

	return istream;
}

/**
* Reads C string char into value "value" (with 0-terminating if size is 0).
* @param size If this value is 0 it will stop when reaching 0-terminating char.
* @param terminatingChar Customizable terminating char.
*/
inline std::istream& readCString(std::istream &istream, char *value, std::streamsize &sizeCounter, std::size_t size = 0, char terminatingChar = '\0')
{
	if (value != 0)
		throw Exception(_("readCString: Value should be 0."));

	if (size == 0)
	{
		// get 0 terminating character, get name
		std::streampos position = istream.tellg();
		char character;

		if (!istream)
			throw Exception(_("Input stream error."));

		istream.get(character);

		std::size_t i = position;
		std::size_t length = 0;

		while (character != terminatingChar)
		{
			++i;
			++length;

			if (!istream)
				throw Exception(_("Input stream error."));

			istream.get(character);
		}

		istream.seekg(position);
		size = length; // assign new length
	}

	value = new char[size + 1];
	read(istream, value, sizeCounter, size);
	value[size] = '\0';

	return istream;
}

inline std::istream& readString(std::istream &istream, std::string &value, std::streamsize &sizeCounter, std::size_t size = 0)
{
	char *cString = 0;
	readCString(istream, cString, sizeCounter, size);
	value = cString;
	delete[] cString;

	return istream;
}

inline std::istream& readLine(std::istream &istream, std::string &value, std::streamsize &sizeCounter, std::size_t size = 0)
{
	char *cString = 0;
	readCString(istream, cString, sizeCounter, size, '\n');
	value = cString;
	delete[] cString;

	return istream;
}

template<typename T>
inline std::ostream& write(std::ostream &ostream, const T &value, std::streamsize &sizeCounter, std::size_t size = sizeof(T))
{
	ostream.write(reinterpret_cast<const char*>(&value), size);

	if (!ostream)
		throw Exception(_("Output stream error."));

	sizeCounter += size; /// @todo Why isn't there any .pcount, throw exception if it is not written completely

	return ostream;
}

/**
* Writes C string of value "value" into output (with 0 terminating char if size is 0).
* @param size If size is 0 it will stop writing when reached 0-terminating char.
*/
inline std::ostream& writeCString(std::ostream &ostream, const char *value, std::streamsize &sizeCounter, std::size_t size = 0)
{
	return write(ostream, value, sizeCounter, size == 0 ? strlen(value) + 1 : size);
}

inline std::ostream& writeString(std::ostream &ostream, const std::string &value, std::streamsize &sizeCounter, std::size_t size = 0)
{
	return writeCString(ostream, value.data(), sizeCounter, size);
}

inline std::ostream& writeLine(std::ostream &ostream, const std::string &value, std::streamsize &sizeCounter, std::size_t size = 0)
{
	std::string newValue = value + '\n';

	return writeCString(ostream, newValue.data(), sizeCounter, size);
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
