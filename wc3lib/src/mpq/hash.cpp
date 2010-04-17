/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#include <iostream>

#include <boost/foreach.hpp>

#include "algorithm.hpp"
#include "hash.hpp"
#include "mpq.hpp"
#include "mpqfile.hpp"
#include "block.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{
	
namespace mpq
{

Hash::Hash(class Mpq *mpq) : m_mpq(mpq), m_mpqFile(0), m_filePathHashA(0), m_filePathHashB(0), m_locale(MpqFile::Neutral), m_platform(MpqFile::Default), m_block(0), m_deleted(false)
{
}

std::streamsize Hash::read(std::istream &istream) throw (class Exception)
{
	struct HashTableEntry entry;
	istream.read(reinterpret_cast<char*>(&entry), sizeof(entry));
	std::streamsize bytes = istream.gcount();
	
	if (bytes != sizeof(entry))
		throw Exception(_("Error while reading hash table entry."));
	
	this->m_filePathHashA = entry.filePathHashA;
	this->m_filePathHashB = entry.filePathHashB;
	this->m_locale = entry.locale;
	this->m_platform = entry.platform;
	
	if (entry.fileBlockIndex == Hash::blockIndexDeleted)
	{
		this->m_deleted = true;
		//std::cout << "Hash entry is deleted." << std::endl;
		//exit(0);
	}
	else if (entry.fileBlockIndex != Hash::blockIndexEmpty)
	{
		this->m_block = this->mpq()->m_blockMap[entry.fileBlockIndex];
		this->m_deleted = false;
		
		//std::cout << "BLOCK INDEX: " << entry.fileBlockIndex << " and block address " << this->m_block << std::endl;
		//exit(0);

		if (this->m_block == 0)
			throw Exception(_("Error while searching for corresponding block of hash table entry."));
	}
	// otherwise it's empty (block == 0)
	else
	{
		//std::cout << "Entry is EMPTY WAAAAAAAAAHHHHHHH" << std::hex << " with block index " << entry.fileBlockIndex << std::dec << std::endl;
		//exit(0);
	}
	
	return bytes;
}

/// @todo Clear or write file hash and block data!
void Hash::clear()
{
	//exit(0);
	// If the next entry is empty, mark this one as empty; otherwise, mark this as deleted.
	for (std::list<class Hash*>::const_iterator iterator = this->m_mpq->m_hashes.begin(); iterator != this->m_mpq->m_hashes.end(); ++iterator)
	{
		if (*iterator == this)
		{
			std::list<class Hash*>::const_iterator nextIterator(iterator);
			++nextIterator;
			
			if (nextIterator != this->m_mpq->m_hashes.end() && !(*nextIterator)->empty())
				this->m_deleted = true;
			
			break;
		}
	}
	
	// If the block occupies space, mark the block as free space; otherwise, clear the block table entry.
	if (this->m_block->m_blockSize > 0)
	{
		this->m_block->m_fileSize = 0;
		this->m_block->m_flags = Block::None;
	}
	else
	{
		/// @todo Change file size?
		this->m_mpq->m_blocks.remove(this->m_block);
		delete this->m_block;
	}
		
	this->m_block = 0;
}

/*
bool Hash::isHash(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform) const
{
	uint32 nameHashA = HashString(Mpq::cryptTable(), path.string().c_str(), NameA);
	uint32 nameHashB = HashString(Mpq::cryptTable(), path.string().c_str(), NameB);
	int16 realLocale = MpqFile::localeToInt(locale);
	int16 realPlatform = MpqFile::platformToInt(platform);

	return this->m_filePathHashA == nameHashA && this->m_filePathHashB == nameHashB && this->m_locale == realLocale && this->m_platform == realPlatform;
}

bool Hash::isHash(int32 nameHashA, int32 nameHashB, enum MpqFile::Locale locale, enum MpqFile::Platform platform) const
{
	int16 realLocale = MpqFile::localeToInt(locale);
	int16 realPlatform = MpqFile::platformToInt(platform);

	return this->isHash(nameHashA, nameHashB, locale, platform);
}
*/

bool Hash::isHash(int32 nameHashA, int32 nameHashB, int16 locale, int16 platform) const
{
	return this->m_filePathHashA == nameHashA && this->m_filePathHashB == nameHashB && this->m_locale == locale && this->m_platform == platform;
}

/// @todo Write data into hash table.
void Hash::changePath(const boost::filesystem::path &path)
{
	this->m_filePathHashA = HashString(Mpq::cryptTable(), path.string().c_str(), NameA);
	this->m_filePathHashB = HashString(Mpq::cryptTable(), path.string().c_str(), NameB);
}

}

}
