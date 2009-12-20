/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
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
#include <list>
#include <cstring>
#include <cstdlib>
//POSIX
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "parser.hpp"
#include "string.hpp"
#include "options.hpp"
#include "internationalisation.hpp"


using namespace vjasstrans;

static std::list<class String*> strings;

static int select(const struct dirent *dirEntry)
{
	return 1;
}

static void addDirectory(const std::string &directoryPath, std::list<std::string> &filePaths)
{
	DIR *dir = opendir(directoryPath.c_str());
	
	if (dir == NULL)
	{
		fprintf(stderr, _("Error while opening directory \"%s\".\n"), directoryPath.c_str());
		
		return;
	}
	
	struct dirent **dirEntries = 0;
	int entries = scandir(directoryPath.c_str(), &dirEntries,
              select,
              0);
	
	if (entries == -1)
	{
		fprintf(stderr, _("Error while reading directory \"%s\".\n"), directoryPath.c_str());
		
		return;
	}
	
	for (int i = 0; i < entries; ++i)
	{
		if (strcmp(dirEntries[i]->d_name, ".") != 0 && strcmp(dirEntries[i]->d_name, "..") != 0)
		{
			struct stat fileInfo;
			std::string path = directoryPath + '/' + dirEntries[i]->d_name;
			
			if (stat(path.c_str(), &fileInfo) == -1)
			{
				fprintf(stderr, _("Error while reading file \"%s\".\n"), path.c_str());
				
				continue;
			}
			
			if (fileInfo.st_mode & S_IFDIR)
			{
				fprintf(stderr, _("Is another dir \"%s\".\n"), path.c_str());
			
				if (optionRecursive)
					addDirectory(path.c_str(), filePaths);
			}
			else
				filePaths.push_back(path.c_str());
		}
	}
	
	delete[] dirEntries;
	
	if (closedir(dir) == -1)
		fprintf(stderr, _("Error while closing directory \"%s\".\n"), directoryPath.c_str());

}

int main(int argc, char *argv[])
{
	// Set the current locale.
	setlocale(LC_ALL, "");
	// Set the text message domain.
	bindtextdomain("vjasstrans", LOCALE_DIR);
	textdomain("vjasstrans");
	
	if (argc == 1)
	{
		std::cout << _("Missing arguments.") << std::endl;
		
		return EXIT_FAILURE;
	}
	
	
	if (strcmp(argv[1], "--version") == 0)
	{
		printf("vjasstrans %s.\n", version);
		std::cout << _(
		"Copyright © 2009 Tamino Dauth\n"
		"License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>\n"
		"This is free software: you are free to change and redistribute it.\n"
		"There is NO WARRANTY, to the extent permitted by law."
		) << std::endl;
		
		return EXIT_SUCCESS;
	}
	else if (strcmp(argv[1], "--help") == 0)
	{
		std::cout <<
		_("Usage:\n") <<
		_("vjasstrans [options] <files or directories>\n\n") <<
		_("Options:\n") <<
		_("--version                   Shows the current version of vjasstrans.\n") <<
		_("--fill                      Fills string values with default strings when adding them from code files.\n") <<
		_("--replace                   Replaces all translated strings by STRING_<id> in code files.\n") <<
		_("--recursive                 Parses all source files in sub directories.\n") <<
		_("--fdf                       Creates file with all parsed strings.\n") <<
		_("--wts                       Creates file with all parsed strings.\n") <<
		_("--afdf                      Appends file with all parsed strings.\n") <<
		_("--awts                      Appends file with all parsed strings.")
		<< std::endl
		;
		fprintf(stdout, _("--trans <arg>               <arg> has to be the name of the translation function. Default value: %s.\n"), optionTrans.c_str());
		fprintf(stdout, _("--wtspath <arg>             <arg> has to be the file path of wts file. Default value: %s.\n"), optionWtspath.c_str());
		fprintf(stdout, _("--fdfpath <arg>             <arg> has to be the file path of fdf file. Default value: %s.\n"), optionFdfpath.c_str());
		std::cout <<
		_("\nReport bugs to tamino@cdauth.de or on http://sourceforge.net/projects/vjasssdk/") <<
		std::endl;
		
		return EXIT_SUCCESS;
	}
	
	std::list<std::string> filePaths;
	
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--fill") == 0)
		{
			optionFill = true;
		}
		if (strcmp(argv[i], "--replace") == 0)
		{
			optionReplace = true;
		}
		else if (strcmp(argv[i], "--recursive") == 0)
		{
			optionRecursive = true;
		}
		else if (strcmp(argv[i], "--fdf") == 0)
		{
			optionFdf = true;
		}
		else if (strcmp(argv[i], "--wts") == 0)
		{
			optionWts = true;
		}
		else if (strcmp(argv[i], "--awts") == 0)
		{
			optionAwts = true;
		}
		else if (strcmp(argv[i], "--afdf") == 0)
		{
			optionAfdf = true;
		}
		else if (strcmp(argv[i], "--trans") == 0)
		{
			if (++i == argc)
			{
				std::cerr << "Missing argument of option --trans." << std::endl;
				
				return EXIT_FAILURE;
			}
			
			optionTrans = argv[i];
		}
		else if (strcmp(argv[i], "--wtspath") == 0)
		{
			if (++i == argc)
			{
				std::cerr << "Missing argument of option --wtspath." << std::endl;
				
				return EXIT_FAILURE;
			}
			
			optionWtspath = argv[i];
		}
		else if (strcmp(argv[i], "--fdfpath") == 0)
		{
			if (++i == argc)
			{
				std::cerr << "Missing argument of option --fdfpath." << std::endl;
				
				return EXIT_FAILURE;
			}
			
			optionFdfpath = argv[i];
		}
		else
			filePaths.push_back(argv[i]);
	}
			
	// filtering non-existing files and directories
	for (std::list<std::string>::iterator iterator = filePaths.begin(); iterator != filePaths.end(); ++iterator)
	{
		struct stat fileInfo;
		
		if (stat(iterator->c_str(), &fileInfo) != 0)
		{
			fprintf(stderr, "File or directory \"%s\" does not exist.\n", iterator->c_str());
			//filePaths.erase(iterator); /// @todo 
		}
		// is directory
		else if (fileInfo.st_mode & S_IFDIR)
		{
			if (optionRecursive)
				addDirectory(*iterator, filePaths);
		
			//filePaths.erase(iterator);
		}
	}
	
	if (optionAfdf)
		Parser::readFdf(optionFdfpath, strings);
	
	if (optionAwts)
		Parser::readWts(optionWtspath, strings);
	
	for (std::list<std::string>::iterator iterator = filePaths.begin(); iterator != filePaths.end(); ++iterator)
		Parser::parse(*iterator, strings, optionReplace, optionTrans);
	
	if (optionFdf || optionAfdf)
		Parser::writeFdf(optionFdfpath, strings);
	
	if (optionWts || optionAwts)
		Parser::writeWts(optionWtspath, strings);

	return EXIT_SUCCESS;
}
