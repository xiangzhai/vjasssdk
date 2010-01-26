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
#include <cstring>

#include <boost/format.hpp>

#include "../internationalisation.hpp"
#include "../map/environment.hpp"

int main(int argc, char *argv[])
{
	std::cout << boost::format(_("Tilesetter version %1%")) % version << std::endl;
	std::cout << _(
	"Copyright (C) 2009 by Tamino Dauth\n"
	"tamino@cdauth.de\n"
	"\n"
	"This program is free software; you can redistribute it and/or modify\n"
	"it under the terms of the GNU General Public License as published by\n"
	"the Free Software Foundation; either version 2 of the License, or\n"
	"(at your option) any later version.\n"
	"\n"
	"This program is distributed in the hope that it will be useful,\n"
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	"GNU General Public License for more details.\n"
	"\n"
	"You should have received a copy of the GNU General Public License\n"
	"along with this program; if not, write to the\n"
	"Free Software Foundation, Inc.,\n"
	"59 Temple Place - Suite 330, Boston, MA  02111-1307, USA."
	) << std::endl;
	
	static const std::size_t commandsCount = 10;
	
	struct Command
	{
		char *name;
		char shortName;
		char *help;
		std::size_t argCount;
		bool (*callbackFunction)(const struct Command &command, const std::list<std::string> &argParameters);
	} commands[commandsCount] =
		{
			
		}
	;
	
	std::cout << _("Usage 1: <command name/command short name> <argument1> <argument2> ... <argumentn>") << std::endl;
	std::cout << _("Usage 2: <command name/command short name> help") << std::endl;
	
	for (std::size_t i = 0; i < commandsCount; ++i)
		std::cout << boost::format(_("\tCommand \"%1%\" (\"%2%\") - %3% arguments")) % commands[i].name % commands[i].shortName % commands[i].argCount << std::endl;
	
	std::string input;
	
	do
	{
		std::getline(std::cin, input);
		std::string command(strtok(input.c_str(), " "));
		bool found = false;
		
		for (std::size_t i = 0; i < commandsCount; ++i)
		{
			if (command == commands[i].name || (command.size() == 1 && command[0] == commands[i].shortName))
			{
				found = true;
				std::list<std::string> arguments;
				
				for (std::size_t i = 0; i < commands[i].argCount; ++i)
				{
					//char *
				}

				//bool (*callbackFunction)(const struct Command &command, const std::list<std::string> &argParameters);
				
				/// @todo Call callback function with arguments or show help!
				
				break;
			}
		}
		
		if (!found)
			std::cout << _("Unknown command.") << std::endl;
	}
	while (input != "quit" && input != "q");
	
	return EXIT_SUCCESS;
}
