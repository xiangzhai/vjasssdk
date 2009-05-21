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
	return false;
}

class Object* Parser::searchObjectInCustomList(const std::list<class Object*> &objectList, const class Object *object, const std::string &identifier, const enum Parser::SearchMode &searchMode)
{
	if (objectList.size() == 0)
		return 0;

	class Object *resultObject = 0;
	bool checkContainer = false;
	bool checkScope = false;
	bool checkLibrary = false;
	
	if (object->container() != 0 || searchMode & CheckContainer)
		checkContainer = true;
	
	if (object->scope() != 0 || searchMode & CheckScope)
		checkScope = true;
	
	if (object->library() != 0 || searchMode & checkLibrary)
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
	if (!Vjassdoc::getDatabases().empty())
	{
		if (Vjassdoc::showVerbose())
			std::cout << _("You've selected one or several databases..") << std::endl;

		for (std::list<std::string>::iterator iterator = Vjassdoc::getDatabases().begin(); iterator != Vjassdoc::getDatabases().end(); ++iterator)
		{
			std::cout << "Database path " << *iterator << std::endl;
			this->addDatabase((*iterator).c_str());
		}
	}
	
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
		std::cout << "File path " << identifier << std::endl;
		int position;
		
		position = identifier.find_last_of(Vjassdoc::dirSeparator);
		
		if (position != std::string::npos)
			identifier = identifier.substr(position + 1);

		std::cout << "New identifier " << identifier << std::endl;

		this->add(new SourceFile(identifier, *iterator));
	}

	std::cout << "After construction." << std::endl;
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
	
	for (std::vector<struct Database*>::iterator iterator = this->databaseVector.begin(); iterator != this->databaseVector.end(); ++iterator)
		delete *iterator;

	this->databaseVector.clear();
}

/// @todo FIXME
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
	<< "\t\t<ul>\n"
	;

	for (std::list<class Interface*>::iterator iterator = this->interfaceList.begin(); iterator != this->interfaceList.end(); ++iterator)
	{
		sstream << "\t\t\t<li>" << (*iterator)->pageLink() << '\n';
		this->getStructInheritanceList(*iterator, "\t\t\t\t", sstream);
		sstream << "\t\t\t</li>\n";
	}

	sstream
	<< "\t\t</ul>\n"
	<< "\t\t<h2><a name=\"Structs\">" << _("Structs") << "</a></h2>\n"
	<< "\t\t<ul>\n"
	;

	for (std::list<class Struct*>::iterator iterator = this->structList.begin(); iterator != this->structList.end(); ++iterator)
	{
		if ((*iterator)->extension() == 0)
		{
			sstream << "\t\t\t<li>" << (*iterator)->pageLink() << '\n';
			this->getStructInheritanceList(*iterator, "\t\t\t\t", sstream);
			sstream << "\t\t\t</li>\n";
		}
	}

	sstream
	<< "\t\t</ul>\n"
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
		if ((*iterator)->requirement() == 0)
			continue;

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
			(*iterator)->init();
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
			if (!Vjassdoc::shouldParseObjectsOfList(Parser::List(i)))
				continue;

			sout << "\t\t\t<li><a href=\"#" << Parser::title[i] << "\">" << Parser::title[i] << " (" << this->getList(Parser::List(i)).size() << ")</a></li>" << std::endl;
		}
		
		/// @todo Test.
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
			if (!Vjassdoc::shouldParseObjectsOfList(Parser::List(i)))
				continue;

			sout
			<< "\t\t<h2><a name=\"" <<  Parser::title[i] << "\">" << Parser::title[i] << " (" << this->getList(Parser::List(i)).size() << ")</a></h2>" << std::endl;
			this->addObjectList(sout, List(i));
		}
		
		if (Vjassdoc::createSpecialPages())
		{
			this->createInheritanceListPage();
			this->createRequirementListPage();

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
		
		if (Parser::fileExists(fileName))
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
						fprintf(stderr, _("Was unable to run execution of table \"%s\" from list %d.\nState: %d.\nMessage: %s\n"), Parser::getTableName(Parser::List(i)).c_str(), i, state, message);
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
	std::cout << "bla" << std::endl;
	if (Vjassdoc::showVerbose())
		printf(_("Load database %s.\n"), filePath);

	int result = -1;
	sqlite3 *database;
	std::cout << "Before open" << std::endl;
	int state = sqlite3_open_v2(filePath, &database, SQLITE_OPEN_READONLY, NULL);
	std::cout << "After open" << std::endl;

	if (state == SQLITE_OK)
	{
		Parser::Database *databaseStruct = new Parser::Database;
		result = this->databaseVector.size();
		this->databaseVector.push_back(databaseStruct);
	
		for (int i = 0; i < Parser::MaxLists; ++i)
		{
			if (!Vjassdoc::shouldParseObjectsOfList(Parser::List(i)))
				continue;

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
				continue;
			}
			
			for (state = sqlite3_step(statement); state == SQLITE_ROW; state = sqlite3_step(statement))
			{
				int columns = sqlite3_data_count(statement);
				std::vector<const unsigned char*> columnVector;
				
				for (int j = 0; j < columns; ++j)
				{
					columnVector.push_back(sqlite3_column_text(statement, j));
					std::cout << "Vector data " << columnVector[j] << std::endl;
				}
				
				databaseStruct->listList.push_back(Parser::List(i));
				std::cout << "List " << i << std::endl;
				class Object *object = this->addObjectByColumnVector(Parser::List(i), columnVector);
				//std::cout << "Created object with identifier " << object->identifier() << std::endl;
				databaseStruct->objectList.push_back(object);
			}
			
			state = sqlite3_finalize(statement);
			
			if (state != SQLITE_OK)
				fprintf(stderr, _("Was unable to finalize prepared SQL statement of table %s.\nState %d.\n"),  Parser::getTableName(Parser::List(i)).c_str(), state);
		}
	}
	else
		fprintf(stderr, _("Was unable to open database.\nState %d.\n"), state);
		
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
		++iterator0;
		++iterator1;
	}
}
#endif

class Object* Parser::searchObjectInList(const class Object *object, const std::string &identifier, const enum List &list, const enum SearchMode &searchMode)
{
	if (!Vjassdoc::shouldParseObjectsOfList(list))
		return 0;

	std::list<class Object*> objectList = this->getList(list);

	return Parser::searchObjectInCustomList(objectList, object, identifier, searchMode);
}

std::list<class Object*> Parser::getSpecificList(const class Object *object, const enum List &list, const struct Comparator &comparator)
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

#ifdef SQLITE
bool Parser::fileExists(const std::string &fileName)
{
	struct stat fileInfo; 
	int state;
	state = stat(fileName.c_str(), &fileInfo);
	
	if (state == 0)
		return true;
	
	return false;
}
#endif

std::list<class Object*>& Parser::getList(const enum Parser::List &list)
{
	switch (list)
	{
		case Parser::Comments:
			return reinterpret_cast<std::list<class Object*>& >(this->commentList);

		case Parser::Keywords:
			return reinterpret_cast<std::list<class Object*>& >(this->keywordList);

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

class Object* Parser::addObjectByColumnVector(const enum Parser::List &list, std::vector<const unsigned char*> &columnVector)
{
	switch (list)
	{
		case Parser::Comments:
			this->add(new Comment(columnVector));
			break;

		case Parser::Keywords:
			this->add(new Keyword(columnVector));
			break;

		case Parser::TextMacros:
			this->add(new TextMacro(columnVector));
			break;

		case Parser::TextMacroInstances:
			this->add(new TextMacroInstance(columnVector));
			break;

		case Parser::Types:
			this->add(new Type(columnVector));
			break;

		case Parser::Locals:
			this->add(new Local(columnVector));
			break;

		case Parser::Globals:
			this->add(new Global(columnVector));
			break;

		case Parser::Members:
			this->add(new Member(columnVector));
			break;

		case Parser::Parameters:
			this->add(new Parameter(columnVector));
			break;

		case Parser::FunctionInterfaces:
			this->add(new FunctionInterface(columnVector));
			break;

		case Parser::Functions:
			this->add(new Function(columnVector));
			break;

		case Parser::Methods:
			this->add(new Method(columnVector));
			break;

		case Parser::Implementations:
			this->add(new Implementation(columnVector));
			break;

		case Parser::Interfaces:
			this->add(new Interface(columnVector));
			break;

		case Parser::Structs:
			this->add(new Struct(columnVector));
			break;

		case Parser::Modules:
			this->add(new Interface(columnVector)); /// @todo Module
			break;

		case Parser::Scopes:
			this->add(new Scope(columnVector));
			break;

		case Parser::Libraries:
			this->add(new Library(columnVector));
			break;

		case Parser::SourceFiles:
			this->add(new SourceFile(columnVector));
			break;

		case Parser::DocComments:
			this->add(new DocComment(columnVector));
			break;
	}
	
	return this->getList(list).back();
}
#endif

void Parser::getStructInheritanceList(const class Interface *extension, const std::string &prefix, std::stringstream &sstream)
{
	std::list<class Object*> structList = this->getSpecificList(extension, Parser::Structs, Struct::HasExtension());

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

void Parser::getLibraryRequirementList(const class Library *requirement, const std::string &prefix, std::stringstream &sstream)
{
	std::list<class Object*> libraryList = this->getSpecificList(requirement, Parser::Libraries, Library::HasRequirement());

	if (structList.empty())
		return;

	sstream << prefix << "<ul>\n";

	for (std::list<class Object*>::iterator iterator = libraryList.begin(); iterator != libraryList.end(); ++iterator)
	{
		if ((*iterator)->requirement() == 0)
			continue;

		sstream << prefix << "\t<li>" << (*iterator)->pageLink() << '\n';
		this->getLibraryRequirementList(static_cast<class Library*>(*iterator), prefix + "\t\t", sstream);
		sstream << prefix << "\t</li>\n";
	}

	sstream << prefix << "</ul>\n";
}

}
