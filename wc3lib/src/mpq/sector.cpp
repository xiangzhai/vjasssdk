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

#include <iostream> // debug

#include <fstream>

#include <boost/format.hpp>
#include <boost/filesystem/fstream.hpp>

#include "../internationalisation.hpp"
#include "algorithm.hpp"
#include "sector.hpp"
#include "mpq.hpp"
#include "mpqfile.hpp"

namespace wc3lib
{
	
namespace mpq
{

Sector::Sector(class MpqFile *mpqFile) : m_mpqFile(mpqFile), m_sectorIndex(0), m_sectorOffset(0), m_sectorSize(0), m_compression(Sector::Uncompressed)
{
}

std::streamsize Sector::readData(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Sector::writeData(std::ostream &ostream) const throw (class Exception)
{
	boost::filesystem::ifstream ifstream(this->m_mpqFile->mpq()->path(), std::ios_base::in | std::ios_base::binary);

	if (!ifstream)
		throw Exception(boost::str(boost::format(_("Sector: Unable to open file \"%1%\".")) % this->m_mpqFile->mpq()->path().string()));

	std::streampos position = int32(this->m_mpqFile->mpq()->startPosition()) + this->m_sectorOffset;

	if (this->m_mpqFile->mpq()->format() == Mpq::Mpq1)
		position += this->m_mpqFile->hash()->block()->blockOffset();
	else
		position += this->m_mpqFile->hash()->block()->extendedBlockOffset();

	ifstream.seekg(position);
//	std::cout << "Sector position " << position << std::endl;
	byte *data = new byte[this->m_sectorSize];
	int32 dataSize = this->m_sectorSize;
	std::cout << "Sector position " << position << " and data size " << dataSize << std::endl;

	if (this->m_mpqFile->isCompressed())
		dataSize -= 1;

	std::streamsize bytes = 0;

	try
	{
		ifstream.read((char*)data, dataSize);
		bytes = ifstream.gcount();

		/*
		If the file is encrypted, each sector (after compression/implosion, if applicable) is encrypted with the file's key.
		Each sector is encrypted using the key + the 0-based index of the sector in the file.
		*/
		if (this->m_mpqFile->isEncrypted())
			DecryptData(Mpq::cryptTable(), (void*)data, uint32(dataSize), this->m_mpqFile->fileKey() + uint32(this->m_sectorIndex));

		// Imploded sectors are the raw compressed data following compression with the implode algorithm (these sectors can only be in imploded files).
		if (this->m_mpqFile->isImploded())
		{
			throw Exception(_("Sector: Imploded files are not supported yet."));
		}
		// Compressed sectors (only found in compressed - not imploded - files) are compressed with one or more compression algorithms.
		else if (this->m_mpqFile->isCompressed())
		{
			/// @todo IMPLEMENT.
			if (this->m_compression & Sector::ImaAdpcmMono) // IMA ADPCM mono
				;

			/// @todo IMPLEMENT.
			if (this->m_compression & Sector::ImaAdpcmStereo) // IMA ADPCM stereo
				;

			if (this->m_compression & Sector::Huffman) // Huffman encoded
			{
				int state = huffman_decode_memory((unsigned char*)data, unsigned(dataSize), (unsigned char**)(&data), (unsigned*)(&dataSize));

				if (state != 0)
					throw Exception(boost::str(boost::format(_("Sector: Huffman error %1%.")) % state));
			}

			if (this->m_compression & Sector::Deflated) // Deflated (see ZLib)
				bytes += inflateStream(ifstream, ostream);

			if (this->m_compression & Sector::Imploded)
			{
				throw Exception(_("Sector: Imploded compression is not supported."));
			}

			if (this->m_compression & Sector::Bzip2Compressed)// BZip2 compressed (see BZip2)
			{
				int state = BZ2_bzBuffToBuffDecompress((char*)(data), (unsigned int*)(&dataSize), (char*)(data), unsigned(&dataSize), 0, 1);

				if (state != BZ_OK)
					throw Exception(boost::str(boost::format(_("Sector: Bzip2 error %1%.")) % state));
			}
		}

		ostream.write((const char*)(data), dataSize);
	}
	catch (class Exception &exception)
	{
		delete[] data;

		throw exception;
	}

	delete[] data;

	return bytes;
}

void Sector::setCompression(byte value)
{
	this->m_compression = static_cast<enum Compression>(value);
}

}

}
