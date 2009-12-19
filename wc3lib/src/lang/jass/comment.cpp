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

#include "objects.hpp"
#include "internationalisation.hpp"

namespace vjassdoc
{
	
#ifdef HTML
std::string Comment::List::htmlCategoryName() const
{
	return _("Comments");
}

std::string Comment::List::htmlFolderName() const
{
	return _("comments");
}
#endif

#ifdef SQLITE
std::string Comment::List::sqlTableName() const
{
	return _("Comments");
}

std::size_t Comment::List::sqlColumns() const
{
	return Object::List::sqlColumns();
}

std::string Comment::List::sqlColumnDataType(std::size_t column) const throw (std::exception)
{
	return Object::List::sqlColumnDataType(column);
}

std::string Comment::List::sqlColumnName(std::size_t column) const throw (std::exception)
{
	return Object::List::sqlColumnName(column);
}
#endif

Comment::Comment(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment) : Object(identifier, sourceFile, line, docComment)
{
}

#ifdef SQLITE
Comment::Comment(std::vector<Object::SqlValueDataType> &columnVector) : Object(columnVector)
{
}
#endif

Comment::~Comment()
{
}

void Comment::init()
{
}

void Comment::writeHtmlPageNavigation(std::ostream &ostream) const
{
	ostream
	<< "\t\t\t<li><a href=\"#Text\">"		<< _("Text") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Description\">"	<< _("Description") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Source File\">"	<< _("Source File") << "</a></li>\n"
	;
}

void Comment::writeHtmlPageContent(std::ostream &ostream) const
{
	ostream
	<< "\t\t<h2><a name=\"Text\">" << _("Text") << "</a></h2>\n"
	<< "\t\t<p>\n"
	<< "\t\t" << this->identifier() << "\n"
	<< "\t\t</p>\n"
	<< "\t\t<h2><a name=\"Description\">" << _("Description") << "</a></h2>\n"
	<< "\t\t<p>\n"
	<< "\t\t" << Object::objectPageLink(this->docComment()) << "\n"
	<< "\t\t</p>\n"
	<< "\t\t<h2><a name=\"Source File\">" << _("Source File") << "</a></h2>\n"
	<< "\t\t" << SourceFile::sourceFileLineLink(this) << '\n'
	;
}

}
