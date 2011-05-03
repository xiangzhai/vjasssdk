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

#ifndef WC3LIB_BLP_BLP_HPP
#define WC3LIB_BLP_BLP_HPP

#include <istream>
#include <ostream>
#include <map>
#include <utility>
#include <list>

#include <boost/format.hpp>

#include "platform.hpp"
#include "../format.hpp"
#include "../exception.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace blp
{

/**
* @brief Provides access to the BLP format.
* BLP is Blizzard Entertainment's texture format.
* There are two different format specifications: BLP1 and BLP2.
* Besides there is the BLP0 format which isn't different from BLP1.
* BLP0 and BLP1 are used in games Warcraft 3 and Warcraft 3 The Frozen Throne.
* BLP2 is used in World of Warcraft.
* BLP0 and BLP1 images can have two different compression modes:
* <ul>
* <li>JPEG compression (JFIF)</li>
* <li>Paletted compression</li>
* </ul>
* Additionally the BLP format allows you to save up to 16 mip maps per file.
* A full mipmap chain must be present. The last mipmap must be 1x1 (no larger).
* If an image is 32x8 the mipmap chain must be 32x8, 16x4, 8x2, 4x1, 2x1, 1x1.
* Sizes not of powers of 2 seems to work fine too, the same rules for mipmaps
* still applies. Ex: 24x17, 12x8 (rounded down), 6x4, 3x2, 1x1 (rounded down).
* \note JFIF compression and decompression is realized by using library "jpeg" which is loaded dynamically (at runtime) by using class \ref LibraryLoader. Unfortunatelly some properties (structures for instance) cannot be loaded from shared object/DLL and therefore are compiled statically into wc3lib. Please make sure that you're having installed a compatible version of library "jpeg".
* 
* 
* Little loading example:
* @code
* #include <iostream>
* #include <boost/filesystem/fstream.hpp>
* #include <boost/foreach.hpp>
* #include <boost/format.hpp>
* #include <wc3lib/blp.hpp>
* ...
* using namespace wc3lib::blp;
* std::ifstream ifstream("test.blp", std::ifstream::binary | std::ifstream::in);
* Blp blp = new Blp();
* blp->read(istream);
* std::cout << boost::format("We have %1% MIP maps here.") % blp->mipMaps().size() << std::endl;
*
* BOOST_FOREACH(const class Blp::MipMap *mipMap, blp->mipMaps())
* 	std::cout << boost::format("This mip map has height %1% and width %2%.") % mipMap->height() % mipMap->width() << std::endl;
* {
* 	BOOST_FOREACH(const Blp::MipMap::MapEntryType &colorEntry, mipMap->colors())
* 		std::cout << boost::format("Color %1% at position (%2%|%3%)") % colorEntry.second % colorEntry.first.first % colorEntry.first.second << std::endl;
* }
* 
* delete blp;
* @endcode
* \section Conversion
* If you want to convert BLP images into other formats (e. g. JPEG or PNG) you can either write your own converter or use wc3lib's BLP Qt plugin.
*/
class Blp : public Format
{
	public:
		/**
		* Imagine each mip map as single image containing several pixels with various colors (and alpha values).
		* Mip map colors are stored in a map hashed by their coordinates for fast access.
		* Palette index is also stored for providing paletted compression support when writing the BLP/mip map into output stream.
		* BLP supports ARGB colors.
		*/
		class MipMap
		{
			public:
				class Color
				{
					public:
						/**
						 * Compares color with color \p other.
						 * \return Returns true if ARGB and alpha values are equal. Otherwise it returns false.
						 */
						bool operator==(const class Color &other) const;
						/**
						 * Compares color with color \p other.
						 * \return Returns true if ARGB or alpha values are not equal. Otherwise it returns false.
						 */
						bool operator!=(const class Color &other) const;

						Color();
						~Color();

						class MipMap* mipMap() const;
						void setArgb(color argb);
						color argb() const;
						void setAlpha(byte alpha);
						byte alpha() const;
						/**
						 * Sets palette index to value \p paletteIndex.
						 * \copydoc Color::paletteIndex()
						 */
						void setPaletteIndex(byte paletteIndex);
						/**
						 * \ref Color::paletteIndex() is an optional value for paletted compression (\ref Blp::Compression::Paletted) which refers two the colors color value in palette.
						 */
						byte paletteIndex() const;

					protected:
						friend class MipMap;

						Color(class MipMap *mipMap, color argb, byte alpha, byte paletteIndex);

						class MipMap *m_mipMap;
						color m_argb;
						byte m_alpha;
						byte m_paletteIndex; // only used for paletted compression
				};

				/**
				* @brief Each color is hashed by it's 2-dimensional coordinates on the image. So this is the hash/key type of color values on mip maps. First is width, second is height.
				*/
				typedef std::pair<dword, dword> Coordinates;
				/**
				* @brief This type can be used for constant iteration access to color map of mip map.
				*/
				typedef std::pair<const Coordinates&, const class Color&> MapEntryType;

				void scale(dword newWidth, dword newHeight) throw (class Exception);

				dword width() const;
				dword height() const;

				/**
				 * Assigns MIP map color \p argb at position (\p width | \p height) with alpha \p alpha and palette index \p paletteIndex.
				 * \note Throws an exception if position is out of range.
				 */
				void setColor(dword width, dword height, color argb, byte alpha = 0, byte paletteIndex = 0) throw (class Exception);
				void setColorAlpha(dword width, dword height, byte alpha) throw (class Exception);
				/**
				 * Colors are stored as map where the key is the corresponding coordinates of the color and the value is the color itself.
				 * Using \ref MipMap::MapEntryType helps you to iterate colors easily.
				 */
				const std::map<Coordinates, class Color>& colors() const;
				const class Color& colorAt(dword width, dword height) const;

			protected:
				friend class Blp;

				MipMap(class Blp *blp, dword width, dword height);
				~MipMap();

				class Blp *m_blp;
				dword m_width;
				dword m_height;
				std::map<Coordinates, class Color> m_colors; //[mip map width * mip map height];
		};

		enum Format
		{
			Blp0, /// Reign of Chaos
			Blp1, /// Warcraft The Frozen Throne
			Blp2 /// World of Warcraft
		};

		enum Compression
		{
			Jpeg = 0, /// JFIF!
			Paletted = 1,
			Uncompressed = 2, // BLP2
			DirectXCompression = 3 // BLP2
		};

		enum Flags
		{
			NoAlpha = 0,
			Alpha1Bit = 1, // BLP2
			Alpha4Bit = 4,  // BLP2 (DXT3 only)
			Alpha = 8
		};

		/// File header identifier of format BLP0.
		static const byte identifier0[4];
		/// File header identifier of format BLP1.
		static const byte identifier1[4];
		/// File header identifier of format BLP2.
		static const byte identifier2[4];
		/// Number of maximum mip maps which can be hold by one BLP file.
		static const std::size_t maxMipMaps;
		/// Size of color palette (always the same) used by BLP files with compression BLP::Paletted.
		static const std::size_t compressedPaletteSize;

		Blp();
		~Blp();

		void setFormat(enum Format format);
		enum Format format() const;
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
		const std::list<class MipMap*>& mipMaps() const;
		std::list<class MipMap*>& mipMaps();

		/**
		* Cleans all BLP data (deletes MIP maps and resets properties).
		*/
		void clean();
		/**
		 * Reads BLP texture from input stream \p istream and detects its format and necessary data automatically.
		 * Throws an exception of type \ref Exception if any error occured during read process.
		 * \note JFIF decompression is implemented by using exactly one thread for each MIP map. First buffer is filled with required compressed MIP map data from stream and after that a thread is started which decompresses the buffer's data.
		 * \todo Add multithreading support for MIP maps of paletted compression (JPEG is already implemented).
		 * \param istream Input stream which is read from.
		 * \return Size of read bytes (\ref byte). Note that this value can be smaller than the BLP file since it seems that there are unnecessary 0 bytes in some BLP files.
		 */
		std::streamsize read(InputStream &istream) throw (class Exception);
		std::streamsize write(OutputStream &ostream) const throw (class Exception);
		virtual uint32_t version() const;

		/**
		 * Assigns exactly \p number MIP maps to the BLP image.
		 * \param number Has to be at least 1 and less than \ref Blp::maxMipMaps.
		 * \param regenerate If this value is false function only compares current MIP map number to \p number and if they are not equal it generates or removes necessary or unnecessary MIP maps. Otherwise it clears all MIP maps and truly regenerates them.
		 * \return Returns number of truly newly created MIP maps (negative if some has been removed and 0 if it remains like it was before).
		 * \sa Blp::mipMaps()
		 */
		int generateMipMaps(std::size_t number = Blp::maxMipMaps, bool regenerate = false) throw (class Exception);

		/**
		 * \note You have to take care of the returned palettes memory release.
		 * \return Returns newly allocated color palette with size \ref Blp::compressedPaletteSize.
		 */
		color* palette() const;

	protected:
		dword mipMapWidth(std::size_t index) const;
		dword mipMapHeight(std::size_t index) const;

		// header
		enum Format m_format;
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

inline class Blp::MipMap* Blp::MipMap::Color::mipMap() const
{
	return this->m_mipMap;
}

inline void Blp::MipMap::Color::setArgb(color argb)
{
	this->m_argb = argb;
}

inline color Blp::MipMap::Color::argb() const
{
	return this->m_argb;
}

inline void Blp::MipMap::Color::setAlpha(byte alpha)
{
	this->m_alpha = alpha;
}

inline byte Blp::MipMap::Color::alpha() const
{
	return this->m_alpha;
}

inline void Blp::MipMap::Color::setPaletteIndex(byte paletteIndex)
{
	this->m_paletteIndex = paletteIndex;
}

inline byte Blp::MipMap::Color::paletteIndex() const
{
	return this->m_paletteIndex;
}

inline dword Blp::MipMap::width() const
{
	return this->m_width;
}

inline dword Blp::MipMap::height() const
{
	return this->m_height;
}

inline void Blp::MipMap::setColor(dword width, dword height, color argb, byte alpha, byte paletteIndex) throw (class Exception)
{
	if (width >= this->m_width || height >= this->m_height)
		throw Exception(boost::str(boost::format(_("Mip map: Invalid indices (width %1%, height %2%).")) % width % height));

	//if (this->m_colors.find(std::make_pair(width, height)) != this->m_colors.end())
		//std::cout << "Warning: Color at " << width << " | " << height << " does already exist." << std::endl;

	this->m_colors[std::make_pair(width, height)] = Color(this, argb, alpha, paletteIndex);
}

inline void Blp::MipMap::setColorAlpha(dword width, dword height, byte alpha) throw (class Exception)
{
	if (width >= this->m_width || height >= this->m_height)
		throw Exception(boost::str(boost::format(_("Mip map: Invalid indices (width %1%, height %2%).")) % width % height));

	if (this->m_colors.find(std::make_pair(width, height)) == this->m_colors.end())
		throw Exception(boost::format(_("Warning: Color at %1% | %2% does not exist.")) % width % height);

	this->m_colors[std::make_pair(width, height)].setAlpha(alpha);
}

inline const std::map<Blp::MipMap::Coordinates, class Blp::MipMap::Color>& Blp::MipMap::colors() const
{
	return this->m_colors;
}

inline const class Blp::MipMap::Color& Blp::MipMap::colorAt(dword width, dword height) const
{
	return const_cast<const class Blp::MipMap::Color&>(const_cast<class MipMap*>(this)->m_colors[std::make_pair(width, height)]);
}

inline void Blp::setFormat(enum Format format)
{
	this->m_format = format;
}

inline enum Blp::Format Blp::format() const
{
	return this->m_format;
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

inline const std::list<class Blp::MipMap*>& Blp::mipMaps() const
{
	return this->m_mipMaps;
}

inline std::list<class Blp::MipMap*>& Blp::mipMaps()
{
	return this->m_mipMaps;
}

}

}

#endif
