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

 #include <getopt.h>

#include "../internationalisation.hpp"
#include "../mdlx/mdlx.hpp"

using namespace wc3lib;

static const char *version = "0.1";

static const int maxFormats = 5;
static const char *formatExpression[maxFormats] =
{
	"mdl",
	"mdx",
	"blend",
	"3ds"
};

static inline bool checkFormatString(const char *formatString)
{
	for (int i = 0; i < maxFormats; ++i)
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
	
	static struct option options[] =
	{
		{"version",  no_argument,             0, 'v'},
		{"help",     no_argument,             0, 'h'},
		{"iformat",  required_argument,       0, 'i'},
		{"oformat",  required_argument,       0, 'o'},
		{0, 0, 0, 0}
	};
	
	std::string optionIformat;
	std::string optionOformat;
	std::list<std::string> optionFiles;
	int optionShortcut;
	
	while (true)
	{
		int optionIndex = 0;
		optionShortcut = getopt_long(argc, argv, "vhi:o:", options, &optionIndex);

		if (optionShortcut == -1)
			break;
     
		switch (optionShortcut)
		{
			case 'v':
			{
				printf("mdlxtest %s.\n", version);
				std::cout << _(
				"Copyright © 2009 Tamino Dauth\n"
				"License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law."
				) << std::endl;
				
				break;
			}
			
			case 'h':
			{
				std::cout <<
				_("mdlxtest\n") <<
				_("\nUsage:\n") <<
				_("\tmdlxtest [-io] <input files>\n") <<
				_("\nOptions:\n") <<
				_("\t-v, --version             Shows the current version of mdlxtest.\n") <<
				_("\t-h, --help                Shows this text.\n") <<
				_("\t-i, --iformat <arg>       <arg> has to be replaced by input files format.\n") <<
				_("\t-o, --oformat <arg>       <arg> has to be replaced by output files format.\n") <<
				_("\nReport bugs to tamino@cdauth.de or on http://sourceforge.net/projects/vjasssdk/") <<
				std::endl;
				
				break;
			}
			
			case 'i':
			{
				if (!checkFormatString(optarg))
				{
					fprintf(stderr, _("Invalid format \"%s\".\n"), optarg);
					
					return EXIT_FAILURE;
				}
				
				if (optionOformat == optarg)
				{
					std::cerr << _("Input and output formats are equal.") << std::endl;
							    
					return EXIT_FAILURE;
				}
				
				optionIformat = optarg;
				
				break;
			}
			
			case 'o':
			{
				if (!checkFormatString(optarg))
				{
					fprintf(stderr, _("Invalid format \"%s\".\n"), optarg);
					
					return EXIT_FAILURE;
				}
				
				if (optionIformat == optarg)
				{
					std::cerr << _("Input and output formats are equal.") << std::endl;
							    
					return EXIT_FAILURE;
				}
				
				optionOformat = optarg;
				
				break;
			}
		}
	}
	
	if (optind < argc)
	{
		while (optind < argc)
			optionFiles.push_back(argv[optind++]);

	}
	else
	{
		std::cerr << _("Missing file arguments.") << std::endl;
		
		return EXIT_FAILURE;
	}
	
	if (optionIformat.empty())
	{
		std::cerr << _("Input format was not defined.") << std::endl;
		
		return EXIT_FAILURE;
	}
	
	if (optionOformat.empty())
	{
		std::cerr << _("Output format was not defined.") << std::endl;
		
		return EXIT_FAILURE;
	}
	
	if (optionFiles.empty())
	{
		std::cerr << _("No input files were chosen.") << std::endl;
		
		return EXIT_FAILURE;
	}
	

	for (std::list<std::string>::iterator iterator = optionFiles.begin(); iterator != optionFiles.end(); ++iterator)
	{
		std::cout << "Loop" << std::endl;
		
		std::ios_base::openmode openMode = std::ifstream::in;
		
		if (optionIformat == "mdx")
		{
			std::cout << "Is binary!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			openMode |= std::ifstream::binary;
		}
		
		std::fstream fstream((*iterator).c_str(), openMode);
		
		if (!fstream)
		{
			fprintf(stderr, _("Error while opening file \"%s\". Continuing with next one.\n"), (*iterator).c_str());
			
			continue;
		}
		
		class mdlx::Mdlx mdlx;
		
		try
		{
			if (optionIformat == "mdx")
			{
				long32 bytes = mdlx.readMdx(fstream);
				printf(_("Read MDX file successfully. %d Bytes.\n"), bytes);
			}
			else if (optionIformat == "mdl")
				mdlx.readMdl(fstream);
		}
		catch (class Exception &exception)
		{
			fprintf(stderr, _("Error while reading file \"%s\":\n \"%s\"\n"), (*iterator).c_str(), exception.what());
			std::cerr << _("Skiping file.") << std::endl;
			
			continue;
		}
		
		fstream.close();
		
		openMode = std::ifstream::out;
		std::string extension;
		
		if (optionOformat == "mdx")
		{
			openMode |= std::ifstream::binary;
			extension = ".mdx";
		}
		else if (optionOformat == "mdl")
		{
			extension = ".mdl";
		}
		
		/// @todo Remove old extension.
		
		std::string filePath = *iterator + extension;
		fstream.open(filePath.c_str(), openMode);
		
		if (!fstream)
		{
			fprintf(stderr, _("Error while opening file \"%s\". Continuing with next one.\n"), filePath.c_str());
			
			continue;
		}
		
		try
		{
			if (optionOformat == "mdx")
			{
				long32 bytes = mdlx.writeMdx(fstream);
				printf(_("Wrote MDX file successfully. %d Bytes.\n"), bytes);
			}
			else if (optionOformat == "mdl")
				mdlx.writeMdl(fstream);
		}
		catch (class Exception &exception)
		{
			fprintf(stderr, _("Error while writing file \"%s\":\n \"%s\"\n"), filePath.c_str(), exception.what());
			std::cerr << _("Skiping file.") << std::endl;
			
			continue;
		}
		
		fstream.close();
	}

	return EXIT_SUCCESS;
}
