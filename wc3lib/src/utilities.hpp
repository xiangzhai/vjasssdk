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

#ifndef WC3LIB_WC3LIB_UTILITIES_HPP
#define WC3LIB_WC3LIB_UTILITIES_HPP

#include <algorithm> 

/*
extern unsigned long ByteSwap(unsigned long value);
extern unsigned short ByteSwap(unsigned short value);
*/
inline unsigned long ByteSwap(unsigned long value)
{
	return (((value&0x000000FF)<<24)+((value&0x0000FF00)<<8)+ ((value&0x00FF0000)>>8)+((value&0xFF000000)>>24));
}

inline unsigned short ByteSwap(unsigned short value)
{
   return (((value>> 8)) | (value << 8));

}

inline void ByteSwap(unsigned char *bytes, int size)
{
   register int i = 0;
   register int j = size - 1;
   
   while (i < j)
   {
      std::swap(bytes[i], bytes[j]);
      i++, j--;
   }
}

#endif
