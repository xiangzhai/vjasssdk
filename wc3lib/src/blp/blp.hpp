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

#ifndef WC3LIB_BLP_BLP_HPP
#define WC3LIB_BLP_BLP_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace blp
{

class BlpJpeg;
class BlpUncompressed1;
class BlpUncompressed2;

union BlpCompression
{
	class BlpJpeg *m_jpeg;
	class BlpUncompressed1 *m_uncompressed1;
	class BlpUncompressed2 *m_uncompressed2;
};

/**
* @brief Provides access to the BLP 1 format.
* A full mipmap chain must be present. The last mipmap must be 1x1 (no larger).
* If an image is 32x8 the mipmap chain must be 32x8, 16x4, 8x2, 4x1, 2x1, 1x1.
* Sizes not of powers of 2 seems to work fine too, the same rules for mipmaps
* still applies. Ex: 24x17, 12x8 (rounded down), 6x4, 3x2, 1x1 (rounded down).
*/
class Blp
{
	public:
		struct Header
		{
			dword m_isBlp1;
			dword m_compression;		//0 - Uses JPEG compression
							//1 - Uses palettes (uncompressed)
			dword m_flags;			//#8 - Uses alpha channel (?)
			dword m_width;
			dword m_height;
			dword m_pictureType;		//3 - Uncompressed index list + alpha list
							//4 - Uncompressed index list + alpha list
							//5 - Uncompressed index list
			dword m_pictureSubType;		//1 - ???
			dword m_mipMapOffset[16];
			dword m_mipMapSize[16];
		};

		enum Format
		{
			BlpFormat
		};

		enum Compression
		{
			Jpeg = 0,
			Uncompressed = 1
		};

		Blp();
		~Blp();

		struct Header* header() const;
		dword width() const;
		dword height() const;

		void read(std::fstream &fstream, enum Format format, int *bytes = 0) throw (class Exception);
		void readBlp(std::fstream &fstream, int *bytes = 0) throw (class Exception);
		void write(std::fstream &fstream, enum Format format, int *bytes = 0) throw (class Exception);
		void writeBlp(std::fstream &fstream, int *bytes = 0) throw (class Exception);


	private:
		struct Header *m_header;
		union BlpCompression m_blpCompression;
};

inline struct Blp::Header* Blp::header() const
{
	return this->m_header;
}

inline dword Blp::width() const
{
	return this->m_header->m_width;
}

inline dword Blp::height() const
{
	return this->m_header->m_height;
}

}

}

#endif