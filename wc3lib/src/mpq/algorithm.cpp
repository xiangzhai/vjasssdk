/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#include <cstring>

#include <boost/format.hpp>

#include "algorithm.hpp"
#include "../internationalisation.hpp"

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

struct DataInfo
{
	char *inBuffer; // Pointer to input data buffer
	int inPosition; // Current offset in input data buffer
	int inBytes; // Number of bytes in the input buffer
	char *outBuffer; // Pointer to output data buffer
	int outPosition; // Position in the output buffer
	int maxOut; // Maximum number of bytes in the output buffer
};

/**
* Function loads data from the input buffer. Used by Pklib's "implode"
* and "explode" function as user-defined callback.
* @return Returns number of bytes loaded.
* @author StormLib
* @param buf Pointer to a buffer where to store loaded data.
* @param size Max. number of bytes to read.
* @param param Custom pointer, parameter of implode/explode.
*/
static unsigned int readBuffer(char *buf, unsigned int *size, void *param)
{
	struct DataInfo *info = reinterpret_cast<struct DataInfo*>(param);
	unsigned int maxAvailable = (info->inBytes - info->inPosition);
	unsigned int toRead = *size;

	// Check the case when not enough data available
	if(toRead > maxAvailable)
		toRead = maxAvailable;

	// Load data and increment offsets
	memcpy(buf, info->inBuffer + info->inPosition, toRead);
	info->inPosition += toRead;

	 return toRead;
}

/**
* Function for store output data. Used by Pklib's "implode" and "explode"
* as user-defined callback.
* @author StormLib
* @param buf Pointer to data to be written.
* @param size Number of bytes to write.
* @param param Custom pointer, parameter of implode/explode
*/
static void writeBuffer(char *buf, unsigned int *size, void *param)
{
	struct DataInfo *info = reinterpret_cast<struct DataInfo*>(param);
	unsigned int maxWrite = (info->maxOut - info->outPosition);
	unsigned int toWrite = *size;

	// Check the case when not enough space in the output buffer
	if(toWrite > maxWrite)
		toWrite = maxWrite;

	// Write output data and increments offsets
	memcpy(info->outBuffer + info->outPosition, buf, toWrite);
	info->outPosition += toWrite;
}

void compressPklib(char *&outBuffer, int &outLength, char* const &inBuffer, int inLength, int &compressionType, int /* compressionLevel */)  throw (class Exception)
{
	struct DataInfo info; // Data information
	// Fill data information structure
	info.inBuffer  = inBuffer;
	info.inPosition = 0;
	info.inBytes = inLength;
	info.outBuffer = outBuffer;
	info.outPosition  = 0;
	info.maxOut  = outLength;
	char *workBuffer = new char[CMP_BUFFER_SIZE]; // Pklib's work buffer
	unsigned int dictonarySize; // Dictionary size
	unsigned int ctype; // Compression type

	// Set the compression type and dictionary size
	ctype = (compressionType == 2) ? CMP_ASCII : CMP_BINARY;

	if (inLength < 0x600)
		dictonarySize = 0x400;
	else if(0x600 <= inLength && inLength < 0xC00)
		dictonarySize = 0x800;
	else
		dictonarySize = 0x1000;

	// Do the compression
	unsigned int state = implode(readBuffer, writeBuffer, workBuffer, &info, &ctype, &dictonarySize);
	outLength = info.outPosition;
	delete[] workBuffer;

	if (state != CMP_NO_ERROR)
		throw Exception(boost::str(boost::format(_("Implode error %1%.")) % state));
}

void decompressPklib(char *&outBuffer, int &outLength, char* const &inBuffer, int inLength) throw (class Exception)
{
	struct DataInfo info; // Data information
	// Fill data information structure
	info.inBuffer = inBuffer;
	info.inPosition = 0;
	info.inBytes = inLength;
	info.outBuffer = outBuffer;
	info.outPosition = 0;
	info.maxOut = outLength;
	char *workBuffer = new char[EXP_BUFFER_SIZE]; // Pklib's work buffer

	// Do the decompression
	unsigned int state = explode(readBuffer, writeBuffer, workBuffer, &info);

	// Fix: If PKLIB is unable to decompress the data, they are uncompressed
	if(info.outPosition == 0)
	{
		info.outPosition = std::min<int>(outLength, inLength);
		memcpy(outBuffer, inBuffer, info.outPosition);
	}

	outLength = info.outPosition;
	delete[] workBuffer;

	if (state != CMP_NO_ERROR)
		throw Exception(boost::str(boost::format(_("Explode error %1%.")) % state));
}

int compressWaveMono(short* const &inBuffer, int inBufferLength, unsigned char *&outBuffer, int &outBufferLength, int compressionLevel) throw (class Exception)
{
	// Prepare the compression level for the next compression
	// (After us, the Huffmann compression will be called)
	if (0 < compressionLevel && compressionLevel <= 2)
		compressionLevel = 4;
	else if (compressionLevel == 3)
		compressionLevel = 6;
	else
		compressionLevel = 5;

	if (outBufferLength > 0)
		delete[] outBuffer;

	outBufferLength = inBufferLength;
	outBuffer = new unsigned char[inBufferLength];
	CompressWave(outBuffer, outBufferLength, inBuffer, inBufferLength, 1, compressionLevel);

	return outBufferLength;
}

int decompressWaveMono(unsigned char* const &inBuffer, int inBufferLength, unsigned char *&outBuffer, int &outBufferLength) throw (class Exception)
{
	if (outBufferLength > 0)
		delete[] outBuffer;

	outBufferLength = inBufferLength;
	outBuffer = new unsigned char[inBufferLength];
	DecompressWave(outBuffer, outBufferLength, inBuffer, inBufferLength, 1);

	return outBufferLength;
}

int compressWaveStereo(short* const &inBuffer, int inBufferLength, unsigned char *&outBuffer, int &outBufferLength, int compressionLevel) throw (class Exception)
{
	// Prepare the compression type for the next compression
	// (After us, the Huffmann compression will be called)
	if(0 < compressionLevel && compressionLevel <= 2)
		compressionLevel = 4;
	else if(compressionLevel == 3)
		compressionLevel = 6;
	else
		compressionLevel = 5;

	if (outBufferLength > 0)
		delete[] outBuffer;

	outBufferLength = inBufferLength;
	outBuffer = new unsigned char[inBufferLength];
	CompressWave(outBuffer, outBufferLength, inBuffer, inBufferLength, 2, compressionLevel);

	return outBufferLength;
}

int decompressWaveStereo(unsigned char* const &inBuffer, int inBufferLength, unsigned char *&outBuffer, int &outBufferLength) throw (class Exception)
{
	if (outBufferLength > 0)
		delete[] outBuffer;

	outBufferLength = inBufferLength;
	outBuffer = new unsigned char[inBufferLength];
	DecompressWave(outBuffer, outBufferLength, inBuffer, inBufferLength, 2);

	return outBufferLength;
}

std::streamsize deflateStream(istream &istream, ostream &ostream) throw (class Exception)
{
	throw Exception(_("deflateStream: ZLib compression is not supported yet!"));

	return 0;
}

std::streamsize inflateStream(istream &istream, ostream &ostream) throw (class Exception)
{
	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;
	int state = inflateInit(&stream);
	std::streamsize streamsize = 0;

	if (state != Z_OK)
		throw Exception(boost::str(boost::format(_("Sector: ZLib error %1%.")) % zError(state)));

	static const std::size_t bufferSize = 16384;

	do
	{
		byte inputBuffer[bufferSize];
		istream.read(inputBuffer, bufferSize);
		stream.avail_in = istream.gcount();

		if (!istream)
		{
			state = inflateEnd(&stream);

			throw Exception(boost::str(boost::format(_("Sector: ZLib stream error %1%.")) % zError(state)));
		}

		if (stream.avail_in == 0)
			break;

		stream.next_in = reinterpret_cast<Bytef*>(inputBuffer);

		/* run inflate() on input until output buffer not full */
		do
		{
			byte outputBuffer[bufferSize];
			stream.avail_out = bufferSize;
			stream.next_out = reinterpret_cast<Bytef*>(outputBuffer);

			state = inflate(&stream, Z_NO_FLUSH);

			if (state == Z_STREAM_ERROR)
				throw Exception(boost::str(boost::format(_("Sector: ZLib stream error %1%.")) % zError(state))); // state not clobbered

			switch (state)
			{
			    case Z_NEED_DICT:
				state = Z_DATA_ERROR; // and fall through
			    case Z_DATA_ERROR:
			    case Z_MEM_ERROR:

				inflateEnd(&stream);

				throw Exception(boost::str(boost::format(_("Sector: ZLib stream error %1%.")) % zError(Z_MEM_ERROR)));
			}

			unsigned int have = bufferSize - stream.avail_out;
			ostream.write(outputBuffer, have);

			if (!ostream)
			{
				inflateEnd(&stream);

				throw Exception(boost::str(boost::format(_("Sector: ZLib stream error %1%.")) % zError(Z_ERRNO)));
			}

			streamsize += have;
		}
		while (stream.avail_out == 0);
	}
	while (state != Z_STREAM_END);

	state = inflateEnd(&stream);

	if (state != Z_OK)
		throw Exception(boost::str(boost::format(_("Sector: ZLib error %1%.")) % zError(state)));

	return streamsize;
}

}

}
