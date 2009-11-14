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

const char Blp::identifier[4] = "BLP1";
const std::size_t Blp::maxMipMaps = 16;

Blp::Blp()
{
}

Blp::~Blp()
{
	this->clear();
}

void Blp::clear()
{
	for (std::list<struct MipMap*>::iterator iterator = this->m_mipMaps.begin(); iterator != this->m_mipMaps.end(); ++iterator)
		delete *iterator;
}

dword Blp::read(std::istream &istream, enum Format format) throw (class Exception)
{
	switch (format)
	{
		case Blp::BlpFormat:
			return this->readBlp(istream);
			
		default:
			throw Exception(_("Unknown format."));
	}
	
	return 0;
}

/**
* @author PitzerMike
*/
static int requiredMipMaps(int width, int height)
{
	int mips = 0;
	
	while (width > 0 && height > 0)
	{
		++mips;
		width = width / 2;
		height = height / 2;
	}

	return mips;
}

dword Blp::readBlp(std::istream &istream) throw (class Exception)
{
	this->clear();
	// header
	dword identifier[4];
	istream.read(reinterpret_cast<char*>(identifier), sizeof(identifier));
	dword bytes = istream.gcount();
	
	if (memcmp(reinterpret_cast<const void*>(identifier), Blp::identifier, sizeof(identifier)) != 0)
	{
		char message[50];
		sprintf(message, _("Error while reading BLP file. Expected \"%s\" keyword, got \"%s\".\n"), Blp::identifier, identifier);
		
		throw Exception(message);
	}
	
	istream.read(reinterpret_cast<char*>(&this->m_compression), sizeof(this->m_compression));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_flags), sizeof(this->m_flags));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_width), sizeof(this->m_width));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_height), sizeof(this->m_height));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_pictureType), sizeof(this->m_pictureType));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_pictureSubType), sizeof(this->m_pictureSubType));
	bytes += istream.gcount();
	int mipMaps = requiredMipMaps(this->m_width, this->m_height);
	std::list<dword> mipMapOffsets;
	std::list<dword> mipMapSizes;
	
	for (int i = 0; i < Blp::maxMipMaps; ++i)
	{
		// header data
		dword offset;
		istream.read(reinterpret_cast<char*>(&offset), sizeof(offset));
		bytes += istream.gcount();
		dword size;
		istream.read(reinterpret_cast<char*>(&size), sizeof(size));
		bytes += istream.gcount();
		
		if (i < mipMaps)
		{
			mipMapOffsets.push_back(offset);
			mipMapSizes.push_back(size);
			
			struct MipMap *mipMap = new MipMap;
			mipMap->m_width = this->mipMapWidth(i);
			mipMap->m_height = this->mipMapHeight(i);
			this->m_mipMaps.push_back(mipMap);
		}
		
	}
	
	if (this->m_compression == Blp::Jpeg)
	{
		std::cout << "Detected JPEG compression mode." << std::endl;
		//bytes += this->m_blp->readBlpJpeg(istream);
	}
	else if (this->m_compression == Blp::Uncompressed && (this->m_pictureType == 3 || this->m_pictureType == 4))
	{
		std::cout << "Detected uncompressed mode 1." << std::endl;
		
		for (int i = 0; i < 256; ++i)
		{
			istream.read(reinterpret_cast<char*>(&this->m_palette[i]), sizeof(this->m_palette[i]));
			bytes += istream.gcount();
		}
	
		std::list<dword>::iterator offset = mipMapOffsets.begin();
		std::list<dword>::iterator size = mipMapSizes.begin();
		
		for (std::list<struct MipMap*>::iterator iterator = this->m_mipMaps.begin(); iterator != this->m_mipMaps.end(); ++iterator)
		{
			istream.seekg(*offset);
			dword toReadBytes = *size;
			
			for (int j = 0; j < (*iterator)->m_width; ++j)
			{
				for (int k = 0; k < (*iterator)->m_height; ++k)
				{
					byte value;
					istream.read(reinterpret_cast<char*>(&value), sizeof(value));
					bytes += istream.gcount();
					toReadBytes -= istream.gcount();
					(*iterator)->m_indexList.push_back(value);
					istream.read(reinterpret_cast<char*>(&value), sizeof(value));
					bytes += istream.gcount();
					toReadBytes -= istream.gcount();
					(*iterator)->m_alphaList.push_back(value);
				}
			}
			
			if (toReadBytes != 0)
			{
				istream.seekg(toReadBytes, std::ios_base::cur);
				std::cout << "Skipping " << toReadBytes << " unnecessary bytes." << std::endl;
			}
			
			++offset;
			++size;
		}
	}
	else if (this->m_compression == Blp::Uncompressed && this->m_pictureType == 5)
	{
		std::cout << "Detected uncompressed mode 2." << std::endl;
		//bytes += this->m_blp->readBlpUncompressed2(istream);
	}
	else
	{
		char message[50];
		sprintf(message, _("Unknown compression mode.\nCompression mode %d.\nPicture type: %d.\n"), this->m_compression, this->m_pictureType);
		
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
	if (this->m_mipMaps.empty() || this->m_mipMaps.back()->m_width != 1 || this->m_mipMaps.back()->m_height != 1)
		throw Exception(_("Last mip map does not exist or has not a size of 1x1."));
	
	return bytes;
}

dword Blp::write(std::ostream &ostream, enum Format format) throw (class Exception)
{
	switch (format)
	{
		case Blp::BlpFormat:
			return this->writeBlp(ostream);
			
		default:
			throw Exception(_("Unknown format."));
	}
	
	return 0;
}


dword Blp::writeBlp(std::ostream &ostream) throw (class Exception)
{
	dword identifier[4];
	memcpy(identifier, Blp::identifier, sizeof(identifier));
	ostream.write(reinterpret_cast<const char*>(identifier), sizeof(identifier));
	dword bytes = sizeof(identifier);	
	ostream.write(reinterpret_cast<const char*>(&this->m_compression), sizeof(this->m_compression));
	bytes += sizeof(this->m_compression);
	ostream.write(reinterpret_cast<const char*>(&this->m_flags), sizeof(this->m_flags));
	bytes += sizeof(this->m_flags);
	ostream.write(reinterpret_cast<const char*>(&this->m_width), sizeof(this->m_width));
	bytes += sizeof(this->m_width);
	ostream.write(reinterpret_cast<const char*>(&this->m_height), sizeof(this->m_height));
	bytes += sizeof(this->m_height);
	ostream.write(reinterpret_cast<const char*>(&this->m_pictureType), sizeof(this->m_pictureType));
	bytes += sizeof(this->m_pictureType);
	ostream.write(reinterpret_cast<const char*>(&this->m_pictureSubType), sizeof(this->m_pictureSubType));
	bytes += sizeof(this->m_pictureSubType);	
	dword startOffset = bytes;
	std::list<dword> mipMapOffsets;
	std::list<dword> mipMapSizes;
	
	for (std::list<struct MipMap*>::iterator iterator = this->m_mipMaps.begin(); iterator != this->m_mipMaps.end(); ++iterator)
	{
		mipMapOffsets.push_back(startOffset);
		dword size = (*iterator)->m_indexList.size() * sizeof(byte);
		
		if (this->usesAlphaList())
			size += (*iterator)->m_alphaList.size() * sizeof(byte);
			
		mipMapSizes.push_back(size);
		startOffset += size;	
	}
	
	std::list<dword>::iterator offsetIterator = mipMapOffsets.begin();
	std::list<dword>::iterator sizeIterator = mipMapSizes.begin();
	
	for (int i = 0; i < Blp::maxMipMaps; ++i)
	{
		// header data
		if (offsetIterator != mipMapOffsets.end())
		{
			ostream.write(reinterpret_cast<const char*>(&(*offsetIterator)), sizeof(*offsetIterator));
			bytes += sizeof(*offsetIterator);
			ostream.write(reinterpret_cast<const char*>(&(*sizeIterator)), sizeof(*sizeIterator));
			bytes += sizeof(*sizeIterator);
			++offsetIterator;
			++sizeIterator;
		}
		else
		{
			dword value = 0;
			ostream.write(reinterpret_cast<const char*>(&value), sizeof(value));
			bytes += sizeof(value);
			ostream.write(reinterpret_cast<const char*>(&value), sizeof(value));
			bytes += sizeof(value);
		}
	}
	
	std::cout << "Wrote " << bytes << " bytes header data." << std::endl;
	
	// image data

	if (this->m_compression == Blp::Jpeg)
	{
		std::cout << "Detected JPEG compression mode." << std::endl;
		//bytes += this->m_blp->writeBlpJpeg(ostream);
	}
	else if (this->m_compression == Blp::Uncompressed && (this->m_pictureType == 3 || this->m_pictureType == 4))
	{
		std::cout << "Detected uncompressed mode 1." << std::endl;
		
		for (int i = 0; i < 256; ++i)
		{
			ostream.write(reinterpret_cast<const char*>(&this->m_palette[i]), sizeof(this->m_palette[i]));
			bytes += sizeof(this->m_palette[i]);
		}
	
		for (std::list<struct MipMap*>::iterator iterator = this->m_mipMaps.begin(); iterator != this->m_mipMaps.end(); ++iterator)
		{
			std::list<byte>::iterator index = (*iterator)->m_indexList.begin();
			std::list<byte>::iterator alpha = (*iterator)->m_alphaList.begin();
			
			while (index != (*iterator)->m_indexList.end())
			{
			
				ostream.write(reinterpret_cast<const char*>(&(*index)), sizeof(*index));
				bytes += sizeof(*index);
				ostream.write(reinterpret_cast<const char*>(&(*alpha)), sizeof(*alpha));
				bytes += sizeof(*alpha);
				
				++index;
				++alpha;
			}
		}
	}
	else if (this->m_compression == Blp::Uncompressed && this->m_pictureType == 5)
	{
		std::cout << "Detected uncompressed mode 2." << std::endl;
		//bytes += this->m_blp->writeBlpUncompressed2(ostream);
	}
	else
	{
		char message[50];
		sprintf(message, _("Unknown compression mode.\nCompression mode %d.\nPicture type: %d.\n"), this->m_compression, this->m_pictureType);
		
		throw Exception(message);
	}

	std::cout << "Wrote " << bytes << " bytes." << std::endl;
	
	return bytes;
}


#ifdef TGA
dword Blp::readTga(std::istream &istream) throw (class Exception)
{
	return 0;
}

dword Blp::writeTga(std::ostream &ostream) throw (class Exception)
{
	return 0;
}
#endif

bool Blp::generateMipMaps(struct MipMap *initialMipMap, std::size_t number)
{
	if (initialMipMap->m_width != this->m_width || initialMipMap->m_height != this->m_height)
		return false;
	
	this->m_mipMaps.push_back(initialMipMap);
	number = std::min<std::size_t>(number, Blp::maxMipMaps);
	dword width = this->m_width;
	dword height = this->m_height;
	std::list<byte> indexList = initialMipMap->m_indexList;
	std::list<byte> alphaList = initialMipMap->m_alphaList;
	
	for (int i = 1; i < number; ++i)
	{
		width /= 2;
		height /= 2;
		struct MipMap *mipMap = new MipMap;
		mipMap->m_width = width;
		mipMap->m_height = height;
		/// @todo Generate new scaled index and alpha list.
			
		
		this->m_mipMaps.push_back(mipMap);
	}
	
	return true;
}

dword Blp::mipMapWidth(int index) const
{
	dword width = this->m_width;
	
	for (int i = 0; i < index; ++i)
		width /= 2;
	
	return width;
}

dword Blp::mipMapHeight(int index) const
{
	dword height = this->m_height;
	
	for (int i = 0; i < index; ++i)
		height /= 2;
	
	return height;
}

bool Blp::usesAlphaList() const
{
	return (this->m_compression == Blp::Uncompressed && (this->m_pictureType == 3 || this->m_pictureType == 4));
}

}

}
