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

#include <cstring>

#include <boost/format.hpp>

#include "pathmap.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{
	
namespace map
{

Pathmap::identifier[4] = { 'M', 'P', '3', 'W' };

static struct Header
{
	char8 fileId[4]; //[4]: file ID = 'MP3W'
	int32 fileVersion; //: file version = 0
	int32 width; //: path map width (=map_width*4)
	int32 height; //: path map height (=map_height*4)
};

Pathmap::Pathmap(class W3m *w3m) : m_w3m(w3m), m_fileVersion(0), m_width(0), m_height(0), m_data(0)
{
}

Pathmap::~Pathmap()
{
	if (this->m_data != 0)
		delete[] this->m_data;
}

std::streamsize Pathmap::read(std::istream &istream) throw (class Exception)
{
	struct Header header;
	istream.read(reinterpret_cast<char*>(&header), sizeof(header));
	std::streamsize bytes = istream.gcount();
	
	if (memcmp(header.fileId, Pathmap::identifier, sizeof(header.fileId)) != 0)
		throw Exception(boost::format(_("Pathmap: Unknown file id \"%1%\". Expected \"%2%\".")) % header.fileId % Pathmap::identifier);
	
	this->m_fileVersion = header.fileVersion;
	this->m_width = header.width;
	this->m_height = header.height;
	
	if (this->m_data != 0)
		delete[] this->m_data;
	
	this->m_data = new Pathmap::Data[header.width][header.height];
	istream.read(reinterpret_cast<char*>(&this->m_data), header.width * header.height);
	bytes += istream.gcount();
	
	return bytes;
}

std::streamsize Pathmap::write(std::istream &istream) throw (class Exception)
{
	return 0;
}

}

}
