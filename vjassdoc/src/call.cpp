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

#include "objects.hpp"
#include "internationalisation.hpp"

namespace vjassdoc
{

#ifdef SQLITE
const char *Call::sqlTableName = "Calls";
unsigned int Call::sqlColumns;
std::string Call::sqlColumnStatement;

void Call::initClass()
{
	Call::sqlColumns = Object::sqlColumns + 1;
	Call::sqlColumnStatement = Object::sqlColumnStatement +
	",Function INT";
}
#endif

Call::Call(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, class Function *function) : Object(identifier, sourceFile, line, docComment), m_function(function)
{
}

#ifdef SQLITE
Call::Call(std::vector<const unsigned char*> &columnVector) : Object(columnVector)
{
	this->prepareVector();
}
#endif

Call::~Call()
{
}

void Call::init()
{
}

void Call::pageNavigation(std::ofstream &file) const
{
	file
	<< "\t\t\t<li><a href=\"#Description\">"	<< _("Description") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Source File\">"	<< _("Source File") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Function\">"		<< _("Function") << "</a></li>\n"
	;
}

void Comment::page(std::ofstream &file) const
{
	file
	<< "\t\t<h2><a name=\"Description\">" << _("Description") << "</a></h2>\n"
	<< "\t\t<p>\n"
	<< "\t\t" << Object::objectPageLink(this->docComment()) << "\n"
	<< "\t\t</p>\n"
	<< "\t\t<h2><a name=\"Source File\">" << _("Source File") << "</a></h2>\n"
	<< "\t\t" << SourceFile::sourceFileLineLink(this) << '\n'
	<< "\t\t<h2><a name=\"Function\">" << _("Function") << "</a></h2>\n"
	<< "\t\t" << SourceFile::objectLink(this->function()) << '\n'
	;
}

}
