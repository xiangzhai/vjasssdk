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

#ifndef WC3LIB_MPQ_ALGORITHM_HPP
#define WC3LIB_MPQ_ALGORITHM_HPP

/**
* \file mpq/algorithm.hpp Includes all possible compression algorithm used in MPQ archives.
*/

#include <cassert>
#include <cctype>
#include <istream>
#include <ostream>

//#include <huffman/huff.h>
#include <huffman/huffman.h> // C implementation, libhuffman
#include <zlib.h> // system library
#include <pklib/pklib.h>
#include <bzlib.h> // system library
#include <wave/wave.h>

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

void compressPklib(char *&outBuffer, int &outLength, char* const &inBuffer, int inLength, int &compressionType, int /* compressionLevel */) throw (class Exception);
void decompressPklib(char *&outBuffer, int &outLength, char* const &inBuffer, int inLength) throw (class Exception);
/**
* @fn compressWaveMono, decompressWaveMono, compressWaveStereo, decompressWaveStereo
* Wrapper of StormLib functions.
* @param inBuffer Buffer which is read from.
* @param inBufferLength Buffer length which should be set to amount of bytes which should be read from buffer.
* @param outBuffer Buffer which is read into. If @param outBufferLength is > 0 it this buffer will be deleted automatically. Note that this buffer does not have to be allocated by function's user.
* @param outBuffer Buffer length which should have the initial size of buffer @param outBuffer. If the buffer is empty this value should be 0.
* @return Returns witten bytes.
*/
int compressWaveMono(short* const &inBuffer, int inBufferLength, unsigned char *&outBuffer, int &outBufferLength, int compressionLevel) throw (class Exception);
int decompressWaveMono(unsigned char* const &inBuffer, int inBufferLength, unsigned char *&outBuffer, int &outBufferLength) throw (class Exception);
int compressWaveStereo(short* const &inBuffer, int inBufferLength, unsigned char *&outBuffer, int &outBufferLength, int compressionLevel) throw (class Exception);
int decompressWaveStereo(unsigned char* const &inBuffer, int inBufferLength, unsigned char *&outBuffer, int &outBufferLength) throw (class Exception);

std::streamsize deflateStream(std::istream &istream, std::ostream &ostream) throw (class Exception);
/**
* @return Returns written bytes.
*/
std::streamsize inflateStream(std::istream &istream, std::ostream &ostream) throw (class Exception);

}

}

#endif
