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

#ifndef WC3LIB_MPQ_PLATFORM_HPP
#define WC3LIB_MPQ_PLATFORM_HPP

#include <stdint.h>

namespace wc3lib
{

namespace mpq
{

typedef int8_t byte;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef uint32_t uint32; // used for various encryption and hash functions
typedef int64_t int64;
//typedef int2048 int2048; /// @todo WTF?!?!?!!

}

}

#endif
