/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#ifndef WC3LIB_MPQ_SECTOR_HPP
#define WC3LIB_MPQ_SECTOR_HPP

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mpq
{

/**
 * Actually there is no read and write member functions since Sectors are created by \ref MpqFile instances when data is being read or written.
 */
class Sector
{
	public:
		enum Compression /// TODO : byte
		{
			Uncompressed = 0,
			ImaAdpcmMono = 0x40, // IMA ADPCM mono
			ImaAdpcmStereo = 0x80, // IMA ADPCM stereo
			Huffman = 0x01, // Huffman encoded
			Deflated = 0x02, // Deflated (see ZLib)
			Imploded = 0x08, // Imploded (see PKWare Data Compression Library)
			Bzip2Compressed = 0x10 // BZip2 compressed (see BZip2)
		};
		
		Sector(class MpqFile *mpqFile);
		
		/**
		 * Reads new sector data from input stream \p istream and writes it into the MPQ archive.
		 * Effectively changes sector's size!
		 * \return Returns size of read data.
		 */
		std::streamsize readData(istream &istream) throw (class Exception);
		/// This version is required by class \ref MpqFile for reading sector data.
		std::streamsize readData(const byte *buffer, const std::size_t bufferSize) throw (class Exception);
		/**
		 * Writes sector data into output stream \p ostream.
		 * \return Returns size of written data.
		 */
		std::streamsize writeData(ostream &ostream) const throw (class Exception);
		
	protected:
		friend class Mpq;
		friend class MpqFile;
		
		void setCompression(byte value);
		uint32 sectorKey() const;
		
		class MpqFile *m_mpqFile;
		int32 m_sectorIndex;
		int32 m_sectorOffset;
		int32 m_sectorSize; // not required, added by wc3lib, should be the compressed size!
		enum Compression m_compression;
};

}

}

#endif
