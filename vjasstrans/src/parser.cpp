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
#include <fstream>
#include <sstream>
#include <iostream> //verbose
//POSIX
#include <sys/stat.h>

#include <boost/tokenizer.hpp>

#include "parser.hpp"
#include "string.hpp"
#include "options.hpp"
#include "internationalisation.hpp"

namespace vjasstrans
{

bool Parser::parse(const std::string &filePath, std::list<class String*> &strings, bool replace, const std::string &translationFunction)
{
	fprintf(stdout, "Parsing file \"%s\".\n", filePath.c_str());

	std::fstream fstream(filePath.c_str(), std::ios_base::in);

	if (!fstream)
	{
		fprintf(stdout, "Error while opening file \"%s\".\n", filePath.c_str());
		return false;
	}

	static std::string searchedString = translationFunction + "(\"";
	std::list<std::string> lines;
	std::string line;

	for (unsigned int i = 0; std::getline(fstream, line); ++i)
	{
		for (std::string::size_type position = line.find(searchedString); position != std::string::npos; position = line.find(searchedString, position))
		{
			position += searchedString.length();
			std::string::size_type length = line.find("\"", position);

			if (length != std::string::npos && line[length - 1] != '\\') //tr("Ich heiße \"Hans\"") -> ignore escape sequences
			{
				length = length - position;
				std::string defaultString = line.substr(position, length);
				int id = strings.size();
				std::list<class String*>::iterator iterator = strings.begin();
				
				for (int j = 0; iterator != strings.end(); ++iterator, ++j)
				{
					if ((*iterator)->defaultString() == defaultString)
					{
						id = j;
						break;
					}
				}
				
				if (id == strings.size())
				{
					std::stringstream sstream;
					sstream << "STRING_" << id;
					strings.push_back(new String(filePath, i, sstream.str(), defaultString, ""));
					
					if (replace)
						line.replace(position, length, sstream.str());
				}
				else if (replace)
					line.replace(position, length, (*iterator)->idString());
			}
		}

		if (replace)
			lines.push_back(line);
	}

	fstream.close();

	if (replace)
	{
		fstream.open(filePath.c_str(), std::ios_base::out);

		for (std::list<std::string>::iterator iterator = lines.begin(); iterator != lines.end(); ++iterator)
		{
			if (iterator != lines.begin())
				fstream << '\n';

			fstream << *iterator;
		}

		fstream.close();
	}

	return true;
}

bool Parser::readFdf(const std::string &filePath, std::list<class String*> &strings)
{
	fprintf(stdout, _("Reading fdf file \"%s\".\n"), filePath.c_str());
	std::ifstream fstream(filePath.c_str(), std::ios_base::in);

	if (!fstream)
	{
		fprintf(stderr, _("Error while opening file \"%s\".\n"), filePath.c_str());
		return false;
	}

	std::string line;

	for (unsigned int i = 0; std::getline(fstream, line); ++i)
	{
		if (line.empty())
			continue;

		boost::tokenizer<> tokenizer(line);
		boost::tokenizer<>::iterator iterator = tokenizer.begin();

		if (iterator != tokenizer.end() && (*iterator).length() > 7 && (*iterator).substr(0, 7)  == "STRING_")
		{
			std::string idString = *iterator;
			std::size_t index = line.find_last_of("/*");

			if (index == std::string::npos)
			{
				std::cerr << "Error at line " << i << ", missing */." << std::endl;
				continue;
			}

			index += 3;
			std::size_t length = line.find_last_of("*/", index);

			if (length == std::string::npos)
			{
				std::cerr << "Error at line " << i << ", missing /*." << std::endl;
				continue;
			}

			length -= index + 3;
			std::string defaultString = line.substr(index, length);

			if (defaultString.empty())
			{
				std::cerr << "Error at line " << line << ", missing default string." << std::endl;
				continue;
			}

			index = line.find('"');

			if (index == std::string::npos)
			{
				std::cerr << "Error at line " << i << ", missing \"." << std::endl;
				continue;
			}

			index += 2;
			length = line.find("\"", index);

			if (length == std::string::npos)
			{
				std::cerr << "Error at line " << i << ", missing \"." << std::endl;
				continue;
			}

			length -= index + 2;
			std::string valueString = line.substr(index, length);
			strings.push_back(new String("", 0, idString, defaultString, valueString));
		}
	}

	fstream.close();
	std::cout << "Listening file strings:" << std::endl;

	for (std::list<class String*>::iterator iterator = strings.begin(); iterator != strings.end(); ++iterator)
		std::cout << "- " << (*iterator)->idString() << "\t\t" << (*iterator)->defaultString() << std::endl;

	return true;
}

bool Parser::writeFdf(const std::string &filePath, const std::list<class String*> &strings)
{
	fprintf(stdout, _("Writing fdf file \"%s\".\n"), filePath.c_str());
	std::ofstream fstream(filePath.c_str());

	if (!fstream)
	{
		fprintf(stdout, _("Error while opening file \"%s\".\n"), filePath.c_str());
		return false;
	}

	fstream << "StringList {" << std::endl;
	int i = 0;

	for (std::list<class String*>::const_iterator iterator = strings.begin(); iterator != strings.end(); ++iterator, ++i)
	{
		fstream << "\t" << (*iterator)->idString() << "\t\t\t\"" << (*iterator)->valueString() << '\"';
		
		if (i != strings.size() - 1)
			fstream << ',';

		fstream << "\t/* " << (*iterator)->defaultString() << " */" << std::endl;
	}

	fstream << '}' << std::endl;

	return true;
}

bool Parser::readWts(const std::string &filePath, std::list<class String*> &strings)
{
	fprintf(stdout, _("Reading wts file \"%s\".\n"), filePath.c_str());
	std::ifstream fstream(filePath.c_str());
	
	if (!fstream)
	{
		fprintf(stderr, _("Error while opening map strings file \"%s\".\n"), filePath.c_str());
		return false;
	}

	std::list<std::string> lines;
	std::string line;

	for (unsigned int i = 0; std::getline(fstream, line); ++i, lines.push_back(line))
	{
		std::string::size_type position = line.find("STRING");
		
		if (line.substr(0, 6) != "STRING")
			continue;

		std::string idString = line.substr(0, 6) + "_" + line.substr(7);

		if (std::getline(fstream, line) && ++i && line.substr(2) == "//")
		{
			
			std::string defaultString = line.length() < 4 ? "" : line.substr(3);

			if (std::getline(fstream, line) && ++i && line == "{" && std::getline(fstream, line) && ++i)
			{
				std::string valueString = line;
				strings.push_back(new String("", 0, idString, defaultString, line));
			}
			else
			{
				fprintf(stderr, _("Error at line %i: Missing value string line or { character line.\n"), i);
				break;
			}
		}
		else
		{
			fprintf(stderr, _("Error at line %i: Missing default string line.\n"), i);
			break;
		}
	}

	fstream.close();

	return true;
}

bool Parser::writeWts(const std::string &filePath, const std::list<class String*> &strings)
{
	fprintf(stdout, _("Writing wts file \"%s\".\n"), filePath.c_str());
	std::ofstream fstream(filePath.c_str());

	if (!fstream)
	{
		fprintf(stderr, _("Error while creating wts file \"%s\".\n"), filePath.c_str());
		return false;
	}

	for (std::list<class String*>::const_iterator iterator = strings.begin(); iterator != strings.end(); ++iterator)
	{
		fstream << "STRING " << (*iterator)->id() << '\n'
		<< "// " << (*iterator)->defaultString() << '\n'
		<< "{\n"
		<< (*iterator)->valueString() << '\n'
		<< "}\n"
		<< std::endl;
	}

	fstream.close();

	return true;
}

}
