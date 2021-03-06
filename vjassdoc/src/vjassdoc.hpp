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

#ifndef VJASSDOC_VJASSDOC_HPP
#define VJASSDOC_VJASSDOC_HPP

#include <string>
#include <list>

#include <boost/filesystem.hpp>

#include "parser.hpp"
#include "compiler.hpp"

namespace vjassdoc
{

class Parser;
class Language;

class Vjassdoc
{
	public:
		static const char *version;
		static const bool supportsDatabaseCreation;

		static void init();
		static void configure(bool optionJass, bool optionDebug, bool optionPrivate, bool optionTextmacros, bool optionFunctions, bool optionHtml, bool optionPages, bool optionSpecialpages, bool optionSyntax, const std::string &optionCompile, const std::string &optionDatabase, bool optionVerbose, bool optionTime, bool optionAlphabetical, bool optionParseObjectsOfList[Parser::MaxLists], const std::string &optionTitle, const std::string &optionDir, std::list<boost::filesystem::path> optionImport, std::list<boost::filesystem::path> optionFiles, std::list<boost::filesystem::path> optionDatabases);
		static void run();
		static void clear();
		static class Parser* parser();
		static class Compiler* compiler();
		static const std::list<class Language*>& languages();
		static std::size_t lines();
		static std::size_t files();
		static double duration();
		static double cpuDuration();
		static bool optionJass();
		static bool optionDebug();
		static bool optionPrivate();
		static bool optionTextmacros();
		static bool optionFunctions();
		static bool optionHtml();
		static bool optionPages();
		static bool optionSpecialpages();
		static bool optionSyntax();
		static std::string optionCompile();
		static std::string optionDatabase();
		static bool optionVerbose();
		static bool optionTime();
		static bool optionAlphabetical();
		static bool optionParseObjectsOfList(const enum Parser::List &list);
		static std::string optionTitle();
		static std::string optionDir();
		static std::list<boost::filesystem::path> optionImport();
		static std::list<boost::filesystem::path> optionFiles();
		static std::list<boost::filesystem::path> optionDatabases();

	protected:
		static class Parser *m_parser;
		static class Compiler *m_compiler;
		static std::list<class Language*> m_languages;
		static std::size_t m_lines;
		static std::size_t m_files;
		static double m_duration;
		static double m_cpuDuration;
		static bool m_optionJass;
		static bool m_optionDebug;
		static bool m_optionPrivate;
		static bool m_optionTextmacros;
		static bool m_optionFunctions;
		static bool m_optionHtml;
		static bool m_optionPages;
		static bool m_optionSpecialpages;
		static bool m_optionSyntax;
		static std::string m_optionCompile;
		static std::string m_optionDatabase;
		static bool m_optionVerbose;
		static bool m_optionTime;
		static bool m_optionAlphabetical;
		static bool m_optionParseObjectsOfList[Parser::MaxLists];
		static std::string m_optionTitle;
		static std::string m_optionDir;
		static std::list<boost::filesystem::path> m_optionImport;
		static std::list<boost::filesystem::path> m_optionFiles;
		static std::list<boost::filesystem::path> m_optionDatabases;
		
	private:
		Vjassdoc();
		Vjassdoc(Vjassdoc&);
		~Vjassdoc();
};

inline Parser* Vjassdoc::parser()
{
	if (Vjassdoc::m_parser == 0)
		Vjassdoc::m_parser = new Parser();

	return Vjassdoc::m_parser;
}

inline Compiler* Vjassdoc::compiler()
{
	if (Vjassdoc::m_compiler == 0)
		Vjassdoc::m_compiler = new Compiler();

	return Vjassdoc::m_compiler;
}

inline const std::list<class Language*>& Vjassdoc::languages()
{
	return Vjassdoc::m_languages;
}

inline std::size_t Vjassdoc::lines()
{
	return Vjassdoc::m_lines;
}

inline std::size_t Vjassdoc::files()
{
	return Vjassdoc::m_files;
}

inline double Vjassdoc::duration()
{
	return Vjassdoc::m_duration;
}

inline double Vjassdoc::cpuDuration()
{
	return Vjassdoc::m_cpuDuration;
}

inline bool Vjassdoc::optionJass()
{
	return Vjassdoc::m_optionJass;
}

inline bool Vjassdoc::optionDebug()
{
	return Vjassdoc::m_optionDebug;
}

inline bool Vjassdoc::optionPrivate()
{
	return Vjassdoc::m_optionPrivate;
}

inline bool Vjassdoc::optionTextmacros()
{
	return Vjassdoc::m_optionTextmacros;
}

inline bool Vjassdoc::optionFunctions()
{
	return Vjassdoc::m_optionFunctions;
}

inline bool Vjassdoc::optionHtml()
{
	return Vjassdoc::m_optionHtml;
}

inline bool Vjassdoc::optionPages()
{
	return Vjassdoc::m_optionPages;
}

inline bool Vjassdoc::optionSpecialpages()
{
	return Vjassdoc::m_optionSpecialpages;
}

inline bool Vjassdoc::optionSyntax()
{
	return Vjassdoc::m_optionSyntax;
}

inline std::string Vjassdoc::optionCompile()
{
	return Vjassdoc::m_optionCompile;
}

inline std::string Vjassdoc::optionDatabase()
{
	return Vjassdoc::m_optionDatabase;
}

inline bool Vjassdoc::optionVerbose()
{
	return Vjassdoc::m_optionVerbose;
}

inline bool Vjassdoc::optionTime()
{
	return Vjassdoc::m_optionTime;
}

inline bool Vjassdoc::optionAlphabetical()
{
	return Vjassdoc::m_optionAlphabetical;
}

inline bool Vjassdoc::optionParseObjectsOfList(const enum Parser::List &list)
{
	return Vjassdoc::m_optionParseObjectsOfList[list];
}

inline std::string Vjassdoc::optionTitle()
{
	return Vjassdoc::m_optionTitle;
}

inline std::string Vjassdoc::optionDir()
{
	return Vjassdoc::m_optionDir;
}

inline std::list<boost::filesystem::path> Vjassdoc::optionImport()
{
	return Vjassdoc::m_optionImport;
}

inline std::list<boost::filesystem::path> Vjassdoc::optionFiles()
{
	return Vjassdoc::m_optionFiles;
}

inline std::list<boost::filesystem::path> Vjassdoc::optionDatabases()
{
	return Vjassdoc::m_optionDatabases;
}

}

#endif
