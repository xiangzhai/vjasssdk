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

#ifndef VJASSDOC_PARSER_H
#define VJASSDOC_PARSER_H

#include <functional>
#include <list>
#include <string>
#include <sstream>
#include <vector>

namespace vjassdoc
{

class Comment;
class Keyword;
class TextMacro;
class TextMacroInstance;
class Type;
class Local;
class Global;
class Member;
class Parameter;
class FunctionInterface;
class Function;
class Method;
class Implementation;
class Interface;
class Struct;
class Module;
class Scope;
class Library;
class SourceFile;
class DocComment;
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
			Types,
			Locals,
			Globals,
			Members,
			Parameters,
			FunctionInterfaces,
			Functions,
			Methods,
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
			CheckLibrary = 0x06
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
		void createInheritanceListPage();
		void createRequirementListPage();
		void parse();

#ifdef SQLITE
		int addDatabase(const char *filePath);
		void removeDatabase(const int &index);
		/**
		* Searches for an object with id @param id in last added database. Each id has to be unique so there only can be one hit.
		* @return If no object was found it will return 0.
		*/
		class Object* searchObjectInLastDatabase(const unsigned int &id);
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
		std::list<class Object*> getSpecificList(const enum List &list, const struct Comparator &comparator, const class Object *object = 0);
		
		void add(class Comment *comment);
		void add(class Keyword *keyword);
		void add(class Key *key);
		void add(class TextMacro *textMacro);
		void add(class TextMacroInstance *textMacroInstance);
		void add(class Type *type);
		void add(class Local *local);
		void add(class Global *global);
		void add(class Member *member);
		void add(class Parameter *parameter);
		void add(class FunctionInterface *functionInterface);
		void add(class Function *function);
		void add(class Method *method);
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
		class SourceFile* currentSourceFile() const;

		void add(class SyntaxError *syntaxError);
	private:
#ifdef SQLITE
		struct Database
		{
			std::list<enum List> listList;
			std::list<class Object*> objectList;
		};
#endif	

		static const char *title[Parser::MaxLists];
		class Type *m_integerType;
		class Type *m_realType;
		class Type *m_stringType;
		class Type *m_booleanType;
		class Type *m_handleType;
		class Type *m_codeType;
		std::list<class Comment*> commentList;
		std::list<class Keyword*> keywordList;
		std::list<class Key*> keyList;
		std::list<class TextMacro*> textMacroList;
		std::list<class TextMacroInstance*> textMacroInstanceList;
		std::list<class Type*> typeList;
		std::list<class Local*> localList;
		std::list<class Global*> globalList;
		std::list<class Member*> memberList;
		std::list<class Parameter*> parameterList;
		std::list<class FunctionInterface*> functionInterfaceList;
		std::list<class Function*> functionList;
		std::list<class Method*> methodList;
		std::list<class Implementation*> implementationList;
		std::list<class Hook*> hookList;
		std::list<class Interface*> interfaceList;
		std::list<class Struct*> structList;
		std::list<class Module*> moduleList;
		std::list<class Scope*> scopeList;
		std::list<class Library*> libraryList;
		std::list<class SourceFile*> sourceFileList;
		std::list<class DocComment*> docCommentList;
		class SourceFile *m_currentSourceFile;
		std::list<class SyntaxError*> syntaxErrorList;
#ifdef SQLITE
		std::vector<struct Database*> databaseVector;
#endif

		std::list<class Object*>& getList(const enum List &list);
		
		//HTML
		std::stringstream& addObjectList(std::stringstream &output, const enum Parser::List &list);
		
		//SQLite
#ifdef SQLITE
		static std::string getTableName(const enum Parser::List &list);
		static unsigned int getTableColumns(const enum Parser::List &list);
		static std::string getTableCreationStatement(const enum Parser::List &list);
		static class Object* createObjectByVector(std::vector<const unsigned char*> &columnVector, const enum Parser::List &list);
#endif

		void getStructInheritanceList(const class Interface *extension, const std::string &prefix, std::stringstream &sstream);
		void getLibraryRequirementList(const class Library *requirement, const std::string &prefix, std::stringstream &sstream);
};

inline void Parser::add(class Comment *comment)
{
	this->commentList.push_back(comment);
}

inline void Parser::add(class Key *key)
{
	this->keyList.push_back(key);
}

inline void Parser::add(class Keyword *keyword)
{
	this->keywordList.push_back(keyword);
}

inline void Parser::add(class TextMacro *textMacro)
{
	this->textMacroList.push_back(textMacro);
}

inline void Parser::add(class TextMacroInstance *textMacroInstance)
{
	this->textMacroInstanceList.push_back(textMacroInstance);
}

inline void Parser::add(class Type *type)
{
	this->typeList.push_back(type);
}

inline void Parser::add(class Local *local)
{
	this->localList.push_back(local);
}

inline void Parser::add(class Global *global)
{
	this->globalList.push_back(global);
}

inline void Parser::add(class Member *member)
{
	this->memberList.push_back(member);
}

inline void Parser::add(class Parameter *parameter)
{
	this->parameterList.push_back(parameter);
}

inline void Parser::add(class FunctionInterface *functionInterface)
{
	this->functionInterfaceList.push_back(functionInterface);
}

inline void Parser::add(class Function *function)
{
	this->functionList.push_back(function);
}

inline void Parser::add(class Method *method)
{
	this->methodList.push_back(method);
}

inline void Parser::add(class Implementation *implementation)
{
	this->implementationList.push_back(implementation);
}

inline void Parser::add(class Hook *hook)
{
	this->hookList.push_back(hook);
}

inline void Parser::add(class Interface *interface)
{
	this->interfaceList.push_back(interface);
}

inline void Parser::add(class Struct *usedStruct)
{
	this->structList.push_back(usedStruct);
}

inline void Parser::add(class Module *module)
{
	this->moduleList.push_back(module);
}

inline void Parser::add(class Scope *scope)
{
	this->scopeList.push_back(scope);
}

inline void Parser::add(class Library *library)
{
	this->libraryList.push_back(library);
}

inline void Parser::add(class SourceFile *sourceFile)
{
	this->sourceFileList.push_back(sourceFile);
}

inline void Parser::add(class DocComment *docComment)
{
	this->docCommentList.push_back(docComment);
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

inline class SourceFile* Parser::currentSourceFile() const
{
	return this->m_currentSourceFile;
}

inline void Parser::add(class SyntaxError *syntaxError)
{
	this->syntaxErrorList.push_back(syntaxError);
}

}

#endif
