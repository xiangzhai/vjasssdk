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

#include "doccomment.hpp"
#include "internationalisation.hpp"
#include "utilities.hpp"

namespace wc3lib
{
	
namespace lang
{
	
DocComment::List::List(class Language *language) : Object::List(language)
{
}

DocComment::List::~List()
{
}

#ifdef HTML
const std::string& DocComment::List::htmlCategoryName() const
{
	return _("Documentation comments");
}

const std::string& DocComment::List::htmlFolderName() const 
{
	return "doccomments";
}
#endif
#ifdef SQLITE
const std::string& DocComment::List::sqlTableName() const
{
}

std::size_t DocComment::List::sqlColumns() const
{
}

const std::string& DocComment::List::sqlColumnDataType(std::size_t column) const throw (class Exception)
{
}

const std::string& DocComment::List::sqlColumnName(std::size_t column) const throw (class Exception)
{
}
#endif

#ifdef SQLITE
const std::size_t DocComment::sqlMaxAuthors = 10;
const std::size_t DocComment::sqlMaxSeeObjects = 10;
#endif

DocComment::DocComment(class Object::List *list, class SourceFile *sourceFile, std::size_t line) : Object(list, "", sourceFile, line), m_object(0)
{
}

#ifdef SQLITE		
DocComment::DocComment(std::vector<Object::SqlValueDataType> &columnVector) : Object(columnVector)
{
}
#endif

DocComment::~DocComment()
{
}

void DocComment::init()
{
}

#ifdef SQLITE
const std::string& DocComment::sqlValue(std::size_t column) const
{
}
#endif

#ifdef HTML
void DocComment::writeHtmlPageNavigation(std::ostream &ostream) const
{
}

void DocComment::writeHtmlPageContent(std::ostream &ostream) const
{
}
#endif

#ifdef SQLITE
const std::string& DocComment::sqlStatement() const
{
}
#endif

#ifdef HTML
/// @todo Generate hash value?
const std::string& DocComment::htmlPageName() const
{
	return "";
}
#endif

}

}
