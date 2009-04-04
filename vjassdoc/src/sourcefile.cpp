/***************************************************************************
 *   Copyright (C) 2008, 2009 by Tamino Dauth                              *
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

#include "objects.h"
#include "internationalisation.h"

namespace vjassdoc
{

SourceFile::SourceFile(const std::string &identifier, const std::string &path) : m_path(path), Object(identifier, 0, 0, 0)
{
}

SourceFile::SourceFile(std::vector<const unsigned char*> &columnVector) : Object(columnVector)
{
}

void SourceFile::init()
{
}

void SourceFile::pageNavigation(std::ofstream &file) const
{
	file
	<< "\t\t\t<li><a href=\"#Full path\">" << _("Full path") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Code\">" << _("Code") << "</a></li>\n"
	;
}

void SourceFile::page(std::ofstream &file) const
{
	file
	<< "\t\t<h2><a name=\"Full path\">" << _("Full path") << "</a></h2>\n"
	<< "\t\t" << this->path() << '\n'
	<< "\t\t<h2><a name=\"Code\">" << _("Code") << "</a></h2>\n"
	<< "\t\t<p>\n"
	;
	
	std::ifstream sourceFile(this->path().c_str());
	int i = 1;
	
	while (sourceFile.good())
	{
		std::string line;
		std::getline(sourceFile, line);
		file << "\t\t\t" << i << " - " << "<a name=\"" << i << "\" class=\"sourcefilecode\">" << line << "</a><br>\n";
		++i;
	}

	sourceFile.close();
	file
	<< "\t\t</p>\n"
	;

}

std::string SourceFile::sqlStatement() const
{
	std::ostringstream sstream;
	sstream
	<< "Identifier=\"" << Object::sqlFilteredString(this->identifier()) << "\", "
	<< "Path=\"" << Object::sqlFilteredString(this->path()) << '\"';

	return sstream.str();
}

}
