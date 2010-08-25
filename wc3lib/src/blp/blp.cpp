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

#include <jpeglib.h>

#include "blp.hpp"
#include "../libraryloader.hpp"

namespace wc3lib
{

namespace blp
{

//#ifndef STATIC
// jpeg read functions
typedef struct jpeg_error_mgr* (*jpeg_std_errorType)(struct jpeg_error_mgr*);
typedef void (*jpeg_CreateDecompressType)(j_decompress_ptr, int, size_t);
typedef void (*jpeg_mem_srcType)(j_decompress_ptr, unsigned char*, unsigned long);
typedef int (*jpeg_read_headerType)(j_decompress_ptr, boolean);
typedef boolean (*jpeg_start_decompressType)(j_decompress_ptr);
typedef void (*jpeg_abort_decompressType)(j_decompress_ptr);
typedef void (*jpeg_destroy_decompressType)(j_decompress_ptr);
typedef JDIMENSION (*jpeg_read_scanlinesType)(j_decompress_ptr cinfo, JSAMPARRAY scanlines, JDIMENSION max_lines);
typedef boolean (*jpeg_finish_decompressType)(j_decompress_ptr cinfo);
//#endif

Blp::MipMap::MipMap(dword width, dword height) : m_width(width), m_height(height)
{
}

/// @todo Implement.
void Blp::MipMap::scale(dword newWidth, dword newHeight) throw (class Exception)
{
	throw Exception(_("MipMap::scale - Not implemented yet!"));
}

const byte Blp::identifier0[4] = { 'B', 'L', 'P', '0' };
const byte Blp::identifier1[4] = { 'B', 'L', 'P', '1' };
const byte Blp::identifier2[4] = { 'B', 'L', 'P', '2' };
const std::size_t Blp::maxMipMaps = 16;
const std::size_t Blp::compressedPaletteSize = 256;

Blp::Blp()
{
	this->clean();
}

Blp::~Blp()
{
	this->clean();
}

void Blp::clean()
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

namespace
{

/**
* @author PitzerMike, Jean-Francois Roy, Tamino Dauth
*/
std::size_t requiredMipMaps(std::size_t width, std::size_t height)
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

std::string jpegError(jpeg_std_errorType jpeg_std_error, const std::string &message)
{
	return boost::str(boost::format(message) % (jpeg_std_error(0)->jpeg_message_table != 0 ? jpeg_std_error(0)->jpeg_message_table[jpeg_std_error(0)->last_jpeg_message] : _("No error")));
}

}

std::streamsize Blp::readBlp(std::istream &istream) throw (class Exception)
{
	this->clean();
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
		class LibraryLoader::Handle *libraryHandle = 0;
		jpeg_std_errorType jpeg_std_error = 0;
		jpeg_CreateDecompressType jpeg_CreateDecompress = 0;
		jpeg_mem_srcType jpeg_mem_src = 0;
		jpeg_read_headerType jpeg_read_header = 0;
		jpeg_start_decompressType jpeg_start_decompress = 0;
		jpeg_abort_decompressType jpeg_abort_decompress = 0;
		jpeg_destroy_decompressType jpeg_destroy_decompress = 0;
		jpeg_read_scanlinesType jpeg_read_scanlines = 0;
		jpeg_finish_decompressType jpeg_finish_decompress = 0;

		try
		{
			libraryHandle = LibraryLoader::loadLibrary("jpeg");
			std::cout << "This is the library handle " << libraryHandle << std::endl;
			void *symbol = LibraryLoader::librarySymbol(*libraryHandle, "jpeg_std_error");
			std::cout << "This is the library symbol " << symbol << std::endl;
			//jpeg_std_error = (jpeg_std_error)(symbol);
			jpeg_std_error = *((jpeg_std_errorType*)(&symbol));
			std::cout << "And this is the function pointer " << jpeg_std_error << " and this is the pointer of the pointer " << (jpeg_std_errorType*)(&symbol) << " and this is its target " << *(jpeg_std_errorType*)(&symbol) << std::endl;
			symbol = LibraryLoader::librarySymbol(*libraryHandle, "jpeg_CreateDecompress");
			jpeg_CreateDecompress = *((jpeg_CreateDecompressType*)(&symbol));
			symbol = LibraryLoader::librarySymbol(*libraryHandle, "jpeg_mem_src");
			jpeg_mem_src = *((jpeg_mem_srcType*)(&symbol));
			symbol = LibraryLoader::librarySymbol(*libraryHandle, "jpeg_read_header");
			jpeg_read_header = *((jpeg_read_headerType*)(&symbol));
			symbol = LibraryLoader::librarySymbol(*libraryHandle, "jpeg_start_decompress");
			jpeg_start_decompress = *((jpeg_start_decompressType*)(&symbol));
			symbol = LibraryLoader::librarySymbol(*libraryHandle, "jpeg_abort_decompress");
			jpeg_abort_decompress = *((jpeg_abort_decompressType*)(&symbol));
			symbol = LibraryLoader::librarySymbol(*libraryHandle, "jpeg_destroy_decompress");
			jpeg_destroy_decompress = *((jpeg_destroy_decompressType*)(&symbol));
			symbol = LibraryLoader::librarySymbol(*libraryHandle, "jpeg_read_scanlines");
			jpeg_read_scanlines = *((jpeg_read_scanlinesType*)(&symbol));
			symbol = LibraryLoader::librarySymbol(*libraryHandle, "jpeg_finish_decompress");
			jpeg_finish_decompress = *((jpeg_finish_decompressType*)(&symbol));

			std::cout << "Some function pointers: " << *jpeg_CreateDecompress << *jpeg_mem_src << *jpeg_read_header << *jpeg_start_decompress << std::endl;
		}
		catch (class Exception &exception)
		{
			exception.what().append("\nRequired for BLP reading.");

			throw exception;
		}

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
			// all mipmaps use the same header, jpeg header has been allocated before and is copied into each mip map buffer.
			std::size_t bufferSize = jpegHeaderSize + boost::numeric_cast<std::size_t>(*size);
			unsigned char *buffer = new unsigned char[bufferSize];
			memcpy(reinterpret_cast<void*>(buffer), reinterpret_cast<const void*>(jpegHeader), jpegHeaderSize); // copy header data

			// moving to offset, skipping null bytes
			std::streampos position = istream.tellg();
			istream.seekg(*offset);
			std::size_t nullBytes = istream.tellg() - position;

			if (nullBytes > 0)
				std::cout << boost::format(_("Ignoring %1% 0 bytes.")) % nullBytes << std::endl;

			// read mip map data starting at header offset, header has already been copied into buffer
			istream.read((char*)(&buffer[jpegHeaderSize]), boost::numeric_cast<std::streamsize>(*size));

			try
			{
				std::cout << boost::format(_("Using header of library \"jpeglib\" version %1%.")) % JPEG_LIB_VERSION << std::endl;

				JSAMPARRAY scanlines = 0; // will be filled later

				struct jpeg_decompress_struct cinfo;
				struct jpeg_error_mgr jerr;
				cinfo.err = jpeg_std_error(&jerr);
				jpeg_create_decompress(&cinfo);

				try
				{
					jpeg_mem_src(&cinfo, buffer, bufferSize);
					std::cout << "Buffer size is " << bufferSize << " and header size is " << jpegHeaderSize << std::endl;
					//jpeg_mem_dest(cinfo, &buffer, &bufferSize);

					if (jpeg_read_header(&cinfo, true) != JPEG_HEADER_OK)
						throw Exception(jpegError(jpeg_std_error, _("Did not find header. Error: %1%.")));

					std::cout << "After reading header ... " << std::endl;

					if (!jpeg_start_decompress(&cinfo))
						throw Exception(jpegError(jpeg_std_error, _("Could not start decompress. Error: %1%.")));

					if (mipMap->width() != cinfo.image_width)
						std::cerr << boost::format(_("Warnung: Image width (%1%) is not equal to mip map width (%2%).")) % cinfo.image_width % mipMap->width() << std::endl;

					if (mipMap->height() != cinfo.image_height)
						std::cerr << boost::format(_("Warnung: Image height (%1%) is not equal to mip map height (%2%).")) % cinfo.image_height % mipMap->height() << std::endl;

					std::cout << "JPEG image has width " << cinfo.image_width << " and height " << cinfo.image_height << std::endl;
					std::cout << "JPEG image has scaled width " << cinfo.output_width << " and scaled height " << cinfo.output_height << std::endl;
					std::cout << "Color map has size " << cinfo.actual_number_of_colors << std::endl;

					// JSAMPLEs per row in output buffer
					JDIMENSION scanlinesSize = cinfo.output_width * cinfo.output_components;
					scanlines = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, scanlinesSize, 1); /// @todo Memory should be allocated before but we don't have size?!
					//scanlines = new JSAMPROW[scanlinesSize];
					std::size_t i = 0;
					std::cout << "Number of scanlines " << scanlinesSize << std::endl;
					std::cout << "we do have " << cinfo.output_components << " components/color channels." << std::endl;

					// per scanline
					while (cinfo.output_scanline < cinfo.output_height)
					{
						//std::cout << "Output scanline is " << cinfo.output_scanline << " and buffer size is " << scanlinesSize << " and height is " <<  cinfo.output_height << std::endl;

						//scanlines[cinfo.output_scanline] = new JSAMPLE[cinfo.output_height];
						JDIMENSION dimension = jpeg_read_scanlines(&cinfo, scanlines, 1); // scanlinesSize

						if (dimension == 0)
							std::cerr << _("Warning: Number of scanned lines is 0.") << std::endl;

						int width = 0;
						int height = cinfo.output_scanline - 1; // cinfo.output_scanline is increased by one after calling jpeg_read_scanlines

						/// @todo Check if we got the right values!
						for (int component = 0; component < scanlinesSize; component += cinfo.output_components)
						{
							color rgb = (scanlines[0][component] << 2) + (scanlines[0][component + 1] << 1) + (scanlines[0][component + 2]);
							byte alpha = 0;

							if (cinfo.output_components == 4) // we do have an alpha channel
								alpha = scanlines[0][component + 3];

							mipMap->setColor(width, height, rgb, alpha);
							width++;
						}


						//put_scanline_someplace(scanlines[0], scanlinesSize);
						std::cout << "I is " << i << " and read lines " << dimension << std::endl;
						++i;
					}
				}
				catch (...)
				{
					// jpeg_abort_decompress is only used when cinfo has to be used again.
					jpeg_destroy_decompress(&cinfo); // discard object

					throw;
				}

				jpeg_finish_decompress(&cinfo);

				if (!cinfo.saw_JFIF_marker)
					std::cerr << boost::format(_("Warning: Did not find JFIF marker. JFIF format is used by default!\nThis is the JFIF version of the image %1%.%2%")) % cinfo.JFIF_major_version % cinfo.JFIF_minor_version << std::endl;

				jpeg_destroy_decompress(&cinfo);
				delete[] buffer;
				buffer = 0;
			}
			catch (class Exception &exception)
			{
				LibraryLoader::unloadLibrary(libraryHandle);
				libraryHandle = 0;
				delete[] jpegHeader;
				jpegHeader = 0;
				delete[] buffer;
				buffer = 0;

				throw exception;
			}
			catch (...)
			{
				LibraryLoader::unloadLibrary(libraryHandle);
				libraryHandle = 0;
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

		LibraryLoader::unloadLibrary(libraryHandle);
		libraryHandle = 0;
		delete[] jpegHeader;
		jpegHeader = 0;
	}
	else if (this->m_compression == Blp::Paletted)
	{
		std::cout << "Detected paletted compression." << std::endl;

		if (this->m_flags == Blp::Alpha)
			std::cout << "With alphas!" << std::endl;
		else
			std::cout << "Without alphas!" << std::endl;

		std::vector<color> palette(Blp::compressedPaletteSize); // uncompressed 1 and 2 only use 256 different colors.

		for (std::size_t i = 0; i < Blp::compressedPaletteSize; ++i)
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

std::streamsize Blp::writeBlp(std::ostream &ostream) const throw (class Exception)
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
	dword startOffset = ostream.tellp(); // offset where mip map header data starts, required by later mip map writing operations

	// First write mip maps and set header data afterwards but hold header data free until!
	// this is necessary because we don't know the exact size or offset of mip maps which use the JPEG format!
	// omit Blp::maxMipMaps * 2 = offsets and sizes
	std::size_t emptyBufferSize = Blp::maxMipMaps * 2;
	dword emptyBuffer[emptyBufferSize];
	memset(reinterpret_cast<void*>(&emptyBuffer), 0, emptyBufferSize);
	ostream.write(reinterpret_cast<const char*>(&emptyBuffer), emptyBufferSize);
	bytes += emptyBufferSize;
	std::cout << "Wrote " << bytes << " bytes header data." << std::endl;

	// image data

	if (this->m_compression == Blp::Jpeg)
	{
		/// @todo Use class LibraryLoader to load JPEG functions and write data.
		std::cout << "Detected JPEG compression mode. Not implemented yet." << std::endl;
		throw Exception(_("Compiled without JPEG support."));
	}
	else if (this->m_compression == Blp::Paletted)
	{
		std::cout << "Detected paletted compression." << std::endl;

		if (this->m_flags == Blp::Alpha)
			std::cout << "With alphas!" << std::endl;
		else
			std::cout << "Without alphas!" << std::endl;

		// fill palette, palette has always size of Blp::compressedPaletteSize (remaining colors have value 0).
		std::vector<const struct MipMap::Color*> palette(Blp::compressedPaletteSize, 0);

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
		std::vector<dword> offsets(this->m_mipMaps.size(), 0);
		std::vector<dword> sizes(this->m_mipMaps.size(), 0);
		std::size_t index = 0;

		// write mip maps
		BOOST_FOREACH(const class MipMap *mipMap, this->m_mipMaps)
		{
			dword offset = ostream.tellp();
			dword size = bytes;

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

			// set header data
			size = bytes - size;

			offsets[index] = offset;
			sizes[index] = size;

			++index;
		}

		// write header data, jump to header
		ostream.seekp(startOffset);

		for (index = 0; index < this->m_mipMaps.size(); ++index)
		{
			ostream.write(reinterpret_cast<const char*>(&offsets[index]), sizeof(dword));
			ostream.write(reinterpret_cast<const char*>(&sizes[index]), sizeof(dword));
			bytes += 2 * sizeof(dword);
		}

		std::cout << "BYTES " << bytes << std::endl;
	}
	else
		throw Exception(boost::str(boost::format(_("Unknown compression mode: %1%.")) % this->m_compression));

	std::cout << "Wrote " << bytes << " bytes." << std::endl;

	return bytes;
}

std::streamsize Blp::readJpeg(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Blp::writeJpeg(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize Blp::readTga(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Blp::writeTga(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize Blp::readPng(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Blp::writePng(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

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

	for (std::size_t i = 1; i < number; ++i)
	{
		width /= 2;
		height /= 2;
		class MipMap *mipMap = new MipMap(width, height);
		/// @todo Generate new scaled index and alpha list.


		this->m_mipMaps.push_back(mipMap);
	}

	return true;
}

dword Blp::mipMapWidth(std::size_t index) const
{
	dword width = this->m_width;

	for (std::size_t i = 0; i < index; ++i)
		width /= 2;

	return width;
}

dword Blp::mipMapHeight(std::size_t index) const
{
	dword height = this->m_height;

	for (std::size_t i = 0; i < index; ++i)
		height /= 2;

	return height;
}

}

}
