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
#include <iostream> //debug

#include "objects.h"
#include "sourcefile.h"
#include "doccomment.h"
#include "vjassdoc.h"
#include "internationalisation.h"
#include "file.h"

namespace vjassdoc
{

unsigned int Object::sqlColumns;
std::string Object::sqlColumnStatement;
int Object::maxIds = 0;

void Object::initClass()
{
	Object::sqlColumns = 4;
	Object::sqlColumnStatement =
	"Identifier VARCHAR(255),"
	"SourceFile INT,"
	"Line INT,"
	"DocComment INT";
}

std::string Object::sqlFilteredString(const std::string &usedString)
{
	std::string result;
	char character;
	int i = 0;
	
	do
	{
		character = usedString[i];
		
		switch (character)
		{
			case '\'':
				result += "\'\'";
				break;
			
			case '\"':
				result += "\"\"";
				break;
			
			default:
				result += character;
				break;
		}
		
		++i;
	}
	while (i != usedString.length());

	return result;
}

std::string Object::sqlTableHeader(const std::string &tableName, const std::string &entries)
{
	return "CREATE TABLE " + tableName + "(Id INT PRIMARY KEY," + entries + ')';
}

Object::Object(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment) : m_id(maxIds), m_identifier(identifier), m_sourceFile(sourceFile), m_line(line), m_docComment(docComment), m_container(0), m_scope(0), m_library(0)
{
	++maxIds;
	
	if (docComment != 0)
		docComment->setObject(this);
}

Object::Object(std::vector<const unsigned char*> &columnVector) : m_container(0), m_scope(0), m_library(0)
{
	m_id = atoi((const char*)columnVector[0]);
	m_identifier = (const char*)(columnVector[1]);
	//m_sourceFile = columnVector[2]; //TODO By id
	m_line = atoi((const char*)columnVector[3]);
	//m_docComment = columnVector[4]; //TODO By id
	
	if (m_docComment != 0)
		m_docComment->setObject(this);
}

Object::~Object()
{
}

std::string Object::sqlStatement() const
{
	std::ostringstream sstream;
	sstream
	<< "Identifier=\"" << Object::sqlFilteredString(this->identifier()) << "\", "
	<< "SourceFile=" << Object::objectId(this->sourceFile()) << ", "
	<< "Line=" << this->line() << ", "
	<< "DocComment=" << Object::objectId(this->docComment());

	return sstream.str();
}

//empty condition methods
class Object* Object::container() const
{
	return m_container;
}

class Scope* Object::scope() const
{
	return m_scope;
}

class Library* Object::library() const
{
	return m_library;
}

bool Object::IsInContainer::operator()(const class Object *thisObject, const class Object *container) const
{
	return thisObject->container() == container;
}

bool Object::IsInScope::operator()(const class Object *thisObject, const class Object *scope) const
{
	return thisObject->scope() == static_cast<const class Scope*>(scope);
}

bool Object::IsInLibrary::operator()(const class Object *thisObject, const class Object *library) const
{
	return thisObject->library() == static_cast<const class Library*>(library);
}

bool Object::hasToSearchValueObject(class Object *type, const std::string &expression)
{
	std::cout << "Has to search with expression " << expression << " and type " << type << std::endl;

	if (type == 0)
		return false;

	std::cout << "type name " << type->identifier() << std::endl;

	if (dynamic_cast<class Type*>(type) != 0)
	{
		do
		{
			if (type == Vjassdoc::getParser()->integerType())
			{
				if (isdigit(expression[0]) || expression[0] == '-' || expression[0] == '+')
					return false;
				
				return true;
			}
			else if (type == Vjassdoc::getParser()->realType())
			{
				if (isdigit(expression[0]) || expression[0] == '-' || expression[0] == '+' || expression[0] == '.')
					return false;
				
				return true;
			}
			else if (type == Vjassdoc::getParser()->stringType())
			{
				if (expression[0] == '\"' || expression == "null")
					return false;
				
				return true;
			}
			else if (type == Vjassdoc::getParser()->booleanType())
			{
				if (expression == "true" || expression == "false")
					return false;
				
				return true;
			}
			else if (type == Vjassdoc::getParser()->handleType() || type == Vjassdoc::getParser()->codeType()) /// @todo Code type == null?!
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
		std::cout << "Before has to search." << std::endl;
		
		//FIXME The value can be a function or method call with literal arguments.
		if (Object::hasToSearchValueObject(type, valueExpression))
		{
			std::cout << "Has to search!" << std::endl;
			//FIXME Detect . separators correctly.
			class Object *valueContainer = 0;
			std::string::size_type separatorPosition = valueExpression.find('.');
			
			if (separatorPosition != std::string::npos)
			{
				std::cout << "Found . in value expression at position " << separatorPosition << std::endl;
				std::string containerIdentifier = valueExpression.substr(0, separatorPosition);
				std::cout << "Container identifier: " << containerIdentifier << std::endl;
				
				if (containerIdentifier == File::expressionText[File::ThistypeExpression])
				{
					valueContainer = this->container();
					std::cout << "thistype" << std::endl;
				}
				else if (containerIdentifier == File::expressionText[File::SuperExpression])
				{
					//FIXME Error? Test it!
					//valueContainer = static_cast<class Struct*>(this->container())->extension();
					std::cout << "super" << std::endl;
				}
				
				//FIXME, does not work
				valueExpression.erase(separatorPosition);
				std::cout << "New value expression " << valueExpression << std::endl;
				std::cout << "New position " << separatorPosition << std::endl;
			}
			
			//FIXME Detect _ separators correctly?!
			
			bool functionCall = false;
			int position = valueExpression.find('(');
			
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
				std::cout << "Is function call " << std::endl;
				
				//methods only
				if (valueContainer != 0)
				{
					std::cout << "with value container." << std::endl;
					std::list<class Object*> list = Vjassdoc::getParser()->getSpecificList(valueContainer, Parser::Methods, Object::IsInContainer());
					value = Parser::searchObjectInCustomList(list, this, newExpression, Parser::Unspecified);
				}
				//functions only
				else
				{
					std::cout << "Has no container " << std::endl;
					value = this->searchObjectInList(newExpression, Parser::Functions);
					//std::cout << "Value = " << value << std::endl;
				}
			}
			else
			{
				//members only
				if (valueContainer != 0)
				{

					std::list<class Object*> list = Vjassdoc::getParser()->getSpecificList(valueContainer, Parser::Members, Object::IsInContainer());
					value = Parser::searchObjectInCustomList(list, this, newExpression, Parser::Unspecified);
				}
				//globals only
				else
					value = this->searchObjectInList(newExpression, Parser::Globals);
			}
			
			if (position == std::string::npos && value == 0)
				valueExpression.clear();
		}
		else
			std::cout << "has not to search" << std::endl;
	}
	else
		valueExpression = '-';
	
	return value;
}

}
