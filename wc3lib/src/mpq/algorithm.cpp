/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#include "algorithm.hpp"

namespace wc3lib
{
	
namespace mpq
{
	
void InitializeCryptTable(uint32 dwCryptTable[0x500])
{
    uint32 seed = 0x00100001;
    uint32 index1 = 0;
    uint32 index2 = 0;
    int16 i;

    for (index1 = 0; index1 < 0x100; index1++)
    {
        for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
        {
            uint32 temp1, temp2;

            seed  = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;

            seed  = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);

            dwCryptTable[index2] = (temp1 | temp2);
        }
    }
}

void EncryptData(const uint32 dwCryptTable[0x500], void *lpbyBuffer, uint32 dwLength, uint32 dwKey)
{
    assert(lpbyBuffer);

    uint32 *lpdwBuffer = (uint32 *)lpbyBuffer;
    uint32 seed = 0xEEEEEEEE;
    uint32 ch;

    dwLength /= sizeof(uint32);

    while(dwLength-- > 0)
    {
        seed += dwCryptTable[0x400 + (dwKey & 0xFF)];
        ch = *lpdwBuffer ^ (dwKey + seed);

        dwKey = ((~dwKey << 0x15) + 0x11111111) | (dwKey >> 0x0B);
        seed = *lpdwBuffer + seed + (seed << 5) + 3;
	
	*lpdwBuffer++ = ch;
    }
}

void DecryptData(const uint32 dwCryptTable[0x500], void *lpbyBuffer, uint32 dwLength, uint32 dwKey)
{
    assert(lpbyBuffer);

    uint32 *lpdwBuffer = (uint32 *)lpbyBuffer;
    uint32 seed = 0xEEEEEEEEL;
    uint32 ch;

    dwLength /= sizeof(uint32);

    while(dwLength-- > 0)
    {
        seed += dwCryptTable[0x400 + (dwKey & 0xFF)];
        ch = *lpdwBuffer ^ (dwKey + seed);

        dwKey = ((~dwKey << 0x15) + 0x11111111L) | (dwKey >> 0x0B);
        seed = ch + seed + (seed << 5) + 3;

	*lpdwBuffer++ = ch;
    }
}

/// Based on code from StormLib.
uint32 HashString(const uint32 dwCryptTable[0x500], const char *lpszString, enum HashType hashType)
{
    assert(lpszString);
    
    uint32 dwHashType = uint32(hashType);
    uint32 seed1 = 0x7FED7FEDL;
    uint32 seed2 = 0xEEEEEEEEL;
    int16 ch;

    while (*lpszString != 0)
    {
        ch = toupper(*lpszString++);

        seed1 = dwCryptTable[(dwHashType * 0x100) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
    }
    
    return seed1;
}
	
}

}
