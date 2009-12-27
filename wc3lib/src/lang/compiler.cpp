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
#include <boost/format.hpp>

#include "parser.hpp"
#include "object.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{
	
namespace lang
{

void Compiler::compile(std::ostream &ostream)
{
	ostream << "globals" << std::endl;
	this->writeGlobals(ostream);
	this->writeMembers(ostream);
	this->writeFunctionPrototypeGlobals(ostream);
	this->writeMethodPrototypeGlobals(ostream);
	ostream << "endglobals" << std::endl;
}

void Compiler::writeGlobals(std::ostream &ostream)
{
	if (Vjassdoc::optionVerbose())
		std::cout << _("Writing globals.") << std::endl;

	std::list<class Object*> list = Vjassdoc::parser()->getSpecificList(Parser::Globals, Parser::Comparator());
	
	for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
	{
		class Global *global = static_cast<class Global*>(*iterator);
	
		if (global->type() != 0)
			ostream << global->type()->identifier();
		else
			ostream << global->typeExpression();
		
		if (global->size() != 0 || global->sizeLiteral() != 0)
			ostream << " array ";
		
		if (global->library() != 0)
			ostream << global->library()->identifier() << "__";
		
		if (global->scope() != 0)
			ostream << global->scope()->identifier() << "__";
		
		ostream << global->identifier() << '\n';
	}
}

void Compiler::writeMembers(std::ostream &ostream)
{
	if (Vjassdoc::optionVerbose())
		std::cout << _("Writing members.") << std::endl;

	std::list<class Object*> list = Vjassdoc::parser()->getSpecificList(Parser::Members, Parser::Comparator());
	
	for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
	{
		class Member *member = static_cast<class Member*>(*iterator);
	
		if (member->isDelegate())
			continue;

		if (member->type() != 0)
		{
			// Check if type is an interface or struct and use integers for those types.
			if (dynamic_cast<class Interface*>(member->type()) != 0)
				ostream << "integer";
			else
				ostream << member->type()->identifier();
		}
		else
			ostream << member->typeExpression();
		
		if (member->size() != 0 || member->sizeLiteral() != 0 || !member->isStatic())
			ostream << " array ";
		
		if (member->library() != 0)
			ostream << member->library()->identifier() << "__";
		
		if (member->scope() != 0)
			ostream << member->scope()->identifier() << "__";
		
		ostream << member->container()->identifier() << "__";
		
		ostream << member->identifier() << '\n';
	}
}

void Compiler::writeFunctionPrototypeGlobals(std::ostream &ostream)
{
	if (Vjassdoc::optionVerbose())
		std::cout << _("Writing function prototype globals.") << std::endl;
	
	// check if there are .execute or .evaluate calls of a function.
}

void Compiler::writeMethodPrototypeGlobals(std::ostream &ostream)
{
	if (Vjassdoc::optionVerbose())
		std::cout << _("Writing method prototype globals.") << std::endl;
	
	// check if there are .execute or .evaluate calls of a method.
	// Note that .evaluate and .execute aren't used automatically like in vJass.
}

void Compiler::writeLibraries(std::ostream &ostream)
{
	if (Vjassdoc::optionVerbose())
		std::cout << _("Writing libraries.") << std::endl;
	
	// write all functions and methods and text macro instances in the right order.
	// text macros and preprocessors should be called before.
}

}

}
