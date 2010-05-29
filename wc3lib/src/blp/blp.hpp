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
#include <map>
#include <utility>
#include <list>

#include <boost/format.hpp>

#include "platform.hpp"
#include "../exception.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace blp
{

/**
* @brief Provides access to the BLP format.
*
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
				struct Color
				{
					color rgb;
					byte alpha;
					byte paletteIndex; // only used for paletted compression

					bool compareRgb(const Color &other) const
					{
						return this->rgb == other.rgb;
					};
				};

				MipMap(dword width, dword height);
				
				void scale(dword newWidth, dword newHeight) throw (class Exception);
				
				dword width() const;
				dword height() const;
				
				void setColor(dword width, dword height, color rgb, byte alpha, byte paletteIndex = 0) throw (class Exception);
				const std::map<std::pair<dword, dword>, struct Color>& colors() const;
				const struct Color& colorAt(dword width, dword height) const;

			protected:
				friend class Blp;

				dword m_width;
				dword m_height;
				std::map<std::pair<dword, dword>, struct Color> m_colors; //[mip map width * mip map height];
		};

		enum Format
		{
			BlpFormat,
			JpegFormat,
			TgaFormat,
			PngFormat
		};
		
		enum Version
		{
			Blp0, //ROC
			Blp1, //TFT
			Blp2 // WoW
		};

		enum Compression
		{
			Jpeg = 0, // JFIF!
			Paletted = 1
		};
		
		enum Flags
		{
			NoAlpha = 0,
			Alpha = 8
		};
		
		static const byte identifier0[4];
		static const byte identifier1[4];
		static const byte identifier2[4];
		static const std::size_t maxMipMaps;
		static const std::size_t maxCompressedPaletteSize;

		Blp();
		~Blp();

		void setCompression(enum Compression compression);
		enum Compression compression() const;
		void setFlags(enum Flags flags);
		enum Flags flags() const;
		void setWidth(dword width);
		dword width() const;
		void setHeight(dword height);
		dword height() const;
		void setPictureType(dword pictureType);
		dword pictureType() const;
		void setPictureSubType(dword pictureSubType);
		dword pictureSubType() const;
		void addMipMap(class MipMap *mipMap);
		const std::list<class MipMap*>& mipMaps() const;

		/**
		* Cleans all BLP data.
		*/
		void clean();
		std::streamsize read(std::istream &istream, enum Format format) throw (class Exception);
		std::streamsize write(std::ostream &ostream, enum Format format) throw (class Exception);
		/**
		* @return Read bytes. Note that this value can be smaller than the BLP file since it seems that there are unnecessary 0 bytes in some BLP files.
		*/
		std::streamsize readBlp(std::istream &istream) throw (class Exception);
		std::streamsize writeBlp(std::ostream &ostream) throw (class Exception);
		std::streamsize readJpeg(std::istream &istream) throw (class Exception);
		std::streamsize writeJpeg(std::ostream &ostream) throw (class Exception);
		std::streamsize readTga(std::istream &istream) throw (class Exception);
		std::streamsize writeTga(std::ostream &ostream) throw (class Exception);
		std::streamsize readPng(std::istream &istream) throw (class Exception);
		std::streamsize writePng(std::ostream &ostream) throw (class Exception);

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
		enum Compression m_compression;		//0 - Uses JPEG compression
						//1 - Uses palettes (uncompressed)
		enum Flags m_flags;			//#8 - Uses alpha channel (?)
		dword m_width;
		dword m_height;
		dword m_pictureType;		//3 - Uncompressed index list + alpha list
						//4 - Uncompressed index list + alpha list
						//5 - Uncompressed index list
		dword m_pictureSubType;		//1 - ???
		std::list<class MipMap*> m_mipMaps;
};

inline dword Blp::MipMap::width() const
{
	return this->m_width;
}

inline dword Blp::MipMap::height() const
{
	return this->m_height;
}

inline void Blp::MipMap::setColor(dword width, dword height, color rgb, byte alpha, byte paletteIndex) throw (class Exception)
{
	if (width >= this->m_width || height >= this->m_height)
		throw Exception(boost::str(boost::format(_("Mip map: Invalid indices (width %1%, height %2%).")) % width % height));

	this->m_colors[std::make_pair(width, height)].rgb = rgb;
	this->m_colors[std::make_pair(width, height)].alpha = alpha;
	this->m_colors[std::make_pair(width, height)].paletteIndex = paletteIndex;
}

inline const std::map<std::pair<dword, dword>, struct Blp::MipMap::Color>& Blp::MipMap::colors() const
{
	return this->m_colors;
}

inline const struct Blp::MipMap::Color& Blp::MipMap::colorAt(dword width, dword height) const
{
	return const_cast<const struct Blp::MipMap::Color&>(const_cast<class MipMap*>(this)->m_colors[std::make_pair(width, height)]);
}

inline void Blp::setCompression(enum Blp::Compression compression)
{
	this->m_compression = compression;
}

inline enum Blp::Compression Blp::compression() const
{
	return this->m_compression;
}

inline void Blp::setFlags(enum Blp::Flags flags)
{
	this->m_flags = flags;
}

inline enum Blp::Flags Blp::flags() const
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
