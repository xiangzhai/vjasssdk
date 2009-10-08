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

#include "blp.hpp"
#include "blpjpeg.hpp"
#include "blpuncompressed1.hpp"
#include "blpuncompressed2.hpp"

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

void Blp::read(std::fstream &fstream, enum Format format) throw (Exception)
{
	switch (format)
	{
		case Blp::BlpFormat:
			this->readBlp(fstream);
			break;
	}
}

void Blp::readBlp(std::fstream &fstream) throw (class Exception)
{
	fstream.seekg(0);
	long counter = 0;
	this->m_header = new struct Header;
	fstream.read(reinterpret_cast<char*>(this->m_header), sizeof(struct Header));
	counter += fstream.gcount();
	std::cout << "Read " << counter << " bytes core data." << std::endl;
	
	if (this->m_header->m_compression == Blp::Jpeg)
	{
		std::cout << "Detected JPEG compression mode." << std::endl;
		this->m_blpCompression.m_jpeg = new BlpJpeg;
		fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_jpeg->m_jpegHeaderSize), sizeof(this->m_blpCompression.m_jpeg->m_jpegHeaderSize));
		counter += fstream.gcount();
		std::cout << "JPEG header size is " << this->m_blpCompression.m_jpeg->m_jpegHeaderSize << std::endl;
		this->m_blpCompression.m_jpeg->m_jpegHeader = new byte[this->m_blpCompression.m_jpeg->m_jpegHeaderSize];

		for (int i = 0; i < this->m_blpCompression.m_jpeg->m_jpegHeaderSize; ++i)
		{
			std::cout << "Reading header data " << i << std::endl;
			fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_jpeg->m_jpegHeader[i]), sizeof(this->m_blpCompression.m_jpeg->m_jpegHeader[i]));
			counter += fstream.gcount();
		}

		int maxMipMaps = BlpJpeg::requiredMipMaps(this->width(), this->height());
		std::cout << "Max mip maps " << maxMipMaps << std::endl;

		for (int i = 0; i < 16; ++i)
		{
			std::cout << "Reading mipmap data " << i << std::endl;
			this->m_blpCompression.m_jpeg->m_mipMaps[i].m_jpegData = new byte[maxMipMaps];
			
			for (int j = 0; j < maxMipMaps; ++j)
			{
				fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_jpeg->m_mipMaps[i].m_jpegData[j]), sizeof(this->m_blpCompression.m_jpeg->m_mipMaps[i])); /// @todo Reading 156 bytes instead of one.
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
			fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_palette[i]), sizeof(this->m_blpCompression.m_uncompressed1->m_palette[i]));
			counter += fstream.gcount();
		}

		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < this->m_header->m_width; ++j)
			{
				for (int k = 0; k < this->m_header->m_height; ++k)
				{
					fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_indexList[j * k]), sizeof(this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_indexList[j * k]));
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
					fstream.read(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_alphaList[j * k]), sizeof(this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_alphaList[j * k]));
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
		std::cerr << "Unknown compression mode." << std::endl;
		throw Exception("");
	}

	std::cout << "Read " << counter << " bytes." << std::endl;
}

void Blp::write(std::fstream &fstream, enum Format format) throw (class Exception)
{
	switch (format)
	{
		case Blp::BlpFormat:
			this->writeBlp(fstream);
			break;
	}
}

void Blp::writeBlp(std::fstream &fstream) throw (class Exception)
{
	fstream.seekg(0);
	long counter = 0;
	fstream.write(reinterpret_cast<char*>(this->m_header), sizeof(struct Header));
	counter += sizeof(struct Header);
	std::cout << "Wrote " << counter << " bytes core data." << std::endl;

	if (this->m_header->m_compression == Blp::Jpeg)
	{
		std::cout << "Detected JPEG compression mode." << std::endl;
		fstream.write(reinterpret_cast<char*>(&this->m_blpCompression.m_jpeg->m_jpegHeaderSize), sizeof(this->m_blpCompression.m_jpeg->m_jpegHeaderSize));
		;
		counter += sizeof(this->m_blpCompression.m_jpeg->m_jpegHeaderSize);

		for (int i = 0; i < this->m_blpCompression.m_jpeg->m_jpegHeaderSize; ++i)
		{
			std::cout << "Writing header data " << i << std::endl;
			fstream.write(reinterpret_cast<char*>(&this->m_blpCompression.m_jpeg->m_jpegHeader[i]), sizeof(this->m_blpCompression.m_jpeg->m_jpegHeader[i]));
			counter += sizeof(this->m_blpCompression.m_jpeg->m_jpegHeader[i]);
		}

		int maxMipMaps = BlpJpeg::requiredMipMaps(this->width(), this->height());
		
		for (int i = 0; i < 16; ++i)
		{
			std::cout << "Writing mip map data " << i << std::endl;
			
			for (int j = 0; j < maxMipMaps; ++j)
			{
				fstream.write(reinterpret_cast<char*>(&this->m_blpCompression.m_jpeg->m_mipMaps[i].m_jpegData[j]), sizeof(this->m_blpCompression.m_jpeg->m_mipMaps[i].m_jpegData[j]));
				counter += sizeof(this->m_blpCompression.m_jpeg->m_mipMaps[i].m_jpegData[j]);
			}
		}
	}
	else if (this->m_header->m_compression == Blp::Uncompressed && (this->m_header->m_pictureType == 3 || this->m_header->m_pictureType == 4))
	{
		std::cout << "Detected uncompressed mode 1." << std::endl;

		for (int i = 0; i < 256; ++i)
			fstream.write(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_palette[i]), sizeof(this->m_blpCompression.m_uncompressed1->m_palette[i]));

		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < this->m_header->m_width; ++j)
			{
				for (int k = 0; k < this->m_header->m_height; ++k)
					fstream.write(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_indexList[j * k]), this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_indexList[j * k]);
			}
		}

		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < this->m_header->m_width; ++j)
			{
				for (int k = 0; k < this->m_header->m_height; ++k)
					fstream.write(reinterpret_cast<char*>(&this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_alphaList[j * k]), sizeof(this->m_blpCompression.m_uncompressed1->m_mipMaps[i].m_alphaList[j * k]));
			}
		}
	}
	else if (this->m_header->m_compression == Blp::Uncompressed && this->m_header->m_pictureType == 5)
		std::cout << "Detected uncompressed mode 2." << std::endl;
	else
	{
		std::cerr << "Unknown compression mode." << std::endl;
		throw Exception("");
	}

	std::cout << "Wrote " << counter << " bytes." << std::endl;
}

}

}
