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

#include <sstream>
#include <cstdio>
#include <iostream> //debug

#include <boost/tokenizer.hpp>

#include "version.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Version::Version(class Mdlx *mdlx) : MdxBlock("VERS"), m_mdlx(mdlx)
{
}

Version::~Version()
{
}

void Version::readMdl(std::fstream &fstream) throw (class Exception)
{
	std::string line;

	while (std::getline(fstream, line))
	{
		boost::tokenizer<> tokenizer(line);
		boost::tokenizer<>::iterator iterator = tokenizer.begin();
		
		if (iterator == tokenizer.end())
			throw Exception("Version: Missing tokens.");

		if ((*iterator) == "//")
			continue;
		else if ((*iterator) == "Version")
		{
			++iterator; /// @todo Is not {?!

			if (iterator == tokenizer.end())
				throw Exception("Version: Missing tokens.");
			else if ((*iterator) != "{")
				throw Exception("Version: Syntax error.");
		}
		else if ((*iterator) == "FormatVersion")
		{
			++iterator;

			if (iterator == tokenizer.end())
				throw Exception("Version: Missing tokens.");

			std::stringstream sstream;
			sstream << *iterator;
			sstream >> this->m_version;
		}
		else if ((*iterator) == "}")
			break;
	}
}

void Version::writeMdl(std::fstream &fstream) throw (class Exception)
{
	fstream <<
	"// Current FormatVersion is 800\n"
	"Version {\n"
	"\tFormatVersion " << this->m_version << ",\n"
	"}\n"
	;
}

void Version::readMdx(std::fstream &fstream) throw (class Exception)
{
	MdxBlock::readMdx(fstream);
	long32 bytes;
	fstream >> bytes;
	//fstream.read((char*)bytes, 4);
	std::cout << "Bytes " << bytes << std::endl;
	fstream >> this->m_version;
	std::cout << "Version " << this->m_version << std::endl;
	/*
	bytes -= this->m_version;
	std::cout << "New bytes " << bytes << std::endl;
	std::cout << "Bytes " << bytes << " Position 0 " << fstream.tellg() << std::endl;
	fstream.seekg(bytes, std::ios_base::cur); // seek byte rest
	std::streampos newPosition = fstream.rdbuf()->pubseekoff(bytes, std::ios_base::cur, std::ios_base::in);
	std::cout << "Position 1 " << newPosition << std::endl;
	*/
	
	if (this->m_version != Version::currentVersion)
		fprintf(stdout, _("Warning: Version %d probably is not supported. Current version is %d.\n"), this->m_version, Version::currentVersion);
}

void Version::writeMdx(std::fstream &fstream) throw (class Exception)
{
	MdxBlock::writeMdx(fstream);
}

}

}
