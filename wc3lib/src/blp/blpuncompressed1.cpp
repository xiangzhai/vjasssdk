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

#include "blpuncompressed1.hpp"
#include "blp.hpp"

namespace wc3lib
{

namespace blp
{

BlpUncompressed1::BlpUncompressed1(class Blp *blp) : m_blp(blp)
{
	for (int i = 0; i < 16; ++i)
	{
		this->m_mipMaps[i].m_indexList = new byte[blp.width() * blp.height()];
		this->m_mipMaps[i].m_alphaList = new byte[blp.width() * blp.height()];
	}
}

BlpUncompressed1::~BlpUncompressed1()
{
	for (int i = 0; i < 16; ++i)
	{
		delete[] this->m_mipMaps[i].m_indexList;
		delete[] this->m_mipMaps[i].m_alphaList;
	}
}

dword BlpUncompressed1::read(std::istream &istream) throw (class Exception)
{
	dword bytes = 0;
	
	for (int i = 0; i < 256; ++i)
	{
		istream.read(reinterpret_cast<char*>(&this->m_palette[i]), sizeof(color));
		bytes += istream.gcount();
	}

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < this->blp()->header()->m_width; ++j)
		{
			for (int k = 0; k < this->blp()->header()->m_height; ++k)
			{
				istream.read(reinterpret_cast<char*>(&this->m_mipMaps[i].m_indexList[j * k]), sizeof(byte));
				bytes += istream.gcount();
			}
		}
	}

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < this->blp()->header()->m_width; ++j)
		{
			for (int k = 0; k < this->blp()->header()->m_height; ++k)
			{
				istream.read(reinterpret_cast<char*>(&this->m_mipMaps[i].m_alphaList[j * k]), sizeof(byte));
				bytes += istream.gcount();
			}
		}
	}
	
	return bytes;
}

dword BlpUncompressed1::write(std::ostream &ostream) throw (class Exception)
{
	dword bytes = 0;
	
	for (int i = 0; i < 256; ++i)
	{
		ostream.write(reinterpret_cast<const char*>(&this->m_palette[i]), sizeof(color));
		bytes += sizeof(color);
	}

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < this->blp()->header()->m_width; ++j)
		{
			for (int k = 0; k < this->blp()->header()->m_height; ++k)
			{
				ostream.write(reinterpret_cast<const char*>(&this->m_mipMaps[i].m_indexList[j * k]), sizeof(byte));
				bytes += sizeof(byte);
			}
		}
	}

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < this->blp()->header()->m_width; ++j)
		{
			for (int k = 0; k < this->blp()->header()->m_height; ++k)
			{
				ostream.write(reinterpret_cast<const char*>(&this->m_mipMaps[i].m_alphaList[j * k]), sizeof(byte));
				bytes += sizeof(byte);
			}
		}
	}
	
	return bytes;
}

}

}