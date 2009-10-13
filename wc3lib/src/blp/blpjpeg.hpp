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

#ifndef WC3LIB_BLP_BLPJPEG_HPP
#define WC3LIB_BLP_BLPJPEG_HPP

#include <fstream>

#include "../exception.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace blp
{
	
class Blp;

/**
* BLP JPEG structure (Compression == 0)
* Up to 16 mipmaps can be stored in a blp image. 2^16 = 65536, so there's
* little risk it won't be enough. Each JPEG (JFIF to be more exact) image
* is constructed by merging the header with the mipmap (all mipmaps uses
* the same header. It seems like Warcraft 3 can handle JPEG header sizes
* of 0 (in case you have trouble generating JPEG images using the same
* header) however there are other fan tools that does not. Specifying a
* low number like 4 will work too as the only shared data are the initial
* JPEG markers.
*
* Each mipmap has a certain size and is located at  a certain offset as
* specified in the main blp header. There can be (and sometimes are in
* Blizzard's images) unused space between the JPEG header and the JPEG
* data. Why this is I don't know!
*
* The JPEG header of Blizzard's images is usually 624 bytes long. This
* may or may not be true for your own generated images depending on how
* you generated them.
*
* The JPEG format is advanced so I won't go into detail here.
*/
class BlpJpeg
{
	public:
		struct MipMap
		{
			byte *m_jpegData; //[???], current height * current width? size == ????
		};
		
		static int requiredMipMaps(int width, int height);
		
		BlpJpeg(class Blp *blp);
		~BlpJpeg();

		class Blp* blp() const;
		
		dword read(std::fstream &fstream) throw (class Exception);
		dword write(std::fstream &fstream) throw (class Exception);

	private:
		class Blp *m_blp;
		dword m_jpegHeaderSize;
		byte *m_jpegHeader;
		MipMap m_mipMaps[16];
};

inline class Blp* BlpJpeg::blp() const
{
	return this->m_blp;
}

}

}

#endif
