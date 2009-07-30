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

#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#ifdef SQLITE
#include <sqlite3.h>
#include "utilities.h"
#endif
#include <sstream>

#include "parser.h"
#include "internationalisation.h"
#include "objects.h"
#include "vjassdoc.h"
#include "file.h"
#include "syntaxerror.h"

namespace vjassdoc
{

const char *Parser::title[Parser::MaxLists] =
{
	_("Comments"),
	_("Keywords"),
	_("Keys"),
	_("Text Macros"),
	_("Text Macro Instances"),
	_("Types"),
	_("Locals"),
	_("Globals"),
	_("Members"),
	_("Parameters"),
	_("Function Interfaces"),
	_("Functions"),
	_("Methods"),
	_("Implementations"),
	_("Hooks"),
	_("Interfaces"),
	_("Structs"),
	_("Modules"),
	_("Scopes"),
	_("Libraries"),
	_("Source Files"),
	_("Documentation Comments")
};

bool Parser::Comparator::operator()(const class Object *thisObject, const class Object *otherObject) const
{
	return true;
}

class Object* Parser::searchObjectInCustomList(const std::list<class Object*> &objectList, const std::string &identifier, const enum Parser::SearchMode &searchMode, const class Object *object)
{
	if (objectList.size() == 0)
		return 0;

	class Object *resultObject = 0;
	bool checkContainer = false;
	bool checkScope = false;
	bool checkLibrary = false;
	
	if ((object != 0 && object->container() != 0) || searchMode & CheckContainer)
		checkContainer = true;
	
	if ((object != 0 && object->scope() != 0) || searchMode & CheckScope)
		checkScope = true;
	
	if ((object != 0 && object->library() != 0) || searchMode & checkLibrary)
		checkLibrary = true;

	for (std::list<class Object*>::const_iterator iterator = objectList.begin(); iterator != objectList.end(); ++iterator)
	{
		if ((*iterator)->identifier() == identifier)
		{
			if (resultObject == 0 && searchMode == Unspecified)
			{
				resultObject = *iterator;
				
				if (checkContainer)
				{
					if (resultObject->container() == object->container())
						checkContainer = false;
				}
				
				if (checkScope)
				{
					if (resultObject->scope() == object->scope())
						checkScope = false;
				}
				
				if (checkLibrary)
				{
					if (resultObject->library() == object->library())
						checkLibrary = false;
				}
				
				//found the object
				if (!checkContainer && !checkScope && !checkLibrary)
					break;
				
				continue;
			}
			
			
			if (checkContainer)
			{
				if ((*iterator)->container() == object->container())
				{
					checkContainer = false;
					resultObject = *iterator;
					
					continue;
				}
			}
			
			if (checkScope)
			{
				if ((*iterator)->scope() == object->container())
				{
					checkScope = false;
					resultObject = *iterator;
					
					continue;
				}
			}
			
			if (checkLibrary)
			{
				if ((*iterator)->library() == object->library())
				{
					checkLibrary = false;
					resultObject = *iterator;
					
					continue;
				}
			}
		}
	}

	return resultObject;
}

Parser::Parser() :
		m_integerType(new Type("integer", 0, 0, 0, "", "")),
		m_realType(new Type("real", 0, 0, 0, "", "")),
		m_stringType(new Type("string", 0, 0, 0, "", "")),
		m_booleanType(new Type("boolean", 0, 0, 0, "", "")),
		m_handleType(new Type("handle", 0, 0, 0, "", "")),
		m_codeType(new Type("code", 0, 0, 0, "", ""))
{
#ifdef SQLITE
	if (!Vjassdoc::getDatabases().empty())
	{
		if (Vjassdoc::showVerbose())
			std::cout << _("You've selected one or several databases.") << std::endl;
		
		std::list<std::string> databases = Vjassdoc::getDatabases();

		for (std::list<std::string>::iterator iterator = databases.begin(); iterator != databases.end(); ++iterator)
			this->addDatabase((*iterator).c_str());
	}
#endif
	
	//add default types
	if (Vjassdoc::showVerbose())
		std::cout << _("Adding default Jass types.") << std::endl;

	this->add(m_integerType);
	this->add(m_realType);
	this->add(m_stringType);
	this->add(m_booleanType);
	this->add(m_handleType);
	this->add(m_codeType);
	std::list<std::string> list = Vjassdoc::getFilePaths();

	for (std::list<std::string>::const_iterator iterator = list.begin(); iterator != list.end(); ++iterator)
	{
		std::string identifier = *iterator;
		int position;
		
		position = identifier.find_last_of(Vjassdoc::dirSeparator);
		
		if (position != std::string::npos)
			identifier = identifier.substr(position + 1);

		this->add(new SourceFile(identifier, *iterator));
	}
}

//Default Jass types are in lists!
Parser::~Parser()
{

	for (std::list<class Comment*>::iterator iterator = this->commentList.begin(); iterator != this->commentList.end(); ++iterator)
		delete *iterator;

	this->commentList.clear();

	for (std::list<class Keyword*>::iterator iterator = this->keywordList.begin(); iterator != this->keywordList.end(); ++iterator)
		delete *iterator;

	this->keywordList.clear();

	for (std::list<class TextMacro*>::iterator iterator = this->textMacroList.begin(); iterator != this->textMacroList.end(); ++iterator)
		delete *iterator;

	this->textMacroList.clear();

	for (std::list<class TextMacroInstance*>::iterator iterator = this->textMacroInstanceList.begin(); iterator != this->textMacroInstanceList.end(); ++iterator)
		delete *iterator;

	this->textMacroInstanceList.clear();

	for (std::list<class Type*>::iterator iterator = this->typeList.begin(); iterator != this->typeList.end(); ++iterator)
		delete *iterator;

	this->typeList.clear();

	for (std::list<class Global*>::iterator iterator = this->globalList.begin(); iterator != this->globalList.end(); ++iterator)
		delete *iterator;

	this->globalList.clear();

	for (std::list<class Member*>::iterator iterator = this->memberList.begin(); iterator != this->memberList.end(); ++iterator)
		delete *iterator;

	this->memberList.clear();

	for (std::list<class Parameter*>::iterator iterator = this->parameterList.begin(); iterator != this->parameterList.end(); ++iterator)
		delete *iterator;

	this->parameterList.clear();

	for (std::list<class FunctionInterface*>::iterator iterator = this->functionInterfaceList.begin(); iterator != this->functionInterfaceList.end(); ++iterator)
		delete *iterator;

	this->functionInterfaceList.clear();

	for (std::list<class Function*>::iterator iterator = this->functionList.begin(); iterator != this->functionList.end(); ++iterator)
		delete *iterator;

	this->functionList.clear();

	for (std::list<class Method*>::iterator iterator = this->methodList.begin(); iterator != this->methodList.end(); ++iterator)
		delete *iterator;

	this->methodList.clear();

	for (std::list<class Implementation*>::iterator iterator = this->implementationList.begin(); iterator != this->implementationList.end(); ++iterator)
		delete *iterator;

	this->implementationList.clear();

	for (std::list<class Interface*>::iterator iterator = this->interfaceList.begin(); iterator != this->interfaceList.end(); ++iterator)
		delete *iterator;

	this->interfaceList.clear();

	for (std::list<class Struct*>::iterator iterator = this->structList.begin(); iterator != this->structList.end(); ++iterator)
		delete *iterator;

	this->structList.clear();

	for (std::list<class Scope*>::iterator iterator = this->scopeList.begin(); iterator != this->scopeList.end(); ++iterator)
		delete *iterator;

	this->scopeList.clear();

	for (std::list<class Library*>::iterator iterator = this->libraryList.begin(); iterator != this->libraryList.end(); ++iterator)
		delete *iterator;

	this->libraryList.clear();

	for (std::list<class SourceFile*>::iterator iterator = this->sourceFileList.begin(); iterator != this->sourceFileList.end(); ++iterator)
		delete *iterator;

	this->sourceFileList.clear();

	for (std::list<class DocComment*>::iterator iterator = this->docCommentList.begin(); iterator != this->docCommentList.end(); ++iterator)
		delete *iterator;

	this->docCommentList.clear();
	
#ifdef SQLITE
	for (std::vector<struct Database*>::iterator iterator = this->databaseVector.begin(); iterator != this->databaseVector.end(); ++iterator)
		delete *iterator;

	this->databaseVector.clear();
#endif
}

void Parser::createInheritanceListPage()
{
	//inheritance list
	std::stringstream sstream;
	sstream
	<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	<< "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
	<< "\t\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
	<< "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"" << _("en") << "\">\n"
	<< "<html>\n"
	<< "\t<head>\n"
	<< "\t\t<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/>\n"
	<< "\t\t<title>" << _("Inheritance List") << "</title>\n"
	<< "\t\t<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\"/>\n"
	<< "\t</head>\n"
	<< "\t<body>\n"
	<< "\t\t<a href=\"index.html\">" << _("Return to start page") << "</a>\n"
	<< "\t\t<h1>" << _("Inheritance List") << "</h1>\n"
	<< "\t\t<ul>\n"
	<< "\t\t\t<li><a href=\"#Interfaces\">"	<< _("Interfaces") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Structs\">"	<< _("Structs") << "</a></li>\n"
	<< "\t\t</ul>\n"
	<< "\t\t<h2><a name=\"Interfaces\">" << _("Interfaces") << "</a></h2>\n"
	;

	if (!this->interfaceList.empty())
	{
		sstream << "\t\t<ul>\n";

		for (std::list<class Interface*>::iterator iterator = this->interfaceList.begin(); iterator != this->interfaceList.end(); ++iterator)
		{
			sstream << "\t\t\t<li>" << (*iterator)->pageLink() << '\n';
			this->getStructInheritanceList(*iterator, "\t\t\t\t", sstream);
			sstream << "\t\t\t</li>\n";
		}

		sstream << "\t\t</ul>\n";
	}
	else
		sstream << "\t\t<p>-</p>\n";

	sstream
	<< "\t\t<h2><a name=\"Structs\">" << _("Structs") << "</a></h2>\n";

	if (!this->structList.empty())
	{
		sstream << "\t\t<ul>\n";

		for (std::list<class Struct*>::iterator iterator = this->structList.begin(); iterator != this->structList.end(); ++iterator)
		{
			if ((*iterator)->extension() == 0)
			{
				sstream << "\t\t\t<li>" << (*iterator)->pageLink() << '\n';
				this->getStructInheritanceList(*iterator, "\t\t\t\t", sstream);
				sstream << "\t\t\t</li>\n";
			}
		}

		sstream << "\t\t</ul>\n";
	}
	else
		sstream << "\t\t<p>-</p>\n";

	sstream
	<< "\t</body>\n"
	<< "</html>\n"
	;
	std::ofstream fout((Vjassdoc::getDir() + Vjassdoc::dirSeparator + "inheritancelist.html").c_str());

	if (!fout.good())
	{
		fout.close();
		std::cerr << _("Was unable to create file \"inheritancelist.html\".") << std::endl;
		return;
	}

	fout << sstream.str();
	fout.close();
}

void Parser::createRequirementListPage()
{
	//requirement list
	std::stringstream sstream;
	sstream
	<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	<< "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
	<< "\t\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
	<< "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"" << _("en") << "\">\n"
	<< "<html>\n"
	<< "\t<head>\n"
	<< "\t\t<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/>\n"
	<< "\t\t<title>" << _("Requirement List") << "</title>\n"
	<< "\t\t<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\"/>\n"
	<< "\t</head>\n"
	<< "\t<body>\n"
	<< "\t\t<a href=\"index.html\">" << _("Return to start page") << "</a>\n"
	<< "\t\t<h1>" << _("Requirement List") << "</h1>\n"
	<< "\t\t<ul>\n"
	;

	for (std::list<class Library*>::iterator iterator = this->libraryList.begin(); iterator != this->libraryList.end(); ++iterator)
	{
		//requirement has to be 0
		//if ((*iterator)->requirement() != 0)
			//continue;

		sstream << "\t\t\t<li>" << (*iterator)->pageLink() << '\n';
		this->getLibraryRequirementList(*iterator, "\t\t\t\t", sstream);
		sstream << "\t\t\t</li>\n";
	}

	sstream
	<< "\t\t</ul>\n"
	<< "\t</body>\n"
	<< "</html>\n"
	;
	std::ofstream fout((Vjassdoc::getDir() + Vjassdoc::dirSeparator + "requirementlist.html").c_str());

	if (!fout.good())
	{
		fout.close();
		std::cerr << _("Was unable to create file \"requirementlist.html\".") << std::endl;
		return;
	}

	fout << sstream.str();
	fout.close();
}

void Parser::parse()
{
	for (std::list<class SourceFile*>::const_iterator iterator = this->sourceFileList.begin(); iterator != this->sourceFileList.end(); ++iterator)
	{
		this->m_currentSourceFile = *iterator;
		File file((*iterator)->path());
	}

	//objects should be initialized before using them
	for (int i = 0; i < Parser::MaxLists; ++i)
	{
		if (!Vjassdoc::shouldParseObjectsOfList(Parser::List(i)))
			continue;

		std::list<class Object*> list = this->getList(Parser::List(i));

		for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
		{
#ifdef SQLITE
/*
			bool result = false;

			//if is contained by any database do not init!!!!
			for (std::vector<struct Database*>::iterator databaseIterator = this->databaseVector.end(); databaseIterator != this->databaseVector.end() && !result; ++databaseIterator)
			{
				for (std::list<class Object*>::iterator objectIterator = (*databaseIterator)->objectList.begin(); objectIterator != (*databaseIterator)->objectList.end() && !result; ++objectIterator)
				{
					if (*objectIterator == *iterator)
						result = true;
				}
			}

			if (result)
			{
				std::cout << "Do not initializer object. It was added by database." << std::endl;
				continue;
			}
*/
#endif

			(*iterator)->init();
		}
	}

	//alphabetical sort
	if (Vjassdoc::sortAlphabetically())
	{
		if (Vjassdoc::showVerbose())
			std::cout << _("Sorting alphabetically.") << std::endl;

		for (int i = 0; i < MaxLists; ++i)
		{
			if (!Vjassdoc::shouldParseObjectsOfList(Parser::List(i)))
				continue;

			this->getList(Parser::List(i)).sort(Object::AlphabeticalComparator());
		}
	}

	//create HTML file
	if (Vjassdoc::saveAsHtml())
	{
		if (Vjassdoc::showVerbose())
			std::cout << _("Creating HTML file.") << std::endl;
	
		std::string title = Vjassdoc::getTitle();
		std::stringstream sout;
		sout
		<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
		<< "\t\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
		<< "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"" << _("en") << "\">\n"
		<< "<html>\n"
		<< "\t<head>\n"
		<< "\t\t<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/>\n"
		<< "\t\t<title>" << title << "</title>\n"
		<< "\t\t<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\"/>\n"
		<< "\t</head>\n"
		<< "\t<body>\n"
		<< "\t\t<h1>" << title << "</h1>\n";
		
		char buffer[50];
		sprintf(buffer, _("Generated by vjassdoc %s."), Vjassdoc::version);

		sout
		<< "\t\t<p>" << buffer << "</p>\n"
		<< "\t\t<ul>\n"
		<< std::endl;

		for (int i = 0; i < Parser::MaxLists; ++i)
		{
			if (!Vjassdoc::shouldParseObjectsOfList(Parser::List(i)) || this->getList(Parser::List(i)).empty())
				continue;

			sout << "\t\t\t<li><a href=\"#" << Parser::title[i] << "\">" << Parser::title[i] << " (" << this->getList(Parser::List(i)).size() << ")</a></li>" << std::endl;
		}

		if (Vjassdoc::createSpecialPages())
		{
			sout
			<< "\t\t\t<li>" << "<a href=\"inheritancelist.html\">" << _("Inheritance List") << "</a></li>\n"
			<< "\t\t\t<li>" << "<a href=\"requirementlist.html\">" << _("Requirement List") << "</a></li>\n"
			<< "\t\t\t<li>" << "<a href=\"#Undocumented Objects\">" << _("Undocumented Objects") << "</a></li>\n"
			//<< "\t\t\t<li>" << "<a href=\"#Authors\">" << _("Authors") << "</a></li>\n"
			//<< "\t\t\t<li>" << "<a href=\"#Todos\">" << _("Todos") << "</a></li>\n"
			//<< "\t\t\t<li>" << "<a href=\"#States\">" << _("States") << "</a></li>\n"
			//<< "\t\t\t<li>" << "<a href=\"#Sources\">" << _("Sources") << "</a></li>\n"
			;
		}
		
		sout << "\t\t</ul>\n";

		//Für alle Objekte werden Link-Listen erstellt. Dann werden die Objekte in Dateien aufgeteilt, die im Index verlinkt werden.
		for (int i = 0; i < Parser::MaxLists; ++i)
		{
			if (!Vjassdoc::shouldParseObjectsOfList(Parser::List(i)) || this->getList(Parser::List(i)).empty())
				continue;

			sout
			<< "\t\t<h2><a name=\"" <<  Parser::title[i] << "\">" << Parser::title[i] << " (" << this->getList(Parser::List(i)).size() << ")</a></h2>" << std::endl;
			this->addObjectList(sout, List(i));
		}

		if (Vjassdoc::createSpecialPages())
		{
			this->createInheritanceListPage();
			//this->createRequirementListPage();

			sout
			<< "\t\t<h2><a name=\"Undocumented Objects\">" << _("Undocumented Objects") << "</h2>\n"
			<< "\t\t<ul>\n"
			;
		
			for (int i = 0; i < Parser::MaxLists; ++i)
			{
				if (Parser::List(i) == Parser::SourceFiles || Parser::List(i) == Parser::DocComments || !Vjassdoc::shouldParseObjectsOfList(Parser::List(i)))
					continue;
			
				std::list<class Object*> list = this->getList(Parser::List(i));
			
				for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
				{
					if ((*iterator)->docComment() == 0)
						sout << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";
				}
			}
	
			sout
			<< "\t\t</ul>\n"
			;
			/// @todo Implement these special pages:
/*
			<< "\t\t\t<li>" << "<a href=\"#Authors\">" << _("Authors") << "</a></li>\n"
			<< "\t\t\t<li>" << "<a href=\"#Todos\">" << _("Todos") << "</a></li>\n"
			<< "\t\t\t<li>" << "<a href=\"#States\">" << _("States") << "</a></li>\n"
			<< "\t\t\t<li>" << "<a href=\"#Sources\">" << _("Sources") << "</a></li>\n"
*/
		}
		
		sout
		<< "\t</body>" << std::endl
		<< "</html>";
	
		//write output
		std::ofstream fout((Vjassdoc::getDir() + Vjassdoc::dirSeparator + "index.html").c_str());
		fout << sout.str();
		fout.close();
		
		//create pages
		if (Vjassdoc::createPages())
		{
			for (int i = 0; i < Parser::MaxLists; ++i)
			{
				if (!Vjassdoc::shouldParseObjectsOfList(Parser::List(i)))
					continue;
				
				std::list<class Object*> list = this->getList(Parser::List(i));
				
				for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
				{
					std::ostringstream sstream;
					sstream << Vjassdoc::getDir() << '/' << (*iterator)->id() << ".html";
					std::ofstream fout(sstream.str().c_str());
					
					fout
					<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<< "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
					<< "\t\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
					<< "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"" << _("en") << "\">\n"
					<< "<html>\n"
					<< "\t<head>\n"
					<< "\t\t<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/>\n"
					<< "\t\t<title>" << (*iterator)->identifier() << "</title>\n"
					<< "\t\t<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\"/>\n"
					<< "\t</head>\n"
					<< "\t<body>\n"
					<< "\t\t<a href=\"index.html\">" << _("Return to start page") << "</a>\n"
					<< "\t\t<h1>" << (*iterator)->identifier() << "</h1>\n"
					<< "\t\t<ul>\n"
					;
					(*iterator)->pageNavigation(fout);
					fout << "\t\t</ul>\n";
					(*iterator)->page(fout);
					fout
					<< "\t</body>\n"
					<< "</html>\n";
					fout.close();
				}
			}
		}
	}

	if (Vjassdoc::checkSyntax())
	{
		/// @todo compare all object identifiers with Jass and vJass keywords.

		fprintf(stdout, _("%d syntax errors.\n"), this->syntaxErrorList.size());
		this->syntaxErrorList.sort(SyntaxError::Comparator);
		
		for (std::list<class SyntaxError*>::iterator iterator = this->syntaxErrorList.begin(); iterator != this->syntaxErrorList.end(); ++iterator)
		{
			fprintf(stdout, _("File %s, line %d: %s\n"), (*iterator)->sourceFile()->identifier().c_str(), (*iterator)->line(), (*iterator)->message().c_str());
		}
	}

#ifdef SQLITE
	//create SQL database for search functions
	if (Vjassdoc::createDatabase())
	{
		if (Vjassdoc::showVerbose())
			printf(_("Creating database.\nUsing SQLite version %s.\n"), SQLITE_VERSION);

		std::string fileName = Vjassdoc::getDir() + Vjassdoc::dirSeparator + "database.db";
		
		if (fileExists(fileName))
		{
			printf(_("Database \"%s\" does already exist. Do you want to replace it by the newer one?\n"), fileName.c_str());
			std::cout << _("Answer possiblities: y, yes, n, no.\n");
			
			std::string answer;
			
			do
			{
				std::cin >> answer;
				
				if (answer == _("y") || answer == _("yes"))
				{
					if (remove(fileName.c_str()) != 0)
					{
						std::cout << _("Was unable to replace old database.") << std::endl;
						return;
					}
					
					break;
				}
				else if (answer == _("n") || answer == _("no"))
				{
					std::cout << _("Canceled database creation.") << std::endl;
					return;
				}
				else
					std::cout << _("Unknown answer.") << std::endl;
			}
			while (true);
		}
		
		sqlite3 *database;
		int state = sqlite3_open(fileName.c_str(), &database);
		
		if (state == SQLITE_OK)
		{
			char *message = 0;
			state = sqlite3_exec(database, "BEGIN TRANSACTION", 0, 0, &message);
			
			if (state != SQLITE_OK)
			{
				fprintf(stderr, _("Was unable to begin transaction.\nState: %d.\nMessage: %s\n"), state, message);
				sqlite3_free(message);
			}
		
			for (int i = 0; i < Parser::MaxLists; ++i)
			{
				if (!Vjassdoc::shouldParseObjectsOfList(Parser::List(i)))
					continue;

				std::cout << "Table list " << i << std::endl;
				std::cout << "Table creation statement: " << this->getTableCreationStatement(Parser::List(i)).c_str() << std::endl;
				state = sqlite3_exec(database, this->getTableCreationStatement(Parser::List(i)).c_str(), 0, 0, &message);
				std::cout << "After table creation" << std::endl;

				if (state != SQLITE_OK)
				{
					fprintf(stderr, _("Was unable to create table \"%s\" from list %d.\nState: %d.\nMessage: %s\n"), Parser::getTableName(Parser::List(i)).c_str(), i, state, message);
					sqlite3_free(message);
				}

				std::list<class Object*> list = this->getList(Parser::List(i));

				for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
				{
					std::ostringstream sstream;
					sstream << "INSERT INTO " << Parser::getTableName(Parser::List(i)) << " (Id) VALUES (" << (*iterator)->id() << ')';
					
					state = sqlite3_exec(database, sstream.str().c_str(), 0, 0, &message);

					std::cerr << "Execution " << sstream.str().c_str() << std::endl;

					if (state != SQLITE_OK)
					{
						fprintf(stderr, _("Was unable to insert id of list %d into table \"%s\".\nState: %d.\nMessage: %s\n"), i, Parser::getTableName(Parser::List(i)).c_str(), state, message);
						sqlite3_free(message);
					}
			
					sstream.str("");
					sstream.clear();

					std::cout << "Before update of object " << (*iterator)->identifier() << std::endl;
					sstream << "UPDATE " << Parser::getTableName(Parser::List(i)).c_str() << " SET " << (*iterator)->sqlStatement() << " WHERE Id=" << (*iterator)->id();
					std::cout << "After update" << std::endl;
					//std::cout << "Execution command: " << sstream.str().c_str() << std::endl; //NOTE debug


					state = sqlite3_exec(
							database, /* An open database */
							sstream.str().c_str(), /* SQL to be evaluated */ //FIXME
							0, //int (*callback)(void*,int,char**,char**), /* Callback function */
							0, //void *, /* 1st argument to callback */
							&message /* Error msg written here */
					);
					
					if (state != SQLITE_OK)
					{
						fprintf(stderr, _("Was unable to run execution \"%s\" of table \"%s\" from list %d.\nState: %d.\nMessage: %s\n"), sstream.str().c_str(), Parser::getTableName(Parser::List(i)).c_str(), i, state, message);
						sqlite3_free(message);
					}
				}
			}
			
			state = sqlite3_exec(database, "COMMIT", 0, 0, &message);
			
			if (state != SQLITE_OK)
			{
				fprintf(stderr, _("Was unable to commit.\nState: %d.\nMessage: %s\n"), state, message);
				sqlite3_free(message);
			}
		}
		else
			fprintf(stderr, _("Was unable to create database. State %d.\n"), state);
		
		sqlite3_close(database);
	}
#endif
}

#ifdef SQLITE
int Parser::addDatabase(const char *filePath)
{
	if (Vjassdoc::showVerbose())
		printf(_("Load database %s.\n"), filePath);

	int result = -1;
	sqlite3 *database;
	int state = sqlite3_open_v2(filePath, &database, SQLITE_OPEN_READONLY, NULL);

	if (state == SQLITE_OK)
	{
		Parser::Database *databaseStruct = new Parser::Database;
		result = this->databaseVector.size();
		this->databaseVector.push_back(databaseStruct);
	
		for (int i = 0; i < Parser::MaxLists; ++i)
		{
			if (!Vjassdoc::shouldParseObjectsOfList(Parser::List(i)))
				continue;

			std::cout << "List " << i << std::endl;
			std::ostringstream sstream;
			sstream << "SELECT * FROM " << Parser::getTableName(Parser::List(i));
			sqlite3_stmt *statement;
			std::cout << "Before statement " << sstream.str() << std::endl;
			state = sqlite3_prepare_v2(
				database,            /* Database handle */
				sstream.str().c_str(),       /* SQL statement, UTF-8 encoded */
				-1,              /* Maximum length of zSql in bytes. */
				&statement,  /* OUT: Statement handle */
				0);     /* OUT: Pointer to unused portion of zSql */

			std::cout << "After statement " << sstream.str() << std::endl;

			if (state != SQLITE_OK)
				fprintf(stderr, _("Was unable to prepare SQL statement of table %s.\nState %d.\n"),  Parser::getTableName(Parser::List(i)).c_str(), state);

			if (statement == NULL)
			{
				fprintf(stderr, _("SQL statement of table %s is NULL.\n"), Parser::getTableName(Parser::List(i)).c_str());
				std::cout << "After fprintf" << std::endl;
			}
			else
			{
				int row = 0;

				for (state = sqlite3_step(statement); state == SQLITE_ROW; state = sqlite3_step(statement), ++row)
				{
					int columns = sqlite3_data_count(statement);
					std::vector<Object::VectorDataType> columnVector;
					
					for (int column = 0; column < columns; ++column)
					{
						const unsigned char *data = 0;

						if (sqlite3_column_type(statement, column) == SQLITE_NULL)
						{
							fprintf(stderr, _("SQL database entry of table %s in row %d and column %d is NULL.\n"),  Parser::getTableName(Parser::List(i)).c_str(), row, column);
						}
						else
							data = sqlite3_column_text(statement, column);

						columnVector.push_back(data);
						std::cout << "Vector data " << columnVector[column] << std::endl;
					}
					
					databaseStruct->listList.push_back(Parser::List(i));
					std::cout << "List " << i << std::endl;
					class Object *object = Parser::createObjectByVector(columnVector, Parser::List(i)); /// @todo Don't use the @class Object constructor. Use the specific list class constructor (virtual methods).
					std::cout << "Created object with identifier " << object->identifier() << std::endl;
					databaseStruct->objectList.push_back(object);
				}
			}
			
			std::cout << "before finalizing and i is " << i << std::endl;
			state = sqlite3_finalize(statement);
			
			if (state != SQLITE_OK)
				fprintf(stderr, _("Was unable to finalize prepared SQL statement of table %s.\nState %d.\n"),  Parser::getTableName(Parser::List(i)).c_str(), state);
		}

		/// @todo Initialize all database objects by initVector.
		std::list<class Object*>::iterator objectIterator = databaseStruct->objectList.begin();

		while (objectIterator != databaseStruct->objectList.end())
		{
			(*objectIterator)->initByVector();
			++objectIterator;
		}

		/// @todo Add database objects into usual lists and increase ids.
		std::list<enum List>::iterator listIterator = databaseStruct->listList.begin();
		objectIterator = databaseStruct->objectList.begin();
		
		while (listIterator != databaseStruct->listList.end())
		{
			this->getList(*listIterator).push_back(*objectIterator);
			(*objectIterator)->setId((*objectIterator)->id() + Object::maxIds());
			++listIterator;
			++objectIterator;
		}

		Object::setMaxIds(Object::maxIds() + databaseStruct->listList.size());
	}
	else
		fprintf(stderr, _("Was unable to open database \"%s\".\nState %d.\n"), filePath, state);
	
	sqlite3_close(database);
	
	return result;
}

void Parser::removeDatabase(const int &index)
{
	/*
	if (index >= this->databaseVector.size())
	{
		fprintf(stderr, _("Invalid index in removeDatabase(): %d."), index);
		return;
	}
	*/
	
	if (Vjassdoc::showVerbose())
		printf(_("Remove database %d."), index);
	
	Database *database = this->databaseVector[index];
	std::list<enum List>::iterator iterator0 = database->listList.begin();
	std::list<class Object*>::iterator iterator1 = database->objectList.begin();
	
	while (iterator0 != database->listList.end())
	{
		this->getList(*iterator0).remove(*iterator1);
		delete *iterator1;
		++iterator0;
		++iterator1;
	}

	database->listList.clear();
	database->objectList.clear();
	this->databaseVector.erase(this->databaseVector.begin() + index - 1);
	delete database;
}


class Object* Parser::searchObjectInLastDatabase(const Object::IdType &id)
{
	if (databaseVector.empty())
		return 0;

	std::list<class Object*> objectList = databaseVector.back()->objectList;

	for (std::list<class Object*>::iterator iterator = objectList.begin(); iterator != objectList.end(); ++iterator)
	{
		if ((*iterator)->id() == id)
			return *iterator;
	}

	return 0;
}
#endif

class Object* Parser::searchObjectInList(const std::string &identifier, const enum List &list, const enum SearchMode &searchMode, const class Object *object)
{
	if (!Vjassdoc::shouldParseObjectsOfList(list))
		return 0;

	std::list<class Object*> objectList = this->getList(list);

	return Parser::searchObjectInCustomList(objectList, identifier, searchMode, object);
}

std::list<class Object*> Parser::getSpecificList(const enum List &list, const struct Comparator &comparator, const class Object *object)
{
	if (!Vjassdoc::shouldParseObjectsOfList(list))
		return std::list<class Object*>();

	std::list<class Object*> objectList = this->getList(list);
	std::list<class Object*> result;
	
	for (std::list<class Object*>::const_iterator iterator = objectList.begin(); iterator != objectList.end(); ++iterator)
	{
		if (comparator(*iterator, object))
			result.push_back(*iterator);
	}
	
	return result;
}

std::list<class Object*>& Parser::getList(const enum Parser::List &list)
{
	switch (list)
	{
		case Parser::Comments:
			return reinterpret_cast<std::list<class Object*>& >(this->commentList);

		case Parser::Keywords:
			return reinterpret_cast<std::list<class Object*>& >(this->keywordList);

		case Parser::Keys:
			return reinterpret_cast<std::list<class Object*>& >(this->keyList);

		case Parser::TextMacros:
			return reinterpret_cast<std::list<class Object*>& >(this->textMacroList);

		case Parser::TextMacroInstances:
			return reinterpret_cast<std::list<class Object*>& >(this->textMacroInstanceList);

		case Parser::Types:
			return reinterpret_cast<std::list<class Object*>& >(this->typeList);

		case Parser::Locals:
			return reinterpret_cast<std::list<class Object*>& >(this->localList);

		case Parser::Globals:
			return reinterpret_cast<std::list<class Object*>& >(this->globalList);

		case Parser::Members:
			return reinterpret_cast<std::list<class Object*>& >(this->memberList);

		case Parser::Parameters:
			return reinterpret_cast<std::list<class Object*>& >(this->parameterList);

		case Parser::FunctionInterfaces:
			return reinterpret_cast<std::list<class Object*>& >(this->functionInterfaceList);

		case Parser::Functions:
			return reinterpret_cast<std::list<class Object*>& >(this->functionList);

		case Parser::Methods:
			return reinterpret_cast<std::list<class Object*>& >(this->methodList);

		case Parser::Implementations:
			return reinterpret_cast<std::list<class Object*>& >(this->implementationList);

		case Parser::Hooks:
			return reinterpret_cast<std::list<class Object*>& >(this->hookList);

		case Parser::Interfaces:
			return reinterpret_cast<std::list<class Object*>& >(this->interfaceList);

		case Parser::Structs:
			return reinterpret_cast<std::list<class Object*>& >(this->structList);

		case Parser::Modules:
			return reinterpret_cast<std::list<class Object*>& >(this->moduleList);

		case Parser::Scopes:
			return reinterpret_cast<std::list<class Object*>& >(this->scopeList);

		case Parser::Libraries:
			return reinterpret_cast<std::list<class Object*>& >(this->libraryList);

		case Parser::SourceFiles:
			return reinterpret_cast<std::list<class Object*>& >(this->sourceFileList);

		case Parser::DocComments:
			return reinterpret_cast<std::list<class Object*>& >(this->docCommentList);
	}
	
	std::cerr << "Unknown list." << list << std::endl;

	return reinterpret_cast<std::list<class Object*>& >(this->commentList);
}

std::stringstream& Parser::addObjectList(std::stringstream &output, const enum Parser::List &list)
{
	std::list<class Object*> objectsList = this->getList(list);
	
	if (objectsList.empty())
	{
		output << "-\n";
		return output;
	}
	
	output << "\t\t<ul>\n";

	for (std::list<class Object*>::const_iterator iterator = objectsList.begin(); iterator != objectsList.end(); ++iterator)
		output << "\t\t\t<li>" << Object::objectPageLink(*iterator) << "</li>\n";

	output << "\t\t</ul>\n";
	return output;
}

#ifdef SQLITE
std::string Parser::getTableName(const enum Parser::List &list)
{
	switch (list)
	{
		case Parser::Comments:
			return Comment::sqlTableName;

		case Parser::Keywords:
			return Keyword::sqlTableName;

		case Parser::Keys:
			return Key::sqlTableName;

		case Parser::TextMacros:
			return TextMacro::sqlTableName;

		case Parser::TextMacroInstances:
			return TextMacroInstance::sqlTableName;

		case Parser::Types:
			return Type::sqlTableName;

		case Parser::Locals:
			return Local::sqlTableName;

		case Parser::Globals:
			return Global::sqlTableName;

		case Parser::Members:
			return Member::sqlTableName;

		case Parser::Parameters:
			return Parameter::sqlTableName;

		case Parser::FunctionInterfaces:
			return FunctionInterface::sqlTableName;

		case Parser::Functions:
			return Function::sqlTableName;

		case Parser::Methods:
			return Method::sqlTableName;

		case Parser::Implementations:
			return Implementation::sqlTableName;

		case Parser::Hooks:
			return Hook::sqlTableName;

		case Parser::Interfaces:
			return Interface::sqlTableName;

		case Parser::Structs:
			return Struct::sqlTableName;

		case Parser::Modules:
			return Module::sqlTableName;

		case Parser::Scopes:
			return Scope::sqlTableName;

		case Parser::Libraries:
			return Library::sqlTableName;

		case Parser::SourceFiles:
			return SourceFile::sqlTableName;

		case Parser::DocComments:
			return DocComment::sqlTableName;
	}

	std::cerr << "Unknown list " << list << std::endl;

	return std::string();
}

unsigned int Parser::getTableColumns(const enum Parser::List &list)
{
	switch (list)
	{
		case Parser::Comments:
			return Comment::sqlColumns;

		case Parser::Keywords:
			return Keyword::sqlColumns;

		case Parser::Keys:
			return Key::sqlColumns;

		case Parser::TextMacros:
			return TextMacro::sqlColumns;

		case Parser::TextMacroInstances:
			return TextMacroInstance::sqlColumns;

		case Parser::Types:
			return Type::sqlColumns;

		case Parser::Locals:
			return Local::sqlColumns;

		case Parser::Globals:
			return Global::sqlColumns;

		case Parser::Members:
			return Member::sqlColumns;

		case Parser::Parameters:
			return Parameter::sqlColumns;

		case Parser::FunctionInterfaces:
			return FunctionInterface::sqlColumns;

		case Parser::Functions:
			return Function::sqlColumns;

		case Parser::Methods:
			return Method::sqlColumns;

		case Parser::Implementations:
			return Implementation::sqlColumns;

		case Parser::Hooks:
			return Hook::sqlColumns;

		case Parser::Interfaces:
			return Interface::sqlColumns;

		case Parser::Structs:
			return Struct::sqlColumns;

		case Parser::Modules:
			return Module::sqlColumns;

		case Parser::Scopes:
			return Scope::sqlColumns;

		case Parser::Libraries:
			return Library::sqlColumns;

		case Parser::SourceFiles:
			return SourceFile::sqlColumns;

		case Parser::DocComments:
			return DocComment::sqlColumns;
	}

	std::cerr << "Unknown list " << list << std::endl;

	return 0;
}

std::string Parser::getTableCreationStatement(const enum Parser::List &list)
{
	std::string result = std::string("CREATE TABLE ") + Parser::getTableName(list) + "(Id INT PRIMARY KEY,";

	switch (list)
	{
		case Parser::Comments:
			result +=
			Comment::sqlColumnStatement;
			break;

		case Parser::Keywords:
			result +=
			Keyword::sqlColumnStatement;
			break;

		case Parser::Keys:
			result +=
			Key::sqlColumnStatement;
			break;

		case Parser::TextMacros:
			result +=
			TextMacro::sqlColumnStatement;
			break;

		case Parser::TextMacroInstances:
			result +=
			TextMacroInstance::sqlColumnStatement;
			break;

		case Parser::Types:
			result +=
			Type::sqlColumnStatement;
			break;

		case Parser::Locals:
			result +=
			Local::sqlColumnStatement;
			break;

		case Parser::Globals:
			result +=
			Global::sqlColumnStatement;
			break;

		case Parser::Members:
			result +=
			Member::sqlColumnStatement;
			break;

		case Parser::Parameters:
			result += Parameter::sqlColumnStatement;
			break;

		case Parser::FunctionInterfaces:
			result +=
			FunctionInterface::sqlColumnStatement;
			break;

		case Parser::Functions:
			result +=
			Function::sqlColumnStatement;
			break;

		case Parser::Methods:
			result +=
			Method::sqlColumnStatement;
			break;

		case Parser::Implementations:
			result +=
			Implementation::sqlColumnStatement;
			break;

		case Parser::Hooks:
			result +=
			Hook::sqlColumnStatement;
			break;

		case Parser::Interfaces:
			result +=
			Interface::sqlColumnStatement;
			break;

		case Parser::Structs:
			result +=
			Struct::sqlColumnStatement;
			break;

		case Parser::Modules:
			result +=
			Module::sqlColumnStatement;
			break;

		case Parser::Scopes:
			result +=
			Scope::sqlColumnStatement;
			break;

		case Parser::Libraries:
			result +=
			Library::sqlColumnStatement;
			break;

		case Parser::SourceFiles:
			result +=
			SourceFile::sqlColumnStatement;
			break;

		case Parser::DocComments:
			result +=
			DocComment::sqlColumnStatement;
			break;

		default:
			std::cerr << "Unknown list " << list << std::endl;
			break;
	}
	
	result += ")";
	
	return result;
}

class Object* Parser::createObjectByVector(std::vector<const unsigned char*> &columnVector, const enum Parser::List &list)
{
	switch (list)
	{
		case Parser::Comments:
			return static_cast<class Object*>(new Comment(columnVector));

		case Parser::Keywords:
			return static_cast<class Object*>(new Keyword(columnVector));

		case Parser::Keys:
			return static_cast<class Object*>(new Key(columnVector));

		case Parser::TextMacros:
			return static_cast<class Object*>(new TextMacro(columnVector));

		case Parser::TextMacroInstances:
			return static_cast<class Object*>(new TextMacroInstance(columnVector));

		case Parser::Types:
			return static_cast<class Object*>(new Type(columnVector));

		case Parser::Locals:
			return static_cast<class Object*>(new Local(columnVector));

		case Parser::Globals:
			return static_cast<class Object*>(new Global(columnVector));

		case Parser::Members:
			return static_cast<class Object*>(new Member(columnVector));

		case Parser::Parameters:
			return static_cast<class Object*>(new Parameter(columnVector));

		case Parser::FunctionInterfaces:
			return static_cast<class Object*>(new FunctionInterface(columnVector));

		case Parser::Functions:
			return static_cast<class Object*>(new Function(columnVector));

		case Parser::Methods:
			return static_cast<class Object*>(new Method(columnVector));

		case Parser::Implementations:
			return static_cast<class Object*>(new Implementation(columnVector));

		case Parser::Hooks:
			return static_cast<class Object*>(new Hook(columnVector));

		case Parser::Interfaces:
			return static_cast<class Object*>(new Interface(columnVector));

		case Parser::Structs:
			return static_cast<class Object*>(new Struct(columnVector));

		case Parser::Modules:
			return static_cast<class Object*>(new Module(columnVector));

		case Parser::Scopes:
			return static_cast<class Object*>(new Scope(columnVector));

		case Parser::Libraries:
			return static_cast<class Object*>(new Library(columnVector));

		case Parser::SourceFiles:
			return static_cast<class Object*>(new SourceFile(columnVector));

		case Parser::DocComments:
			return static_cast<class Object*>(new DocComment(columnVector));

		default:
			std::cerr << "Unknown list " << list << std::endl;
			break;
	}
	
	return 0;
}
#endif

void Parser::getStructInheritanceList(const class Interface *extension, const std::string &prefix, std::stringstream &sstream)
{
	std::list<class Object*> structList = this->getSpecificList(Parser::Structs, Struct::HasExtension(), extension);

	if (structList.empty())
		return;

	sstream << prefix << "<ul>\n";

	for (std::list<class Object*>::iterator iterator = structList.begin(); iterator != structList.end(); ++iterator)
	{
		sstream << prefix << "\t<li>" << (*iterator)->pageLink() << '\n';
		this->getStructInheritanceList(static_cast<class Interface*>(*iterator), prefix + "\t\t", sstream);
		sstream << prefix << "\t</li>\n";
	}

	sstream << prefix << "</ul>\n";
}

/// @todo FIXME
void Parser::getLibraryRequirementList(const class Library *requirement, const std::string &prefix, std::stringstream &sstream)
{
	std::cout << "Running with library " << requirement->identifier() << std::endl;

	std::list<class Object*> specifiedList = this->getSpecificList(Parser::Libraries, Library::HasRequirement(), requirement);

	if (specifiedList.empty())
	{
		//std::cout << "Requirement list is empty." << std::endl;
		return;
	}

	sstream << prefix << "<ul>\n";

	for (std::list<class Object*>::iterator iterator = specifiedList.begin(); iterator != specifiedList.end(); ++iterator)
	{
		sstream << prefix << "\t<li>" << (*iterator)->pageLink() << '\n';
		this->getLibraryRequirementList(static_cast<class Library*>(*iterator), prefix + "\t\t", sstream);
		sstream << prefix << "\t</li>\n";
	}

	sstream << prefix << "</ul>\n";
}

}
