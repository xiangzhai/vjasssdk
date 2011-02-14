/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
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
#include "../utilities.hpp"

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

bool Blp::MipMap::Color::operator==(const class Color &other) const
{
	/*
	color argb0 = this->argb();
	byte alpha0 = 0;
	color argb1 = other.argb();
	byte alpha1 = 0;

	if (this->m_mipMap->m_blp->flags() & Alpha)
		alpha0 = this->alpha();

	if (other.m_mipMap->m_blp->flags() & Alpha)
		alpha1 = other.alpha();
	*/

	return this->argb() == other.argb() && this->alpha() == other.alpha();
}

bool Blp::MipMap::Color::operator!=(const class Color &other) const
{
	return !(*this == other);
}

Blp::MipMap::Color::Color() : m_mipMap(0), m_argb(0), m_alpha(0), m_paletteIndex(0)
{
}

Blp::MipMap::Color::~Color()
{
}

Blp::MipMap::Color::Color(class Blp::MipMap *mipMap, color argb, byte alpha, byte paletteIndex) : m_mipMap(mipMap), m_argb(argb), m_alpha(alpha), m_paletteIndex(paletteIndex)
{
}

/// @todo Implement.
void Blp::MipMap::scale(dword newWidth, dword newHeight) throw (class Exception)
{
	throw Exception(_("MipMap::scale - Not implemented yet!"));
}

Blp::MipMap::MipMap(class Blp *blp, dword width, dword height) : m_blp(blp), m_width(width), m_height(height)
{
}

Blp::MipMap::~MipMap()
{
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

namespace
{

struct MipMapHeaderData
{
	dword offset, size;
};

}

std::streamsize Blp::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	this->clean();
	// header
	dword identifier;
	std::streamsize size = 0;
	wc3lib::read(istream, identifier, size);

	if (memcmp(reinterpret_cast<const void*>(&identifier), Blp::identifier0, sizeof(Blp::identifier0)) == 0)
		this->m_version = Blp::Blp0;
	else if (memcmp(reinterpret_cast<const void*>(&identifier), Blp::identifier1, sizeof(Blp::identifier1)) == 0)
		this->m_version = Blp::Blp1;
	else if (memcmp(reinterpret_cast<const void*>(&identifier), Blp::identifier2, sizeof(Blp::identifier2)) == 0)
		this->m_version = Blp::Blp2;
	else
		throw Exception(boost::str(boost::format(_("Error while reading BLP file. Missing BLP identifier, got \"%1%\".")) % reinterpret_cast<const char*>(&identifier)));

	std::vector<struct MipMapHeaderData*> mipMapData(Blp::maxMipMaps, 0);

	if (this->m_version == Blp::Blp1 || this->m_version == Blp::Blp2)
	{
		struct BlpHeader header;
		wc3lib::read(istream, header, size);
		this->m_compression = static_cast<enum Compression>(header.compression);
		this->m_flags = static_cast<enum Flags>(header.flags);
		this->m_width = header.width;
		this->m_height = header.height;
		this->m_pictureType = header.pictureType;
		this->m_pictureSubType = header.pictureSubType;

		for (std::size_t i = 0; i < Blp::maxMipMaps; ++i)
		{
			struct MipMapHeaderData *headerData = new MipMapHeaderData;
			mipMapData[i] = headerData;
			headerData->offset = header.mipMapOffset[i],
			headerData->size = header.mipMapSize[i];
		}
	}
	// BLP2
	else if (this->m_version == Blp::Blp2)
	{
		struct Blp2Header header;
		wc3lib::read(istream, header, size);

		if (header.type == 0)
			this->m_compression = Blp::Jpeg;
		else if (header.type == 1)
		{
			if (header.encoding == 1)
				this->m_compression = Blp::Uncompressed;
			else if (header.encoding == 2)
				this->m_compression = Blp::DirectXCompression;
		}

		this->m_flags = static_cast<enum Flags>(header.alphaDepth);
		// header.alphaEncoding
		/*
		TODO
		0: DXT1 alpha (0 or 1 bit alpha)
		1: DXT2/3 alpha (4 bit alpha)
		7: DXT4/5 alpha (interpolated alpha)
		*/

		// fill palette
		if (this->m_compression == Blp::Uncompressed || this->m_compression == Blp::DirectXCompression)
			//header.palette;
			;

		// TODO header.hasMipMaps

		for (std::size_t i = 0; i < Blp::maxMipMaps; ++i)
		{
			struct MipMapHeaderData *headerData = new MipMapHeaderData;
			mipMapData[i] = headerData;
			headerData->offset = header.mipMapOffset[i],
			headerData->size = header.mipMapSize[i];
		}
	}

	std::size_t mipMapsCount = requiredMipMaps(this->m_width, this->m_height);
	std::cout << "Required mip maps are " << mipMapsCount << " with width " << this->m_width << " and height " << this->m_height << std::endl; // TEST

	std::map<class MipMap*, struct MipMapHeaderData*> mipMaps;
	typedef std::pair<class MipMap*, struct MipMapHeaderData*> KeyType;

	for (std::size_t i = 0; i < mipMapsCount; ++i)
	{
		class MipMap *mipMap = new MipMap(this, this->mipMapWidth(i), this->mipMapHeight(i));
		mipMaps[mipMap] = mipMapData[i];

		if (this->m_compression == Blp::Paletted)
		{
			if (this->m_flags == Blp::NoAlpha && mipMapData[i]->size != mipMap->width() * mipMap->height() * sizeof(byte))
				std::cerr << boost::format(_("Size %1% is not equal to %2%.")) %  mipMapData[i]->size % (mipMap->width() * mipMap->height() * sizeof(color)) << std::endl;
			else if (this->m_flags == Blp::Alpha && mipMapData[i]->size != mipMap->width() * mipMap->height() * 2 * sizeof(byte))
				std::cerr << boost::format(_("Size %1% is not equal to %2%.")) %  mipMapData[i]->size % (mipMap->width() * mipMap->height() * 2 * sizeof(color)) << std::endl;
		}

		this->m_mipMaps.push_back(mipMap);
	}

	mipMapData.clear(); // not required anymore

	try
	{
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

				std::cout << "Some function pointers: " << jpeg_CreateDecompress << jpeg_mem_src << jpeg_read_header << jpeg_start_decompress << std::endl;
			}
			catch (class Exception &exception)
			{
				exception.what().append("\nRequired for BLP reading.");

				throw exception;
			}

			if (BITS_IN_JSAMPLE > sizeof(byte) * 8)
				throw Exception(boost::format(_("Too many bits in one single sample (one single pixel color channel): %1%. BLP/wc3lib allows maximum sample size of %2%.")) % BITS_IN_JSAMPLE % (sizeof(byte) * 8));

			dword jpegHeaderSize;
			wc3lib::read(istream, jpegHeaderSize, size);

			if (jpegHeaderSize != 624) // usual size of headers of Blizzard BLPs
				std::cerr << boost::format(_("Warning: JPEG (JFIF) header size is not equal to 624 which is the usual size of Blizzard's JPEG compressed BLPs. It is %1%.")) % jpegHeaderSize << std::endl;

			std::cout << "JPEG header size " << jpegHeaderSize << std::endl;
			byte *jpegHeader = new byte[jpegHeaderSize];
			std::cout << "JPEG header size " << jpegHeaderSize << std::endl;
			wc3lib::read(istream, *jpegHeader, size, jpegHeaderSize);
			std::cout << "JPEG header size " << jpegHeaderSize << std::endl;


			BOOST_FOREACH(class MipMap *mipMap, this->m_mipMaps)
			{
				dword mipMapOffset = mipMaps[mipMap]->offset;
				dword mipMapSize = mipMaps[mipMap]->size;
				// all mipmaps use the same header, jpeg header has been allocated before and is copied into each mip map buffer.
				std::cout << "JPEG header size " << jpegHeaderSize << std::endl;
				std::cout << "MIP Map size " << boost::numeric_cast<std::size_t>(mipMapSize) << std::endl;
				std::size_t bufferSize = jpegHeaderSize + boost::numeric_cast<std::size_t>(mipMapSize);
				std::cout << "Buffer size (header + MIP map) " << bufferSize << std::endl;
				unsigned char *buffer = new unsigned char[bufferSize];
				memcpy(reinterpret_cast<void*>(buffer), reinterpret_cast<const void*>(jpegHeader), jpegHeaderSize); // copy header data
				std::cout << "Copied JPEG header into buffer " << std::endl;

				// moving to offset, skipping null bytes
				std::streampos position = istream.tellg();
				istream.seekg(mipMapOffset);
				std::size_t nullBytes = istream.tellg() - position;

				if (nullBytes > 0)
					std::cout << boost::format(_("Ignoring %1% 0 bytes.")) % nullBytes << std::endl;

				// read mip map data starting at header offset, header has already been copied into buffer
				wc3lib::read(istream, buffer[jpegHeaderSize], size, boost::numeric_cast<std::streamsize>(mipMapSize));

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
							std::cerr << boost::format(_("Warning: Image width (%1%) is not equal to mip map width (%2%).")) % cinfo.image_width % mipMap->width() << std::endl;

						if (mipMap->height() != cinfo.image_height)
							std::cerr << boost::format(_("Warning: Image height (%1%) is not equal to mip map height (%2%).")) % cinfo.image_height % mipMap->height() << std::endl;

						if (cinfo.out_color_space != JCS_RGB)
							std::cerr << boost::format(_("Warning: Image color space (%1%) is not equal to RGB (%2%).")) % cinfo.out_color_space % JCS_RGB << std::endl;

						std::cout << "JPEG image has width " << cinfo.image_width << " and height " << cinfo.image_height << std::endl;
						std::cout << "JPEG image has scaled width " << cinfo.output_width << " and scaled height " << cinfo.output_height << std::endl;
						std::cout << "Color map has size " << cinfo.actual_number_of_colors << std::endl;

						/// \todo Get as much required scanlines as possible (highest divident) to increase speed. Actually it could be equal to the MIP maps height which will lead to reading the whole MIP map with one single \ref jpeg_read_scanlines call
						static const JDIMENSION requiredScanlines = 1; // increase this value to read more scanlines in one step
						JDIMENSION scanlineSize = cinfo.output_width * cinfo.output_components; // JSAMPLEs per row in output buffer
						scanlines = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, scanlineSize, requiredScanlines);
						std::cout << "Required scanlines " << requiredScanlines << std::endl;
						std::cout << "Size of scanline " << scanlineSize << " (width * components)" << std::endl;
						std::cout << "we do have " << cinfo.output_components << " components/color channels." << std::endl;

						// per scanline
						while (cinfo.output_scanline < cinfo.output_height)
						{
							//std::cout << "Output scanline is " << cinfo.output_scanline << " and buffer size is " << scanlinesSize << " and height is " <<  cinfo.output_height << std::endl;

							//scanlines[cinfo.output_scanline] = new JSAMPLE[cinfo.output_height];
							JDIMENSION width = 0;
							JDIMENSION height = cinfo.output_scanline; // cinfo.output_scanline is increased by one after calling jpeg_read_scanlines
							JDIMENSION dimension = jpeg_read_scanlines(&cinfo, scanlines, requiredScanlines); // scanlinesSize
							//std::cout << "Read " << dimension << " scanlines" << std::endl;

							if (dimension != requiredScanlines)
								std::cerr << boost::format(_("Warning: Number of scanned lines is not equal to %1%. It is %2%.")) % requiredScanlines % dimension << std::endl;

							for (JDIMENSION scanline = 0; scanline < dimension; ++scanline, ++height)
							{
								// cinfo.output_components should be 3 if RGB and 4 if RGBA
								for (int component = 0; component < scanlineSize; component += cinfo.output_components)
								{
									// store as ARGB (BLP)
									// TODO why is component 0 blue, component 1 green and component 2 red?
									color argb = ((color)scanlines[scanline][component]) + ((color)scanlines[scanline][component + 1] << 8) + ((color)scanlines[scanline][component + 2] << 16);

									if (cinfo.output_components == 4) // we do have an alpha channel
										argb += ((color)(0xFF - scanlines[scanline][component + 3]) << 24);
									//std::cout << "ARGB " << std::ios::hex << argb << std::endl;

									mipMap->setColor(width, height, argb, 0); /// \todo Get alpha?!
									width++;
								}
							}

							//std::cout << "read " << i << " blas" << std::endl;


							//put_scanline_someplace(scanlines[0], scanlinesSize);
							//std::cout << "I is " << i << " and read lines " << dimension << std::endl;
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

				++mipMapOffset;
				++mipMapSize;
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
				wc3lib::read(istream, palette[i], size);

			BOOST_FOREACH(class MipMap *mipMap, this->m_mipMaps)
			{
				dword mipMapOffset = mipMaps[mipMap]->offset;
				dword mipMapSize = mipMaps[mipMap]->size;
				std::streampos position = istream.tellg();
				istream.seekg(mipMapOffset);
				std::size_t nullBytes = istream.tellg() - position;

				if (nullBytes > 0)
					std::cout << boost::format(_("Ignoring %1% 0 bytes.")) % nullBytes << std::endl;


				for (dword height = 0; height < mipMap->height(); ++height)
				{
					for (dword width = 0; width < mipMap->width(); ++width)
					{
						byte index;
						std::streamsize readSize = 0;
						wc3lib::read(istream, index, readSize);
						size += readSize;
						mipMapSize -= boost::numeric_cast<dword>(readSize);
						/*
						byte alpha = 0;

						if (this->m_flags == Blp::Alpha)
						{
							readSize = 0;
							wc3lib::read(istream, alpha, readSize);
							size += readSize;
							mipMapSize -= readSize;
						}
						*/

						mipMap->setColor(width, height, palette[index], 0, index);
					}
				}

				if (this->m_flags == Blp::Alpha)
				{
					for (dword height = 0; height < mipMap->height(); ++height)
					{
						for (dword width = 0; width < mipMap->width(); ++width)
						{
							byte alpha;
							std::streamsize readSize = 0;
							wc3lib::read(istream, alpha, readSize);
							size += readSize;
							mipMapSize -= boost::numeric_cast<dword>(readSize);
							mipMap->setColorAlpha(width, height, alpha);
						}
					}
				}

				if (mipMapSize != 0)
				{
					istream.seekg(mipMapSize, std::ios_base::cur);
					std::cout << "Skipping " << mipMapSize << " unnecessary bytes." << std::endl;
				}

				std::cout << "Mip map colors map size " << mipMap->m_colors.size() << std::endl;
			}
		}
		else
			throw Exception(boost::str(boost::format( _("Unknown compression mode: %1%.")) % this->m_compression));
	}
	// clean up mip map data
	catch (...)
	{
		BOOST_FOREACH(KeyType iterator, mipMaps)
			delete iterator.second;

		throw;
	}

	// clean up mip map data
	BOOST_FOREACH(KeyType iterator, mipMaps)
		delete iterator.second;

	std::cout << "Read " << size << " bytes." << std::endl;

	// check mip maps
	/*
	- A full mipmap chain must be present. The last mipmap must be 1x1 (no larger).
	If an image is 32x8 the mipmap chain must be 32x8, 16x4, 8x2, 4x1, 2x1, 1x1.
	Sizes not of powers of 2 seems to work fine too, the same rules for mipmaps
	still applies. Ex: 24x17, 12x8 (rounded down), 6x4, 3x2, 1x1 (rounded down).
	*/
	if (this->m_mipMaps.empty() || this->m_mipMaps.back()->width() != 1 || this->m_mipMaps.back()->height() != 1)
		throw Exception(_("Last mip map does not exist or has not a size of 1x1."));

	return size;
}

std::streamsize Blp::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	std::streamsize size = 0;

	switch (this->m_version)
	{
		case Blp::Blp0:
			wc3lib::write(ostream, Blp::identifier0, size);

			break;

		case Blp::Blp1:
			wc3lib::write(ostream, Blp::identifier1, size);

			break;

		case Blp::Blp2:
			wc3lib::write(ostream, Blp::identifier2, size);

			break;
	}

	dword startOffset = 0;

	if (this->m_version == Blp::Blp1 || this->m_version == Blp::Blp2)
	{
		struct BlpHeader header;
		header.compression = static_cast<dword>(this->compression());
		header.flags = static_cast<dword>(this->flags());
		header.width = this->width();
		header.height = this->height();
		header.pictureType = this->pictureType();
		header.pictureSubType = this->pictureSubType();

		// offsets and sizes are assigned after writing mip maps (later, see below)
		for (std::size_t i = 0; i < Blp::maxMipMaps; ++i)
		{
			header.mipMapOffset[i] = 0;
			header.mipMapSize[i] = 0;
		}

		wc3lib::write(ostream, header, size);
		startOffset = (dword)ostream.tellp() - sizeof(dword) * Blp::maxMipMaps * 2; // offset where mip map header data starts, required by later mip map writing operations
	}
	// BLP2
	else if (this->m_version == Blp::Blp2)
	{
		struct Blp2Header header;

		if (this->compression() == Jpeg)
			header.type = 0;
		else
		{
			header.type = 1;

			if (this->compression() == Blp::Uncompressed)
				header.encoding = 1;
			else if (this->compression() == Blp::DirectXCompression)
				header.encoding = 2;
		}

		header.alphaDepth = static_cast<byte>(this->flags());
		// header.alphaEncoding
		/*
		TODO
		0: DXT1 alpha (0 or 1 bit alpha)
		1: DXT2/3 alpha (4 bit alpha)
		7: DXT4/5 alpha (interpolated alpha)
		*/
		header.alphaEncoding = 0;

		// fill palette
		if (this->m_compression == Blp::Uncompressed || this->m_compression == Blp::DirectXCompression)
		{
			color *colorPalette = palette();
			memcpy(static_cast<void*>(header.palette), static_cast<const void*>(colorPalette), Blp::compressedPaletteSize);
			delete colorPalette;
		}

		header.hasMips = this->mipMaps().size() > 1;

		// offsets and sizes are assigned after writing mip maps (later, see below)
		for (std::size_t i = 0; i < Blp::maxMipMaps; ++i)
		{
			header.mipMapOffset[i] = 0;
			header.mipMapSize[i] = 0;
		}

		wc3lib::write(ostream, header, size);
		startOffset = (dword)ostream.tellp() - sizeof(dword) * Blp::maxMipMaps * 2 + sizeof(color) * Blp::compressedPaletteSize; // offset where mip map header data starts, required by later mip map writing operations
	}

	if (this->m_compression == Blp::Jpeg)
	{
		/// @todo Use class LibraryLoader to load JPEG functions and write data.
		std::cout << "Detected JPEG compression mode. Not implemented yet." << std::endl;
		throw Exception(_("Missing JPEG write support."));
	}
	else if (this->m_compression == Blp::Paletted)
	{
		std::cout << "Detected paletted compression." << std::endl;

		if (this->m_flags == Blp::Alpha)
			std::cout << "With alphas!" << std::endl;
		else
			std::cout << "Without alphas!" << std::endl;

		// write palette, palette has always size of Blp::compressedPaletteSize (remaining colors have value 0).
		color *colorPalette = palette();
		wc3lib::write(ostream, *colorPalette, size, Blp::compressedPaletteSize);
		delete colorPalette;
		colorPalette = 0;

		std::vector<dword> offsets(this->m_mipMaps.size(), 0);
		std::vector<dword> sizes(this->m_mipMaps.size(), 0);
		std::size_t index = 0;

		// write mip maps
		BOOST_FOREACH(const class MipMap *mipMap, this->m_mipMaps)
		{
			dword mipMapOffset = ostream.tellp();
			dword mipMapSize = size;

			for (dword width = 0; width < mipMap->width(); ++width)
			{
				for (dword height = 0; height < mipMap->height(); ++height)
				{
					byte index = mipMap->colorAt(width, height).paletteIndex();
					wc3lib::write(ostream, index, size);

					/*
					if (this->m_flags == Blp::Alpha)
					{
						byte alpha = mipMap->colorAt(width, height).alpha();
						wc3lib::write(ostream, alpha, size);
					}
					*/
				}
			}

			if (this->m_flags == Blp::Alpha)
			{
				for (dword height = 0; height < mipMap->height(); ++height)
				{
					for (dword width = 0; width < mipMap->width(); ++width)
					{
						byte alpha = mipMap->colorAt(width, height).alpha();
						wc3lib::write(ostream, alpha, size);
					}
				}
			}

			// set header data
			mipMapSize = size - mipMapSize;

			offsets[index] = mipMapOffset;
			sizes[index] = mipMapSize;

			++index;
		}

		// write header data, jump to header
		ostream.seekp(startOffset);

		for (index = 0; index < this->m_mipMaps.size(); ++index)
		{
			wc3lib::write(ostream, offsets[index], size);
			wc3lib::write(ostream, sizes[index], size);
		}
	}
	else
		throw Exception(boost::str(boost::format(_("Unknown compression mode: %1%.")) % this->m_compression));

	return size;
}

class Blp::MipMap* Blp::addInitialMipMap(dword width, dword height)
{
	BOOST_FOREACH(class MipMap *mipMap, this->m_mipMaps)
		delete mipMap;

	this->m_mipMaps.clear();
	class MipMap *result = new MipMap(this, width, height);
	this->m_mipMaps.push_back(result);

	return result;
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
		class MipMap *mipMap = new MipMap(this, width, height);
		/// @todo Generate new scaled index and alpha list.


		this->m_mipMaps.push_back(mipMap);
	}

	return true;
}

color* Blp::palette() const
{
	color *palette = new color[compressedPaletteSize];
	memset(reinterpret_cast<void*>(palette), 0, compressedPaletteSize);

	BOOST_FOREACH(Blp::MipMap::MapEntryType entry, this->mipMaps().front()->colors())
		palette[entry.second.paletteIndex()] = entry.second.argb();

	return palette;
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
