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

#include <iostream> //debug
#include <sstream>

#include "objects.h"
#include "internationalisation.h"

namespace vjassdoc
{

const char *FunctionInterface::sqlTableName = "FunctionInterfaces";
unsigned int FunctionInterface::sqlColumns;
std::string FunctionInterface::sqlColumnStatement;

void FunctionInterface::initClass()
{
	FunctionInterface::sqlColumns = Object::sqlColumns + 14;
	/// @todo Maybe you shouldn't use a fixed parameter count. SQL doesn't provide any kinds of arrays but you could use a string list by separating all parameter object id's with a char (for example ;).
	FunctionInterface::sqlColumnStatement = Object::sqlColumnStatement +
	",Library INT,"
	"Scope INT,"
	"IsPrivate BOOLEAN,"
	"Parameter0 INT,"
	"Parameter1 INT,"
	"Parameter2 INT,"
	"Parameter3 INT,"
	"Parameter4 INT,"
	"Parameter5 INT,"
	"Parameter6 INT,"
	"Parameter7 INT,"
	"Parameter8 INT,"
	"Parameter9 INT,"
	"ReturnType INT";
}

FunctionInterface::FunctionInterface(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, class Library *library, class Scope *scope, bool isPrivate, std::list<class Parameter*> parameters, const std::string &returnTypeExpression) : Object(identifier, sourceFile, line, docComment), m_library(library), m_scope(scope), m_isPrivate(isPrivate), m_parameters(parameters), m_returnTypeExpression(returnTypeExpression), m_returnType(0)
{

	for (std::list<class Parameter*>::iterator iterator = this->m_parameters.begin(); iterator != this->m_parameters.end(); ++iterator)
	      (*iterator)->setFunctionInterface(this);
}

FunctionInterface::FunctionInterface(std::vector<const unsigned char*> &columnVector) : m_library(0), m_scope(0), m_returnType(0), Object(columnVector)
{
}

FunctionInterface::~FunctionInterface()
{
}

void FunctionInterface::init()
{
	if (!this->m_returnTypeExpression.empty())
	{
		this->m_returnType = this->searchObjectInList(this->m_returnTypeExpression, Parser::FunctionInterfaces);
		
		if (this->m_returnType == 0)
			this->m_returnType = this->searchObjectInList(this->m_returnTypeExpression, Parser::Types);
		
		if (this->m_returnType == 0)
			this->m_returnType = this->searchObjectInList(this->m_returnTypeExpression, Parser::Interfaces);
		
		if (this->m_returnType == 0)
			this->m_returnType = this->searchObjectInList(this->m_returnTypeExpression, Parser::Structs);
		
		if (this->m_returnType != 0)
			this->m_returnTypeExpression.clear();
	}
	else
		this->m_returnTypeExpression = "-";
}

void FunctionInterface::pageNavigation(std::ofstream &file) const
{
	file
	<< "\t\t\t<li><a href=\"#Description\">"	<< _("Description") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Source File\">"	<< _("Source File") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Library\">"		<< _("Library") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Scope\">"		<< _("Scope") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Private\">"		<< _("Private") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Parameters\">"		<< _("Parameters") << "</a></li>"
	<< "\t\t\t<li><a href=\"#Return Type\">"	<< _("Return Type") << "</a></li>\n"
	;
}

void FunctionInterface::page(std::ofstream &file) const
{
	file
	<< "\t\t<h2><a name=\"Description\">" << _("Description") << "</a></h2>\n"
	<< "\t\t<p>\n"
	<< "\t\t" << Object::objectPageLink(this->docComment()) << '\n'
	<< "\t\t</p>\n"
	<< "\t\t<h2><a name=\"Source File\">" << _("Source File") << "</a></h2>\n"
	<< "\t\t" << SourceFile::sourceFileLineLink(this) << '\n'
	<< "\t\t<h2><a name=\"Library\">" << _("Library") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->library()) << "\n"
	<< "\t\t<h2><a name=\"Scope\">" << _("Scope") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->scope()) << "\n"
	<< "\t\t<h2><a name=\"Private\">" << _("Private") << "</a></h2>\n"
	<< "\t\t" << Object::showBooleanProperty(this->isPrivate()) << "\n"
	<< "\t\t<h2><a name=\"Parameters\">" << _("Parameters") << "</a></h2>\n"
	;

	if (!this->m_parameters.empty())
	{
		file << "\t\t<ul>\n";
		std::list<class Parameter*> list = this->m_parameters;

		for (std::list<class Parameter*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			file << "\t\t\t<li>" << Object::objectPageLink((*iterator)->type(), (*iterator)->typeExpression()) << " - " << Object::objectPageLink(*iterator) << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Return Type\">" << _("Return Type") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->returnType()) << "\n"
	;
}

std::string FunctionInterface::sqlStatement() const
{
	std::ostringstream sstream;
	sstream
	<< Object::sqlStatement() << ", "
	<< "Library=" << Object::objectId(this->library()) << ", "
	<< "Scope=" << Object::objectId(this->scope()) << ", "
	<< "IsPrivate=" << this->isPrivate() << ", ";
	int i = 0;
	std::list<class Parameter*> list = this->m_parameters;
	
	for (std::list<class Parameter*>::iterator iterator = list.begin(); iterator != list.end() && i < 10; ++iterator)
	{
		std::cout << "Number " << i << std::endl;
		std::cout << "Adding parameter " << (*iterator)->id() << " with index " << i << std::endl;
		sstream << "Parameter" << i << "=" << Object::objectId((*iterator)) << ", ";
		std::cout << "After adding" << std::endl;
		++i;
	}
	
	for ( ; i < 10; ++i)
	{
		std::cout << "Parameter " << i << " is free." << std::endl;
		sstream << "Parameter" << i << "=-1, ";
	}
	
	sstream << "ReturnType=" << Object::objectId(this->returnType());
	
	return sstream.str();
}

class Library* FunctionInterface::library() const
{
	return this->m_library;
}

class Scope* FunctionInterface::scope() const
{
	return this->m_scope;
}

}
