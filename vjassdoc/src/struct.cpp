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
#include "file.h"
#include "internationalisation.h"
#include "vjassdoc.h"

namespace vjassdoc
{

bool Struct::HasExtension::operator()(const class Object *thisObject, const class Object *extension) const
{
	return static_cast<const class Struct*>(thisObject)->extension() == static_cast<const class Interface*>(extension);
}

Struct::Struct(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, class Library *library, class Scope *scope, bool isPrivate, const std::string &extensionExpression) : extensionExpression(extensionExpression), m_extension(0), m_constructor(0), m_destructor(0), m_initializer(0), Interface(identifier, sourceFile, line, docComment, library, scope, isPrivate)
{
}

Struct::Struct(std::vector<const unsigned char*> &columnVector) : Interface(columnVector)
{
}

void Struct::init()
{
	Interface::init();
	
	if (!this->extensionExpression.empty() && this->extensionExpression.find(File::expressionText[File::ArrayExpression]) != 0)
	{
		this->m_extension = static_cast<class Interface*>(this->searchObjectInList(this->extensionExpression, Parser::Interfaces));
		
		if (this->m_extension == 0)
			this->m_extension = static_cast<class Interface*>(this->searchObjectInList(this->extensionExpression, Parser::Structs));
		
		if (this->m_extension != 0)
			this->extensionExpression.clear();
	}
	
	m_container = this;
	this->m_constructor = static_cast<class Method*>(this->searchObjectInList("create", Parser::Methods, Parser::CheckContainer));
	this->m_destructor = static_cast<class Method*>(this->searchObjectInList("onDestroy", Parser::Methods, Parser::CheckContainer));
	this->m_initializer = static_cast<class Method*>(this->searchObjectInList("onInit", Parser::Methods, Parser::CheckContainer));
}

void Struct::pageNavigation(std::ofstream &file) const
{
	Interface::pageNavigation(file);
	file
	<< "\t\t\t<li><a href=\"#Extension\">"			<< _("Extension") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Child structs\">"		<< _("Child structs") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Constructor\">"		<< _("Constructor") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Destructor\">"			<< _("Destructor") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Initializer\">"		<< _("Initializer") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Inherited members\">"	<< _("Inherited members") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Inherited methods\">"	<< _("Inherited methods") << "</a></li>\n"
	;
}

void Struct::page(std::ofstream &file) const
{
	Interface::page(file);
	file
	<< "\t\t<h2><a name=\"Extension\">" << _("Extension") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->extension()) << '\n'
	<< "\t\t<h2><a name=\"Child structs\">" << _("Child structs") << "</a></h2>\n"
	;
	
	std::list<class Object*> list = Vjassdoc::getParser()->getSpecificList(this, Parser::Structs, Struct::HasExtension());
	
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
	<< "\t\t<h2><a name=\"Constructor\">" << _("Constructor") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->constructor()) << '\n'
	<< "\t\t<h2><a name=\"Destructor\">" << _("Destructor") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->destructor()) << '\n'
	<< "\t\t<h2><a name=\"Initializer\">" << _("Initializer") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->initializer()) << '\n'
	<< "\t\t<h2><a name=\"Inherited members\">" << _("Inherited members") << "</a></h2>\n"
	;
	
	if (this->extension() != 0 &&  dynamic_cast<class Struct*>(this->extension()) != 0)
	{
		class Struct *extension = static_cast<class Struct*>(this->extension());

		do
		{
			extension->getMemberList(file);
			
			if (extension->extension() != 0 && dynamic_cast<class Struct*>(extension->extension()) != 0)
				extension = static_cast<class Struct*>(extension->extension());
			else
				break;
		}
		while (true);
		
		file << "\t\t<h2><a name=\"Inherited methods\">" << _("Inherited methods") << "</a></h2>\n";
		
		extension = static_cast<class Struct*>(this->extension()); //do not check at twice
		
		do
		{
			extension->getMethodList(file);
			
			if (extension->extension() != 0 && dynamic_cast<class Struct*>(extension->extension()) != 0)
				extension = static_cast<class Struct*>(extension->extension());
			else
				break;
		}
		while (true);
	}
	else
	{
		file
		<< "\t\t-\n"
		<< "\t\t<h2><a name=\"Inherited methods\">" << _("Inherited methods") << "</a></h2>\n"
		<< "\t\t-\n"
		;
	}
}

std::string Struct::sqlStatement() const
{
	std::ostringstream sstream;
	sstream
	<< Interface::sqlStatement() << ", "
	<< "Extension=" << Object::objectId(this->extension()) << ", "
	<< "Constructor=" << Object::objectId(this->constructor()) << ", "
	<< "Destructor=" << Object::objectId(this->destructor()) << ", "
	<< "Initializer=" << Object::objectId(this->initializer());

	return sstream.str();
}

}
