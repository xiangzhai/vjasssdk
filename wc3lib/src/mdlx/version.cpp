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

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#include <boost/format.hpp>
#include <boost/tokenizer.hpp>

#include "version.hpp"
#include "../internationalisation.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

const long32 Version::currentVersion = 800;

Version::Version(class Mdlx *mdlx) : MdxBlock("VERS"), m_mdlx(mdlx)
{
}

Version::~Version()
{
}

void Version::readMdl(std::istream &istream) throw (class Exception)
{
	std::string line;
	bool gotVersion = false;

	while (std::getline(istream, line))
	{
		if (line.empty())
			continue;
		
		boost::tokenizer<> tokenizer(line);
		boost::tokenizer<>::iterator iterator = tokenizer.begin();
		
		if (iterator == tokenizer.end())
			throw Exception(_("Version: Missing tokens."));

		if ((*iterator) == "//" || ((*iterator).length() > 2 && (*iterator).substr(0, 2) == "//"))
			continue;
		else if ((*iterator) == "Version")
		{
			std::cout << "Is version " << *iterator << std::endl;
			std::cout << "Line " << line << std::endl;
			++iterator; /// @todo Is not {?!

			if (iterator == tokenizer.end())
				throw Exception(_("Version: Missing tokens."));
			else if ((*iterator) != "{")
				throw Exception(_("Version: Syntax error."));
		}
		else if ((*iterator) == "FormatVersion")
		{
			++iterator;

			if (iterator == tokenizer.end())
				throw Exception(_("Version: Missing tokens."));

			std::stringstream sstream;
			sstream << *iterator;
			sstream >> this->m_version;
			gotVersion = true;
		}
		else if ((*iterator) == "}")
			break;
	}
	
	if (!gotVersion)
		throw Exception(_("Version: Missing format version number."));
}

void Version::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	ostream <<
	"// Current FormatVersion is 800\n"
	"Version {\n"
	"\tFormatVersion " << this->m_version << ",\n"
	"}\n"
	;
}

std::streamsize Version::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	long32 nbytes;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();
	
	if (nbytes != sizeof(this->m_version))
		throw Exception(boost::format(_("Versions with more than %1% bytes are not supported. Read version has %1% bytes.")) % sizeof(this->m_version) % nbytes);
	
	istream.read(reinterpret_cast<char*>(&this->m_version), sizeof(this->m_version));
	bytes += istream.gcount();
	
	if (this->m_version != Version::currentVersion)
		std::cerr << boost::format(_("Warning: Version %1% is probably not supported. Current version is %2$.")) % this->m_version % Version::currentVersion << std::endl;
	
	return bytes;
}

std::streamsize Version::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	long32 nbytes = sizeof(this->m_version);
	ostream.write(reinterpret_cast<const char*>(&nbytes), sizeof(nbytes));
	bytes += sizeof(nbytes);
	ostream.write(reinterpret_cast<const char*>(&this->m_version), sizeof(this->m_version));
	bytes += sizeof(this->m_version);
	
	return bytes;
}

}

}
