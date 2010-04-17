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

#ifndef WC3LIB_BLP_PLATFORM_HPP
#define WC3LIB_BLP_PLATFORM_HPP

#include <boost/cstdint.hpp>

namespace wc3lib
{

namespace blp
{

/*
//+-----------------------------------------------------------------------------
//| Data types
//+-----------------------------------------------------------------------------
CHAR   - 8bit character
BYTE   - 8bit unsigned integer
WORD   - 16bit unsigned integer
DWORD  - 32bit unsigned integer
FLOAT  - 32bit floating point number
COLOR  - 32bit color value of type RGBA, one byte per channel
X[n]   - An n-dimensional vector of type X
*/
typedef char char8;
typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef float float32;
typedef int32_t color;

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

}

}

#endif 
