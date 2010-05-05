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
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "blp.hpp"
#include "../libraryloader.hpp"

#ifdef JPEG
#include <jpeglib.h>
#endif

namespace wc3lib
{

namespace blp
{

Blp::MipMap::MipMap(dword width, dword height) : m_width(width), m_height(height)
{
}

/// @todo Implement.
void Blp::MipMap::scale(dword newWidth, dword newHeight) throw (class Exception)
{
	throw Exception(_("Not implemented yet!"));
}

const byte Blp::identifier0[4] = { 'B', 'L', 'P', '0' };
const byte Blp::identifier1[4] = { 'B', 'L', 'P', '1' };
const byte Blp::identifier2[4] = { 'B', 'L', 'P', '2' };
const std::size_t Blp::maxMipMaps = 16;
const std::size_t Blp::maxCompressedPaletteSize = 256;

Blp::Blp()
{
	this->clear();
}

Blp::~Blp()
{
	this->clear();
}

void Blp::clear()
{
	this->m_version = Blp::Blp0;
	this->m_compression = Blp::Paletted;
	this->m_flags = Blp::NoAlpha;
	this->m_width = 0;
	this->m_height = 0;
	this->m_pictureType = 5;
	this->m_pictureSubType = 0;
	
	BOOST_FOREACH(class Blp::MipMap *mipMap, this->m_mipMaps)
		delete mipMap;
}

std::streamsize Blp::read(std::istream &istream, enum Format format) throw (class Exception)
{
	switch (format)
	{
		case Blp::BlpFormat:
			return this->readBlp(istream);
#ifdef JPEG
		case Blp::JpegFormat:
			return this->readJpeg(istream);
#endif
#ifdef TGA
		case Blp::TgaFormat:
			return this->readTga(istream);
#endif
#ifdef PNG
		case Blp::PngFormat:
			return this->readPng(istream);
#endif	
			
		default:
			throw Exception(_("Unknown format."));
	}
	
	return 0;
}

std::streamsize Blp::write(std::ostream &ostream, enum Format format) throw (class Exception)
{
	switch (format)
	{
		case Blp::BlpFormat:
			return this->writeBlp(ostream);
#ifdef JPEG
		case Blp::JpegFormat:
			return this->writeJpeg(ostream);
#endif
#ifdef TGA
		case Blp::TgaFormat:
			return this->writeTga(ostream);
#endif
#ifdef PNG
		case Blp::PngFormat:
			return this->writePng(ostream);
#endif	
		default:
			throw Exception(_("Unknown format."));
	}
	
	return 0;
}

/**
* @author PitzerMike, Jean-Francois Roy, Tamino Dauth
*/
static std::size_t requiredMipMaps(std::size_t width, std::size_t height)
{
	std::size_t mips = 0;
	std::size_t value = std::min<int>(width, height);
	
	while (value > 0)
	{
		value /= 2;
		++mips;
	}

	return mips;
}

#ifdef JPEG
static std::string jpegError(const std::string &message)
{
	return boost::str(boost::format(message) % jpeg_std_error(0)->jpeg_message_table != 0 ? jpeg_std_error(0)->jpeg_message_table[jpeg_std_error(0)->last_jpeg_message] : _("No error"));
}
#endif

std::streamsize Blp::readBlp(std::istream &istream) throw (class Exception)
{
	this->clear();
	// header
	dword identifier;
	istream.read(reinterpret_cast<char*>(&identifier), sizeof(identifier));
	std::streamsize bytes = istream.gcount();
	
	if (memcmp(reinterpret_cast<char*>(&identifier), Blp::identifier0, sizeof(Blp::identifier0)) == 0)
		this->m_version = Blp::Blp0;
	else if (memcmp(reinterpret_cast<char*>(&identifier), Blp::identifier1, sizeof(Blp::identifier1)) == 0)
		this->m_version = Blp::Blp1;
	else if (memcmp(reinterpret_cast<char*>(&identifier), Blp::identifier2, sizeof(Blp::identifier2)) == 0)
		this->m_version = Blp::Blp2;
	else
		throw Exception(boost::str(boost::format(_("Error while reading BLP file. Missing BLP identifier, got \"%1%\".")) % reinterpret_cast<const char*>(&identifier)));
	
	istream.read(reinterpret_cast<char*>(&this->m_compression), sizeof(this->m_compression));
	bytes += istream.gcount();
	//dword mipMaps;
	//istream.read(reinterpret_cast<char*>(&mipMaps), sizeof(mipMaps));
	//bytes += istream.gcount();
	//std::cout << "Number of mip maps is " << mipMaps << std::endl;
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
	std::size_t mipMaps = requiredMipMaps(this->m_width, this->m_height);
	std::cout << "Required mip maps are " << mipMaps << std::endl;
	std::list<dword> mipMapOffsets;
	std::list<dword> mipMapSizes;
	
	for (std::size_t i = 0; i < Blp::maxMipMaps; ++i)
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
			std::cout << "Reading mip map's " << i << " offset " << offset << " and size " << size << std::endl;
			mipMapOffsets.push_back(offset);
			mipMapSizes.push_back(size);
			
			class MipMap *mipMap = new MipMap(this->mipMapWidth(i), this->mipMapHeight(i));
			
			if (this->m_flags != Blp::Alpha && size != mipMap->width() * mipMap->height())
				std::cout << "Size " << size << " is not equal to " << mipMap->width() * mipMap->height() << std::endl;
			else if (this->m_flags == Blp::Alpha && size != mipMap->width() * mipMap->height() * 2)
				std::cout << "Size " << size << " is not equal to " << mipMap->width() * mipMap->height() * 2 << std::endl;
			
			this->m_mipMaps.push_back(mipMap);
		}
		
	}
	
	if (this->m_compression == Blp::Jpeg)
	{
		std::cout << "Detected JPEG compression mode." << std::endl;
#ifdef JPEG
		dword jpegHeaderSize;
		istream.read(reinterpret_cast<char*>(&jpegHeaderSize), sizeof(jpegHeaderSize));
		bytes += istream.gcount();
		byte *jpegHeader = new byte[jpegHeaderSize];
		istream.read(reinterpret_cast<char*>(jpegHeader), jpegHeaderSize);
		bytes += istream.gcount();
		std::list<dword>::iterator offset = mipMapOffsets.begin();
		std::list<dword>::iterator size = mipMapSizes.begin();
		
		BOOST_FOREACH(class MipMap *mipMap, this->m_mipMaps)
		{
			std::streampos position = istream.tellg();
			istream.seekg(*offset);
			std::size_t nullBytes = istream.tellg() - position;
			
			if (nullBytes > 0)
				std::cout << boost::format(_("Ignoring %1% 0 bytes.")) % nullBytes << std::endl;
			
			// all mipmaps use the same header.
			std::size_t bufferSize = boost::numeric_cast<std::size_t>(*size) + jpegHeaderSize;
			unsigned char *buffer = new unsigned char[bufferSize];
			memcpy(reinterpret_cast<void*>(buffer), reinterpret_cast<const void*>(jpegHeader), jpegHeaderSize);
			istream.read((char*)buffer, boost::numeric_cast<std::streamsize>(bufferSize));

			try
			{
				/*
				LibraryLoader::loadLibrary("jpeg");
				(void *)(j_decompress_ptr, int, size_t) jpeg_create_decompress = LibraryLoader::librarySymbol("jpeg", "jpeg_CreateDecompress");
				*/
				std::cout << "Using jpeglib version " << JPEG_LIB_VERSION << std::endl;

				j_decompress_ptr cinfo;
				jpeg_create_decompress(cinfo);
				jpeg_mem_src(cinfo, buffer, bufferSize);
				jpeg_mem_dest(cinfo, &buffer, &bufferSize);

				if (jpeg_read_header(cinfo, true) != JPEG_HEADER_OK)
				{
					jpeg_abort_decompress(cinfo);

					throw Exception(jpegError(_("Did not find header. Error: %1%.")));
				}

				if (!cinfo->saw_JFIF_marker)
					std::cerr << _("Warning: Did not find JFIF marker. JFIF format is used by default!") << std::endl;


				if (!jpeg_start_decompress(cinfo))
				{
					jpeg_abort_decompress(cinfo);

					throw Exception(jpegError(_("Could not start decompress. Error: %1%.")));
				}

				/*
				jpeg_has_multiple_scans JPP((j_decompress_ptr cinfo));
				1043 EXTERN(boolean) jpeg_start_output JPP((j_decompress_ptr cinfo,
				1044                                        int scan_number))
				*/

				jpeg_destroy_decompress(cinfo);

				/// @todo Fill mip map color data (user buffer)
				BOOST_FOREACH(class MipMap *mipMap, this->m_mipMaps)
					;

				delete[] buffer;
				buffer = 0;
			}
			catch (class Exception &exception)
			{
				delete[] jpegHeader;
				jpegHeader = 0;
				delete[] buffer;
				buffer = 0;

				throw exception;
			}
			catch (...)
			{
				delete[] jpegHeader;
				jpegHeader = 0;
				delete[] buffer;
				buffer = 0;
				std::cerr << _("Error: Unknown exception while reading JPEG file.") << std::endl;

				throw;
			}
			
			++offset;
			++size;
		}

		delete[] jpegHeader;
		jpegHeader = 0;
#else
		throw Exception(_("Compiled without JPEG support."));
#endif
	}
	else if (this->m_compression == Blp::Paletted)
	{
		std::cout << "Detected paletted compression." << std::endl;
		
		if (this->m_flags == Blp::Alpha)
			std::cout << "With alphas!" << std::endl;
		else
			std::cout << "Without alphas!" << std::endl;
		
		std::vector<color> palette(Blp::maxCompressedPaletteSize); // uncompressed 1 and 2 only use 256 different colors.

		for (int i = 0; i < Blp::maxCompressedPaletteSize; ++i)
		{
                        color paletteColor;
                        istream.read(reinterpret_cast<char*>(&paletteColor), sizeof(paletteColor));
			bytes += istream.gcount();
			palette[i] = paletteColor;
		}
	
		std::list<dword>::iterator offset = mipMapOffsets.begin();
		std::list<dword>::iterator size = mipMapSizes.begin();
		
		BOOST_FOREACH(class MipMap *mipMap, this->m_mipMaps)
		{
			std::streampos position = istream.tellg();
			std::cout << "Position is " << position << std::endl;
			istream.seekg(*offset);
			std::streampos newPosition = istream.tellg();
			std::cout << "New position is " << newPosition << std::endl;
			std::size_t nullBytes = newPosition - position;
			
			if (nullBytes > 0)
				std::cout << boost::format(_("Ignoring %1% 0 bytes.")) % nullBytes << std::endl;
			
			dword toReadBytes = *size;

			for (dword j = 0; j < mipMap->width(); ++j)
			{
				for (dword k = 0; k < mipMap->height(); ++k)
				{
					byte index;
					istream.read(reinterpret_cast<char*>(&index), sizeof(index));
					bytes += istream.gcount();
					toReadBytes -= istream.gcount();
					//std::cout << "To read bytes " << toReadBytes << std::endl;
					byte alpha = 0;

					if (this->m_flags == Blp::Alpha)
					{
						istream.read(reinterpret_cast<char*>(&alpha), sizeof(alpha));
						bytes += istream.gcount();
						toReadBytes -= istream.gcount();
					}

					mipMap->setColor(j, k, palette[index], alpha);
					
					/*
					if (istream.eof())
					{
						std::cout << "End of File" << std::endl;
						
						throw Exception("");
					}
					*/
				}
			}
			
			if (toReadBytes != 0)
			{
				istream.seekg(toReadBytes, std::ios_base::cur);
				std::cout << "Skipping " << toReadBytes << " unnecessary bytes." << std::endl;
			}
			
			++offset;
			++size;

			std::cout << "Mip map colors map size " << mipMap->m_colors.size() << std::endl;
		}
	}
	else
		throw Exception(boost::str(boost::format( _("Unknown compression mode: %1%.")) % this->m_compression));

	std::cout << "Read " << bytes << " bytes." << std::endl;
	
	// check mip maps
	/*
	- A full mipmap chain must be present. The last mipmap must be 1x1 (no larger).
	If an image is 32x8 the mipmap chain must be 32x8, 16x4, 8x2, 4x1, 2x1, 1x1.
	Sizes not of powers of 2 seems to work fine too, the same rules for mipmaps
	still applies. Ex: 24x17, 12x8 (rounded down), 6x4, 3x2, 1x1 (rounded down).
	*/
	if (this->m_mipMaps.empty() || this->m_mipMaps.back()->width() != 1 || this->m_mipMaps.back()->height() != 1)
		throw Exception(_("Last mip map does not exist or has not a size of 1x1."));
	
	return bytes;
}

std::streamsize Blp::writeBlp(std::ostream &ostream) throw (class Exception)
{
	std::streamsize bytes = 0;
	
	switch (this->m_version)
	{
		case Blp::Blp0:
			ostream.write(reinterpret_cast<const char*>(Blp::identifier0), sizeof(Blp::identifier0));
			bytes += sizeof(identifier0);
			
			break;
			
		case Blp::Blp1:
			ostream.write(reinterpret_cast<const char*>(Blp::identifier1), sizeof(Blp::identifier1));
			bytes += sizeof(identifier1);
			
			break;
			
		case Blp::Blp2:
			ostream.write(reinterpret_cast<const char*>(Blp::identifier2), sizeof(Blp::identifier2));
			bytes += sizeof(identifier2);
			
			break;
	}
	
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
	dword startOffset = ostream.tellp();
	std::list<dword> mipMapOffsets;
	std::list<dword> mipMapSizes;
	
	/// @todo change for JPEG?
	BOOST_FOREACH (const class MipMap *mipMap, this->m_mipMaps)
	{
		mipMapOffsets.push_back(startOffset);
		dword size = mipMap->colors().size() * sizeof(byte);
		
		if (this->m_flags == Blp::Alpha)
			size *= 2;
			
		mipMapSizes.push_back(size);
		startOffset += size;
	}
	
	std::list<dword>::const_iterator offsetIterator = mipMapOffsets.begin();
	std::list<dword>::const_iterator sizeIterator = mipMapSizes.begin();
	
	for (std::size_t i = 0; i < Blp::maxMipMaps; ++i)
	{
		// header data
		if (offsetIterator != mipMapOffsets.end())
		{
			ostream.write(reinterpret_cast<const char*>(&(*offsetIterator)), sizeof(*offsetIterator));
			bytes += sizeof(*offsetIterator);
			std::cout << "Wrote " << sizeof(*offsetIterator) << " bytes offset header data." << std::endl;
			ostream.write(reinterpret_cast<const char*>(&(*sizeIterator)), sizeof(*sizeIterator));
			bytes += sizeof(*sizeIterator);
			++offsetIterator;
			++sizeIterator;
		}
		else
		{
			std::cout << "Writing 0 size and 0 offset for mip map " << i << std::endl;
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
		std::cout << "Detected JPEG compression mode. Not implemented yet." << std::endl;
#ifdef JPEG
#else
		throw Exception(_("Compiled without JPEG support."));
#endif
	}
	else if (this->m_compression == Blp::Paletted)
	{
		std::cout << "Detected paletted compression." << std::endl;
		
		if (this->m_flags == Blp::Alpha)
			std::cout << "With alphas!" << std::endl;
		else
			std::cout << "Without alphas!" << std::endl;
		
		// fill palette, palette has always size of Blp::maxCompressedPaletteSize (remaining colors have value 0).
		std::vector<const struct MipMap::Color*> palette(Blp::maxCompressedPaletteSize, 0);

		BOOST_FOREACH(const class MipMap *mipMap, this->m_mipMaps)
		{
			for (dword width = 0; width < mipMap->width(); ++width)
			{
				for (dword height = 0; height < mipMap->height(); ++height)
				{
					if (palette[mipMap->colorAt(width, height).paletteIndex] == 0)
						palette[mipMap->colorAt(width, height).paletteIndex] = &mipMap->colorAt(width, height);
					else if (!palette[mipMap->colorAt(width, height).paletteIndex]->compareRgb(mipMap->colorAt(width, height)))
						std::cerr << _("Warning: There are different mip map colors with same index.") << std::endl;
				}
			}
		}

		// write palette
		BOOST_FOREACH(const struct MipMap::Color *paletteColor, palette)
		{
			color value = paletteColor == 0 ? 0 : paletteColor->rgb; // check if empty
			ostream.write(reinterpret_cast<const char*>(&value), sizeof(value));
			bytes += sizeof(value);
		}

		std::cout << "Wrote " << bytes << " with palette." << std::endl;

		// write mip maps
		BOOST_FOREACH(const class MipMap *mipMap, this->m_mipMaps)
		{
			for (dword width = 0; width < mipMap->width(); ++width)
			{
				for (dword height = 0; height < mipMap->height(); ++height)
				{
					byte index = mipMap->colorAt(width, height).paletteIndex;
					ostream.write(reinterpret_cast<const char*>(&index), sizeof(index));
					bytes += sizeof(index);

					if (this->m_flags == Blp::Alpha)
					{
						byte alpha = mipMap->colorAt(width, height).alpha;
						ostream.write(reinterpret_cast<const char*>(&alpha), sizeof(alpha));
						bytes += sizeof(alpha);
					}
				}
			}
		}

		std::cout << "BYTES " << bytes << std::endl;
	}
	else
		throw Exception(boost::str(boost::format(_("Unknown compression mode: %1%.")) % this->m_compression));

	std::cout << "Wrote " << bytes << " bytes." << std::endl;
	
	return bytes;
}
#ifdef JPEG
std::streamsize Blp::readJpeg(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Blp::writeJpeg(std::ostream &ostream) throw (class Exception)
{
	return 0;
}
#endif
#ifdef TGA
std::streamsize Blp::readTga(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Blp::writeTga(std::ostream &ostream) throw (class Exception)
{
	return 0;
}
#endif
#ifdef PNG
std::streamsize Blp::readPng(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Blp::writePng(std::ostream &ostream) throw (class Exception)
{
	return 0;
}
#endif

bool Blp::generateMipMaps(class MipMap *initialMipMap, std::size_t number)
{
	if (initialMipMap->width() != this->m_width || initialMipMap->height() != this->m_height)
		return false;
	
	this->m_mipMaps.push_back(initialMipMap);
	number = std::min<std::size_t>(number, Blp::maxMipMaps);
	dword width = this->m_width;
	dword height = this->m_height;
	//std::list<byte> indexList = initialMipMap->m_indexList;
	//std::list<byte> alphaList = initialMipMap->m_alphaList;
	
	for (int i = 1; i < number; ++i)
	{
		width /= 2;
		height /= 2;
		class MipMap *mipMap = new MipMap(width, height);
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

}

}
