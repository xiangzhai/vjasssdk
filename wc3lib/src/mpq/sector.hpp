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

#ifndef WC3LIB_MPQ_SECTOR_HPP
#define WC3LIB_MPQ_SECTOR_HPP

#include <istream>
#include <ostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{
	
namespace mpq
{
	
class Mpq;
class MpqFile;

class Sector
{
	public:
		Sector(class MpqFile *mpqFile);
		
		/**
		* Reads new sector data from input stream @param istream and writes it into the MPQ archive.
		*/
		std::streamsize readData(std::istream &istream) throw (class Exception);
		/**
		* Writes sector data into output stream	@param ostream.
		*/
		std::streamsize writeData(std::ostream &ostream) const throw (class Exception);
		
	protected:
		friend class Mpq;
		friend class MpqFile;
		
		enum Compression
		{
			Uncompressed = 0,
			ImaAdpcmMono = 0x40, // IMA ADPCM mono
			ImaAdpcmStereo = 0x80, // IMA ADPCM stereo
			Huffman = 0x01, // Huffman encoded
			Deflated = 0x02, // Deflated (see ZLib)
			Imploded = 0x08, // Imploded (see PKWare Data Compression Library)
			Bzip2Compressed = 0x10 // BZip2 compressed (see BZip2)
		};
		
		void setCompression(byte value);
		
		class MpqFile *m_mpqFile;
		int32 m_sectorIndex;
		int32 m_sectorOffset;
		int32 m_sectorSize; // not required, added by wc3lib, should be the compressed size!
		enum Compression m_compression;
};

}

}

#endif
