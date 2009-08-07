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

#include "parser.h"
#include "string.h"

namespace vjasstrans
{

bool optionReplace = false;
bool optionRecursive = false;
bool optionFdf = false;
bool optionWts = false;

}

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
		fprintf(stderr, "Error while opening directory \"%s\".\n", directoryPath.c_str());
		return;
	}
	
	struct dirent **dirEntries = 0;
	int entries = scandir(directoryPath.c_str(), &dirEntries,
              select,
              0);
	
	if (entries == -1)
	{
		fprintf(stderr, "Error while reading directory \"%s\".\n", directoryPath.c_str());
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
				fprintf(stderr, "Error while reading file \"%s\".\n", path.c_str());
				continue;
			}
			
			if (fileInfo.st_mode & S_IFDIR)
			{
				fprintf(stderr, "Is another dir \"%s\".\n", path.c_str());
			
				if (optionRecursive)
					addDirectory(path.c_str(), filePaths);
			}
			else
				filePaths.push_back(path.c_str());
		}
	}
	
	delete[] dirEntries;
	
	if (closedir(dir) == -1)
		fprintf(stderr, "Error while closing directory \"%s\".\n", directoryPath.c_str());

}

int main(int argc, char *argv[])
{
	if (argc >= 1)
	{
		if (strcmp(argv[1], "--version") == 0)
		{
			std::cout << "1.0" << std::endl;

			return EXIT_SUCCESS;
		}
		else if (strcmp(argv[1], "--help") == 0)
		{
			std::cout << "HELP" << std::endl;

			return EXIT_SUCCESS;
		}
		else
		{
			std::list<std::string> filePaths;
		
			for (int i = 1; i < argc; ++i)
			{
				if (strcmp(argv[i], "--replace") == 0)
				{
					optionReplace = true;
					continue;
				}
				else if (strcmp(argv[i], "--recursive") == 0)
				{
					optionRecursive = true;
					continue;
				}
				else if (strcmp(argv[i], "--fdf") == 0)
				{
					optionFdf = true;
					continue;
				}
				else if (strcmp(argv[i], "--wts") == 0)
				{
					optionWts = true;
					continue;
				}
			
				filePaths.push_back(argv[i]);
			}
			
			// filtering non-existing files and directories
			for (std::list<std::string>::iterator iterator = filePaths.begin(); iterator != filePaths.end(); ++iterator)
			{
				struct stat fileInfo;
				std::cout << "Checking file " << *iterator << std::endl;
				
				if (stat((*iterator).c_str(), &fileInfo) != 0)
				{
					fprintf(stderr, "File or directory \"%s\" does not exist.\n", (*iterator).c_str());
					//filePaths.erase(iterator); /// @todo 
				}
				// is directory
				else if (fileInfo.st_mode & S_IFDIR)
				{
					std::cout << "Is directory " << *iterator << std::endl;
				
					if (optionRecursive)
						addDirectory(*iterator, filePaths);
					
					//filePaths.erase(iterator);
				}
			}
			
			class Parser parser;
			
			for (std::list<std::string>::iterator iterator = filePaths.begin(); iterator != filePaths.end(); ++iterator)
				parser.parse(*iterator, strings, optionReplace);

			if (optionFdf)
				parser.writeFdf("output.fdf", strings);
			
			if (optionWts)
				parser.writeWts("output.wts", "", strings);
		}
	}
	else
	{
		std::cout << "Missing arguments." << std::endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
