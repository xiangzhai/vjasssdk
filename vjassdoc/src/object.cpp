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
#include <cstdlib>
#include <cctype>

#include <boost/foreach.hpp>

//#include "objects.hpp"
#include "internationalisation.hpp"

namespace vjassdoc
{

Object::List::List()
{
}

Object::List::~List()
{
	BOOST_FOREACH(class Object *object, this->m_objects)
			delete object;
}

List& Object::List::operator<<(class Object *object)
{
	this->push_back(object);
	
	return *this;
}

List& Object::List::operator>>(class Object* &object)
{
	object = this->m_back();
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
			
		case 3:
			return "DocComment";
	}
	
	throw std::exception();
}
#endif

Object::Object(const std::string &identifier, class SourceFile *sourceFile, std::size_t line, class DocComment *docComment) : m_container(0), m_scope(0), m_library(0), m_identifier(identifier), m_sourceFile(sourceFile), m_line(line), m_docComment(docComment)
{
	if (docComment != 0)
		docComment->setObject(this);
}

#ifdef SQLITE
Object::Object(std::vector<Object::VectorDataType> &columnVector) :  m_container(0), m_scope(0), m_library(0), m_sourceFile(0), m_docComment(0)
{
}
#endif

Object::~Object()
{
}

bool Object::hasToSearchValueObject(class Object *type, const std::string &expression)
{
	if (type == 0)
		return false;
	
	if (dynamic_cast<class Type*>(type) != 0)
	{
		do
		{
			if (type == Vjassdoc::parser()->integerType())
			{
				if (isdigit(expression[0]) || expression[0] == '-' || expression[0] == '+')
					return false;
				
				return true;
			}
			else if (type == Vjassdoc::parser()->realType())
			{
				if (isdigit(expression[0]) || expression[0] == '-' || expression[0] == '+' || expression[0] == '.')
					return false;
				
				return true;
			}
			else if (type == Vjassdoc::parser()->stringType())
			{
				if (expression[0] == '\"' || expression == "null")
					return false;
				
				return true;
			}
			else if (type == Vjassdoc::parser()->booleanType())
			{
				if (expression == "true" || expression == "false")
					return false;
				
				return true;
			}
			else if (type == Vjassdoc::parser()->handleType() || type == Vjassdoc::parser()->codeType()) /// @todo Code type == null?!
			{
				if (expression == "null")
					return false;
				
				return true;
			}
			
			type = static_cast<class Type*>(type)->type();
		}
		while (type != 0);
	}
	else if (dynamic_cast<class FunctionInterface*>(type) != 0 || dynamic_cast<class Interface*>(type) != 0 || dynamic_cast<class Struct*>(type) != 0)
	{
		if (isdigit(expression[0]) || expression[0] == '-' || expression[0] == '+')
			return false;
		
		return true;
	}

	return true;
}

class Object* Object::findValue(class Object *type, std::string &valueExpression)
{
	class Object *value = 0;

	if (!valueExpression.empty())
	{
		//std::cout << "Before has to search." << std::endl;
		
		//FIXME The value can be a function or method call with literal arguments.
		if (Object::hasToSearchValueObject(type, valueExpression))
		{
			//std::cout << "Has to search!" << std::endl;
			//FIXME Detect . separators correctly.
			class Object *valueContainer = 0;
			std::string::size_type separatorPosition = valueExpression.find('.');
			
			if (separatorPosition != std::string::npos)
			{
				//std::cout << "Found . in value expression at position " << separatorPosition << std::endl;
				std::string containerIdentifier = valueExpression.substr(0, separatorPosition);
				//std::cout << "Container identifier: " << containerIdentifier << std::endl;
				
				if (containerIdentifier == File::expressionText[File::ThistypeExpression])
				{
					valueContainer = this->container();
					//std::cout << "thistype" << std::endl;
				}
				else if (containerIdentifier == File::expressionText[File::SuperExpression])
				{
					//FIXME Error? Test it!
					//valueContainer = static_cast<class Struct*>(this->container())->extension();
					std::cout << "super" << std::endl;
				}
				
				//FIXME, does not work
				valueExpression.erase(separatorPosition);
				//std::cout << "New value expression " << valueExpression << std::endl;
				//std::cout << "New position " << separatorPosition << std::endl;
			}
			
			//FIXME Detect _ separators correctly?!
			
			bool functionCall = false;
			std::string::size_type position = valueExpression.find('(');
			
			if (position != std::string::npos)
				functionCall = true;
			else
				position = valueExpression.find('[');
			
			std::string newExpression;
			
			if (functionCall || position != std::string::npos)
			{
				newExpression = valueExpression.substr(0, position);
				
				if (!functionCall)
				{
					valueExpression = valueExpression.substr(position + 1);
					valueExpression =  valueExpression.substr(0, valueExpression.length() - 1); // FIXME Do not use two function calls.
				}
				else
					valueExpression = valueExpression.substr(position);
			}
			else
				newExpression = valueExpression;
			
			if (functionCall)
			{
				//std::cout << "Is function call " << std::endl;
				
				//methods only
				if (valueContainer != 0)
				{
					//std::cout << "with value container." << std::endl;
					std::list<class Object*> list = Vjassdoc::parser()->getSpecificList(Parser::Methods, Object::IsInContainer(), valueContainer);
					value = Parser::searchObjectInCustomList(list, newExpression);
				}
				//functions only
				else
				{
					//std::cout << "Has no container " << std::endl;
					value = this->searchObjectInList(newExpression, Parser::Functions);
					//std::cout << "Value = " << value << std::endl;
				}
			}
			else
			{
				//members only
				if (valueContainer != 0)
				{

					std::list<class Object*> list = Vjassdoc::parser()->getSpecificList(Parser::Members, Object::IsInContainer(), valueContainer);
					value = Parser::searchObjectInCustomList(list, newExpression);
				}
				//globals only
				else
					value = this->searchObjectInList(newExpression, Parser::Globals);
			}
			
			if (position == std::string::npos && value == 0)
				valueExpression.clear();
		}
		//else
			//std::cout << "has not to search" << std::endl;
	}
	else
		valueExpression = '-';
	
	return value;
}

}
