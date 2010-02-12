/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <list>

#include <getopt.h>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include "../lang/parser.hpp"
#include "../internationalisation.hpp"

using namespace vjassdoc;

int main(int argc, char *argv[])
{
	// Set the current locale.
	setlocale(LC_ALL, "");
	// Set the text message domain.
	bindtextdomain("jassc", LOCALE_DIR);
	textdomain("jassc");
	
	static struct option options[] =
	{
		{"version",                 no_argument,             0, 'V'},
		{"help",                    no_argument,             0, 'h'},
		{"recursive",               required_argument,       0, 'R'},
		{"include",                 required_argument,       0, 'I'},
		{"showerrors",              no_argument,             0, 's'},
		{"verify",                  no_argument,             0, 'v'},
#ifdef HTML
		{"html",                    required_argument,       0, 'H'},
#endif
#ifdef SQLITE
		{"sqlite",                  required_argument,       0, 'L'},
#endif
		{"map",                     required_argument,       0, 'M'},
		{"mapscript",               required_argument,       0, 'T'},
		{"script",                  required_argument,       0, 'P'},
		{"optimize",                required_argument,       0, 'O'},
		{"language",                required_argument,       0, 'A'},
		{0, 0, 0, 0}
	};
	
	static const char *version = "0.1";
	boost::filesystem::path recursivePath;
	std::list<boost::filesystem::path> includeDirs;
	bool showErrors = false;
	bool verify = false;
	boost::filesystem::path htmlPath;
	boost::filesystem::path sqlitePath;
	boost::filesystem::path mapPath;
	boost::filesystem::path mapScriptPath;
	boost::filesystem::path scriptPath;
	std::string optimize;
	std::string language;
	int optionShortcut;
	
	while (true)
	{
		int optionIndex = 0;
		optionShortcut = getopt_long(argc, argv, "VhR:I:svH:L:M:T:P:O:A:", options, &optionIndex);

		if (optionShortcut == -1)
			break;
     
		switch (optionShortcut)
		{
			case 'V':
			{
				std::cout << boost::format(_(
				"jassc %1%.\n"
				"Copyright © 2008, 2009 Tamino Dauth\n"
				"License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law."
				)) % version << std::endl;
				
				return EXIT_SUCCESS;
			}
			
			case 'h':
			{
				std::cout <<
				boost::format(_("jassc %1%.\n\n")) % version <<
				_("Usage: vjassdoc [Options] [Code files]\n\n") <<
				_("Options:\n") <<
				_("\t-V --version                Shows the current version of vjassdoc.\n") <<
				_("\t-h --help                   Shows this text.\n") <<
				_("\t-j --jass                   vJass code will be ignored.\n") <<
				_("\t-d --debug                  Lines starting with the vJass keyword \'debug\' won't be ignored.\n") <<
				_("\t-p --private                Private objects will be parsed.\n") <<
				_("\t-m --textmacros             Code between text macro statements will be parsed (Warning: There can be many parsing errors!).\n") <<
				_("\t-f --functions              Code between function/method statements will be parsed.\n") <<
				_("\t--no<object type name>      Objects of type <object type name> won't added to the output files.\n") <<
				_("\t                            The following object type names are available:\n") <<
				_("\t                            comments\n") <<
				_("\t                            keywords\n") <<
				_("\t                            textmacros\n") <<
				_("\t                            textmacroinstances\n") <<
				_("\t                            externalcalls\n") <<
				_("\t                            types\n") <<
				_("\t                            locals\n") <<
				_("\t                            globals\n") <<
				_("\t                            members\n") <<
				_("\t                            parameters\n") <<
				_("\t                            functioninterfaces\n") <<
				_("\t                            functions\n") <<
				_("\t                            methods\n") <<
				_("\t                            calls\n") <<
				_("\t                            implementations\n") <<
				_("\t                            hooks\n") <<
				_("\t                            interfaces\n") <<
				_("\t                            structs\n") <<
				_("\t                            modules\n") <<
				_("\t                            scopes\n") <<
				_("\t                            libraries\n") <<
				_("\t                            sourcefiles\n") <<
				_("\t                            doccomments\n") <<
				_("\t-l --html                   Creates a simple HTML API documentation.\n") <<
				_("\t-g --pages                  Creates an HTML file for each parsed object.\n") <<
				_("\t-s --specialpages           Creates additional HTML files containing more information about all parsed objects.\n") <<
				_("\t-x --syntax                 Checks syntax. Not implemented yet!\n") <<
				_("\t-C --compile <arg>          Uses file <arg> to create a map Jass script.\n") <<
#ifdef SQLITE
				_("\t-L --database <arg>         Uses file <arg> to create an SQLite3 database which contains all parsed objects.\n") <<
#endif
				_("\t-v --verbose                Shows more information about the process.\n") <<
				_("\t-t --time                   Detects the elapsed time and shows it at the end of the process.\n") <<
				_("\t-a --alphabetical           All objects will be aranged in alphabetical order.\n") <<
				_("\t-T --title <arg>            <arg> has to be replaced by the title which is used for the API documentation.\n") <<
				_("\t-I --importdirs <args>      <args> has to be replaced by one or more import directories (Used for the //! import macro in vJass).\n") <<
				_("\t-D --dir <arg>              <arg> has to be replaced by the output directory path.\n") <<
#ifdef SQLITE			
				_("\t-B --databases <args>       <args> has to be replaced by the SQLite3 databases which should be added to the output.\n") <<
#endif
				std::endl <<
				_("Several arguments has to be separated by using the : character.\n") <<
				_("\nReport bugs to tamino@cdauth.de or on http://sourceforge.net/projects/vjasssdk/") <<
				std::endl;
	
				return EXIT_SUCCESS;
			}
			
			case 'j':
				jass = true;
				
				break;

			case 'd':
				debug = true;
				
				break;
			
			case 'p':
				parsePrivate = true;
				
				break;
			
			case 'm':
				textmacros = true;
				
				break;
			
			case 'f':
				functions = true;
				
				break;
			
			case 'l':
				html = true;
				
				break;
			
			case 'g':
				pages = true;
				
				break;
			
			case 's':
				specialPages = true;
				
				break;
			
			case 'x':
				syntax = true;
				
				break;
				
			case 'C':
				compileFilePath = optarg;
				
				break;
#ifdef SQLITE					
			case 'L':
				databaseFilePath = optarg;
				
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
			
			case 'T':
				title = optarg;
				
				break;

			case 'I':
				for (char *path = strtok(optarg, ":"); path != 0;  path = strtok(0, ":"))
				{
					for (std::list<boost::filesystem::path>::const_iterator iterator = importDirs.begin(); iterator != importDirs.end(); ++iterator)
					{
						if (iterator->string() == path)
						{
							std::cerr << boost::format( _("Import directory path \"%1%\" has already been added to list.")) % path << std::endl;
							
							continue;
						}
					}

					importDirs.push_back(path);
				}
				
				break;
			
			case 'D':
				dir = optarg;
				
				break;
			
#ifdef SQLITE			
			case 'B':
				for (char *path = strtok(optarg, ":"); path != 0;  path = strtok(0, ":"))
				{
					for (std::list<boost::filesystem::path>::const_iterator iterator = databases.begin(); iterator != databases.end(); ++iterator)
					{
						if (iterator->string() == path)
						{
							std::cerr << boost::format(_("Database \"%1%\" has already been added to list.")) % path << std::endl;
							
							continue;
						}
					}
		
					databases.push_back(path);
				}
				
				break;
#endif
				
			default:
				for (int j = 0; j < Parser::MaxLists; ++j)
				{
					if (strcmp(options[optionIndex].name, objectListOption[j]) == 0)
					{					
						if (parseObjectsOfList[j])
							parseObjectsOfList[j] = false;
						else
							std::cerr << boost::format(_("Objects of list %1% already won't be parsed.")) % j << std::endl;
						
						break;
					}
				}
				
				break;
		}
	}
		
	
	if (optind < argc)
	{
		for ( ; optind < argc; ++optind)
		{
			for (std::list<boost::filesystem::path>::const_iterator iterator = filePaths.begin(); iterator != filePaths.end(); ++iterator)
			{
				if (iterator->string() == argv[optind])
				{
						std::cerr << boost::format(_("File path \"%1%\" has already been added to list.")) % argv[optind] << std::endl;
						
						continue;
				}
			}
			
			filePaths.push_back(argv[optind]);
		}

	}
	else
	{
		std::cerr << _("Missing file arguments.") << std::endl;
		
		return EXIT_FAILURE;
	}
	
	if (pages && !html)
		std::cerr << _("Warning: Although there won't be created any HTML files you have used the --pages option.") << std::endl;

	if (title.empty())
		title = _("vJass API Documentation");

	Vjassdoc::configure(jass, debug, parsePrivate, textmacros, functions, html, pages, specialPages, syntax, compileFilePath, databaseFilePath, verbose, time, alphabetical, parseObjectsOfList, title, dir, importDirs, filePaths, databases);

#ifdef SQLITE
	Vjassdoc::initClasses();
#endif

	Vjassdoc::run();
	Vjassdoc::clear();

	return EXIT_SUCCESS;
}
