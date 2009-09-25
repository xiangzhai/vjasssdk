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

#include "mpq.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace mpq
{

struct Header
{
	char magic[4];
	int32 headerSize;
	int32 archiveSize;
	int16 formatVersion;
	int8 sectorSizeShift;
	int32 hashTableOffset;
	int32 blockTableOffset;
	int32 hashTableEntries;
	int32 blockTableEntries;
	int64 extendedBlockTableOffset;
	int16 hashTableOffsetHigh;
	int16 blockTableOffsetHigh;
};

struct BlockTable
{
	int32 blockOffset;
	int32 blockSize;
	int32 fileSize;
	int32 flags;
};

struct HashTable
{
	int32 filePathHashA;
	int32 filePathHashB;
	int16 language;
	int8 platform;
	int32 fileBlockIndex;
};

struct FileData
{
	int32 *sectorOffsetTable;
};

struct ExtendedAttributes
{
	int32 version;
	int32 attributesPresent;
	int32 *CRC32s;
};

struct WeakDigitalSignature
{	
	int32 unknown0;
	int32 unknown1;
};

struct StringDigitalSignature
{
	char magic[4];
//	int2048 signature;
};

void Mpq::open(std::ifstream &fstream, enum Mode mode) throw (class Exception)
{
	struct Header header;
	fstream.read((char*)&header, sizeof(struct Header));

	/// @todo Check magic
	
}

}

}
