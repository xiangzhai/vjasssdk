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

#include <iostream>
#include <list>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace blp
{

class BlpJpeg;
class BlpUncompressed1;

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
		class MipMap
		{
			public:
				void scale(dword newWidth, dword newHeight);
			
				/// @todo Make private, separated file, member methods
				dword m_width;
				dword m_height;
				std::list<byte> m_indexList; //[CurrentWidth * CurrentHeight];
				std::list<byte> m_alphaList; //[CurrentWidth * CurrentHeight];
		};

		enum Format
		{
			BlpFormat,

#ifdef TGA
			TgaFormat,
#endif
			
			UnknownFormat
		};

		enum Compression
		{
			Jpeg = 0,
			Uncompressed = 1
		};
		
		static const char identifier[4];
		static const std::size_t maxMipMaps;

		Blp();
		~Blp();

		void setCompression(dword compression);
		dword compression() const;
		void setFlags(dword flags);
		dword flags() const;
		void setWidth(dword width);
		dword width() const;
		void setHeight(dword height);
		dword height() const;
		void setPictureType(dword pictureType);
		dword pictureType() const;
		void setPictureSubType(dword pictureSubType);
		dword pictureSubType() const;
		void setPaletteColor(std::size_t index, color paletteColor);
		const color* palette() const;
		void addMipMap(class MipMap *mipMap);
		const std::list<class MipMap*>& mipMaps() const;

		void clear();
		dword read(std::istream &istream, enum Format format) throw (class Exception);
		dword write(std::ostream &ostream, enum Format format) throw (class Exception);
		/**
		* @return Read bytes. Note that this value can be smaller than the BLP file since it seems that there are unnecessary 0 bytes in some BLP files.
		*/
		dword readBlp(std::istream &istream) throw (class Exception);
		dword writeBlp(std::ostream &ostream) throw (class Exception);
		
#ifdef TGA
		dword readTga(std::istream &istream) throw (class Exception);
		dword writeTga(std::ostream &ostream) throw (class Exception);
#endif

		/**
		* Adds mip map @param initialMipMap to mip map list and generates number - 1 new mip maps which are added to mip map list, too.
		* Note that the intial mip map has to have the same width and height as the BLP file.
		* @return Returns true if mip map generation was successfully otherwise it returns false.
		*/
		bool generateMipMaps(class MipMap *initialMipMap, std::size_t number = Blp::maxMipMaps);
		
	protected:
		dword mipMapWidth(int index) const;
		dword mipMapHeight(int index) const;
		bool usesAlphaList() const;
		
		// header
		dword m_compression;		//0 - Uses JPEG compression
						//1 - Uses palettes (uncompressed)
		dword m_flags;			//#8 - Uses alpha channel (?)
		dword m_width;
		dword m_height;
		dword m_pictureType;		//3 - Uncompressed index list + alpha list
						//4 - Uncompressed index list + alpha list
						//5 - Uncompressed index list
		dword m_pictureSubType;		//1 - ???
		// uncompressed 1
		color m_palette[256];
		std::list<class MipMap*> m_mipMaps;
};

inline void Blp::setCompression(dword compression)
{
	this->m_compression = compression;
}

inline dword Blp::compression() const
{
	return this->m_compression;
}

inline void Blp::setFlags(dword flags)
{
	this->m_flags = flags;
}

inline dword Blp::flags() const
{
	return this->m_flags;
}

inline void Blp::setWidth(dword width)
{
	this->m_width = width;
}

inline dword Blp::width() const
{
	return this->m_width;
}

inline void Blp::setHeight(dword height)
{
	this->m_height = height;
}

inline dword Blp::height() const
{
	return this->m_height;
}

inline void Blp::setPictureType(dword pictureType)
{
	this->m_pictureType = pictureType;
}

inline dword Blp::pictureType() const
{
	return this->m_pictureType;
}

inline void Blp::setPictureSubType(dword pictureSubType)
{
	this->m_pictureSubType = pictureSubType;
}

inline dword Blp::pictureSubType() const
{
	return this->m_pictureSubType;
}

inline void Blp::setPaletteColor(std::size_t index, color paletteColor)
{
	this->m_palette[index] = paletteColor;
}

inline const color* Blp::palette() const
{
	return this->m_palette;
}

inline void Blp::addMipMap(class MipMap *mipMap)
{
	this->m_mipMaps.push_back(mipMap);
}

inline const std::list<class Blp::MipMap*>& Blp::mipMaps() const
{
	return this->m_mipMaps;
}

}

}

#endif