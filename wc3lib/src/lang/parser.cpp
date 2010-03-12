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

#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <boost/format.hpp>
#include <boost/foreach.hpp>

#ifdef SQLITE
#include <sqlite3.h>
#endif

#include "parser.hpp"
#include "language.hpp"
#include "internationalisation.hpp"
#include "utilities.hpp"

namespace wc3lib
{
	
namespace lang
{

Parser::Parser() : m_file(0), m_currentLanguage(0)
{
}

Parser::~Parser()
{
	BOOST_FOREACH(class SyntaxError *syntaxError, this->m_syntaxErrors)
			delete syntaxError;
}

std::size_t Parser::parse(const boost::filesystem::path &path, class Language *initialSourceFileLanguage) throw (class Exception)
{
	if (this->m_file != 0)
		throw Exception(_("Trying to parse during parsing."));
	
	if (initialSourceFileLanguage == 0)
		initialSourceFileLanguage = this->m_currentLanguage;
	
	if (initialSourceFileLanguage == 0)
		throw Exception(_("Intial language is missing."));
	
	if (!boost::filesystem::exists(path))
		throw Exception(Parser::FilePathError, boost::str(boost::format(_("File path \"%1%\" does not exist.")) % path.string()));

	std::ifstream ifstream(path.string().c_str());
	
	if (!ifstream)
		throw Exception(boost::str(boost::format(_("Unable to open file stream for file path \"%1%\".")) % path.string()));
	
	class SourceFile *sourceFile = new SourceFile(path.filename(), path.string());
	
	initialSourceFileLanguage->push_back(sourceFile);
	this->m_file = new File();
	std::size_t lines = this->m_file->parse(this, sourceFile, ifstream);
	delete this->m_file;
	this->m_file = 0;
	ifstream.close();
	
	return lines;
}

std::size_t Parser::parse(const std::list<boost::filesystem::path> &paths, class Language *initialSourceFileLanguage) throw (class Exception)
{
	std::size_t lines = 0;
	
	BOOST_FOREACH(const boost::filesystem::path &path, paths)
		lines += this->parse(paths, initialSourceFileLanguage);
	
	return lines;
}

void Parser::prepareObjects()
{
	BOOST_FOREACH(class Language *language, this->m_languages)
		language->prepareObjects();
}


void Parser::sortObjectsAlphabetically()
{
	BOOST_FOREACH(class Language *language, this->m_languages)
		language->sort(Object::AlphabeticalComparator());
}

void Parser::showSyntaxErrors(std::ostream &ostream)
{
	/// @todo compare all object identifiers with Jass and vJass keywords.
	ostream << boost::format(_("%1% syntax errors.")) % this->m_syntaxErrors.size() << std::endl;
	this->m_syntaxErrors.sort(SyntaxError::Comparator);
	
	BOOST_FOREACH(class SyntaxError *syntaxError, this->m_syntaxErrors)
		ostream << boost::format(_("File %1%, line %2%: %3%")) % syntaxError->sourceFile()->identifier() % syntaxError->line() % syntaxError->message() << std::endl;
}

#ifdef SQLITE
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
