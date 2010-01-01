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

#ifndef WC3LIB_LANG_COMPILER_HPP
#define WC3LIB_LANG_COMPILER_HPP

#include <iostream>

#include "../exception.hpp"

namespace wc3lib
{
	
namespace lang
{

class Language;

class Compiler
{
	public:
		void compile(std::iostream &iostream, const class Parser &parser) throw (class Exception);
		/**
		* This method should only be called on Jass files. It removes unnecessary white-space characters and comments.
		* Besides functions are inlined and identifiers are shortend.
		*/
		void optimize(std::iostream &iostream) throw (class Exception);
	
	protected:
		
		/*
		void generateFunctionPrototypes();
		
		/**
		* Writes "the global hashtable". Jass++ only.
		*/
		/*
		void writeHashtableGlobal(std::ostream &ostream);
		void writeGlobals(std::ostream &ostream);
		void writeFunctions(std::ostream &ostream);
		
		/**
		* Jass++
		* Writes all globals (including member, function prototype and array globals etc.)
		*/
		/*
		void writePackageGlobals(std::ostream &ostream);
		void writePackageFunctions(std::ostream);
		
		
		std::list<class FunctionPrototype*> m_prototypes;
		*/
};

}

}

#endif
