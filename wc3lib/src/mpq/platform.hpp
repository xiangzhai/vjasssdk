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

#ifndef WC3LIB_MPQ_PLATFORM_HPP
#define WC3LIB_MPQ_PLATFORM_HPP

#include <istream>
#include <ostream>
#include <string>
#include <sstream>

#include <boost/cstdint.hpp>
#include <boost/filesystem/fstream.hpp>

#include "../core.hpp"

namespace wc3lib
{

namespace mpq
{

typedef int8_t byte; // not unsigned
typedef int8_t int8; // not unsigned
typedef int16_t int16;
typedef int32_t int32;
typedef uint32_t uint32; // used for various encryption and hash functions
typedef int64_t int64;
typedef uint64_t uint64; // used for large offsets

typedef int32 CRC32;

typedef std::basic_istream<byte> istream;
typedef std::basic_ostream<byte> ostream;
typedef boost::filesystem::basic_ifstream<byte> ifstream;
typedef boost::filesystem::basic_ofstream<byte> ofstream;
typedef std::basic_string<byte> string;
typedef std::basic_stringstream<byte> stringstream;
typedef std::basic_istringstream<byte> istringstream;
typedef std::basic_ostringstream<byte> ostringstream;
typedef Format<byte> Format;

/// Windows-like file time. Redefined for compatibility.
struct FILETIME
{
	uint32_t lowDateTime;
	uint32_t highDateTime;
};

typedef int16_t MD5; // 128 bit

struct Header
{
	byte magic[4];
	int32 headerSize;
	int32 archiveSize;
	int16 formatVersion;
	int16 sectorSizeShift; // int8
	int32 hashTableOffset;
	int32 blockTableOffset;
	int32 hashTableEntries;
	int32 blockTableEntries;
};

struct ExtendedHeader // MPQ format 2
{
	int64 extendedBlockTableOffset;
	int16 hashTableOffsetHigh;
	int16 blockTableOffsetHigh;
};

struct HashTableEntry
{
	int32 filePathHashA;
	int32 filePathHashB;
	int16 locale;
	int16 platform; // int8
	int32 fileBlockIndex;
};

struct BlockTableEntry
{
	int32 blockOffset;
	int32 blockSize;
	int32 fileSize;
	int32 flags;
};

struct ExtendedBlockTableEntry
{
	int16 extendedBlockOffset;
};

struct FileData
{
	int32 *sectorOffsetTable;
};

struct ExtendedAttributesHeader
{
	int32 version;
	int32 attributesPresent;
};

struct WeakDigitalSignature
{
	int32 unknown0;
	int32 unknown1;
	int8 signature[64]; // int512 Signature : The digital signature. Like all other numbers in the MoPaQ format, this is stored in little-endian order. The structure of this, when decrypted, follows the RSASSA-PKCS1-v1_5 specification; this format is rather icky to work with (I wrote a program to verify this signature using nothing but an MD5 function and huge integer functions; it wasn't pleasant), and best left to an encryption library such as Cryto++.
};

struct StrongDigitalSignature
{
	char magic[4]; // Indicates the presence of a digital signature. Must be "NGIS" ("SIGN" backwards).
	int8 signature[256]; // int2048 Signature : The digital signature, stored in little-endian format.
};

}

}

#endif
