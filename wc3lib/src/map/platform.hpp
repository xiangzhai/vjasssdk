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

#ifndef WC3LIB_MAP_PLATFORM_HPP
#define WC3LIB_MAP_PLATFORM_hPP

#include <stdint.h>

namespace wc3lib
{

namespace map
{

/**
* Integers
* Intergers are stored using 4 bytes in "Little Endian" order. It means that the first byte read is the lowest byte.
* They are just like the C++ "int" (signed) type. In some other documentation of this kind you may see them named "long".
* Size: 4 bytes
* Example: 1234 decimal = [00 00 04 D2]h will be stored in this order: [D2 04 00 00]h
*/
typedef int32_t int32;

/**
* Short Integers
* Short Integers are stored using 2 bytes in "Little Endian" order.
* They are close to the C++ signed short but their range is from -16384 to 16383. It means the 2 highest bit are free of use for a flag for example.
* Size: 2 bytes
*/
typedef int16_t short16;

/**
* Floats
* Floats are using the standard IEEE 32bit float format. They are stored using 4 bytes and the "Little Endian" order.
* They are just like the C++ "float" type.
* Size: 4 bytes
* Example: 7654.32 decimal, this number can't be stored using this format so the system will take the closest value that can be represented using binary digits. The closest one is: 7654.319824 decimal = [45 EF 32 8F]h and will be stored as [8F 32 EF 45]h
*/
typedef float float32;

/**
* Chars and Array of Chars
* They are just stored like standard chars (1 char = 1 byte) and array of chars (no null terminating char needed).
* Size (chars): 1 byte
* Size (array of chars): usually 4 bytes
*/
typedef char char8;
//typedef char[4] char8Array;

}

}

#endif 
