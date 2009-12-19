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

#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "blp.hpp"
#include "../internationalisation.hpp"

#include <openjpeg.h>
//#include <jpeglib.h>

namespace wc3lib
{

namespace blp
{
	
Blp::MipMap::MipMap()
{
}

void Blp::MipMap::scale(dword newWidth, dword newHeight)
{
}

const char Blp::identifier0[4] = { 'B', 'L', 'P', '0' };
const char Blp::identifier1[4] = { 'B', 'L', 'P', '1' };
const char Blp::identifier2[4] = { 'B', 'L', 'P', '2' };
const std::size_t Blp::maxMipMaps = 16;
const std::size_t Blp::maxCompressedPaletteSize = 256;

struct BlpHeader
{
	dword identifier;
	dword compression, mipMapsNumber, sizeX, sizeY, pictureType, pictureSubType;
	dword mipMapOffset[16], mipMapSize[16];
};		

struct BlpJpegHeader : public BlpHeader
{
	dword jpegHeaderSize;
};

struct Blp2Header
{
	char identifier[4];
	int type;
	char flags[4];
	int width;
	int height;
	int mipMapOffsets[16];
	int mipMapLengths[16];
};

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

static void jpegEventManagerFunction(const char *msg, void *client_data)
{
	std::cout << boost::format(_("JPEG event manager message:\n%1%")) % msg << std::endl;
}

dword Blp::readBlp(std::istream &istream) throw (class Exception)
{
	this->clear();
	// header
	dword identifier;
	istream.read(reinterpret_cast<char*>(&identifier), sizeof(identifier));
	dword bytes = istream.gcount();
	
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
			std::cout << "Reading mip map's " << i << " offset " << offset << " and size " << size << std::endl;
			mipMapOffsets.push_back(offset);
			mipMapSizes.push_back(size);
			
			class MipMap *mipMap = new MipMap;
			mipMap->setWidth(this->mipMapWidth(i));
			mipMap->setHeight(this->mipMapHeight(i));
			
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
		dword jpegHeaderSize;
		istream.read(reinterpret_cast<char*>(&jpegHeaderSize), sizeof(jpegHeaderSize));
		bytes += istream.gcount();
		byte *jpegHeader = new byte[jpegHeaderSize];
		istream.read(reinterpret_cast<char*>(jpegHeader), jpegHeaderSize);
		bytes += istream.gcount();
		std::cout << "Using Open JPEG version " << OPENJPEG_VERSION << std::endl;
		std::list<dword>::iterator offset = mipMapOffsets.begin();
		std::list<dword>::iterator size = mipMapSizes.begin();
		
		BOOST_FOREACH(class MipMap *mipMap, this->m_mipMaps)
		{
			std::streampos position = istream.tellg();
			istream.seekg(*offset);
			std::size_t nullBytes = istream.tellg() - position;
			
			if (nullBytes > 0)
				std::cout << boost::format(_("Ignoring %1% 0 bytes.")) % nullBytes << std::endl;
			
			std::size_t bufferSize = boost::numeric_cast<std::size_t>(*size);
			unsigned char *buffer = new unsigned char[bufferSize];
			istream.read((char*)buffer, boost::numeric_cast<std::streamsize>(bufferSize));

			try
			{
				opj_common_ptr cinfo;
				opj_cio_t *decompressIO = opj_cio_open(cinfo, buffer, bufferSize);

				if (decompressIO == NULL)
					throw Exception(_("Error while reading JPEG data."));

				opj_event_mgr_t eventManager;
				std::cout << "TEST --- 1" << std::endl;
				eventManager.error_handler = &jpegEventManagerFunction;
				std::cout << "TEST --- 2" << std::endl;
				eventManager.info_handler = &jpegEventManagerFunction;
				std::cout << "TEST --- 3" << std::endl;
				eventManager.warning_handler = &jpegEventManagerFunction;
				std::cout << "TEST --- 4" << std::endl;
			
				if (opj_set_event_mgr(cinfo, &eventManager, 0) == NULL)	
					throw Exception(_("Error while creating JPEG event manager."));

				//int OPJ_CALLCONV cio_tell(opj_cio_t *cio);
				//void OPJ_CALLCONV cio_seek(opj_cio_t *cio, int pos);

				std::cout << "Test 1" << std::endl;
				opj_dinfo_t *decompressInfo = opj_create_decompress(CODEC_JPT); //CODEC_UNKNOWN, CODEC_JP2
				/*
				typedef enum CODEC_FORMAT {
				CODEC_UNKNOWN
				CODEC_J2K
				CODEC_JPT
				CODEC_JP2
				} OPJ_CODEC_FORMAT;
				*/

				if (decompressInfo == NULL)
					throw Exception(_("Error while creating JPEG decompressor."));

				std::cout << "Test 2" << std::endl;
				opj_dparameters_t decompressParameters;
				opj_set_default_decoder_parameters(&decompressParameters);
				//decompressParameters.jpwl_max_tiles = 16;
				std::cout << "Test 3" << std::endl;
				opj_setup_decoder(decompressInfo, &decompressParameters);
				std::cout << "Test 4" << std::endl;
				cio_seek(decompressIO, 0);
				opj_image_t *decompressImage = opj_decode(decompressInfo, decompressIO);
				std::cout << "Test 5" << std::endl;
				
				
				if (decompressImage == NULL)
					throw Exception(_("Error while creating decompressed JPEG image."));

				std::cout << "Got decompressed and decoded JPEG image with size X " << decompressImage->x1 << ", size Y " << decompressImage->y1 << ", " << decompressImage->numcomps << " components and color space " << decompressImage->color_space << std::endl;

				/// @todo Fill mip map data
				//mipMap

				opj_destroy_decompress(decompressInfo);
				opj_cio_close(decompressIO);
				delete[] jpegHeader;
				jpegHeader = 0;
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
	}
	else if (this->m_compression == Blp::Paletted)
	{
		std::cout << "Detected paletted compression." << std::endl;
		
		if (this->m_flags == Blp::Alpha)
			std::cout << "With alphas!" << std::endl;
		else
			std::cout << "Without alphas!" << std::endl;
		
		for (int i = 0; i < Blp::maxCompressedPaletteSize; ++i)
		{
                        color paletteColor;
                        istream.read(reinterpret_cast<char*>(&paletteColor), sizeof(paletteColor));
			bytes += istream.gcount();
			this->m_palette.push_back(paletteColor);
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

			for (int j = 0; j < mipMap->width(); ++j)
			{
				for (int k = 0; k < mipMap->height(); ++k)
				{
					byte value;
					istream.read(reinterpret_cast<char*>(&value), sizeof(value));
					bytes += istream.gcount();
					toReadBytes -= istream.gcount();
					//std::cout << "To read bytes " << toReadBytes << std::endl;
					mipMap->addIndex(value);
					
					/*
					if (istream.eof())
					{
						std::cout << "End of File" << std::endl;
						
						throw Exception("");
					}
					*/
					
					if (this->m_flags == Blp::Alpha)
					{
						istream.read(reinterpret_cast<char*>(&value), sizeof(value));
						bytes += istream.gcount();
						toReadBytes -= istream.gcount();
						mipMap->addAlpha(value);
					}
				}
			}
			
			if (toReadBytes != 0)
			{
				istream.seekg(toReadBytes, std::ios_base::cur);
				std::cout << "Skipping " << toReadBytes << " unnecessary bytes." << std::endl;
			}
			
			++offset;
			++size;

			std::cout << "Mip map index list size " << mipMap->indices().size() << std::endl;
			std::cout << "Mip map alpha list size " << mipMap->alphas().size() << std::endl;
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

dword Blp::write(std::ostream &ostream, enum Format format) throw (class Exception)
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


dword Blp::writeBlp(std::ostream &ostream) throw (class Exception)
{
	dword bytes = 0;
	
	switch (this->m_version)
	{
		case Blp::Blp0:
			ostream.write(Blp::identifier0, sizeof(Blp::identifier0));
			bytes += sizeof(identifier0);
			
			break;
			
		case Blp::Blp1:
			ostream.write(Blp::identifier1, sizeof(Blp::identifier1));
			bytes += sizeof(identifier1);
			
			break;
			
		case Blp::Blp2:
			ostream.write(Blp::identifier2, sizeof(Blp::identifier2));
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
		dword size = mipMap->indices().size() * sizeof(byte);
		
		if (this->m_flags == Blp::Alpha)
			size += mipMap->alphas().size() * sizeof(byte);
			
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
	}
	else if (this->m_compression == Blp::Paletted)
	{
		std::cout << "Detected paletted compression." << std::endl;
		
		if (this->m_flags == Blp::Alpha)
			std::cout << "With alphas!" << std::endl;
		else
			std::cout << "Without alphas!" << std::endl;
		
		std::size_t i = 0;
		
		BOOST_FOREACH(const color palettColor, this->m_palette)
		{
			if (i >= Blp::maxCompressedPaletteSize)
			{
				std::cout << boost::format(_("Warning: BLP image does contain more than %1% different colors: %2%.\nSkipping writing of palette colors.")) % Blp::maxCompressedPaletteSize % this->m_palette.size() << std::endl;

				break;
                        }

			ostream.write(reinterpret_cast<const char*>(&palettColor), sizeof(palettColor));
			bytes += sizeof(palettColor);
			++i;
		}

		std::cout << "Wrote " << bytes << " with palette." << std::endl;

                /// @todo Write 0 byte colors?
		if (i < Blp::maxCompressedPaletteSize)
		{
			for ( ; i < Blp::maxCompressedPaletteSize; ++i)
			{
				color palettColor = 0;
				ostream.write(reinterpret_cast<const char*>(&palettColor), sizeof(palettColor));
				bytes += sizeof(palettColor);
			}
		}

		std::cout << "Wrote " << bytes << " with empty palette." << std::endl;

		BOOST_FOREACH(const class MipMap *mipMap, this->m_mipMaps)
		{
			std::cout << "Mip map index list size " << mipMap->indices().size() << std::endl;
			std::cout << "Mip map alpha list size " << mipMap->alphas().size() << std::endl;
			std::list<byte>::const_iterator index = mipMap->indices().begin();
			std::list<byte>::const_iterator alpha = mipMap->alphas().begin();
			
			while (index != mipMap->indices().end())
			{
				ostream.write(reinterpret_cast<const char*>(&(*index)), sizeof(*index));
				bytes += sizeof(*index);
				//std::cout << "Wrote index with size " << sizeof(*index) << std::endl;
				++index;
				
				if (this->m_flags == Blp::Alpha)
				{
					ostream.write(reinterpret_cast<const char*>(&(*alpha)), sizeof(*alpha));
					bytes += sizeof(*alpha);
					++alpha;
				}
				
				//std::cout << "Wrote index with alpha " << sizeof(*alpha) << std::endl;
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
dword Blp::readJpeg(std::istream &istream) throw (class Exception)
{
	return 0;
}

dword Blp::writeJpeg(std::ostream &ostream) throw (class Exception)
{
	return 0;
}
#endif
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
#ifdef PNG
dword Blp::readPng(std::istream &istream) throw (class Exception)
{
	return 0;
}

dword Blp::writePng(std::ostream &ostream) throw (class Exception)
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
		class MipMap *mipMap = new MipMap;
		mipMap->setWidth(width);
		mipMap->setHeight(height);
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
