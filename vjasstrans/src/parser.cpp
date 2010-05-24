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
#include <list>

#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem/fstream.hpp>

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

static bool checkForStringConflict(Parser::StringList &strings, class String *string)
{
	BOOST_FOREACH(Parser::StringListValue value, strings)
	{
		if (value.second->defaultString() == value.second->defaultString())
		{
			std::cerr << boost::format(_("Detected string conflict:\nString 0: \"%1%\"\nString 0 translation: \"%2%\"\nString 1: \"%3%\"\nString 1 translation: \"%4%\"\nEnter 0 to use string 0 or 1 to use string 1.\n")) % value.second->idString() % value.second->valueString() % string->idString() % string->valueString() << std::endl;
			int answer;
			std::cin >> answer;
			
			if (answer == 0)
			{
				std::cout << _("Using string 0.") << std::endl;
				strings.erase(value.first);
				delete value.second;
				strings.insert(std::make_pair(string->id(), string));
			}
			else if (answer == 1)
			{
				std::cout << _("Using string 1.") << std::endl;
				delete string;
			}
			
			return true;
		}
	}
	
	strings.insert(std::make_pair(string->id(), string));
	
	return false;
}

bool Parser::parse(const boost::filesystem::path &filePath, StringList &strings, const bool replace, const std::string &translationFunction)
{
	std::cout << boost::format(_("Parsing file \"%1%\".")) % filePath.string() << std::endl;
	boost::filesystem::fstream fstream(filePath, std::ios_base::in);

	if (!fstream)
	{
		std::cerr << boost::format(_("Error while opening file \"%1%\".")) % filePath.string() << std::endl;
		
		return false;
	}
	
	static bool startedAtLast = false;
	std::string line;
	std::list<std::string> lines;

	for (std::size_t i = 0; std::getline(fstream, line); ++i)
	{
		std::string::size_type position = 0;
		std::string::size_type length = 0;
		
		while (getNextTranslationFunctionCall(line, translationFunction, position, length, true))
		{
			std::string defaultString = line.substr(position, length);
			std::size_t id = 0;
			bool exists = false;
			std::size_t j = 0;
			StringListValue existing;

			BOOST_FOREACH(StringListValue value, strings)
			{
				if (value.second->defaultString() == defaultString)
				{
					id = j;
					exists = true;
					existing = value;
					
					break;
				}

				++j;
			}
			
			// check if id is already being used
			if (!exists)
			{
				// find biggest id and start afterwards
				if (optionStartAtLast && !startedAtLast)
				{
					if (!strings.empty()) // otherwise, start at 0
					{
						std::size_t biggestId = 0;

						BOOST_FOREACH(StringListValueConst value, strings)
						{
							if (value.second->id() > biggestId)
								biggestId = value.second->id();
						}

						try
						{
							id = biggestId + 1;
						}
						catch (...)
						{
							std::cerr << boost::format(_("Error while trying to increase id with value %1%.")) % biggestId << std::endl;

							return false;
						}
					}

					startedAtLast = true;

					if (optionVerbose)
						std::cout << boost::format(_("Starting at id: %1%")) % id << std::endl;
				}
				// find free id otherwise take next.
				else
				{
					do
					{
						if (strings.find(id) == strings.end())
							break;

						try
						{
							++id;
						}
						catch (...)
						{
							std::cerr << boost::format(_("Error while trying to increase id with value %1%.")) % id << std::endl;

							return false;
						}

					}
					while (true);
				}
				
				std::stringstream sstream;
				sstream << "STRING_" << id;
				std::string valueString = optionFill ? defaultString : "";
				strings.insert(std::make_pair(id, new String(filePath, i, sstream.str(), defaultString, valueString)));
				
				if (replace)
					line.replace(position, length, sstream.str());
			}
			else if (replace)
				line.replace(position, length, existing.second->idString());
		}

		if (replace)
			lines.push_back(line);
	}

	fstream.close();

	if (replace)
	{
		fstream.open(filePath, std::ios_base::out);

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

bool Parser::readFdf(const boost::filesystem::path &filePath, StringList &strings)
{
	std::cout << boost::format(_("Reading fdf file \"%1%\".")) % filePath << std::endl;
	boost::filesystem::fstream fstream(filePath, std::ios_base::in);

	if (!fstream)
	{
		std::cerr << boost::format(_("Error while opening fdf file \"%1%\".")) << filePath.string() << std::endl;

		return false;
	}

	bool foundEndCharacter = false;
	std::string line;

	for (std::size_t i = 0; std::getline(fstream, line); ++i)
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
					std::cerr << boost::format(_("Error at line %1%: Missing */.")) % i << std::endl;
					
					continue;
				}
		
				index += 3;
				std::size_t length = line.find_last_of("*/", index);
		
				if (length == std::string::npos)
				{
					std::cerr << boost::format(_("Error at line %1%: Missing */.")) % i << std::endl;
					
					continue;
				}
		
				length -= index + 3;
				std::string defaultString = line.substr(index, length);
		
				if (defaultString.empty())
				{
					std::cerr << boost::format(_("Error at line %1%: Missing default string.")) % i << std::endl;
					
					continue;
				}
		
				index = line.find('"');
		
				if (index == std::string::npos)
				{
					std::cerr << boost::format(_("Error at line %1%: Missing \".\n")) % i << std::endl;
					
					continue;
				}
		
				index += 2;
				length = line.find("\"", index);
		
				if (length == std::string::npos)
				{
					std::cerr << boost::format(_("Error at line %1%: Missing \".")) % i << std::endl;
					
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

	BOOST_FOREACH(StringListValueConst value, strings)
		std::cout << boost::format(_("- %1%\t\t%2%")) % value.second->idString() % value.second->defaultString() << std::endl;

	return true;
}

bool Parser::writeFdf(const boost::filesystem::path &filePath, const StringList &strings)
{
	std::cout << boost::format(_("Writing fdf file \"%1%\".")) % filePath.string() << std::endl;
	boost::filesystem::ofstream fstream(filePath);

	if (!fstream)
	{
		std::cerr << boost::format(_("Error while opening fdf file \"%1%\".")) % filePath.string() << std::endl;
		
		return false;
	}

	fstream << "StringList {" << std::endl;
	std::size_t i = 0;

	BOOST_FOREACH(const StringListValueConst value, strings)
	{
		fstream << "\t" << value.second->idString() << "\t\t\t\"" << value.second->valueString() << '\"';
		
		if (i != strings.size() - 1)
			fstream << ',';

		fstream << " /* " << value.second->defaultString() << " */" << std::endl;
		++i;
	}

	fstream << '}' << std::endl;

	return true;
}

bool Parser::readWts(const boost::filesystem::path &filePath, StringList &strings)
{
	std::cout << boost::format(_("Reading wts file \"%1%\".")) % filePath.string() << std::endl;
	boost::filesystem::ifstream fstream(filePath);
	
	if (!fstream)
	{
		std::cerr << boost::format(_("Error while opening wts file \"%1%\".")) % filePath << std::endl;
		
		return false;
	}

	std::list<std::string> lines;
	std::string line;

	for (std::size_t i = 0; std::getline(fstream, line); ++i, lines.push_back(line))
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
				std::cerr << boost::format(_("Error at line %1%: Missing value string line or { character line.")) % i << std::endl;
				
				break;
			}
		}
		else
		{
			std::cerr << boost::format(_("Error at line %1%: Missing default string line.")) % i << std::endl;
			//std::cout << "Line: " << line << " and sub string " << line.substr(2) << std::endl;
			
			break;
		}
	}

	fstream.close();

	return true;
}

bool Parser::writeWts(const boost::filesystem::path &filePath, const StringList &strings)
{
	std::cout << boost::format(_("Writing wts file \"%1%\".")) % filePath.string() << std::endl;
	boost::filesystem::ofstream fstream(filePath);

	if (!fstream)
	{
		std::cerr << boost::format(_("Error while creating wts file \"%1%\".")) % filePath.string() << std::endl;
		
		return false;
	}

	BOOST_FOREACH(StringListValueConst value, strings)
	{
		fstream << "STRING " << value.second->id() << '\n'
		<< "// " << value.second->defaultString() << '\n'
		<< "{\n"
		<< value.second->valueString() << '\n'
		<< "}\n"
		<< std::endl;
	}

	fstream.close();

	return true;
}

}
