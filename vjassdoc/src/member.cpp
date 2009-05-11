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
#include <iostream> //debug

#include "objects.h"
#include "file.h"
#include "internationalisation.h"

namespace vjassdoc
{

const char *Member::sqlTableName = "Members";
unsigned int Member::sqlColumns;
std::string Member::sqlColumnStatement;

void Member::initClass()
{
	Member::sqlColumns = Global::sqlColumns + 3;
	Member::sqlColumnStatement = Global::sqlColumnStatement +
	",Container INT,"
	"IsStatic BOOLEAN,"
	"IsDelegate BOOLEAN";
}

Member::Member(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, class Library *library, class Scope *scope, bool isPrivate, bool isPublic, bool isConstant, const std::string &typeExpression, const std::string &valueExpression, const std::string &sizeExpression, class Object *container, bool isStatic, bool isDelegate) : m_container(container), m_isStatic(isStatic), m_isDelegate(isDelegate), Global(identifier, sourceFile, line, docComment, library, scope, isPrivate, isPublic, isConstant, typeExpression, valueExpression, sizeExpression)
{
}

Member::Member(std::vector<const unsigned char*> &columnVector) : Global(columnVector)
{
}

void Member::init()
{
	if (this->typeExpression() == File::expressionText[File::ThistypeExpression]) //is not as fast as direct set
		this->m_typeExpression = this->container()->identifier();
	else if (this->typeExpression() == File::expressionText[File::SuperExpression])
	{
		//FIXME Error!
		//this->m_typeExpression = static_cast<class Struct*>(this->container())->extension()->identifier();
		//std::cout << "Identifier " << static_cast<class Struct*>(this->container())->identifier() << std::endl;
	}
	
	Global::init();
}

void Member::pageNavigation(std::ofstream &file) const
{
	Global::pageNavigation(file);
	file
	<< "\t\t\t<li><a href=\"#Container\">"	<< _("Container") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Static\">"	<< _("Static") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Delegate\">"	<< _("Delegate") << "</a></li>\n"
	;
}

void Member::page(std::ofstream &file) const
{
	Global::page(file);
	file
	<< "\t\t<h2><a name=\"Container\">" << _("Container") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->container()) << "\n"
	<< "\t\t<h2><a name=\"Static\">" << _("Static") << "</a></h2>\n"
	<< "\t\t" << Object::showBooleanProperty(this->isStatic()) << "\n"
	<< "\t\t<h2><a name=\"Delegate\">" << _("Delegate") << "</a></h2>\n"
	<< "\t\t" << Object::showBooleanProperty(this->isDelegate()) << "\n"
	;
}

std::string Member::sqlStatement() const
{
	std::ostringstream sstream;
	sstream
	<< Global::sqlStatement() << ", "
	<< "Container=" << Object::objectId(this->container()) << ", "
	<< "IsStatic=" << this->isStatic() << ", "
	<< "IsDelegate=" << this->isDelegate();

	return sstream.str();
}

class Object* Member::container() const
{
	return this->m_container;
}

}
