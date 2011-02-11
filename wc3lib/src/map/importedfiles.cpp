/***************************************************************************
 *   Copyright (C) 2011 by Tamino Dauth                                    *
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

#include <iostream>

#include <boost/foreach.hpp>

#include "importedfiles.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace map
{

const string ImportedFiles::mapPrefix = "war3mapImported";
const string ImportedFiles::campaignPrefix = "war3campaignImported";

ImportedFiles::Path::Path()
{
}

std::streamsize ImportedFiles::Path::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = 0;
	byte hasPrefix;
	wc3lib::read(istream, hasPrefix, size);

	if (hasPrefix == 5 || hasPrefix == 8)
		this->m_hasPrefix = true;
	else if (hasPrefix == 10 || hasPrefix == 13)
		this->m_hasPrefix = false;
	else
	{
		this->m_hasPrefix = false;

		std::cerr << boost::format(_("Imported files: Invalid path byte %1%.")) % hasPrefix << std::endl;
	}

	wc3lib::readString(istream, this->m_path, size);

	return size;
}

std::streamsize ImportedFiles::Path::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	std::streamsize size = 0;

	if (this->hasPrefix())
		wc3lib::write<byte>(ostream, 5, size);
	else
		wc3lib::write<byte>(ostream, 10, size);

	wc3lib::writeString(ostream, this->m_path, size);

	return size;
}

ImportedFiles::ImportedFiles()
{
}

ImportedFiles::~ImportedFiles()
{
	BOOST_FOREACH(class Path *path, this->m_paths)
		delete path;
}

std::streamsize ImportedFiles::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read(istream, this->m_version, size);

	if (this->version() != latestFileVersion())
		throw Exception(boost::format("Imported files: Unsupported version %1%, expected %2%") % this->version() % latestFileVersion());

	int32 number;
	wc3lib::read(istream, number, size);

	for ( ; number > 0; --number)
	{
		Path *path = new Path();
		size += path->read(istream);
		this->m_paths.push_back(path);
	}

	return size;
}

std::streamsize ImportedFiles::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	if (this->version() != latestFileVersion())
		throw Exception(boost::format("Imported files: Unsupported version %1%, expected %2%") % this->version() % latestFileVersion());

	std::streamsize size = 0;
	wc3lib::write(ostream, this->m_version, size);
	wc3lib::write<int32>(ostream, this->paths().size(), size);

	BOOST_FOREACH(const class Path *path, this->paths())
		size += path->write(ostream);

	return size;
}

}

}
