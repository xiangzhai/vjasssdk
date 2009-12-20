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
#include <cstring>
//POSIX
#include <sys/stat.h>

#include <boost/tokenizer.hpp>
#include <boost/format.hpp>

#include "parser.hpp"
#include "string.hpp"
#include "options.hpp"
#include "internationalisation.hpp"

namespace vjasstrans
{

/// Returns true if a string has been found.
static bool getNextTranslationFunctionCall(const std::string &line, const std::string &translationFunction, std::string::size_type &position, std::string::size_type &length, const bool ignoreReplacedValues = true)
{
	//std::cout << "Checking line " << line << " with position " << position << std::endl;
	std::string searchedString = translationFunction + "(\""; // not static! translation function can be changed by main call
	//std::cout << "Searched string is " << searchedString << std::endl;
	position = line.find(searchedString, position);
	
	if (position == std::string::npos)
		return false;
	
	position += searchedString.length();
	//std::cout << "Set position with current value " << position << std::endl;
	
	if (position >= line.length())
		return false;
	
	do
	{
		length = line.find("\"", position);
		
		if (length == std::string::npos)
			return false;
		else if (line[length - 1] == '\\') // escape sequence
			position = length + 1;
		else
			break;
	}
	while (true);
	
	length = length - position;
	//std::cout << "Set length to " << length << std::endl;
	
	static const char *replacedValueSchema = "STRING_";
	
	if (ignoreReplacedValues && line.substr(position, strlen(replacedValueSchema)) == replacedValueSchema)
		return false;
	
	//std::cout << "Returning true" << std::endl;
	
	return true;
}

static bool checkForStringConflict(std::list<class String*> &strings, class String *string)
{
	for (std::list<class String*>::iterator iterator = strings.begin(); iterator != strings.end(); ++iterator)
	{
		if ((*iterator)->defaultString() == string->defaultString())
		{
			std::cerr << boost::format(_("Detected string conflict:\nString 0: \"%1%\"\nString 0 translation: \"%2%\"\nString 1: \"%3%\"\nString 1 translation: \"%4%\"\nEnter 0 to use string 0 or 1 to use string 1.\n")) % (*iterator)->idString() % (*iterator)->valueString() % string->idString() % string->valueString() << std::endl;
			int value;
			std::cin >> value;
			
			if (value == 0)
			{
				std::cout << _("Using string 0.") << std::endl;
				delete *iterator;
				strings.erase(iterator);
				strings.push_back(string);
			}
			else if (value == 1)
			{
				std::cout << _("Using string 1.") << std::endl;
				delete string;
			}
			
			return true;
		}
	}
	
	strings.push_back(string);
	
	return false;
}

bool Parser::parse(const std::string &filePath, std::list<class String*> &strings, const bool replace, const std::string &translationFunction)
{
	fprintf(stdout, _("Parsing file \"%s\".\n"), filePath.c_str());
	std::fstream fstream(filePath.c_str(), std::ios_base::in);

	if (!fstream)
	{
		fprintf(stdout, _("Error while opening file \"%s\".\n"), filePath.c_str());
		
		return false;
	}
	
	std::string line;
	std::list<std::string> lines;

	for (unsigned int i = 0; std::getline(fstream, line); ++i)
	{
		std::string::size_type position = 0;
		std::string::size_type length = 0;
		
		while (getNextTranslationFunctionCall(line, translationFunction, position, length, true))
		{
			std::string defaultString = line.substr(position, length);
			int id = 0;
			std::list<int> usedIds;
			bool exists = false;
			std::list<class String*>::iterator iterator = strings.begin();
			
			for (int j = 0; iterator != strings.end(); ++iterator, ++j)
			{
				if ((*iterator)->defaultString() == defaultString)
				{
					id = j;
					exists = true;
					
					break;
				}
				
				usedIds.push_back((*iterator)->id());
			}
			
			// check if id is already used
			if (!exists)
			{
				while (true)
				{
					bool notUsed = true;
					
					for (std::list<int>::iterator iterator = usedIds.begin(); notUsed && iterator != usedIds.end(); ++iterator)
					{
						if (*iterator == id)
							notUsed = false;
					}
					
					if (notUsed)
						break;
					
					// get overflows
					try
					{
						++id;
					}
					catch (...)
					{
						fprintf(stderr, _("Error while trying to increase id with value %d.\n"), id);
						
						return false;
					}
				}
				
				std::stringstream sstream;
				sstream << "STRING_" << id;
				std::string valueString = optionFill ? defaultString : "";
				strings.push_back(new String(filePath, i, sstream.str(), defaultString, valueString));
				
				if (replace)
					line.replace(position, length, sstream.str());
			}
			else if (replace)
				line.replace(position, length, (*iterator)->idString());
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
		fprintf(stderr, _("Error while opening fdf file \"%s\".\n"), filePath.c_str());
		return false;
	}

	bool foundEndCharacter = false;
	std::string line;

	for (unsigned int i = 0; std::getline(fstream, line); ++i)
	{
		if (line.empty())
			continue;
		
		boost::tokenizer<> tokenizer(line);
		boost::tokenizer<>::iterator iterator = tokenizer.begin();

		if (iterator != tokenizer.end())
		{
			if (*iterator == "StringList")
			{
				++iterator;
				
				/// @todo Missing { character ________|________
				if (iterator == tokenizer.end() || *iterator != "{")
				{
					fprintf(stderr, _("Error at line %d: Missing \"{\" character.\n"), i);
					
					return false;
				}
				
				
			}
			else if (*iterator == "}") // end string list
			{
				foundEndCharacter = true;
				
				break;
			}
			else if ((*iterator).length() > 7 && (*iterator).substr(0, 7)  == "STRING_")
			{
				//std::cout << "Length is bigger than 7." << std::endl;
				std::string idString = *iterator;
				std::size_t index = line.find_last_of("/*");
		
				if (index == std::string::npos)
				{
					fprintf(stderr, _("Error at line %d: Missing */.\n"), i);
					
					continue;
				}
		
				index += 3;
				std::size_t length = line.find_last_of("*/", index);
		
				if (length == std::string::npos)
				{
					fprintf(stderr, _("Error at line %d: Missing */.\n"), i);
					
					continue;
				}
		
				length -= index + 3;
				std::string defaultString = line.substr(index, length);
		
				if (defaultString.empty())
				{
					fprintf(stderr, _("Error at line %d: Missing default string.\n"), i);
					
					continue;
				}
		
				index = line.find('"');
		
				if (index == std::string::npos)
				{
					fprintf(stderr, _("Error at line %d: Missing \".\n"), i);
					
					continue;
				}
		
				index += 2;
				length = line.find("\"", index);
		
				if (length == std::string::npos)
				{
					fprintf(stderr, _("Error at line %d: Missing \".\n"), i);
					
					continue;
				}
		
				length -= index + 2;
				std::string valueString = line.substr(index, length);
				class String *string = new String("", 0, idString, defaultString, valueString);
				checkForStringConflict(strings, string);
				//std::cout << "Adding string from fdf file" << std::endl;
			}
		}
	}

	fstream.close();
	
	if (!foundEndCharacter)
		std::cerr << _("Warning: Did not find closing \"}\" character.") << std::endl;
	
	std::cout << _("Listening file strings:") << std::endl;

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
		fprintf(stdout, _("Error while opening fdf file \"%s\".\n"), filePath.c_str());
		
		return false;
	}

	fstream << "StringList {" << std::endl;
	int i = 0;

	for (std::list<class String*>::const_iterator iterator = strings.begin(); iterator != strings.end(); ++iterator, ++i)
	{
		fstream << "\t" << (*iterator)->idString() << "\t\t\t\"" << (*iterator)->valueString() << '\"';
		
		if (i != strings.size() - 1)
			fstream << ',';

		fstream << " /* " << (*iterator)->defaultString() << " */" << std::endl;
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
		fprintf(stderr, _("Error while opening wts file \"%s\".\n"), filePath.c_str());
		
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

		if (std::getline(fstream, line) && ++i && line.substr(0, 2) == "//")
		{
			std::string defaultString = line.length() < 4 ? "" : line.substr(3);

			if (std::getline(fstream, line) && ++i && line == "{" && std::getline(fstream, line) && ++i)
			{
				std::string valueString = line;
				class String *string = new String("", 0, idString, defaultString, line);
				if (!checkForStringConflict(strings, string))
					std::cout << "Adding string from wts file" << std::endl;
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
			//std::cout << "Line: " << line << " and sub string " << line.substr(2) << std::endl;
			
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
