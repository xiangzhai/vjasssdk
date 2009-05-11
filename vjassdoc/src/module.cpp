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

const char *Module::sqlTableName = "Modules";
unsigned int Module::sqlColumns;
std::string Module::sqlColumnStatement;

void Module::initClass()
{
	Module::sqlColumns = Interface::sqlColumns;
	Module::sqlColumnStatement = Interface::sqlColumnStatement;
}

Module::Module(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, class Library *library, class Scope *scope, bool isPrivate) : Interface(identifier, sourceFile, line, docComment, library, scope, isPrivate)
{
}

void Module::pageNavigation(std::ofstream &file) const
{
	Interface::pageNavigation(file);
	file
	<< "\t\t\t<li><a href=\"#Implementations\">"	<< _("Implementations") << "</a></li>\n"
	;
}

void Module::page(std::ofstream &file) const
{
	Interface::page(file);
	file
	<< "\t\t<h2><a name=\"Implementations\">" << _("Implementations") << "</a></h2>\n";
	std::list<class Object*> implementationList = Vjassdoc::getParser()->getSpecificList(this, Parser::Implementations, Implementation::UsesModule());
	
	if (!implementationList.empty())
	{
		file << "\t\t<ul>\n";
	
		for (std::list<class Object*>::iterator iterator = implementationList.begin(); iterator != implementationList.end(); ++iterator)
		{
			class Implementation *implementation = static_cast<class Implementation*>(*iterator);
		
			file << "\t\t\t<li>";
			
			if (implementation->isOptional())
				file << "optional ";
			
			file <<  Object::objectPageLink(implementation->container()) << " - " << Object::objectPageLink(implementation);
			file << "</li>\n";
		}
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
}

}