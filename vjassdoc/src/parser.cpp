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

#include <boost/format.hpp>

#include "parser.hpp"
#include "internationalisation.hpp"
#include "objects.hpp"
#include "vjassdoc.hpp"
#include "file.hpp"
#include "syntaxerror.hpp"
#include "utilities.hpp"

namespace vjassdoc
{

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
		m_codeType(new Type("code", 0, 0, 0, "", "")),
		m_file(0),
		m_lists(std::vector<class Object::List*>(Parser::MaxLists))
{
	this->m_lists[Parser::Comments] = new Comment::List();
	this->m_lists[Parser::Keywords] = new Keyword::List();
	this->m_lists[Parser::Keys] = new Key::List();
	this->m_lists[Parser::TextMacros] = new TextMacro::List();
	this->m_lists[Parser::TextMacroInstances] = new TextMacroInstance::List();
	this->m_lists[Parser::ExternalCalls] = new ExternalCall::List();
	this->m_lists[Parser::Types] = new Type::List();
	this->m_lists[Parser::Locals] = new Local::List();
	this->m_lists[Parser::Globals] = new Global::List();
	this->m_lists[Parser::Members] = new Member::List();
	this->m_lists[Parser::Parameters] = new Parameter::List();
	this->m_lists[Parser::FunctionInterfaces] = new FunctionInterface::List();
	this->m_lists[Parser::Functions] = new Function::List();
	this->m_lists[Parser::Methods] = new Method::List();
	this->m_lists[Parser::Calls] = new Call::List();
	this->m_lists[Parser::Implementations] = new Implementation::List();
	this->m_lists[Parser::Hooks] = new Hook::List();
	this->m_lists[Parser::Interfaces] = new Interface::List();
	this->m_lists[Parser::Structs] = new Struct::List();
	this->m_lists[Parser::Modules] = new Module::List();
	this->m_lists[Parser::Scopes] = new Scope::List();
	this->m_lists[Parser::Libraries] = new Library::List();
	this->m_lists[Parser::SourceFiles] = new SourceFile::List();
	this->m_lists[Parser::DocComments] = new DocComment::List();
	
	//add default types
	if (Vjassdoc::optionVerbose())
		std::cout << _("Adding default Jass types.") << std::endl;

	this->add(m_integerType);
	this->add(m_realType);
	this->add(m_stringType);
	this->add(m_booleanType);
	this->add(m_handleType);
	this->add(m_codeType);
}

//Default Jass types are in lists!
Parser::~Parser()
{

	BOOST_FOREACH(class Object::List *list, this->m_lists)
			delete list;
	
	BOOST_FOREACH(class SyntaxError *syntaxError, this->m_syntaxErrors)
			delete syntaxError;
		
	
	// do not delete database lists since they're saved in this->m_lists, too
}

std::size_t Parser::parse(const boost::filesystem::path &path) throw (class Exception)
{
	if (this->m_file != 0)
		throw Exception(Parser::DoubleParseError, _("Trying to parse during parsing."));
	
	if (!boost::filesystem::exists(path))
		throw Exception(Parser::FilePathError, boost::str(boost::format(_("File path \"%1%\" does not exist.")) % path.string()));

	std::ifstream ifstream(path.string().c_str());
	
	if (!ifstream)
		throw Exception(Parser::FileStreamError, boost::str(boost::format(_("Unable to open file stream for file path \"%1%\".")) % path.string()));
	
	class SourceFile *sourceFile = new SourceFile(path.filename(), path->string());
	this->add(sourceFile);
	this->m_file = new File();
	std::size_t lines = this->m_file->parse(this, sourceFile, ifstream);
	delete this->m_file;
	this->m_file = 0;
	ifstream.close();
	
	return lines;
}

void Parser::initObjects()
{
	//objects should be initialized before using them
	for (std::size_t i = 0; i < Parser::MaxLists; ++i)
	{
		if (!Vjassdoc::optionParseObjectsOfList(Parser::List(i)))
			continue;

		BOOST_FOREACH(const class Object *object, this->m_lists[i]->objects())
		{
			/// @todo Init database objects separated?

			object->init();
		}
	}
}


void Parser::sortAlphabetically()
{
	for (std::size_t i = 0; i < Parser::MaxLists; ++i)
	{
		if (!Vjassdoc::optionParseObjectsOfList(Parser::List(i)))
			continue;

		this->m_lists[i].objects().sort(Object::AlphabeticalComparator());
	}
}

void Parser::showSyntaxErrors()
{
	/// @todo compare all object identifiers with Jass and vJass keywords.
	std::cout << boost::format(_("%1% syntax errors.")) % this->m_syntaxErrors.size() << std::endl;
	this->m_syntaxErrors.sort(SyntaxError::Comparator);
	
	BOOST_FOREACH(class SyntaxError *syntaxError, this->m_syntaxErrors)
		std::cout << boost::format(_("File %1%, line %2%: %3%")) % syntaxError->sourceFile()->identifier() % syntaxError->line() % syntaxError->message() << std::endl;
}
#ifdef HTML
void Parser::createInheritanceListPage(std::ostream &ostream)
{	
	createHtmlHeader(ostream, _("Inheritance List"));
	ostream
	<< "\t<body>\n"
	<< "\t\t<a href=\"index.html\">" << _("Return to start page") << "</a>\n"
	<< "\t\t<h1>" << _("Inheritance List") << "</h1>\n"
	<< "\t\t<ul>\n"
	<< "\t\t\t<li><a href=\"#Interfaces\">"	<< _("Interfaces") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Structs\">" << _("Structs") << "</a></li>\n"
	<< "\t\t</ul>\n"
	<< "\t\t<h2><a name=\"Interfaces\">" << _("Interfaces") << "</a></h2>\n"
	;

	if (!this->m_lists[Parser::Interfaces].objects().empty())
	{
		ostream << "\t\t<ul>\n";

		BOOST_FOREACH(const class Interface *interface, this->m_lists[Parser::Interfaces].objects())
		{
			ostream << "\t\t\t<li>" << interface->pageLink() << '\n';
			this->getStructInheritanceList(interface, "\t\t\t\t", ostream);
			ostream << "\t\t\t</li>\n";
		}

		ostream << "\t\t</ul>\n";
	}
	else
		ostream << "\t\t<p>-</p>\n";

	ostream
	<< "\t\t<h2><a name=\"Structs\">" << _("Structs") << "</a></h2>\n";

	if (!!this->m_lists[Parser::Structs].objects().empty())
	{
		ostream << "\t\t<ul>\n";

		BOOST_FOREACH(const class Struct *whichStruct, this->m_lists[Parser::Structs].objects())
		{
			if (whichStruct->extension() == 0)
			{
				ostream << "\t\t\t<li>" << whichStruct->pageLink() << '\n';
				this->getStructInheritanceList(whichStruct, "\t\t\t\t", ostream);
				ostream << "\t\t\t</li>\n";
			}
		}

		ostream << "\t\t</ul>\n";
	}
	else
		ostream << "\t\t<p>-</p>\n";

	ostream
	<< "\t</body>\n"
	<< "</html>\n"
	;
}

void Parser::createRequirementListPage(std::ostream &ostream)
{
	createHtmlHeader(ostream, _("Requirement List"));
	ostream
	<< "\t<body>\n"
	<< "\t\t<a href=\"index.html\">" << _("Return to start page") << "</a>\n"
	<< "\t\t<h1>" << _("Requirement List") << "</h1>\n"
	<< "\t\t<ul>\n"
	;

	BOOST_FOREACH(const class Library *library, this->m_lists[Parser::Libraries].objects())
	{
		//requirement has to be 0
		if (library->requirement() != 0)
			continue;

		ostream << "\t\t\t<li>" << library->pageLink() << '\n';
		this->getLibraryRequirementList(library, "\t\t\t\t", ostream);
		ostream << "\t\t\t</li>\n";
	}

	ostream
	<< "\t\t</ul>\n"
	<< "\t</body>\n"
	<< "</html>\n"
	;
}

void Parser::createUndocumentatedListPage(std::ostream &ostream)
{
	createHtmlHeader(ostream, _("Undocumentated Objects"));
	ostream
	<< "\t<body>\n"
	<< "\t\t<a href=\"index.html\">" << _("Return to start page") << "</a>\n"
	<< "\t\t<h1>" << _("Undocumentated Objects") << "</h1>\n"
	<< "\t\t<ul>\n"
	;
	
	for (std::size_t i = 0; i < Parser::MaxLists; ++i)
	{
		if (Parser::List(i) == Parser::SourceFiles || Parser::List(i) == Parser::DocComments || !Vjassdoc::optionParseObjectsOfList(Parser::List(i)))
			continue;
	
		BOOST_FOREACH(const class Object *object, this->m_lists[i].objects())
		{
			if (object->docComment() == 0)
				ostream << "\t\t\t<li>" << Object::objectPageLink(object) << "</li>\n";
		}
	}

	ostream
	<< "\t\t</ul>\n"
	<< "\t</body>\n"
	<< "</html>\n"
	;
}

void Parser::createHtmlFiles(const boost::filesystem::path &dirPath, const std::string &title, bool pages, bool extraPages) throw (std::exception)
{
	std::ofstream fstream((dirPath / "index.html").string().c_str());
	
	if (!fstream)
	//{
		//fstream.close();
		//std::cerr << _("Was unable to create file \"index.html\".") << std::endl;
		
		throw std::exception();
	//}
	
	createHtmlHeader(fstream, title);
	fstream
	<< "\t<body>\n"
	<< "\t\t<h1>" << title << "</h1>\n"
	<< "\t\t<p>" << boost::format(_("Generated by vjassdoc %1%.")) % Vjassdoc::version << "</p>\n"
	<< "\t\t<ul>\n"
	<< std::endl;

	for (std::size_t i = 0; i < Parser::MaxLists; ++i)
	{
		if (!Vjassdoc::optionParseObjectsOfList(Parser::List(i)) || this->m_lists[i->.objects().empty())
			continue;

		fstream << "\t\t\t<li><a href=\"#" << this->m_lists[i]->htmlCategoryName() << "\">" << boost::str(boost::format(_("%1% (%2%)")) % this->m_lists[i]->htmlCategoryName() % this->m_lists[i]->objects().size()) << "</a></li>" << std::endl;
	}

	if (extraPages)
	{
		fstream
		<< "\t\t\t<li>" << "<a href=\"inheritancelist.html\">" << _("Inheritance List") << "</a></li>\n"
		<< "\t\t\t<li>" << "<a href=\"requirementlist.html\">" << _("Requirement List") << "</a></li>\n"
		<< "\t\t\t<li>" << "<a href=\"undocumentated.html\">" << _("Undocumented Objects") << "</a></li>\n"
		//<< "\t\t\t<li>" << "<a href=\"#Authors\">" << _("Authors") << "</a></li>\n"
		//<< "\t\t\t<li>" << "<a href=\"#Todos\">" << _("Todos") << "</a></li>\n"
		//<< "\t\t\t<li>" << "<a href=\"#States\">" << _("States") << "</a></li>\n"
		//<< "\t\t\t<li>" << "<a href=\"#Sources\">" << _("Sources") << "</a></li>\n"
		;
	}
	
	fstream << "\t\t</ul>\n";

	for (std::size_t i = 0; i < Parser::MaxLists; ++i)
	{
		if (!Vjassdoc::optionParseObjectsOfList(Parser::List(i)) || this->m_lists[i]->objects().empty())
			continue;

		this->m_lists[i]->writeHtmlList(fstream);
	}

	if (extraPages)
	{
		this->createInheritanceListPage(fstream);
		//this->createRequirementListPage(fstream); /// @todo Bugged
		this->createUndocumentatedListPage(fstream);
		/// @todo Implement these special pages:
/*
		<< "\t\t\t<li>" << "<a href=\"#Authors\">" << _("Authors") << "</a></li>\n"
		<< "\t\t\t<li>" << "<a href=\"#Todos\">" << _("Todos") << "</a></li>\n"
		<< "\t\t\t<li>" << "<a href=\"#States\">" << _("States") << "</a></li>\n"
		<< "\t\t\t<li>" << "<a href=\"#Sources\">" << _("Sources") << "</a></li>\n"
*/
	}
	
	fstream
	<< "\t</body>" << std::endl
	<< "</html>";	
	fstream.close();
	
	//create pages
	if (pages)
	{
		for (std::size_t i = 0; i < Parser::MaxLists; ++i)
		{
			if (!Vjassdoc::optionParseObjectsOfList(Parser::List(i)))
				continue;
			
			if (!boost::filesystem::create_directory(dirPath / this->m_lists[i]->htmlFolderName()))
				throw std::exception();
			
			BOOST_FOREACH(const class Object *object, this->m_lists[i]->objects())
			{
				std::ostringstream sstream;
				std::ofstream fout((dirPath / object->htmlPagePath()).string().c_str());
				
				if (!fout)
					throw std::exception();
				
				createHtmlHeader(fout, object->identifier());
				fout
				<< "\t<body>\n"
				<< "\t\t<a href=\"../index.html\">" << _("Return to start page") << "</a>\n"
				<< "\t\t<h1>" << object->identifier() << "</h1>\n"
				<< "\t\t<ul>\n"
				;
				object->writeHtmlPageNavigation(fout);
				fout << "\t\t</ul>\n";
				object->writeHtmlPageContent(fout);
				fout
				<< "\t</body>\n"
				<< "</html>\n";
				fout.close();
			}
		}
	}
}
#endif
#ifdef SQLITE
void Parser::createDatabase(const boost::filesystem::path &path)
{
	if (boost::filesystem::exists(path))
	{
		std::cout << boost::format(_("Database \"%1%\" does already exist. Do you want to replace it by the newer one?")) % path << std::endl;
		std::cout << _("Answer possiblities: y, yes, n, no.") << std::endl;
		
		std::string answer;
		
		do
		{
			std::cin >> answer;
			
			if (answer == _("y") || answer == _("yes"))
			{
				if (!boost::filesystem::remove(path))
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
	int state = sqlite3_open(path.string().c_str(), &database);
	
	if (state == SQLITE_OK)
	{
		char *message = 0;
		state = sqlite3_exec(database, "BEGIN TRANSACTION", 0, 0, &message);
		
		if (state != SQLITE_OK)
		{
			std::cerr << boost::str(boost::format(_("Was unable to begin transaction.\nState: %1%.\nMessage: %2%")) % state % message) << std::endl;
			sqlite3_free(message);
		}
	
		for (int i = 0; i < Parser::MaxLists; ++i)
		{
			if (!Vjassdoc::optionParseObjectsOfList(Parser::List(i)) || this->getList(Parser::List(i)).empty())
				continue;

			state = sqlite3_exec(database, this->getTableCreationStatement(Parser::List(i)).c_str(), 0, 0, &message);

			if (state != SQLITE_OK)
			{
				/// @todo test output
				std::cout << "Table list " << i << std::endl;
				std::cout << "Table creation statement: " << this->getTableCreationStatement(Parser::List(i)).c_str() << std::endl;
				
				fprintf(stderr, _("Was unable to create table \"%s\" from list %d.\nState: %d.\nMessage: %s\n"), Parser::getTableName(Parser::List(i)).c_str(), i, state, message);
				sqlite3_free(message);
			}

			std::list<class Object*> list = this->getList(Parser::List(i));

			for (std::list<class Object*>::iterator iterator = list.begin(); iterator != list.end(); ++iterator)
			{
				std::ostringstream sstream;
				sstream << "INSERT INTO " << Parser::getTableName(Parser::List(i)) << " (Id) VALUES (" << (*iterator)->id() << ')';
				
				state = sqlite3_exec(database, sstream.str().c_str(), 0, 0, &message);

				//std::cerr << "Execution " << sstream.str().c_str() << std::endl;

				if (state != SQLITE_OK)
				{
					fprintf(stderr, _("Was unable to insert id of list %d into table \"%s\".\nState: %d.\nMessage: %s\n"), i, Parser::getTableName(Parser::List(i)).c_str(), state, message);
					sqlite3_free(message);
				}
		
				sstream.str("");
				sstream.clear();

				//std::cout << "Before update of object " << (*iterator)->identifier() << std::endl;
				sstream << "UPDATE " << Parser::getTableName(Parser::List(i)).c_str() << " SET " << (*iterator)->sqlStatement() << " WHERE Id=" << (*iterator)->id();
				//std::cout << "After update" << std::endl;
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

std::size_t Parser::addDatabase(const boost::filesystem::path &path)
{
	std::size_t result = -1;
	sqlite3 *database;
	int state = sqlite3_open_v2(path.string().c_str(), &database, SQLITE_OPEN_READONLY, NULL);

	if (state == SQLITE_OK)
	{
		Parser::Database *databaseStruct = new Parser::Database;
		result = this->m_databases.size();
		this->m_databases.push_back(databaseStruct);
	
		for (int i = 0; i < Parser::MaxLists; ++i)
		{
			if (!Vjassdoc::optionParseObjectsOfList(Parser::List(i)))
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
				NULL);     /* OUT: Pointer to unused portion of zSql */

			std::cout << "After statement " << sstream.str() << std::endl;

			if (state != SQLITE_OK)
				fprintf(stderr, _("Was unable to prepare SQL statement of table %s.\nState %d.\nMessage: \"%s\".\n"),  Parser::getTableName(Parser::List(i)).c_str(), state, sqlite3_errmsg(database));
			else
			{
				if (statement == NULL)
				{
					fprintf(stderr, _("SQL statement of table %s is NULL.\n"), Parser::getTableName(Parser::List(i)).c_str());
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

					state = sqlite3_finalize(statement);

					if (state != SQLITE_OK)
						fprintf(stderr, _("Was unable to finalize prepared SQL statement of table %s.\nState %d.\n"),  Parser::getTableName(Parser::List(i)).c_str(), state);
				}
			}
		}

		if (Vjassdoc::optionVerbose())
		{
			printf(_("Got %d database objects.\n"), databaseStruct->objectList.size());
			std::cout << _("Intializing database objects.") << std::endl;
		}

		/// @todo Initialize all database objects by initVector.
		std::list<class Object*>::iterator objectIterator = databaseStruct->objectList.begin();

		while (objectIterator != databaseStruct->objectList.end())
		{
			(*objectIterator)->initByVector();
			++objectIterator;
		}

		if (Vjassdoc::optionVerbose())
			std::cout << _("Adding database objects.") << std::endl;

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

		/// @todo Usual initialization if there are identifier properties without corresponding object ids?
	}
	else
		fprintf(stderr, _("Was unable to open database \"%s\".\nState %d.\n"), filePath.c_str(), state);
	
	sqlite3_close(database);
	
	return result;
}

void Parser::removeDatabase(std::size_t index)
{
	/*
	if (index >= this->databaseVector.size())
	{
		fprintf(stderr, _("Invalid index in removeDatabase(): %d."), index);
		return;
	}
	*/
	
	if (Vjassdoc::optionVerbose())
		printf(_("Removing database %d.\n"), index);
	
	class Database *database = this->m_databases[index];
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
	this->m_databases.erase(this->m_databases.begin() + index - 1);
	delete database;
}


class Object* Parser::searchObjectDatabase(std::size_t index, std::size_t id)
{
	if (this->m_databases.empty())
		return 0;

	std::list<class Object*> objectList = this->m_databases.back()->objectList;

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
	if (!Vjassdoc::optionParseObjectsOfList(list))
		return 0;

	std::list<class Object*> objectList = this->getList(list);

	return Parser::searchObjectInCustomList(objectList, identifier, searchMode, object);
}

std::list<const class Object*> Parser::getSpecificList(enum Parser::List list, const struct Comparator &comparator, const class Object *object)
{
	if (!Vjassdoc::optionParseObjectsOfList(list))
		return std::list<class Object*>();

	std::list<class Object*> result;
	
	BOOST_FOREACH(const class Object *iterator, this->m_lists[list].objects())
	{
		if (comparator(iterator, object))
			result.push_back(iterator);
	}
	
	return result;
}

std::list<const class Object*> Parser::autoCompletion(const std::string &line, std::size_t &index)
{
	std::string token = getToken(line, index);

	if (token.empty())
		return std::list<const class Object*>();

	if (token == "private" || token == "public" || token == "//!")
		token = getToken(line, index);

	enum Keyword
	{
		None,
		Extension,
		Requirement,
		Delegate,
		Hook,
		ReturnType,
		Call,
		ReturnValue
	};

	Keyword keyword = None;
	std::list<enum Parser::List> lists;

	if (token == "extends")
	{
		std::cout << "Found extends token" << std::endl;
		lists.push_back(Parser::Structs);
		lists.push_back(Parser::Interfaces);
		keyword = Extension;
	}
	else if (token == "requires" || token == "needs")
		lists.push_back(Parser::Libraries);
	else if (token == "delegate")
		lists.push_back(Parser::Structs);
	else if (token == "implement")
		lists.push_back(Parser::Modules);
	else if (token == "hook")
	{
		lists.push_back(Parser::Functions);
		lists.push_back(Parser::Methods); //statics
	}
	else if (token == "returns")
	{
		lists.push_back(Parser::Types);
		lists.push_back(Parser::FunctionInterfaces);
		lists.push_back(Parser::Interfaces);
		lists.push_back(Parser::Structs);
	}
	else if (token == "call")
	{
		lists.push_back(Parser::FunctionInterfaces);
		lists.push_back(Parser::Functions);
		lists.push_back(Parser::Methods);
		lists.push_back(Parser::Structs); //calling static methods
	}
	else if (token == "return")
	{
		lists.push_back(Parser::Types); // integer(10)
		lists.push_back(Parser::Globals); // bj_MAX_PLAYERS
		lists.push_back(Parser::Locals); // i
		lists.push_back(Parser::FunctionInterfaces); // FunctionInterface.bla
		lists.push_back(Parser::Interfaces); // Widget(1)
		lists.push_back(Parser::Structs); // Widget(1)
	}

	token = getToken(line, index);
	std::cout << "Search identifier token " << token << std::endl;
	std::list<class Object*> results;

	BOOST_FOREACH(enum Parser::List list, lists)
	{
		BOOST_FOREACH(const class Object *object, this->m_lists[list])
		{
			if (token == object->identifier())
				results.push_back(object);
		}
	}

	return results;
}

void Parser::getStructInheritanceList(const class Interface *extension, const std::string &prefix, std::ostream &ostream)
{
	std::list<class Object*> structList = this->getSpecificList(Parser::Structs, Struct::HasExtension(), extension);

	if (structList.empty())
		return;

	ostream << prefix << "<ul>\n";

	for (std::list<class Object*>::iterator iterator = structList.begin(); iterator != structList.end(); ++iterator)
	{
		ostream << prefix << "\t<li>" << (*iterator)->pageLink() << '\n';
		this->getStructInheritanceList(static_cast<class Interface*>(*iterator), prefix + "\t\t", ostream);
		ostream << prefix << "\t</li>\n";
	}

	ostream << prefix << "</ul>\n";
}

/// @todo FIXME
void Parser::getLibraryRequirementList(const class Library *requirement, const std::string &prefix, std::ostream &ostream)
{
	std::cout << "Running with library " << requirement->identifier() << std::endl;

	std::list<class Object*> specifiedList = this->getSpecificList(Parser::Libraries, Library::HasRequirement(), requirement); /// @todo Comparator is not called.

	if (specifiedList.empty())
	{
		std::cout << "Requirement list is empty." << std::endl;
		
		return;
	}

	ostream << prefix << "<ul>\n";

	for (std::list<class Object*>::iterator iterator = specifiedList.begin(); iterator != specifiedList.end(); ++iterator)
	{
		ostream << prefix << "\t<li>" << (*iterator)->pageLink() << '\n';
		this->getLibraryRequirementList(static_cast<class Library*>(*iterator), prefix + "\t\t", ostream);
		ostream << prefix << "\t</li>\n";
	}

	ostream << prefix << "</ul>\n";
}

}
