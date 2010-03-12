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

#include <istream>
#include <ostream>
#include <list>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace blp
{

/**
* @brief Provides access to the BLP format.
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
				MipMap();
				
				void scale(dword newWidth, dword newHeight);
				
				void setWidth(dword width);
				dword width() const;
				void setHeight(dword height);
				dword height() const;
				
				void addIndex(byte index);
				const std::list<byte>& indices() const;
				void addAlpha(byte alpha);
				const std::list<byte>& alphas() const;
				
			protected:
				dword m_width;
				dword m_height;
				std::list<byte> m_indices; //[mip map width * mip map height];
				std::list<byte> m_alphas; //[mip map width * mip map height];
		};

		enum Format
		{
			BlpFormat,

#ifdef JPEG
			JpegFormat,
#endif
#ifdef TGA
			TgaFormat
#endif
#ifdef PNG
			,PngFormat
#endif
		};
		
		enum Version
		{
			Blp0, //ROC
			Blp1, //TFT
			Blp2 // WoW
		};

		enum Compression
		{
			Jpeg = 0,
			Paletted = 1
		};
		
		enum Flag
		{
			NoAlpha = 0,
			Alpha = 8
		};
		
		static const char identifier0[4];
		static const char identifier1[4];
		static const char identifier2[4];
		static const std::size_t maxMipMaps;
		static const std::size_t maxCompressedPaletteSize;

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
		void setPalette(std::list<color> &palette);
		const std::list<color>& palette() const;
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
#ifdef JPEG
		dword readJpeg(std::istream &istream) throw (class Exception);
		dword writeJpeg(std::ostream &ostream) throw (class Exception);
#endif
#ifdef TGA
		dword readTga(std::istream &istream) throw (class Exception);
		dword writeTga(std::ostream &ostream) throw (class Exception);
#endif
#ifdef PNG
		dword readPng(std::istream &istream) throw (class Exception);
		dword writePng(std::ostream &ostream) throw (class Exception);
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
		
		// header
		enum Version m_version;
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
                std::list<color> m_palette; // uncompressed 1 and 2 only use 256 different colors.
		std::list<class MipMap*> m_mipMaps;
};

inline void Blp::MipMap::setWidth(dword width)
{
	this->m_width = width;
}

inline dword Blp::MipMap::width() const
{
	return this->m_width;
}

inline void Blp::MipMap::setHeight(dword height)
{
	this->m_height = height;
}

inline dword Blp::MipMap::height() const
{
	return this->m_height;
}

inline void Blp::MipMap::addIndex(byte index)
{
	this->m_indices.push_back(index);
}

inline const std::list<byte>& Blp::MipMap::indices() const
{
	return this->m_indices;
}

inline void Blp::MipMap::addAlpha(byte alpha)
{
	this->m_alphas.push_back(alpha);
}

inline const std::list<byte>& Blp::MipMap::alphas() const
{
	return this->m_alphas;
}

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

inline void Blp::setPalette(std::list<color> &palette)
{
	this->m_palette = palette;
}

inline const std::list<color>& Blp::palette() const
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
