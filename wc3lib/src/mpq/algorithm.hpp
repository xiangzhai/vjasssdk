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

#ifndef WC3LIB_MPQ_ALGORITHM_HPP
#define WC3LIB_MPQ_ALGORITHM_HPP

/**
* @file Includes all possible compression algorithm used in MPQ archives.
*/

#include <cassert>
#include <cctype>
#include <istream>
#include <ostream>

//#include <huffman/huff.h>
#include <huffman/huffman.h> // C implementation, libhuffman
#include <zlib.h>
#include <pklib/pklib.h>
#include <bzlib.h>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{
	
namespace mpq
{

enum HashType
{
	TableOffset = 0,
	NameA = 1,
	NameB = 2,
	FileKey = 3
};

/// The encryption and hashing functions use a number table in their procedures. This table must be initialized before the functions are called the first time.
void InitializeCryptTable(uint32 dwCryptTable[0x500]);
void EncryptData(const uint32 dwCryptTable[0x500], void *lpbyBuffer, uint32 dwLength, uint32 dwKey);
void DecryptData(const uint32 dwCryptTable[0x500], void *lpbyBuffer, uint32 dwLength, uint32 dwKey);

/// Based on code from StormLib.
uint32 HashString(const uint32 dwCryptTable[0x500], const char *lpszString, enum HashType hashType);

std::streamsize deflateStream(std::istream &istream, std::ostream &ostream) throw (class Exception);
/**
* @return Returns written bytes.
*/
std::streamsize inflateStream(std::istream &istream, std::ostream &ostream) throw (class Exception);

}

}

#endif
