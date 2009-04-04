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
#include <string>
//#include <stdlib>
#include <cstdio>
#include <vector>
#include <list>

#include "internationalisation.h"
#include "vjassdoc.h"
#include "parser.h"

using namespace vjassdoc;

int main(int argc, char *argv[])
{

#ifdef UNIX
	//bindtextdomain("vjassdoc", "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/locale");
	bindtextdomain("vjassdoc", "/usr/share/locale");
	textdomain("vjassdoc.mo");
#elif defined WIN32
	bindtextdomain("vjassdoc", ""); //NOTE Change this value
#endif


/*
TODO set LOCALE_DIR by cmake
#ifndef LOCALE_DIR
#error You have to define LOCALE_DIR.
#else
	bindtextdomain("vjassdoc", "LOCALE_DIR");
	textdomain("vjassdoc");
#endif
*/
	if (argc >= 1)
	{
		if (strcmp(argv[1], "--version") == 0)
		{
			printf("vjassdoc %d.\n", Vjassdoc::version);
			std::cout << _(
			"Copyright © 2008, 2009 Tamino Dauth\n"
			"License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>\n"
			"This is free software: you are free to change and redistribute it.\n"
			"There is NO WARRANTY, to the extent permitted by law."
			) << std::endl;
			
			return EXIT_SUCCESS;
		}
		else if (strcmp(argv[1], "--help") == 0)
		{
			std::cout <<
			_("vjassdoc options:\n") <<
			_("--version            Shows the current version of vjassdoc.\n") <<
			_("-j --jass            vJass code will be ignored.\n") <<
			_("-d --debug           Lines starting with the vJass keyword \'debug\' won't be ignored.\n") <<
			_("--private            Private objects will be parsed.\n") <<
			_("-m --textmacros      Code between text macro statements will be parsed.\n") <<
			_("-h --html            Program creates a simple HTML API documentation.\n") <<
			_("-p --pages           Program creates an HTML file for each parsed object.\n") <<
			_("-s --specialpages    Program creates additional HTML files containing more information about all parsed objects.\n") <<
#ifdef SQLITE
			_("-b --database        Parsed objects will be saved in a SQLite3 database which could be read out by other programs.\n") <<
#endif
			_("-v --verbose         Program shows more information about the process.\n") <<
			_("-t --time            Detects the remaining time and shows it at the end of the process.\n") <<
			_("-a --alphabetical    All objects will be aranged in an alphabetical order.\n") <<
			_("--title <arg>        <arg> has to be replaced by the title which is used for the API documentation.\n") <<
			_("--importdirs <args>  <args> has to be replaced by one or more import directories (Used for the //! import macro in vJass).\n") <<
			_("--files <args>       <args> has to be replaced by the files which should be parsed.\n") <<
			_("--dir <arg>          <arg> has to be replaced by the output directory path.\n") <<
			_("\nReport bugs to tamino@cdauth.de") <<
			std::endl;

			return EXIT_SUCCESS;
		}
	}
	else
	{
		std::cerr << _("Missing arguments.\nUse --help to get some information about all available options.") << std::endl;
		
		return EXIT_FAILURE;
	}

	bool jass = false;
	bool debug = false;
	bool parsePrivate = false;
	bool textmacros = false;
	bool html = false;
	bool pages = false;
	bool specialPages = false;
	bool database = false;
	bool verbose = false;
	bool time = false;
	bool alphabetical = false;
	static const char* objectListOption[Parser::MaxLists] =
	{
		"--nocomments",
		"--nokeywords",
		"--notextmacros",
		"--notextmacroinstances",
		"--notypes",
		"--noglobals",
		"--nomembers",
		"--nofunctioninterfaces",
		"--nofunctions",
		"--nomethods",
		"--nointerfaces",
		"--nostructs",
		"--noscopes",
		"--nolibraries",
		"--nosourcefiles",
		"--nodoccomments"
	};
	bool parseObjectsOfList[Parser::MaxLists] =
	{
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true,
		true
	};
	std::string title;
	std::string dir;
	bool needImportDirs = false;
	bool needFileDirs = false;
	std::list<std::string> *importDirs = new std::list<std::string>;
	std::list<std::string> *filePaths = new std::list<std::string>;

	for (int i = 0; i < argc; ++i)
	{
		if (argv[i][0] == '-' && argv[i][1] != '-')
		{
			char option = argv[i][1];
			int j = 2;
			
			do
			{
				switch (option)
				{
					case 'j':
						jass = true;
						break;
					
					case 'd':
						debug = true;
						break;
					
					case 'm':
						textmacros = true;
						break;
					
					case 'h':
						html = true;
						break;
					
					case 'p':
						pages = true;
						break;
					
					case 's':
						specialPages = true;
						break;
#ifdef SQLITE					
					case 'b':
						database = true;
						break;
#endif
					
					case 'v':
						verbose = true;
						break;
					
					case 't':
						time = true;
						break;
					
					case 'a':
						alphabetical = true;
						break;
					
					default:
					{
						fprintf(stderr, _("Unknown option: %c.\n"), option);
						break;
					}
				}
				
				option = argv[i][j];
				++j;
			}
			while (option != '\0');
		}
		else if (strcmp(argv[i], "--jass") == 0)
		{
			jass = true;
		}
		else if (strcmp(argv[i], "--debug") == 0)
		{
			debug = true;
		}
		else if (strcmp(argv[i], "--private") == 0)
		{
			parsePrivate = true;
		}
		else if (strcmp(argv[i], "--textmacros") == 0)
		{
			textmacros = true;
		}
		else if (strcmp(argv[i], "--html") == 0)
		{
			html = true;
		}
		else if (strcmp(argv[i], "--pages") == 0)
		{
			pages = true;
		}
		else if (strcmp(argv[i], "--specialpages") == 0)
		{
			specialPages = true;
		}
#ifdef SQLITE
		else if (strcmp(argv[i], "--database") == 0)
		{
			database = true;
		}
#endif
		else if (strcmp(argv[i], "--verbose") == 0)
		{
			verbose = true;
		}
		else if (strcmp(argv[i], "--time") == 0)
		{
			time = true;
		}
		else if (strcmp(argv[i], "--alphabetical") == 0)
		{
			alphabetical = true;
		}
		else if (strcmp(argv[i], "--title") == 0)
		{
			if (i == argc - 1)
			{
				std::cerr << _("Missing title argument.") << std::endl;
				break;
			}
			
			++i;
			title = argv[i];
		}
		else if (strcmp(argv[i], "--importdirs") == 0)
		{
			needFileDirs = false;
			needImportDirs = true;
		}
		else if (strcmp(argv[i], "--files") == 0)
		{
			needImportDirs = false;
			needFileDirs = true;
		}
		else if (strcmp(argv[i], "--dir") == 0)
		{
			if (i == argc - 1)
			{
				std::cerr << _("Missing directory argument.") << std::endl;
				break;
			}
			
			++i;
			dir = argv[i];
		}
		else if (needImportDirs)
		{
			importDirs->push_back(argv[i]);
		}
		else if (needFileDirs)
		{
			filePaths->push_back(argv[i]);
		}
		else
		{
			bool found = false;
		
			for (int j = 0; j < Parser::MaxLists; ++j)
			{
				if (strcmp(argv[i], objectListOption[j]) == 0)
				{
					found = true;
				
					if (parseObjectsOfList[j])
						parseObjectsOfList[j] = false;
					else
						fprintf(stderr, _("Objects of list %d already won't be parsed.\n"), j);
				}
			}
			
			if (!found)
				fprintf(stderr, _("Unknown argument: %s.\n"), argv[i]);
		}
	}

	if (filePaths->size() == 0)
	{
		std::cerr << _("Missing file arguments.") << std::endl;
		return EXIT_FAILURE;
	}
	
	if (pages && !html)
		std::cerr << _("Warning: Although there won't be created any HTML files you have used the --pages option.") << std::endl;

	if (title.empty())
		title = _("vJass API Documentation");
	
	Vjassdoc::run(jass, debug, parsePrivate, textmacros, html, pages, specialPages, database, verbose, time, alphabetical, parseObjectsOfList, title, dir, importDirs, filePaths);

	return EXIT_SUCCESS;
}
