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

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include "parser.hpp"
#include "string.hpp"
#include "options.hpp"
#include "internationalisation.hpp"

using namespace vjasstrans;

static Parser::StringList strings;

static void addDirectory(const boost::filesystem::path &directoryPath, std::list<boost::filesystem::path> &filePaths)
{
	std::cout << boost::format(_("Reading directory \"%1%\".")) % directoryPath.string() << std::endl;
	boost::filesystem::directory_iterator endIterator;

	for (boost::filesystem::directory_iterator iterator(directoryPath); iterator != endIterator; ++iterator)
	{
		if (!boost::filesystem::exists(iterator->status()))
		{
			std::cerr << boost::format(_("Error while reading file \"%1%\".")) % iterator->path().string() << std::endl;

			continue;
		}

		if (boost::filesystem::is_directory(iterator->status()))
		{
			if (optionRecursive)
				addDirectory(iterator->path(), filePaths);
			else
				std::cerr << boost::format(_("\"%1%\" is another directory and is being skipped.")) % iterator->path().string() << std::endl;
		}
		else
			filePaths.push_back(iterator->path());
	}
}

int main(int argc, char *argv[])
{
	// Set the current locale.
	setlocale(LC_ALL, "");
	// Set the text message domain.
	bindtextdomain("vjasstrans", "share/locale");
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
		_("--startatlast               Uses the biggest id + 1 of read input files as first string id and not the first free id.\n") <<
		_("--verbose                   Shows verbose output.\n") <<
		_("--fill                      Fills string values with default strings when adding them from code files.\n") <<
		_("--replace                   Replaces all translated strings by STRING_<id> in code files.\n") <<
		_("--recursive                 Parses all source files in sub directories.\n") <<
		_("--fdf                       Creates file with all parsed strings.\n") <<
		_("--wts                       Creates file with all parsed strings.\n") <<
		_("--afdf                      Appends file with all parsed strings.\n") <<
		_("--awts                      Appends file with all parsed strings.")
		<< std::endl
		;
		std::cout << boost::format(_("--trans <arg>               <arg> has to be the name of the translation function. Default value: %1%.")) % optionTrans << std::endl;
		std::cout << boost::format(_("--wtspath <arg>             <arg> has to be the file path of wts file. Default value: %1%.")) % optionWtspath << std::endl;
		std::cout << boost::format(_("--fdfpath <arg>             <arg> has to be the file path of fdf file. Default value: %1%.")) % optionFdfpath << std::endl;
		std::cout <<
		_("\nReport bugs to tamino@cdauth.de or on http://sourceforge.net/projects/vjasssdk/") <<
		std::endl;
		
		return EXIT_SUCCESS;
	}
	
	std::list<boost::filesystem::path> filePaths;
	
	for (std::size_t i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--startatlast") == 0)
			optionStartAtLast = true;
		else if (strcmp(argv[i], "--verbose") == 0)
			optionVerbose = true;
		else if (strcmp(argv[i], "--fill") == 0)
			optionFill = true;
		else if (strcmp(argv[i], "--replace") == 0)
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
				std::cerr << _("Missing argument of option --trans.") << std::endl;
				
				return EXIT_FAILURE;
			}
			
			optionTrans = argv[i];
		}
		else if (strcmp(argv[i], "--wtspath") == 0)
		{
			if (++i == argc)
			{
				std::cerr << _("Missing argument of option --wtspath.") << std::endl;
				
				return EXIT_FAILURE;
			}
			
			optionWtspath = argv[i];
		}
		else if (strcmp(argv[i], "--fdfpath") == 0)
		{
			if (++i == argc)
			{
				std::cerr << _("Missing argument of option --fdfpath.") << std::endl;
				
				return EXIT_FAILURE;
			}
			
			optionFdfpath = argv[i];
		}
		else
			filePaths.push_back(argv[i]);
	}
			
	// filtering non-existing files and directories
	BOOST_FOREACH(boost::filesystem::path path, filePaths)
	{		
		if (!boost::filesystem::exists(path))
		{
			std::cerr << boost::format(_("File or directory \"%1%\" does not exist.")) % path.string() << std::endl;
			filePaths.remove(path);
		}
		// is directory
		else if (boost::filesystem::is_directory(path))
		{
			if (optionRecursive)
				addDirectory(path, filePaths);
		
			//filePaths.erase(iterator);
		}
	}

	if (filePaths.empty())
	{
		std::cerr << _("Missing valid file paths.") << std::endl;

		return EXIT_FAILURE;
	}
	
	if (optionAfdf)
		Parser::readFdf(optionFdfpath, strings);
	
	if (optionAwts)
		Parser::readWts(optionWtspath, strings);
	
	BOOST_FOREACH(boost::filesystem::path path, filePaths)
		Parser::parse(path, strings, optionReplace, optionTrans);
	
	if (optionFdf || optionAfdf)
		Parser::writeFdf(optionFdfpath, strings);
	
	if (optionWts || optionAwts)
		Parser::writeWts(optionWtspath, strings);

	if (optionVerbose)
	{
		std::size_t averageLength = 0;

		BOOST_FOREACH(Parser::StringListValueConst value, strings)
			averageLength += value.second->defaultString().length();

		averageLength /= strings.size();

		std::cout << boost::format(_("Result: %1% strings, %2% is average length.")) % strings.size() % averageLength << std::endl;
	}

	return EXIT_SUCCESS;
}
