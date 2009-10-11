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
		{
			delete[] this->m_blpCompression.m_jpeg->m_jpegHeader;
			delete this->m_blpCompression.m_jpeg;
		}
		else if (this->m_header->m_compression == Blp::Uncompressed)
			delete this->m_blpCompression.m_uncompressed1;
	}
}

void Blp::read(std::fstream &fstream, enum Format format, int *bytes) throw (class Exception)
{
	switch (format)
	{
		case Blp::BlpFormat:
			this->readBlp(fstream, bytes);
			break;
	}
}

void Blp::readBlp(std::fstream &fstream, int *bytes) throw (class Exception)
{
	fstream.seekg(0);
	std::streamsize counter = 0;
	this->m_header = new struct Header;
	fstream.read(reinterpret_cast<char*>(this->m_header), sizeof(struct Header));
	counter += fstream.gcount();
	
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
	
	std::cout << "Read " << counter << " bytes core data." << std::endl;
	
	if (this->m_header->m_compression == Blp::Jpeg)
	{
		std::cout << "Detected JPEG compression mode." << std::endl;
		this->m_blpCompression.m_jpeg = new BlpJpeg;
		fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_jpeg->m_jpegHeaderSize), sizeof(dword));
		counter += fstream.gcount();
		std::cout << "JPEG header size is " << this->m_blpCompression.m_jpeg->m_jpegHeaderSize << std::endl;
		this->m_blpCompression.m_jpeg->m_jpegHeader = new byte[this->m_blpCompression.m_jpeg->m_jpegHeaderSize];

		for (int i = 0; i < this->m_blpCompression.m_jpeg->m_jpegHeaderSize; ++i)
		{
			std::cout << "Reading header data " << i << std::endl;
			fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_jpeg->m_jpegHeader[i]), sizeof(byte));
			counter += fstream.gcount();
		}

		int maxMipMaps = BlpJpeg::requiredMipMaps(this->width(), this->height());
		std::cout << "Max mip maps " << maxMipMaps << std::endl;

		for (int i = 0; i < 16; ++i)
		{
			std::cout << "Reading mipmap data " << i << std::endl;
			if (counter != this->m_header->m_mipMapOffset[i])
			{
				std::cout << "Reading unnecessary offset" << std::endl;
				char *tmp = new char[this->m_header->m_mipMapOffset[i]];
				fstream.read(tmp, sizeof(tmp));
				counter += fstream.gcount();
				delete tmp;
			}
			
			this->m_blpCompression.m_jpeg->m_mipMaps[i].m_jpegData = new byte[maxMipMaps];
			
			for (int j = 0; j < maxMipMaps; ++j)
			{
				fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_jpeg->m_mipMaps[i].m_jpegData[j]), sizeof(byte)); /// @todo Reading 156 bytes instead of one.
				counter += fstream.gcount();
				std::cout << "Reading mip map jpeg data " << fstream.gcount() << std::endl;
			}
		}
	}
	else if (this->m_header->m_compression == Blp::Uncompressed && (this->m_header->m_pictureType == 3 || this->m_header->m_pictureType == 4))
	{
		std::cout << "Detected uncompressed mode 1." << std::endl;
		this->m_blpCompression.m_uncompressed1 = new BlpUncompressed1(*this);

		for (int i = 0; i < 256; ++i)
		{
			fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_palette[i]), sizeof(color));
			counter += fstream.gcount();
		}

		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < this->m_header->m_width; ++j)
			{
				for (int k = 0; k < this->m_header->m_height; ++k)
				{
					fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_indexList[j * k]), sizeof(byte));
					counter += fstream.gcount();
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
					counter += fstream.gcount();
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

	std::cout << "Read " << counter << " bytes." << std::endl;
	
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
	
	if (bytes != 0)
		*bytes = counter;
}

void Blp::write(std::fstream &fstream, enum Format format, int *bytes) throw (class Exception)
{
	switch (format)
	{
		case Blp::BlpFormat:
			this->writeBlp(fstream, bytes);
			break;
	}
}

void Blp::writeBlp(std::fstream &fstream, int *bytes) throw (class Exception)
{
	fstream.seekg(0);
	std::streamsize counter = 0;
	fstream.write(reinterpret_cast<const char*>(this->m_header), sizeof(struct Header));
	counter += sizeof(struct Header);
	std::cout << "Wrote " << counter << " bytes core data." << std::endl;

	if (this->m_header->m_compression == Blp::Jpeg)
	{
		std::cout << "Detected JPEG compression mode." << std::endl;
		fstream.write(reinterpret_cast<const char*>(&this->m_blpCompression.m_jpeg->m_jpegHeaderSize), sizeof(dword));
		;
		counter += sizeof(dword);

		for (int i = 0; i < this->m_blpCompression.m_jpeg->m_jpegHeaderSize; ++i)
		{
			std::cout << "Writing header data " << i << std::endl;
			fstream.write(reinterpret_cast<const char*>(&this->m_blpCompression.m_jpeg->m_jpegHeader[i]), sizeof(byte));
			counter += sizeof(byte);
		}

		int maxMipMaps = BlpJpeg::requiredMipMaps(this->width(), this->height());
		
		for (int i = 0; i < 16; ++i)
		{
			std::cout << "Writing mip map data " << i << std::endl;
			
			for (int j = 0; j < maxMipMaps; ++j)
			{
				fstream.write(reinterpret_cast<const char*>(&this->m_blpCompression.m_jpeg->m_mipMaps[i].m_jpegData[j]), sizeof(byte));
				counter += sizeof(byte);
			}
		}
	}
	else if (this->m_header->m_compression == Blp::Uncompressed && (this->m_header->m_pictureType == 3 || this->m_header->m_pictureType == 4))
	{
		std::cout << "Detected uncompressed mode 1." << std::endl;

		for (int i = 0; i < 256; ++i)
		{
			fstream.write(reinterpret_cast<const char*>(&this->m_blpCompression.m_uncompressed1->m_palette[i]), sizeof(color));
			counter += sizeof(color);
		}

		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < this->m_header->m_width; ++j)
			{
				for (int k = 0; k < this->m_header->m_height; ++k)
				{
					fstream.write(reinterpret_cast<const char*>(&this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_indexList[j * k]), sizeof(byte));
					counter += sizeof(byte);
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
					counter += sizeof(byte);
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

	std::cout << "Wrote " << counter << " bytes." << std::endl;
	
	if (bytes != 0)
		*bytes = counter;
}

}

}
