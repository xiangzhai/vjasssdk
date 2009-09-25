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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <list>
#include <fstream>

#include "../internationalisation.hpp"
#include "../mdlx/mdlx.hpp"

using namespace wc3lib;

static const char *version = "0.1";

static const char *formatExpression[5] =
{
	"mdl",
	"mdx",
	"blend",
	"3ds"
};

static inline bool checkFormatString(const char *formatString)
{
	for (int i = 0; i < 4; ++i)
	{
		if (strcmp(formatString, formatExpression[i]) == 0)
			return true;
	}
	
	return false;
}

int main(int argc, char *argv[])
{
	// Set the current locale.
	setlocale(LC_ALL, "");
	// Set the text message domain.
	bindtextdomain("wc3lib", LOCALE_DIR);
	textdomain("wc3lib");
	
	if (argc == 1)
	{
		std::cerr << _("Missing arguments.") << std::endl;
		
		return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "--version") == 0)
	{
		printf("mdlxtest %s.\n", version);
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
		_("mdlxtest\n") <<
		_("\nUsage:\n") <<
		_("\tmdlxtest [] <input files>\n") <<
		_("\nOptions:\n") <<
		_("\t--version                 Shows the current version of mdlxtest.\n") <<
		_("\t-i, --iformat <arg>       <arg> has to be replaced by input files format.\n") <<
		_("\t-o, --oformat <arg>       <arg> has to be replaced by output files format.\n") <<
		_("\nReport bugs to tamino@cdauth.de or on http://sourceforge.net/projects/vjasssdk/") <<
		std::endl;
		
		return EXIT_SUCCESS;
	}
	
	std::string optionIformat;
	std::string optionOformat;
	std::list<std::string> optionFiles;
	
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--iformat") == 0)
		{
			if (++i >= argc)
			{
				std::cerr << _("Missing --iformat argument.") << std::endl;
				
				return EXIT_FAILURE;
			}
			
			if (!checkFormatString(argv[i]))
			{
				fprintf(stderr, _("Invalid format \"%s\".\n"), argv[i]);
				
				return EXIT_FAILURE;
			}
			
			if (optionOformat == argv[i])
			{
				std::cerr << _("Input and output formats are equal.") << std::endl;
						    
				return EXIT_FAILURE;
			}
			
			optionIformat = argv[i];
		}
		else if (strcmp(argv[i], "--oformat") == 0)
		{
			if (++i >= argc)
			{
				std::cerr << _("Missing --oformat argument.") << std::endl;
				
				return EXIT_FAILURE;
			}
			
			if (!checkFormatString(argv[i]))
			{
				fprintf(stderr, _("Invalid format \"%s\".\n"), argv[i]);
				
				return EXIT_FAILURE;
			}
			
			if (optionIformat == argv[i])
			{
				std::cerr << _("Input and output formats are equal.") << std::endl;
						    
				return EXIT_FAILURE;
			}
			
			optionIformat = argv[i];
		}
		else
			optionFiles.push_back(argv[i]);
	}
	
	static std::string outputPath = "test.mdl";
	
	std::fstream outputFstream;

	for (std::list<std::string>::iterator iterator = optionFiles.begin(); iterator != optionFiles.end(); ++iterator)
	{
		std::fstream inputFstream;
		inputFstream.open((*iterator).c_str(), std::ifstream::in | std::ifstream::binary);
		
		if (!inputFstream)
		{
			fprintf(stderr, _("Error while opening file \"%s\". Continuing with next one.\n"), (*iterator).c_str());
			
			continue;
		}
		
		class mdlx::Mdlx mdlx;
		
		try
		{
			if (optionIformat == "mdx")
				mdlx.readMdx(inputFstream);
			else if (optionIformat == "mdl")
				mdlx.readMdl(inputFstream);
		}
		catch (class Exception &exception)
		{
			fprintf(stderr, _("Error while reading file \"s\":\n \"%s\"\n"), (*iterator).c_str(), exception.what());
		}
		
		inputFstream.close();
		/// @todo Open file with right extension.
		/*
		outputFstream.open("test.mdl", std::ifstream::out | std::ifstream::binary);
		mdlx.writeMdl(outputFstream);
		outputFstream.close();
		*/
	}

	return EXIT_SUCCESS;
}