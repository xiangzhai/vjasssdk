/***************************************************************************
 *   Copyright (C) 2008 by Tamino Dauth                                    *
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

#include <boost/foreach.hpp>

#include "object.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{
	
namespace lang
{

Object::List::List(class Language *language) : m_language(language)
{
}

Object::List::~List()
{
	BOOST_FOREACH(class Object *object, *this)
			delete object;
}

Object::List& Object::List::operator<<(class Object *object)
{
	this->push_back(object);
	
	return *this;
}

Object::List& Object::List::operator>>(class Object* &object)
{
	object = this->back();
	this->pop_back();
	
	return *this;
}

#ifdef HTML
void Object::List::writeHtmlList(std::ostream &ostream)
{
	this->htmlCategoryName
	ostream
	<< "\t\t<h2><a name=\"" << this->htmlCategoryName() << "\">" << boost::str(boost::format(_("%1% (%2%)")) % this->htmlCategoryName() % this->m_objects.size()) << "</a></h2>" << std::endl
	<< "\t\t<ul>" << std::endl
	;
	
	
	BOOST_FOREACH(class Object *object, this->m_objects)
		ostream << "\t\t<li>" << object->htmlPageLink() << "</li>" << std::endl;
	
	ostream << "\t\t</ul>" << std::endl;
}

void Object::List::writeHtmlPages() throw (std::exception)
{
	BOOST_FOREACH(class Object *object, this->m_objects)
	{
		std::ofstream ofstream(object->htmlPagePath().string());
		
		if (!ofstream)
			throw std::exception();
		
		object->writeHtmlPageNavigation(ofstream);
		object->writeHtmlPageContent(ofstream);
	}
}
#endif
#ifdef SQLITE
void Object::List::writeSqlTable(sqlite3 *database)
{
	//return "CREATE TABLE " + tableName + "(Id INT PRIMARY KEY," + entries + ')';
}

std::string Object::List::sqlTableName() const
{
	return "";
}

std::size_t Object::List::sqlColumns() const
{
	return 4;
}

std::string Object::List::sqlColumnDataType(std::size_t column) const throw (std::exception)
{
	switch (column)
	{
		case 0:
			return "VARCHAR(255)";
			
		case 1:
			return "INT";
			
		case 2:
			return "INT";
			
		case 3:
			return "INT";
	}
	
	throw std::exception();
}

std::string Object::List::sqlColumnName(std::size_t column) const throw (std::exception)
{
	switch (column)
	{
		case 0:
			return "Identifier";
			
		case 1:
			return "SourceFile";
			
		case 2:
			return "Line";
	}
	
	throw std::exception();
}
#endif

Object::Object(class Object::List *list, const std::string &identifier, class SourceFile *sourceFile, std::size_t line) : m_list(list), m_identifier(identifier), m_sourceFile(sourceFile), m_line(line)
{
	list->push_back(this);
}

#ifdef SQLITE
Object::Object(std::vector<Object::VectorDataType> &columnVector) : m_sourceFile(0)
{
}
#endif

Object::~Object()
{
}

}

}
