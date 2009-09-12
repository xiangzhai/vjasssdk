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

#ifndef WC3LIB_BLP_BLPUNCOMPRESSED2_HPP
#define WC3LIB_BLP_BLPUNCOMPRESSED2_HPP

#include "platform.hpp"

namespace wc3lib
{

namespace blp
{

/**
* BLP Uncompressed 2 structure (Compression == 1, PictureType == 5)
* CurrentWidth/CurrentHeight is the width/height for the current mipmap.
* Mipmap size/offset works the same as explained for JPEGs above.
*
* Each cell in the index list refers to a location in the palette where
* the corresponding RGBA value is. The alpha value is inversed so the real
* alpha is "255 - alpha".
*/
struct BlpUncompressed2
{
	struct MipMap
	{
		byte *m_indexList; //[CurrentWidth * CurrentHeight];
	};

	BlpUncompressed2(const class Blp &blp);
	~BlpUncompressed2();

	color m_palette[256];
	MipMap m_mipMaps[16];
};

}

}

#endif
