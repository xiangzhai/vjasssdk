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

#ifndef VJASSDOC_PARSER_HPP
#define VJASSDOC_PARSER_HPP

#include <ostream>
#include <functional>
#include <string>
#include <sstream>
#include <vector>

#include <boost/filesystem.hpp>

namespace vjassdoc
{

class Object
{
	class List;
};

class SyntaxError;

/**
* Provides methods for parsing Jass and vJass files. The Parser class has the ability to create a simple HTML
* API documentation and/or a SQLite3 database.
* It contains a list for each Object child class.
* @class File is the class which handles real specific Jass and vJass parsing. Parser does create one or several File instances by getting all file paths from class @class Vjassdoc.
* After parsing all of those files it is able to create an HTML documentation and an SQLite3 database.
* Besides it provides several search functions which are required for object initialization which usually is runned after parsing for each parsed object.
* @see Vjassdoc, File
*/
class Parser
{
	public:
		struct Comparator : public std::binary_function<const class Object*, const class Object*, bool>
		{
			virtual bool operator()(const class Object *thisObject, const class Object *otherObject) const;
		};
	
		enum List
		{
			Comments,
			Keywords,
			Keys,
			TextMacros,
			TextMacroInstances,
			ExternalCalls,
			Types,
			Locals,
			Globals,
			Members,
			Parameters,
			FunctionInterfaces,
			Functions,
			Methods,
			Calls,
			Implementations,
			Hooks,
			Interfaces,
			Structs,
			Modules,
			Scopes,
			Libraries,
			SourceFiles,
			DocComments,
			MaxLists
		};
		
		enum SearchMode
		{
			Unspecified = 0, /// No specific search mode.
			CheckContainer = 0x02, /// Container has to be the same.
			CheckScope = 0x04,
			CheckLibrary = 0x08
		};

		
		/**
		* Sometimes it can be really useful to create your own small object list. In that case you'll probably want to be able to search in your custom list.
		* @param objectList User-defined custom list where the method has to search.
		* @param identifier Object identifier which is searched for.
		* @param searchMode Used search mode. Check out @enum Parser::SearchMode.
		* @param object Object instance used for comparisons. If searchMode is @enum Parser::SearchMode::Unspecified this value can be 0.
		* @return If no object was found it will return 0.
		* @see Parser::getSpecificList, Parser::searchObjectInLastDatabase, Parser::searchObjectInList
		*/
		static class Object* searchObjectInCustomList(const std::list<class Object*> &objectList, const std::string &identifier, const enum Parser::SearchMode &searchMode = Unspecified, const class Object *object = 0);

		Parser();
		~Parser();
		std::size_t parse(const boost::filesystem::path &path) throw (std::exception);
		void initObjects();
		void sortAlphabetically();
		void showSyntaxErrors();
		//void parse(const std::list<boost::filesystem::path> &filePaths);
#ifdef HTML		
		void createHtmlInheritanceListPage(std::ostream &ostream);
		void createHtmlRequirementListPage(std::ostream &ostream);
		void createHtmlUndocumentatedListPage(std::ostream &ostream);
		void createHtmlFiles(const boost::filesystem::path &dirPath, const std::string &title, bool pages, bool extraPages) throw (std::exception);
#endif
#ifdef SQLITE
		void createDatabase(const boost::filesystem::path &path);
		/**
		* @return Returns -1 if adding database fails.
		*/
		std::size_t addDatabase(const boost::filesystem::path &path);
		void removeDatabase(std::size_t);
		/**
		* Searches for an object with id @param id in last added database. Each id has to be unique so there only can be one hit.
		* @return If no object was found it will return 0.
		*/
		class Object* searchObjectDatabase(std::size_t index, std::size_t id);
#endif

		/**
		* Searches for an object in one of the parsers lists.
		* @param identifier Object identifier which is searched for.
		* @param list Parser list which is searched in. Check out @enum Parser::List.
		* @param searchMode Used search mode. Check out @enum Parser::SearchMode.
		* @param object Object instance used for comparisons. If searchMode is @enum Parser::SearchMode::Unspecified this value can be 0.
		* @return If no object was found it will return 0.
		*/
		class Object* searchObjectInList(const std::string &identifier, const enum Parser::List &list,  const enum Parser::SearchMode &searchMode = Unspecified, const class Object *object = 0);

		/**
		* Returns a user-specific list by iterating one of the parsers lists and calling a comparator.
		* @param list List which is iterated.
		* @param comparator Comparator which is used for comparisions. If it returns true object will be added to user-specific list.
		* @param object Comparators allows to compare two objects. If you don't have to compare two objects this value can be 0.
		* @return Returns the user-specific list.
		* @see Parser::searchObjectInCustomList
		*/
		std::list<const class Object*> getSpecificList(enum Parser::List list, const struct Comparator &comparator, const class Object *object = 0);
		
		void add(class Comment *comment);
		void add(class Keyword *keyword);
		void add(class Key *key);
		void add(class TextMacro *textMacro);
		void add(class TextMacroInstance *textMacroInstance);
		void add(class ExternalCall *externalCall);
		void add(class Type *type);
		void add(class Local *local);
		void add(class Global *global);
		void add(class Member *member);
		void add(class Parameter *parameter);
		void add(class FunctionInterface *functionInterface);
		void add(class Function *function);
		void add(class Method *method);
		void add(class Call *call);
		void add(class Implementation *implementation);
		void add(class Hook *hook);
		void add(class Interface *interface);
		void add(class Struct *usedStruct);
		void add(class Module *module);
		void add(class Scope *scope);
		void add(class Library *library);
		void add(class SourceFile *sourceFile);
		void add(class DocComment *docComment);
		class Type* integerType() const;
		class Type* realType() const;
		class Type* stringType() const;
		class Type* booleanType() const;
		class Type* handleType() const;
		class Type* codeType() const;
		class File* currentFile() const;

		void add(class SyntaxError *syntaxError);
		const class List* list(enum Parser::List list) const;

		/**
		* Parses line @param line from index @param index to the end of line and generates a list of possible objects.
		* This can be useful for IDEs or other editors to create an auto completion for their users.
		* @param line The code line.
		* @param index Start index of code line for the parser.
		* @return A list filled with possible objects.
		*/
		std::list<const class Object*> autoCompletion(const std::string &line, std::size_t &index);
		
	protected:
		void getStructInheritanceList(const class Interface *extension, const std::string &prefix, std::ostream &ostream);
		void getLibraryRequirementList(const class Library *requirement, const std::string &prefix, std::ostream &ostream);
		
		class Type *m_integerType;
		class Type *m_realType;
		class Type *m_stringType;
		class Type *m_booleanType;
		class Type *m_handleType;
		class Type *m_codeType;
		std::vector<class List*> m_lists;
		std::vector<class SyntaxError*> m_syntaxErrors;
#ifdef SQLITE
		std::vector<class List*> m_databaseLists;
#endif
};

inline bool Parser::Comparator::operator()(const class Object *thisObject, const class Object *otherObject) const
{
	return true;
}

inline void Parser::add(class Comment *comment)
{
	this->m_lists[Parser::Comments]->add(comment);
}

inline void Parser::add(class Key *key)
{
	this->m_lists[Parser::Keys]->add(key);
}

inline void Parser::add(class Keyword *keyword)
{
	this->m_lists[Parser::Keywords]->push_back(keyword);
}

inline void Parser::add(class TextMacro *textMacro)
{
	this->m_lists[Parser::TextMacros]->push_back(textMacro);
}

inline void Parser::add(class TextMacroInstance *textMacroInstance)
{
	this->m_lists[Parser::TextMacroInstances]->push_back(textMacroInstance);
}

inline void Parser::add(class ExternalCall *externalCall)
{
	this->m_lists[Parser::ExternalCalls]->push_back(externalCall);
}

inline void Parser::add(class Type *type)
{
	this->m_lists[Parser::Types]->push_back(type);
}

inline void Parser::add(class Local *local)
{
	this->m_lists[Parser::Locals]->push_back(local);
}

inline void Parser::add(class Global *global)
{
	this->m_lists[Parser::Globals]->push_back(global);
}

inline void Parser::add(class Member *member)
{
	this->m_lists[Parser::Members]->push_back(member);
}

inline void Parser::add(class Parameter *parameter)
{
	this->m_lists[Parser::Parameters]->push_back(parameter);
}

inline void Parser::add(class FunctionInterface *functionInterface)
{
	this->m_lists[Parser::FunctionInterfaces]->push_back(functionInterface);
}

inline void Parser::add(class Function *function)
{
	this->m_lists[Parser::Functions]->push_back(function);
}

inline void Parser::add(class Method *method)
{
	this->m_lists[Parser::Methods]->push_back(method);
}

inline void Parser::add(class Call *call)
{
	this->m_lists[Parser::Calls]->push_back(call);
}

inline void Parser::add(class Implementation *implementation)
{
	this->m_lists[Parser::Implementations]->push_back(implementation);
}

inline void Parser::add(class Hook *hook)
{
	this->m_lists[Parser::Hooks]->push_back(hook);
}

inline void Parser::add(class Interface *interface)
{
	this->m_lists[Parser::Interfaces]->push_back(interface);
}

inline void Parser::add(class Struct *usedStruct)
{
	this->m_lists[Parser::Structs]->push_back(usedStruct);
}

inline void Parser::add(class Module *module)
{
	this->m_lists[Parser::Modules]->push_back(module);
}

inline void Parser::add(class Scope *scope)
{
	this->m_lists[Parser::Scopes]->push_back(scope);
}

inline void Parser::add(class Library *library)
{
	this->m_lists[Parser::Libraries]->push_back(library);
}

inline void Parser::add(class SourceFile *sourceFile)
{
	this->m_lists[Parser::SourceFiles]->push_back(sourceFile);
}

inline void Parser::add(class DocComment *docComment)
{
	this->m_lists[Parser::DocComments]->push_back(docComment);
}

inline class Type* Parser::integerType() const
{
	return this->m_integerType;
}

inline class Type* Parser::realType() const
{
	return m_realType;
}

inline class Type* Parser::stringType() const
{
	return m_stringType;
}

inline class Type* Parser::booleanType() const
{
	return m_booleanType;
}

inline class Type* Parser::handleType() const
{
	return m_handleType;
}

inline class Type* Parser::codeType() const
{
	return m_codeType;
}

inline class File* Parser::currentFile() const
{
	return this->m_currentFile;
}

inline void Parser::add(class SyntaxError *syntaxError)
{
	this->m_syntaxErrors.push_back(syntaxError);
}

inline const class List* Parser::list(enum Parser::List list) const
{
	return this->m_lists[list];
}

}

#endif
