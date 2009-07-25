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

#include <sys/time.h>
#include <ctime>
#include <iostream>
#include <cstdio>

#include "vjassdoc.h"
#include "internationalisation.h"
#include "objects.h"

namespace vjassdoc
{

const char *Vjassdoc::version = "0.3";
#ifdef UNIX
const char *Vjassdoc::dirSeparator = "/";
#elif defined WIN32
const char *Vjassdoc::dirSeparator = "\\\\";
#else
#error You have to define UNIX or WIN32.
#endif
#ifdef SQLITE
bool Vjassdoc::supportsDatabaseCreation = true;
#else
bool Vjassdoc::supportsDatabaseCreation = false;
#endif
class Parser *Vjassdoc::parser = 0;
class Compiler *Vjassdoc::compiler = 0;
bool Vjassdoc::jass;
bool Vjassdoc::debug;
bool Vjassdoc::privateSpace;
bool Vjassdoc::functions;
bool Vjassdoc::textmacros;
bool Vjassdoc::html;
bool Vjassdoc::pages;
bool Vjassdoc::specialPages;
bool Vjassdoc::syntax;
std::string Vjassdoc::m_compileFilePath;
bool Vjassdoc::database;
bool Vjassdoc::verbose;
bool Vjassdoc::time;
bool Vjassdoc::alphabetical;
bool Vjassdoc::parseObjectsOfList[Parser::MaxLists];
std::string Vjassdoc::title;
std::string Vjassdoc::dir;
std::list<std::string> Vjassdoc::importDirs = std::list<std::string>();
std::list<std::string> Vjassdoc::filePaths = std::list<std::string>();
std::list<std::string> Vjassdoc::databases = std::list<std::string>();
unsigned int Vjassdoc::lines = 0;
unsigned int Vjassdoc::files = 0;

void Vjassdoc::configure(bool jass, bool debug, bool privateSpace, bool textmacros, bool functions, bool html, bool pages, bool specialPages, bool syntax, const std::string &compileFilePath, bool database, bool verbose, bool time, bool alphabetical, bool parseObjectsOfList[Parser::MaxLists], const std::string &title, const std::string &dir, std::list<std::string> importDirs, std::list<std::string> filePaths, std::list<std::string> databases)
{
	Vjassdoc::jass = jass;
	Vjassdoc::debug = debug;
	Vjassdoc::privateSpace = privateSpace;
	Vjassdoc::textmacros = textmacros;
	Vjassdoc::functions = functions;
	Vjassdoc::html = html;
	Vjassdoc::pages = pages;
	Vjassdoc::specialPages = specialPages;
	Vjassdoc::syntax = syntax;
	Vjassdoc::m_compileFilePath = compileFilePath;
	Vjassdoc::database = database;
	Vjassdoc::verbose = verbose;
	Vjassdoc::alphabetical = alphabetical;

	for (int i = 0; i < Parser::MaxLists; ++i)
		Vjassdoc::parseObjectsOfList[i] = parseObjectsOfList[i];
	
	Vjassdoc::title = title;
	Vjassdoc::dir = dir;
	Vjassdoc::importDirs = importDirs;
	Vjassdoc::filePaths = filePaths;
	Vjassdoc::databases = databases;
}

void Vjassdoc::initClasses()
{
	std::cout << "Before initialization." << std::endl;
	Object::initClass();
	Comment::initClass();
	Keyword::initClass();
	Key::initClass();
	TextMacro::initClass();
	TextMacroInstance::initClass();
	Type::initClass();
	Local::initClass();
	Global::initClass();
	Member::initClass();
	Parameter::initClass();
	FunctionInterface::initClass();
	Function::initClass();
	Method::initClass();
	Implementation::initClass();
	Hook::initClass();
	Interface::initClass();
	Struct::initClass();
	Module::initClass();
	Scope::initClass();
	Library::initClass();
	SourceFile::initClass();
	DocComment::initClass();
	std::cout << "After initialization." << std::endl;
}

void Vjassdoc::run()
{
	Vjassdoc::lines = 0;
	Vjassdoc::files = 0;
	
	timeval timeValue;
	long beginMicroseconds, endMicroseconds;
	clock_t beginCpu, endCpu;
	
	if (time)
	{
		gettimeofday(&timeValue, 0);
		beginMicroseconds = long(1e6) * timeValue.tv_sec + timeValue.tv_usec;
		beginCpu = clock();
	}
	
#ifdef UNIX
		if (verbose)
			std::cout << _("UNIX mode has been detected.") << std::endl;
#elif defined WIN32
		if (verbose)
			std::cout << _("Win32 mode has been detected.") << std::endl;
#else
#error You have to define UNIX or WIN32.
#endif

	
	Vjassdoc::getParser()->parse();

	if (!Vjassdoc::compileFilePath().empty())
		Vjassdoc::getCompiler()->compile();
	
	if (time)
	{
		gettimeofday(&timeValue, 0);
		endMicroseconds = long(1e6) * timeValue.tv_sec + timeValue.tv_usec;
		endCpu = clock();
		double duration = (double)(endMicroseconds - beginMicroseconds) / double(1e6);
		double cpuDuration = (double)(endCpu - beginCpu) / double(CLOCKS_PER_SEC);
		printf(_("Duration:\n%f seconds\nCPU duration:\n%f seconds\n"), duration, cpuDuration);
	}
	
	printf(_("Finished (%d lines, %d files).\n"), lines, files);
}

void Vjassdoc::clear()
{
	if (Vjassdoc::parser != 0)
	{
		delete Vjassdoc::parser;
		Vjassdoc::parser = 0;
	}

	if (Vjassdoc::compiler != 0)
	{
		delete Vjassdoc::compiler;
		Vjassdoc::compiler = 0;
	}
}

}
