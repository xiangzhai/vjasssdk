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

#include <cstdlib>
#include <sstream>
#include <iostream> //debug
#include <cstdio>
#include <cstring>

#include "blp.hpp"
#include "blpjpeg.hpp"
#include "blpuncompressed1.hpp"
#include "blpuncompressed2.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace blp
{

Blp::Blp() : m_header(0)
{
}

Blp::~Blp()
{
	if (this->m_header != 0)
	{
		delete this->m_header;

		if (this->m_header->m_compression == Blp::Jpeg)
			delete this->m_blpCompression.m_jpeg;
		else if (this->m_header->m_compression == Blp::Uncompressed)
			delete this->m_blpCompression.m_uncompressed1;
	}
}

dword Blp::read(std::fstream &fstream, enum Format format) throw (class Exception)
{
	switch (format)
	{
		case Blp::BlpFormat:
			return this->readBlp(fstream);
	}
	
	return 0;
}

dword Blp::readBlp(std::fstream &fstream) throw (class Exception)
{
	dword bytes = 0;
	this->m_header = new struct Header;
	fstream.read(reinterpret_cast<char*>(this->m_header), sizeof(struct Header));
	bytes += fstream.gcount();
	
	static const char keyword[4] =
	{
		'B',
		'L',
		'P',
		'1'
	};
		
	if (memcmp(reinterpret_cast<const void*>(&this->m_header->m_isBlp1), keyword, sizeof(keyword)) != 0)
	{
		char message[50];
		sprintf(message, _("Error while reading BLP file. Expected \"BLP1\" keyword, got \"%s\".\n"), reinterpret_cast<const char*>(&this->m_header->m_isBlp1));
		
		throw Exception(message);
	}
	
	if (this->m_header->m_compression == Blp::Jpeg)
	{
		std::cout << "Detected JPEG compression mode." << std::endl;
		this->m_blpCompression.m_jpeg = new BlpJpeg(this);
		bytes += this->m_blpCompression.m_jpeg->read(fstream);
	}
	else if (this->m_header->m_compression == Blp::Uncompressed && (this->m_header->m_pictureType == 3 || this->m_header->m_pictureType == 4))
	{
		std::cout << "Detected uncompressed mode 1." << std::endl;
		this->m_blpCompression.m_uncompressed1 = new BlpUncompressed1(*this);

		for (int i = 0; i < 256; ++i)
		{
			fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_palette[i]), sizeof(color));
			bytes += fstream.gcount();
		}

		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < this->m_header->m_width; ++j)
			{
				for (int k = 0; k < this->m_header->m_height; ++k)
				{
					fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_indexList[j * k]), sizeof(byte));
					bytes += fstream.gcount();
				}
			}
		}

		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < this->m_header->m_width; ++j)
			{
				for (int k = 0; k < this->m_header->m_height; ++k)
				{
					fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_alphaList[j * k]), sizeof(byte));
					bytes += fstream.gcount();
				}
			}
		}
	}
	else if (this->m_header->m_compression == Blp::Uncompressed && this->m_header->m_pictureType == 5)
	{
		std::cout << "Detected uncompressed mode 2." << std::endl;
	}
	else
	{
		char message[50];
		sprintf(message, _("Unknown compression mode.\nCompression mode %d.\nPicture type: %d.\n"), this->m_header->m_compression, this->m_header->m_pictureType);
		throw Exception(message);
	}

	std::cout << "Read " << bytes << " bytes." << std::endl;
	
	// check mip maps
	/*
	- A full mipmap chain must be present. The last mipmap must be 1x1 (no larger).
	If an image is 32x8 the mipmap chain must be 32x8, 16x4, 8x2, 4x1, 2x1, 1x1.
	Sizes not of powers of 2 seems to work fine too, the same rules for mipmaps
	still applies. Ex: 24x17, 12x8 (rounded down), 6x4, 3x2, 1x1 (rounded down).
	*/
	
	for (int i = 0; i < 16; ++i)
	{
		
	}
	
	return bytes;
}

dword Blp::write(std::fstream &fstream, enum Format format) throw (class Exception)
{
	switch (format)
	{
		case Blp::BlpFormat:
			return this->writeBlp(fstream);
	}
	
	return 0;
}

dword Blp::writeBlp(std::fstream &fstream) throw (class Exception)
{
	dword bytes = 0;
	fstream.write(reinterpret_cast<const char*>(this->m_header), sizeof(struct Header));
	bytes += sizeof(struct Header);
	std::cout << "Wrote " << bytes << " bytes header data." << std::endl;

	if (this->m_header->m_compression == Blp::Jpeg)
	{
		bytes += this->m_blpCompression.m_jpeg->write(fstream);
		std::cout << "Detected JPEG compression mode." << std::endl;
	}
	else if (this->m_header->m_compression == Blp::Uncompressed && (this->m_header->m_pictureType == 3 || this->m_header->m_pictureType == 4))
	{
		std::cout << "Detected uncompressed mode 1." << std::endl;

		for (int i = 0; i < 256; ++i)
		{
			fstream.write(reinterpret_cast<const char*>(&this->m_blpCompression.m_uncompressed1->m_palette[i]), sizeof(color));
			bytes += sizeof(color);
		}

		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < this->m_header->m_width; ++j)
			{
				for (int k = 0; k < this->m_header->m_height; ++k)
				{
					fstream.write(reinterpret_cast<const char*>(&this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_indexList[j * k]), sizeof(byte));
					bytes += sizeof(byte);
				}
			}
		}

		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < this->m_header->m_width; ++j)
			{
				for (int k = 0; k < this->m_header->m_height; ++k)
				{
					fstream.write(reinterpret_cast<const char*>(&this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_alphaList[j * k]), sizeof(byte));
					bytes += sizeof(byte);
				}
			}
		}
	}
	else if (this->m_header->m_compression == Blp::Uncompressed && this->m_header->m_pictureType == 5)
		std::cout << "Detected uncompressed mode 2." << std::endl;
	else
	{
		char message[50];
		sprintf(message, _("Unknown compression mode.\nCompression mode %d.\nPicture type: %d.\n"), this->m_header->m_compression, this->m_header->m_pictureType);
		throw Exception(message);
	}

	std::cout << "Wrote " << bytes << " bytes." << std::endl;
	
	return bytes;
}

}

}
