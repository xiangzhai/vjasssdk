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

#include <fstream>

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

}

#endif
