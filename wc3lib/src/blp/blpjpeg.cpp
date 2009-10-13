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

#include <iostream> /// @todo debug

#include "blpjpeg.hpp"
#include "blp.hpp"

namespace wc3lib
{

namespace blp
{
	
/**
* @author PitzerMike
*/
int BlpJpeg::requiredMipMaps(int width, int height)
{
	int mips = 0;
	
	while (width > 0 && height > 0)
	{
		++mips;
		width = width / 2;
		height = height / 2;
	}

	return mips;
}
	
BlpJpeg::BlpJpeg(class Blp *blp) : m_blp(blp)
{
}

BlpJpeg::~BlpJpeg()
{
	if (this->m_jpegHeader != 0)
		delete this->m_jpegHeader;
}

dword BlpJpeg::read(std::fstream &fstream) throw (class Exception)
{
	dword bytes = 0;
	fstream.read(reinterpret_cast<char*>(&this->m_jpegHeaderSize), sizeof(dword));
	bytes += fstream.gcount();
	std::cout << "JPEG header size is " << this->m_jpegHeaderSize << std::endl;
	this->m_jpegHeader = new byte[this->m_jpegHeaderSize];

	for (int i = 0; i < this->m_jpegHeaderSize; ++i)
	{
		std::cout << "Reading header data " << i << std::endl;
		fstream.read(reinterpret_cast<char*>(&this->m_jpegHeader[i]), sizeof(byte));
		bytes += fstream.gcount();
	}

	int maxMipMaps = BlpJpeg::requiredMipMaps(this->blp()->width(), this->blp()->height());
	std::cout << "Max mip maps " << maxMipMaps << std::endl;

	for (int i = 0; i < 16; ++i)
	{
		std::cout << "Reading mipmap data " << i << std::endl;
		if (bytes != this->blp()->header()->m_mipMapOffset[i])
		{
			std::cout << "Reading unnecessary offset" << std::endl;
			char *tmp = new char[this->blp()->header()->m_mipMapOffset[i]];
			fstream.read(tmp, sizeof(tmp));
			bytes += fstream.gcount();
			delete tmp;
		}
		
		this->m_mipMaps[i].m_jpegData = new byte[maxMipMaps];
		
		for (int j = 0; j < maxMipMaps; ++j)
		{
			fstream.read(reinterpret_cast<char*>(&this-> m_mipMaps[i].m_jpegData[j]), sizeof(byte)); /// @todo Reading 156 bytes instead of one.
			bytes += fstream.gcount();
			std::cout << "Reading mip map jpeg data " << fstream.gcount() << std::endl;
		}
	}
	
	return bytes;
}

dword BlpJpeg::write(std::fstream &fstream) throw (class Exception)
{
	dword bytes = 0;
	fstream.write(reinterpret_cast<const char*>(&this->m_jpegHeaderSize), sizeof(dword));
	bytes += sizeof(dword);

	for (int i = 0; i < this->m_jpegHeaderSize; ++i)
	{
		std::cout << "Writing header data " << i << std::endl;
		fstream.write(reinterpret_cast<const char*>(&this->m_jpegHeader[i]), sizeof(byte));
		bytes += sizeof(byte);
	}

	int maxMipMaps = BlpJpeg::requiredMipMaps(this->blp()->width(), this->blp()->height());
	
	for (int i = 0; i < 16; ++i)
	{
		std::cout << "Writing mip map data " << i << std::endl;
		
		for (int j = 0; j < maxMipMaps; ++j)
		{
			fstream.write(reinterpret_cast<const char*>(&this->m_mipMaps[i].m_jpegData[j]), sizeof(byte));
			bytes += sizeof(byte);
		}
	}
	
	return bytes;
}

}

}
