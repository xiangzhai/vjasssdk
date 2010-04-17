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

#include <iostream> // debug

#include <boost/foreach.hpp>

#include "mpqfile.hpp"
#include "block.hpp"
#include "hash.hpp"
#include "mpq.hpp"
#include "sector.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mpq
{
	
std::streamsize MpqFile::read(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = 0;

	// This table is not present if this information can be calculated.
	// If the file is not compressed/imploded, then the size and offset of all sectors is known, based on the archive's SectorSizeShift. If the file is stored as a single unit compressed/imploded, then the SectorOffsetTable is omitted, as the single file "sector" corresponds to BlockSize and FileSize, as mentioned previously.
	if  ((this->m_hash->block()->flags() & Block::IsSingleUnit) || (!(this->m_hash->block()->flags() & Block::IsCompressed) && !(this->m_hash->block()->flags() & Block::IsImploded)))
	{
		std::cout << "File is not imploded/compressed or a single unit " << std::endl;
		int32 sectors = 0;
		int16 sectorSize = this->m_mpq->sectorSize();

		// If the file is stored as a single unit (indicated in the file's Flags), there is effectively only a single sector, which contains the entire file data.
		if (this->m_hash->block()->flags() & Block::IsSingleUnit)
			sectors = 1;
		else
			sectors = this->m_hash->block()->blockSize() / sectorSize;

		int32 newOffset = 0;
		int32 lastSize = this->m_hash->block()->blockSize() % sectorSize;

		for (std::size_t i = 0; i < sectors; ++i)
		{
			class Sector *sector = new Sector(this);
			sector->m_sectorOffset = newOffset;

			if (this->m_hash->block()->flags() & Block::IsSingleUnit)
				sector->m_sectorSize = this->m_hash->block()->fileSize(); /// @todo blockSize or fileSize?
			else
				sector->m_sectorSize = sectorSize;

			this->m_sectors.push_back(sector);

			newOffset += sectorSize;
		}

		// the last sector may contain less than this, depending on the size of the entire file's data.
		if (!(this->m_hash->block()->flags() & Block::IsSingleUnit) && lastSize > 0)
		{
			class Sector *sector = new Sector(this);
			sector->m_sectorOffset = newOffset;
			sector->m_sectorSize = lastSize;
			this->m_sectors.push_back(sector);
		}

		std::cout << "Real sector count " << this->m_sectors.size() << std::endl;

	}
	// However, the SectorOffsetTable will be present if the file is compressed/imploded and the file is not stored as a single unit, even if there is only a single sector in the file (the size of the file is less than or equal to the archive's sector size).
	// sector offset table
	else
	{
		int32 sectors = 0;
		//int32 sectors = this->m_hash->block()->fileSize() / this->m_mpq->sectorSize(); /// @todo How to get this value?
		std::cout << "Sectors " << sectors << " and flags " << this->m_hash->block()->flags() << std::endl;

		for (std::size_t i = 0; i < sectors; ++i)
		{
			class Sector *sector = new Sector(this);
			istream.read(reinterpret_cast<char*>(&sector->m_sectorOffset), sizeof(sector->m_sectorOffset));
			bytes += istream.gcount();
			this->m_sectors.push_back(sector);
		}

		// The last entry contains the file size, making it possible to easily calculate the size of any given sector.
		int32 size;
		istream.read(reinterpret_cast<char*>(&size), sizeof(size));
		bytes += istream.gcount();

		// calculate sector size, not required but maybe useful at some point
		BOOST_REVERSE_FOREACH(class Sector *sector, this->m_sectors)
		{
			sector->m_sectorSize = size - sector->m_sectorOffset;
			size = sector->m_sectorOffset;
		}
	}

	int32 index = 0;

	BOOST_FOREACH(class Sector *sector, this->m_sectors)
	{
		sector->m_sectorIndex = index;

		// Compressed sectors (only found in compressed - not imploded - files) are compressed with one or more compression algorithms.
		if (this->isCompressed() & Block::IsCompressed)
		{
			byte compression;
			istream.read(reinterpret_cast<char*>(&compression), sizeof(compression));
			bytes += istream.gcount();
			sector->setCompression(compression);
			istream.seekg(sector->m_sectorSize - 1, std::ios_base::cur); // skip sector data
		}
		else
		{
			sector->setCompression(0);
			istream.seekg(sector->m_sectorSize, std::ios_base::cur); // skip sector data
		}

		++index;
	}

	return bytes;
}


std::streamsize MpqFile::write(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize MpqFile::readData(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = 0;

	BOOST_FOREACH(class Sector *sector, this->m_sectors)
		bytes += sector->readData(istream);

	return bytes;
}

std::streamsize MpqFile::appendData(std::istream &istream) throw (class Exception)
{
	throw Exception(_("MpqFile: appendData is not implemented yet!"));

	return 0;
}

std::streamsize MpqFile::writeData(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = 0;

	std::cout << "We have " << this->m_sectors.size() << " sectors." << std::endl;

	BOOST_FOREACH(const class Sector *sector, this->m_sectors)
		bytes += sector->writeData(ostream);

	return bytes;
}

MpqFile::MpqFile(class Mpq *mpq, class Hash *hash) : m_mpq(mpq), m_hash(hash), m_path("")
{
	if (hash != 0)
		hash->m_mpqFile = this;
}

MpqFile::~MpqFile()
{
	BOOST_FOREACH(class Sector *sector, this->m_sectors)
		delete sector;
}

void MpqFile::remove() throw (class Exception)
{
	this->m_hash->clear();

	/// @todo Clear file content?
	//BOOST_FOREACH(class Sector *sector, this->m_sectors)
		//bytes += sector->clear();
}

bool MpqFile::rename(const std::string &newName, bool overwriteExisting) throw (class Exception)
{
	return this->move(std::string(this->m_path.directory_string()).append(newName), overwriteExisting);
}

bool MpqFile::move(const boost::filesystem::path &newPath, bool overwriteExisting) throw (class Exception)
{

	if (this->m_path == newPath)
		return false;

	class MpqFile *mpqFile = this->m_mpq->findFile(newPath, this->locale(), this->platform());

	if (mpqFile != 0)
	{
		if (!overwriteExisting)
			return false;

		mpqFile->remove();
	}

	this->m_hash->changePath(newPath);

	return true;
}

class MpqFile& MpqFile::operator<<(const class MpqFile &mpqFile) throw (class Exception)
{
	std::stringstream sstream;
	mpqFile.writeData(sstream);
	this->appendData(sstream);

	return *this;
}

class MpqFile& MpqFile::operator>>(class Mpq &mpq) throw (class Exception)
{
	mpq.addFile(*this, true, false);

	return *this;
}

class MpqFile& MpqFile::operator>>(class MpqFile &mpqFile) throw (class Exception)
{
	std::stringstream sstream;
	this->writeData(sstream);
	mpqFile.appendData(sstream);

	return *this;
}

}

}
