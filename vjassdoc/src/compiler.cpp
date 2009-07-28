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

#include <cstdio>
#include <iostream>
#include <boost/tokenizer.hpp>

#include "vjassdoc.h"
#include "internationalisation.h"
#include "parser.h"
#include "objects.h"

namespace vjassdoc
{

void Compiler::compile()
{
	std::fstream fstream(Vjassdoc::compileFilePath().c_str());

	if (!fstream.good())
	{
		if (Vjassdoc::showVerbose())
			fprintf(stderr, _("Error while opening or creating file \"%s\" for compilation process.\n"), Vjassdoc::compileFilePath().c_str());
		
		return;
	}
	
	if (Vjassdoc::showVerbose())
		fprintf(stderr, _("Opening or creating file \"%s\" for compilation process.\n"), Vjassdoc::compileFilePath().c_str());
	
	fstream << "globals" << std::endl;
	this->writeGlobals(fstream);
	this->writeMembers(fstream);
	this->writeMethodGlobals(fstream);
	fstream << "endglobals" << std::endl;

	fstream.close();
}

void Compiler::writeGlobals(std::fstream &fstream)
{
	if (Vjassdoc::showVerbose())
		std::cout << _("Writing globals.") << std::endl;

	std::list<class Object*> list = Vjassdoc::getParser()->getSpecificList(Parser::Globals, Parser::Comparator());
	
	for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
	{
		class Global *global = static_cast<class Global*>(*iterator);
	
		if (global->type() != 0)
			fstream << global->type()->identifier();
		else
			fstream << global->typeExpression();
		
		if (global->size() != 0 || global->sizeLiteral() != 0)
			fstream << " array ";
		
		if (global->library() != 0)
			fstream << global->library()->identifier() << "__";
		
		if (global->scope() != 0)
			fstream << global->scope()->identifier() << "__";
		
		fstream << global->identifier() << '\n';
	}
}

void Compiler::writeMembers(std::fstream &fstream)
{
	if (Vjassdoc::showVerbose())
		std::cout << _("Writing members.") << std::endl;

	std::list<class Object*> list = Vjassdoc::getParser()->getSpecificList(Parser::Members, Parser::Comparator());
	
	for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
	{
		class Member *member = static_cast<class Member*>(*iterator);
	
		if (member->isDelegate())
			continue;

		/// @todo Check if type is an interface or struct and use integers for those types.	
		if (member->type() != 0)
			fstream << member->type()->identifier();
		else
			fstream << member->typeExpression();
		
		if (member->size() != 0 || member->sizeLiteral() != 0 || !member->isStatic())
			fstream << " array ";
		
		if (member->library() != 0)
			fstream << member->library()->identifier() << "__";
		
		if (member->scope() != 0)
			fstream << member->scope()->identifier() << "__";
		
		fstream << member->container()->identifier() << "__";
		
		fstream << member->identifier() << '\n';
	}
}

void Compiler::writeMethodGlobals(std::fstream &fstream)
{
	if (Vjassdoc::showVerbose())
		std::cout << _("Writing method globals.") << std::endl;
}

}
