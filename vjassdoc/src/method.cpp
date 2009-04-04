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

Method::Method(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, class Library *library, class Scope *scope, bool isPrivate, std::list<std::string> *parameterTypeExpressions, std::list<std::string> *parameters, const std::string &returnTypeExpression, bool isPublic, bool isConstant, class Object *container, bool isStatic, bool isStub, const std::string &defaultReturnValueExpression) : m_container(container), m_isStatic(isStatic), m_isStub(isStub), m_defaultReturnValueExpression(defaultReturnValueExpression), m_defaultReturnValue(0), Function(identifier, sourceFile, line, docComment, library, scope, isPrivate, parameterTypeExpressions, parameters, returnTypeExpression, isPublic, isConstant, false)
{
}

Method::Method(std::vector<const unsigned char*> &columnVector) : Function(columnVector)
{
}

void Method::init()
{
	Function::init();

	if (!this->m_defaultReturnValueExpression.empty())
	{
		if (Object::hasToSearchValueObject(static_cast<class Type*>(this->returnType()), this->m_defaultReturnValueExpression))
		{
			this->m_defaultReturnValue = this->searchObjectInList(this->m_defaultReturnValueExpression, Parser::Types);
		
			if (this->m_defaultReturnValue == 0)
				this->m_defaultReturnValue = this->searchObjectInList(this->m_defaultReturnValueExpression, Parser::FunctionInterfaces);
			
			if (this->m_defaultReturnValue == 0)
				this->m_defaultReturnValue = this->searchObjectInList(this->m_defaultReturnValueExpression, Parser::Interfaces);
			
			if (this->m_defaultReturnValue == 0)
				this->m_defaultReturnValue = this->searchObjectInList(this->m_defaultReturnValueExpression, Parser::Structs);
			
			if (this->m_defaultReturnValue != 0)
				this->m_defaultReturnValueExpression.clear();
		}
	}
	else
		this->m_defaultReturnValueExpression = '-';
}

void Method::pageNavigation(std::ofstream &file) const
{
	Function::pageNavigation(file);
	file
	<< "\t\t\t<li><a href=\"#Container\">"				<< _("Container") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Static\">"					<< _("Static") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Stub\">"					<< _("Stub") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Default return value\">"	<< _("Default return value") << "</a></li>\n"
	;
}

void Method::page(std::ofstream &file) const
{
	Function::page(file);
	file
	<< "\t\t<h2><a name=\"Container\">" << _("Container") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->container()) << "\n"
	<< "\t\t<h2><a name=\"Static\">" << _("Static") << "</a></h2>\n"
	<< "\t\t" << Object::showBooleanProperty(this->isStatic()) << "\n"
	<< "\t\t<h2><a name=\"Stub\">" << _("Stub") << "</a></h2>\n"
	<< "\t\t" << Object::showBooleanProperty(this->isStub()) << "\n"
	<< "\t\t<h2><a name=\"Default return value\">" << _("Default return value") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->defaultReturnValue(), this->defaultReturnValueExpression()) << "\n"
	;
}

std::string Method::sqlStatement() const
{
	std::ostringstream sstream;
	sstream
	<< Function::sqlStatement() << ", "
	<< "Container=" << Object::objectId(this->container()) << ", "
	<< "IsStatic=" << this->isStatic() << ", "
	<< "IsStub=" << this->isStub() << ", "
	<< "DefaultReturnValue=" << Object::objectId(this->defaultReturnValue());

	return sstream.str();
}

class Object* Method::container() const
{
	return this->m_container;
}

}
