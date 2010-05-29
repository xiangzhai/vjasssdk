/***************************************************************************
 *   Copyright (C) 2008 by Tamino Dauth                                    *
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

#ifndef WC3LIB_LANG_PARSER_HPP
#define WC3LIB_LANG_PARSER_HPP

#include <ostream>
#include <functional>
#include <string>
#include <sstream>
#include <list>
#include <map>

#include <boost/filesystem.hpp>

#include "object.hpp"
#include "syntaxerror.hpp"
#include "file.hpp"
#include "../exception.hpp"

namespace wc3lib
{
	
namespace lang
{

class Language;
class SyntaxError;
class Compiler;

/**
* Provides methods for parsing code files. The Parser class has the ability to create a simple HTML
* API documentation and/or an SQLite3 database.
* It contains lists which hold all parsed objects.
* @class File is the class which treats real specific code/syntax parsing. Parser does create one single @class File instance by getting a single file path.
* After parsing all of those files it is able to create an HTML documentation and an SQLite3 database.
* Besides it provides several search functions which are required for object initialization which usually is runned after parsing for each parsed object.
* @see File
*/
class Parser
{
	public:
		struct Comparator : public std::binary_function<const class Object*, const class Object*, bool>
		{
			virtual bool operator()(const class Object *thisObject, const class Object *otherObject) const;
		};
		
		Parser();
		~Parser();
		
		/**
		* Parses a single source file with given path @param path and adds it to the object list of language @param initialSourceFileLanguage.
		* @param initialSourceFileLanguage If this value is 0 parser's current language is used.
		*/
		std::size_t parse(const boost::filesystem::path &path, class Language *initialSourceFileLanguage = 0) throw (class Exception);
		std::size_t parse(const std::list<boost::filesystem::path> &paths, class Language *initialSourceFileLanguage = 0) throw (class Exception);
		void prepareObjects();
		void showSyntaxErrors(std::ostream &ostream);
#ifdef SQLITE
		/**
		* @return Returns -1 if adding database fails otherwise an unique index will be associated with the added database.
		*/
		std::size_t addDatabase(const boost::filesystem::path &path);
		void removeDatabase(std::size_t index);
#endif

		void addSyntaxError(class SyntaxError *syntaxError);
		void addSyntaxError(const std::string &message);
		
		/**
		* @return Returns current parsed code file.
		*/
		const class File& file() const;
		const std::map<std::string, class Language*>& languages() const;
		/**
		* Use this to set the parser's initial scripting language (used for source files etc.).
		*/
		void setCurrentLanguage(class Language *language);
		const class Language& currentLanguage() const;
		const std::list<class SyntaxError*>& syntaxErrors() const;
		
		/**
		* Searches for object with identifier @param identifier in all languages lists.
		* @return If no object was found it returns 0.
		*/
		class Object* findObject(const std::string &identifier);
		class Object* findObject(const class Language &language, const std::string &identifier);

		/**
		* Parses line @param line from index @param index to the end of line and generates a list of possible objects.
		* This can be useful for IDEs or other editors to create an auto completion for their users.
		* @param line The code line.
		* @param index Start index of code line for the parser.
		* @return A list filled with possible objects.
		*/
		std::list<const class Object*> autoCompletion(const std::string &line, std::size_t &index);
		
	protected:
		friend class File;
		friend class Compiler;
		
#ifdef HTML
		void getStructInheritanceList(const class Interface *extension, const std::string &prefix, std::ostream &ostream);
		void getLibraryRequirementList(const class Library *requirement, const std::string &prefix, std::ostream &ostream);
#endif
		
		class File *m_file;
		std::map<std::string, class Language*> m_languages; // languages hold all parsed objects
		class Language *m_currentLanguage;
		std::list<class SyntaxError*> m_syntaxErrors;
/*
#ifdef SQLITE
		std::list<class List*> m_databaseLists;
#endif
*/
};

inline bool Parser::Comparator::operator()(const class Object *thisObject, const class Object *otherObject) const
{
	return true;
}

inline void Parser::addSyntaxError(class SyntaxError *syntaxError)
{
	this->m_syntaxErrors.push_back(syntaxError);
}

inline void Parser::addSyntaxError(const std::string &message)
{
	this->addSyntaxError(new SyntaxError(this->m_file->m_sourceFile, this->m_file->lines() - 1, message));
}
	
inline const class File& Parser::file() const
{
	return *this->m_file;
}

inline const std::map<std::string, class Language*>& Parser::languages() const
{
	return this->m_languages;
}

inline const class Language& Parser::currentLanguage() const
{
	return *this->m_currentLanguage;
}

inline const std::list<class SyntaxError*>& Parser::syntaxErrors() const
{
	return this->m_syntaxErrors;
}

}

}

#endif
