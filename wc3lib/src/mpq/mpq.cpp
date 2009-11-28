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

#include <cstring>

#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include "mpq.hpp"
#include "mpqfile.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mpq
{

// The encryption and hashing functions use a number table in their procedures. This table must be initialized before the functions are called the first time.
static void InitializeCryptTable(int32 dwCryptTable[0x500])
{
    int32 seed   = 0x00100001;
    int32 index1 = 0;
    int32 index2 = 0;
    int   i;

    for (index1 = 0; index1 < 0x100; index1++)
    {
        for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
        {
            int32 temp1, temp2;

            seed  = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;

            seed  = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);

            dwCryptTable[index2] = (temp1 | temp2);
        }
    }
}

static void EncryptData(const int32 dwCryptTable[0x500], void *lpbyBuffer, int32 dwLength, int32 dwKey)
{
    assert(lpbyBuffer);

    int32 *lpdwBuffer = (int32 *)lpbyBuffer;
    int32 seed = 0xEEEEEEEE;
    int32 ch;

    dwLength /= sizeof(int32);

    while(dwLength-- > 0)
    {
        seed += dwCryptTable[0x400 + (dwKey & 0xFF)];
        ch = *lpdwBuffer ^ (dwKey + seed);

        dwKey = ((~dwKey << 0x15) + 0x11111111) | (dwKey >> 0x0B);
        seed = *lpdwBuffer + seed + (seed << 5) + 3;

		*lpdwBuffer++ = ch;
    }
}

static void DecryptData(const int32 dwCryptTable[0x500], void *lpbyBuffer, int32 dwLength, int32 dwKey)
{
    assert(lpbyBuffer);

    int32 *lpdwBuffer = (int32 *)lpbyBuffer;
    int32 seed = 0xEEEEEEEEL;
    int32 ch;

    dwLength /= sizeof(int32);

    while(dwLength-- > 0)
    {
        seed += dwCryptTable[0x400 + (dwKey & 0xFF)];
        ch = *lpdwBuffer ^ (dwKey + seed);

        dwKey = ((~dwKey << 0x15) + 0x11111111L) | (dwKey >> 0x0B);
        seed = ch + seed + (seed << 5) + 3;

		*lpdwBuffer++ = ch;
    }
}

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

struct BlockTableEntry
{
	int32 blockOffset;
	int32 blockSize;
	int32 fileSize;
	int32 flags;
};

struct HashTableEntry
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

struct StrongDigitalSignature
{
	char magic[4];
	char signature[256];
//	int2048 signature; //int2048, little-endian format
};

const int32 Mpq::identifier1 = 0x1A51504D;
const int16 Mpq::format1Identifier = 0x0000;
const int16 Mpq::format2Identifier = 0x0001;

Mpq::Mpq()
{
}

Mpq::~Mpq()
{
	BOOST_FOREACH(class Block *block, this->m_blocks)
		delete block;

	BOOST_FOREACH(class MpqFile *file, this->m_files)
		delete file;
}

std::streamsize Mpq::read(std::istream &istream, enum Mode mode) throw (class Exception)
{
	struct Header header;
	istream.read(reinterpret_cast<char*>(&header), sizeof(header));
	std::streamsize bytes = istream.gcount();

	if (bytes < sizeof(header))
		throw Exception(_("Error while reading MPQ header."));

	if (memcmp(header.magic, &Mpq::identifier1, sizeof(Mpq::identifier1)))
		throw Exception(boost::str(boost::format(_("Missing MPQ identifier \"%1%\".")) % Mpq::identifier1));

	if (header.formatVersion == Mpq::format1Identifier)
		this->m_format = Mpq::Mpq1;
	else if (header.formatVersion == Mpq::format2Identifier)
		this->m_format = Mpq::Mpq2;
	else
		throw Exception(boost::str(boost::format(_("Unknown MPQ format \"%1%\".")) % header.formatVersion));
	
	/// @todo The block table is encrypted, using the hash of "(block table)" as the key.
	istream.seekg(header.blockTableOffset);

	for (int i = 0; i < header.blockTableEntries; ++i)
	{
		class Mpq::Block *block = new Block(this);
		bytes += block->read(istream);
		this->m_blocks.push_back(block);
	}


	/// @todo Read file !header! data and add @class MpqFile instances to member m_files
	
	return 0;
	
}

std::streamsize Mpq::write(std::ostream &ostream, enum Format format) const
{
	return 0;
}

const class MpqFile* Mpq::findFile(const std::string &path) const
{
	for (std::list<class MpqFile*>::const_iterator iterator = this->m_files.begin(); iterator != this->m_files.end(); ++iterator)
	{
		if ((*iterator)->path() == path)
			return *iterator;
	}
	
	return 0;
}

const class MpqFile* Mpq::findFileByName(const std::string &name) const
{
	for (std::list<class MpqFile*>::const_iterator iterator = this->m_files.begin(); iterator != this->m_files.end(); ++iterator)
	{
		if ((*iterator)->name() == name)
			return *iterator;
	}
	
	return 0;
}

Mpq::Block::Block(class Mpq *mpq) : m_mpq(mpq)
{
}

std::streamsize Mpq::Block::read(std::istream &istream) throw (class Exception)
{
	struct BlockTableEntry entry;
	istream.read(reinterpret_cast<char*>(&entry), sizeof(entry));
	std::streamsize bytes = istream.gcount();

	if (bytes != sizeof(entry))
		throw Exception(_("Error while reading block table entry."));

	this->m_blockOffset = entry.blockOffset;
	this->m_blockSize = entry.blockSize;
	this->m_fileSize = entry.fileSize;
	this->m_flags = static_cast<enum Mpq::Block::Flags>(entry.fileSize);

	return bytes;
}

}

}
