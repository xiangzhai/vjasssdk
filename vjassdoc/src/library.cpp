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

#include "objects.h"
#include "internationalisation.h"
#include "vjassdoc.h"
#include "parser.h"

namespace vjassdoc
{

const char *Library::sqlTableName = "Libraries";
unsigned int Library::sqlColumns;
std::string Library::sqlColumnStatement;

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

Library::Library(std::vector<const unsigned char*> &columnVector) : Object(columnVector)
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
	if (!this->initializerExpression.empty())
	{
		this->m_initializer = static_cast<Function*>(this->searchObjectInList(this->initializerExpression, Parser::Functions));
		
		if (this->m_initializer == 0)
			this->m_initializer = static_cast<Function*>(this->searchObjectInList(this->initializerExpression, Parser::Methods));
		
		if (this->m_initializer != 0)
			this->initializerExpression.clear();
	}
	else
		this->initializerExpression = '-';
	
	if (this->requirementExpressions != 0)
	{
		this->m_requirement = new std::list<Library*>;
		std::list<std::string>::iterator expressionIterator = this->requirementExpressions->begin();
		
		while (expressionIterator != this->requirementExpressions->end())
		{
			class Library *object = static_cast<Library*>(this->searchObjectInList(*expressionIterator, Parser::Libraries));
			
			if (object != 0)
				(*expressionIterator).clear();

			this->m_requirement->push_back(object);
			++expressionIterator;
		}
	}
}

void Library::pageNavigation(std::ofstream &file) const
{
	file
	<< "\t\t\t<li><a href=\"#Description\">"			<< _("Description") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Source file\">"			<< _("Source file") << "</a></li>\n"
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
	<< "\t\t<h2><a name=\"Keywords\">" << _("Keywords") << "</a></h2>\n"
	;
	
	std::list<class Object*> list = Vjassdoc::getParser()->getSpecificList(this, Parser::Keywords, Object::IsInLibrary());
	
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
	
	list = Vjassdoc::getParser()->getSpecificList(this, Parser::TextMacros, Object::IsInLibrary());
	
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
	
	list = Vjassdoc::getParser()->getSpecificList(this, Parser::TextMacroInstances, Object::IsInLibrary());
	
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
	
	list = Vjassdoc::getParser()->getSpecificList(this, Parser::Types, Object::IsInLibrary());
	
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
	
	list = Vjassdoc::getParser()->getSpecificList(this, Parser::Globals, Object::IsInLibrary());
	
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
	
	list = Vjassdoc::getParser()->getSpecificList(this, Parser::FunctionInterfaces, Object::IsInLibrary());
	
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
	
	list = Vjassdoc::getParser()->getSpecificList(this, Parser::Functions, Object::IsInLibrary());
	
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
	
	list = Vjassdoc::getParser()->getSpecificList(this, Parser::Interfaces, Object::IsInLibrary());
	
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
	
	list = Vjassdoc::getParser()->getSpecificList(this, Parser::Structs, Object::IsInLibrary());
	
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
	
	list = Vjassdoc::getParser()->getSpecificList(this, Parser::Scopes, Object::IsInLibrary());
	
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