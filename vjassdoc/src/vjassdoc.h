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

#ifndef VJASSDOC_VJASSDOC_H
#define VJASSDOC_VJASSDOC_H

#include <string>
#include <list>

#include "parser.h"

namespace vjassdoc
{

class Parser;

class Vjassdoc
{
	public:
		static const char *version;
		static const char *dirSeparator;
		static bool supportsDatabaseCreation;

		static void run(bool jass, bool debug, bool privateSpace, bool textmacros, bool html, bool pages, bool specialPages, bool database, bool verbose, bool time, bool alphabetical, bool parseObjectsOfList[Parser::MaxLists], const std::string &title, const std::string &dir, std::list<std::string> importDirs, std::list<std::string> filePaths, std::list<std::string> databases);
		static class Parser* getParser();
		static bool jassOnly();
		static bool useDebugMode();
		static bool parsePrivateSpace();
		static bool parseTextMacroSpace();
		static bool saveAsHtml();
		static bool createPages();
		static bool createSpecialPages();
		static bool createDatabase();
		static bool showVerbose();
		static bool sortAlphabetically();
		static bool shouldParseObjectsOfList(const enum Parser::List &list);
		static std::string getTitle();
		static std::string getDir();
		static std::list<std::string> getImportDirs();
		static std::list<std::string> getFilePaths();
		static std::list<std::string> getDatabases();
		
		static void addLines(const unsigned int &addedLines);
		static void addFile();
		static unsigned int getFiles();
	private:
		Vjassdoc();
		Vjassdoc(Vjassdoc&);
		~Vjassdoc();

		static class Parser *parser;
		static bool jass;
		static bool debug;
		static bool privateSpace;
		static bool textmacros;
		static bool html;
		static bool pages;
		static bool specialPages;
		static bool database;
		static bool verbose;
		static bool time;
		static bool alphabetical;
		static bool parseObjectsOfList[Parser::MaxLists];
		static std::string title;
		static std::string dir;
		static std::list<std::string> importDirs;
		static std::list<std::string> filePaths;
		static std::list<std::string> databases;
		
		static unsigned int lines;
		static unsigned int files;
};

inline Parser* Vjassdoc::getParser()
{
	if (Vjassdoc::parser == 0)
		Vjassdoc::parser = new Parser();

	return Vjassdoc::parser;
}

inline bool Vjassdoc::jassOnly()
{
	return Vjassdoc::jass;
}

inline bool Vjassdoc::useDebugMode()
{
	return Vjassdoc::debug;
}

inline bool Vjassdoc::parsePrivateSpace()
{
	return Vjassdoc::privateSpace;
}

inline bool Vjassdoc::parseTextMacroSpace()
{
	return Vjassdoc::textmacros;
}

inline bool Vjassdoc::saveAsHtml()
{
	return Vjassdoc::html;
}

inline bool Vjassdoc::createPages()
{
	return Vjassdoc::pages;
}

inline bool Vjassdoc::createSpecialPages()
{
	return Vjassdoc::specialPages;
}

inline bool Vjassdoc::createDatabase()
{
	return Vjassdoc::database;
}

inline bool Vjassdoc::showVerbose()
{
	return Vjassdoc::verbose;
}

inline bool Vjassdoc::sortAlphabetically()
{
	return Vjassdoc::alphabetical;
}

inline bool Vjassdoc::shouldParseObjectsOfList(const enum Parser::List &list)
{
	return Vjassdoc::parseObjectsOfList[list];
}

inline std::string Vjassdoc::getTitle()
{
	return Vjassdoc::title;
}

inline std::string Vjassdoc::getDir()
{
	return Vjassdoc::dir;
}

inline std::list<std::string> Vjassdoc::getImportDirs()
{
	return Vjassdoc::importDirs;
}

inline std::list<std::string> Vjassdoc::getFilePaths()
{
	return Vjassdoc::filePaths;
}

inline std::list<std::string> Vjassdoc::getDatabases()
{
	return Vjassdoc::databases;
}

inline void Vjassdoc::addLines(const unsigned int &addedLines)
{
	lines += addedLines;
}

inline void Vjassdoc::addFile()
{
	files += 1;
}

inline unsigned int Vjassdoc::getFiles()
{
	return files;
}

}

#endif
