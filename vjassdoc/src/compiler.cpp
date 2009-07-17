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

#include <boost/tokenizer.hpp>

#include "vjassdoc.h"
#include "utilities.h"
#include "internationalisation.h"
#include "parser.h"
#include "objects.h"

namespace vjassdoc
{

void Compiler::compile()
{
	if (!fileExists(Vjassdoc::compileFilePath()))
	{
		fprintf(stderr, _("File \"%s\" does not exist."), Vjassdoc::compileFilePath().c_str());
		return;
	}
	
	std::fstream fin(Vjassdoc::compileFilePath().c_str());
	
	//globals block
	while (fin.good())
	{
		std::string line;
		std::getline(fin, line);
		boost::tokenizer<> tok(line);
		
		if (tok.begin() != tok.end() && *tok.begin() == "globals")
		{
			std::getline(fin, line);
			
			if (fin.good())
				this->writeGlobals(fin);
		}
	}

}

void Compiler::writeGlobals(std::fstream &fstream)
{
	std::list<class Object*> list = Vjassdoc::getParser()->getSpecificList(Parser::Globals, Parser::Comparator());
	
	for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
	{
		//FIXME
		class Global *global = 0;//static_cast<class Object*>(*iterator);
	
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
		
		fstream << global->identifier();
	}
}

void Compiler::writeMembers(std::fstream &fstream)
{
}

void Compiler::writeMethodGlobals(std::fstream &fstream)
{
}

}
