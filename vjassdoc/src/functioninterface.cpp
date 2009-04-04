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

FunctionInterface::FunctionInterface(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, class Library *library, class Scope *scope, bool isPrivate, std::list<std::string> *parameterTypeExpressions, std::list<std::string> *parameters, const std::string &returnTypeExpression) : m_library(library), m_scope(scope), m_isPrivate(isPrivate), m_parameterTypeExpressions(parameterTypeExpressions), m_parameters(parameters), m_returnTypeExpression(returnTypeExpression), m_parameterTypes(0), m_returnType(0), Object(identifier, sourceFile, line, docComment)
{
}

FunctionInterface::FunctionInterface(std::vector<const unsigned char*> &columnVector) : Object(columnVector)
{
}

FunctionInterface::~FunctionInterface()
{
	if (this->m_parameterTypeExpressions != 0)
		delete this->m_parameterTypeExpressions;
	
	if (this->m_parameterTypes != 0)
		delete this->m_parameterTypes;
	
	if (this->m_parameters != 0) //TODO Can be 0?
		delete this->m_parameters;
}

void FunctionInterface::init()
{
	if (this->parameterTypeExpressions() != 0)
	{
		this->m_parameterTypes = new std::list<Object*>;
		std::list<std::string>::iterator expressionIterator = this->parameterTypeExpressions()->begin();
		
		while (expressionIterator != this->parameterTypeExpressions()->end())
		{
			class Object *object = this->searchObjectInList(*expressionIterator, Parser::FunctionInterfaces);
			
			if (object == 0)
				object = this->searchObjectInList(*expressionIterator, Parser::Types);
			
			if (object == 0)
				object = this->searchObjectInList(*expressionIterator, Parser::Interfaces);
			
			if (object == 0)
				object = this->searchObjectInList(*expressionIterator, Parser::Structs);
			
			if (object != 0)
				(*expressionIterator).clear();
			
			this->m_parameterTypes->push_back(object);
			++expressionIterator;
		}
	}
	
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
	<< "\t\t\t<li><a href=\"#Source file\">"	<< _("Source file") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Library\">"		<< _("Library") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Scope\">"			<< _("Scope") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Private\">"		<< _("Private") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Parameters\">"		<< _("Parameters") << "</a></li>"
	<< "\t\t\t<li><a href=\"#Return type\">"	<< _("Return type") << "</a></li>\n"
	;
}

void FunctionInterface::page(std::ofstream &file) const
{
	file
	<< "\t\t<h2><a name=\"Description\">" << _("Description") << "</a></h2>\n"
	<< "\t\t<p>\n"
	<< "\t\t" << Object::objectPageLink(this->docComment()) << "\n"
	<< "\t\t</p>\n"
	<< "\t\t<h2><a name=\"Source file\">" << _("Source file") << "</a></h2>\n"
	<< "\t\t" << SourceFile::sourceFileLineLink(this) << '\n'
	<< "\t\t<h2><a name=\"Library\">" << _("Library") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->library()) << "\n"
	<< "\t\t<h2><a name=\"Scope\">" << _("Scope") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->scope()) << "\n"
	<< "\t\t<h2><a name=\"Private\">" << _("Private") << "</a></h2>\n"
	<< "\t\t" << Object::showBooleanProperty(this->isPrivate()) << "\n"
	<< "\t\t<h2><a name=\"Parameters\">" << _("Parameters") << "</a></h2>\n"
	;

	if (this->parameterTypeExpressions() != 0)
	{
		file << "\t\t<ul>\n";
		std::list<class Object*>::iterator typeIterator = this->parameterTypes()->begin();
		std::list<std::string>::const_iterator expressionIterator = this->parameterTypeExpressions()->begin();
		std::list<std::string>::const_iterator nameIterator = this->parameters()->begin();
	
		while (expressionIterator != this->parameterTypeExpressions()->end())
		{
			file << "\t\t\t<li>" << Object::objectPageLink(*typeIterator, *expressionIterator) << " - " << *nameIterator << "</li>\n";
			++typeIterator;
			++expressionIterator;
			++nameIterator;
		}
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	file
	<< "\t\t<h2><a name=\"Return type\">" << _("Return type") << "</a></h2>\n"
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
	
	if (this->parameterTypes() != 0)
	{
		sstream
		<< "ParameterTypes=" << Object::objectId(this->parameterTypes()->front()) << ", " //Array
		<< "Parameters=\"" << this->parameters()->front() << "\", "; //Array
	}
	else
	{
		sstream
		<< "ParameterTypes=-1, " //Array
		<< "Parameters=NULL, "; //Array
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
