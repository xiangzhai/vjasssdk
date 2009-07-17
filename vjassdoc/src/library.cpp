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
#include <list>
#include <iostream> //debug

#include "objects.h"
#include "internationalisation.h"
#include "vjassdoc.h"
#include "parser.h"

namespace vjassdoc
{

const char *Library::sqlTableName = "Libraries";
unsigned int Library::sqlColumns;
std::string Library::sqlColumnStatement;

bool Library::HasRequirement::operator()(class Object *thisObject, class Object *library) const
{
	std::cout << "Test" << std::endl;
	class Library *thisLibrary = static_cast<class Library*>(thisObject);

	if (thisLibrary->requirement() != 0)
	{
		for (std::list<class Library*>::iterator iterator = thisLibrary->requirement()->begin(); iterator != thisLibrary->requirement()->end(); ++iterator)
		{
			if (*iterator == static_cast<class Library*>(library))
				return true;
		}
	}
	
	return false;
}

void Library::initClass()
{
	Library::sqlColumns = Object::sqlColumns + 3;
	/// @todo Add class Requirement.
	Library::sqlColumnStatement = Object::sqlColumnStatement +
	",IsOnce BOOL,"
	"Initializer INT,"
	"Requirement INT";
}

Library::Library(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, DocComment *docComment, bool isOnce, const std::string &initializerExpression, std::list<std::string> *requirementExpressions) : m_isOnce(isOnce), initializerExpression(initializerExpression), requirementExpressions(requirementExpressions), m_initializer(0), m_requirement(0), Object(identifier, sourceFile, line, docComment)
{
}

Library::Library(std::vector<const unsigned char*> &columnVector) : requirementExpressions(0), m_initializer(0), m_requirement(0), Object(columnVector)
{
}

Library::~Library()
{
	if (this->requirementExpressions != 0)
		delete this->requirementExpressions;
	
	if (this->m_requirement != 0)
		delete this->m_requirement;
}

void Library::init()
{
	std::cout << "Init" << std::endl;

	if (!this->initializerExpression.empty())
	{
		std::cout << "Initializer expression " << this->initializerExpression << std::endl;
	
		this->m_initializer = static_cast<Function*>(this->searchObjectInList(this->initializerExpression, Parser::Functions));
		
		if (this->m_initializer == 0)
			this->m_initializer = static_cast<Function*>(this->searchObjectInList(this->initializerExpression, Parser::Methods));
		
		if (this->m_initializer != 0)
			this->initializerExpression.clear();
	}
	else
		this->initializerExpression = '-';
	
	std::cout << "After initializer" << std::endl;
	
	if (this->requirementExpressions != 0)
	{
		std::cout << "1" << std::endl;
		
		this->m_requirement = new std::list<Library*>;
		
		std::cout << "2" << std::endl;
		
		std::list<std::string>::iterator expressionIterator = this->requirementExpressions->begin();
		
		std::cout << "3" << std::endl;
		
		while (expressionIterator != this->requirementExpressions->end())
		{
			class Library *object = static_cast<Library*>(this->searchObjectInList(*expressionIterator, Parser::Libraries));
			
			if (object != 0)
				(*expressionIterator).clear();

			this->m_requirement->push_back(object);
			++expressionIterator;
		}
	}
	
	std::cout << "After requirement" << std::endl;
}

void Library::pageNavigation(std::ofstream &file) const
{
	file
	<< "\t\t\t<li><a href=\"#Description\">"			<< _("Description") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Source file\">"			<< _("Source file") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Once\">"				<< _("Once") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Initializer\">"			<< _("Initializer") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Requirement\">"			<< _("Requirement") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Keywords\">"				<< _("Keywords") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Text Macros\">"			<< _("Text Macros") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Text Macro Instances\">"		<< _("Text Macro Instances") << "</a></li>"
	<< "\t\t\t<li><a href=\"#Types\">"				<< _("Types") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Globals\">"				<< _("Globals") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Function Interfaces\">"		<< _("Function Interfaces") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Functions\">"				<< _("Functions") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Interfaces\">"				<< _("Interfaces") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Structs\">"				<< _("Structs") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Scopes\">"				<< _("Scopes") << "</a></li>\n"
	;
}

void Library::page(std::ofstream &file) const
{
	file
	<< "\t\t<h2><a name=\"Description\">" << _("Description") << "</a></h2>\n"
	<< "\t\t<p>\n"
	<< "\t\t" << Object::objectPageLink(this->docComment()) << "\n"
	<< "\t\t</p>\n"
	<< "\t\t<h2><a name=\"Source File\">" << _("Source File") << "</a></h2>\n"
	<< "\t\t" << SourceFile::sourceFileLineLink(this) << '\n'
	<< "\t\t<h2><a name=\"Once\">" << _("Once") << "</a></h2>\n"
	<< "\t\t" << Object::showBooleanProperty(this->isOnce()) << '\n'
	<< "\t\t<h2><a name=\"Initializer\">" << _("Initializer") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->initializer(), this->initializerExpression) << '\n'
	<< "\t\t<h2><a name=\"Requirement\">" << _("Requirement") << "</a></h2>\n"
	;
	
	if (this->requirement() != 0)
	{
		file << "\t\t<ul>\n";
		std::list<std::string>::iterator expressionIterator = this->requirementExpressions->begin();
	
		for (std::list<class Library*>::iterator iterator = this->requirement()->begin(); iterator != this->requirement()->end(); ++iterator, ++expressionIterator)
		{
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator, *expressionIterator) << "</li>\n";
		}
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t<p>-</p>\n";
	
	file
	<< "\t\t<h2><a name=\"Keywords\">" << _("Keywords") << "</a></h2>\n"
	;
	
	std::list<class Object*> list = Vjassdoc::getParser()->getSpecificList(Parser::Keywords, Object::IsInLibrary(), this);
	
	if (!list.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Text Macros\">" << _("Text Macros") << "</a></h2>\n"
	;
	
	list = Vjassdoc::getParser()->getSpecificList(Parser::TextMacros, Object::IsInLibrary(), this);
	
	if (!list.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Text Macro Instances\">" << _("Text Macro Instances") << "</a></h2>\n"
	;
	
	list = Vjassdoc::getParser()->getSpecificList(Parser::TextMacroInstances, Object::IsInLibrary(), this);
	
	if (!list.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Types\">" << _("Types") << "</a></h2>\n"
	;
	
	list = Vjassdoc::getParser()->getSpecificList(Parser::Types, Object::IsInLibrary(), this);
	
	if (!list.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Globals\">" << _("Globals") << "</a></h2>\n"
	;
	
	list = Vjassdoc::getParser()->getSpecificList(Parser::Globals, Object::IsInLibrary(), this);
	
	if (!list.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Function Interfaces\">" << _("Function Interfaces") << "</a></h2>\n"
	;
	
	list = Vjassdoc::getParser()->getSpecificList(Parser::FunctionInterfaces, Object::IsInLibrary(), this);
	
	if (!list.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Functions\">" << _("Functions") << "</a></h2>\n"
	;
	
	list = Vjassdoc::getParser()->getSpecificList(Parser::Functions, Object::IsInLibrary(), this);
	
	if (!list.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Interfaces\">" << _("Interfaces") << "</a></h2>\n"
	;
	
	list = Vjassdoc::getParser()->getSpecificList(Parser::Interfaces, Object::IsInLibrary(), this);
	
	if (!list.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Structs\">" << _("Structs") << "</a></h2>\n"
	;
	
	list = Vjassdoc::getParser()->getSpecificList(Parser::Structs, Object::IsInLibrary(), this);
	
	if (!list.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Scopes\">" << _("Scopes") << "</a></h2>\n"
	;
	
	list = Vjassdoc::getParser()->getSpecificList(Parser::Scopes, Object::IsInLibrary(), this);
	
	if (!list.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";

	//contained keywords
	//contained text macros
	//contained text macro instances
	//contained types
	//contained globals
	//contained function interfaces
	//contained functions
	//contained interfaces
	//contained structs
	//contained scopes
}

std::string Library::sqlStatement() const
{
	std::stringstream sstream;
	sstream
	<< Object::sqlStatement() << ", "
	<< "IsOnce=" << this->isOnce() << ", "
	<< "Initializer=" << Object::objectId(this->initializer()) << ", ";
	
	if (this->requirement() != 0)
		sstream << "Requirement=" << Object::objectId(this->requirement()->front()); //Array
	else
		sstream << "Requirement=-1";
	
	return sstream.str();
}

}
