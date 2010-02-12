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

#ifndef WC3LIB_LANG_LANGUAGE_HPP
#define WC3LIB_LANG_LANGUAGE_HPP

#include <string>
#include <iostream>

#include "object.hpp"

namespace wc3lib
{
	
namespace lang
{


/**
* Each language instance is a list of language object lists.
*/
class Language : public std::list<class Object::List>
{
	public:		
		Language();
		virtual ~Language();
		
		/**
		* @return Returns the real name of the language.
		*/
		virtual const std::string& name() const = 0;
		/**
		* @return Returns if the language is compatible to language @param language. Compatible means that the language can treat code of the other language as well as its own!
		*/		
		virtual bool compatibleTo(const Language &language) const = 0;
#ifdef HTML
		/**
		* Writes all objects categories entries (<objects category name> (number of objects)) into output stream @param ostream by using string prefix @param prefix for each entry.
		*/		
		virtual void writeObjectsCategories(std::ostream &ostream, const std::string &prefix = "") const = 0;
#endif
		/**
		* Prepares all objects and their links to each other. Afterwards they should be compilable.
		*/
		virtual void prepareObjects() = 0;
		/**
		* Writes objects into map script stream.
		*/
		virtual void writeObjects(std::iostream &iostream) = 0;
};

}

}

#endif
