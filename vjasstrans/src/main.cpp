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
#include <fstream>
#include <sstream>
#include <list>
#include <cstring>
#include <cstdlib>

class String
{
	public:
		String(const std::string &filePath, unsigned int line, const std::string &idString, const std::string &defaultString) : m_filePath(filePath), m_line(line), m_idString(idString), m_defaultString(defaultString)
		{
		};

		std::string filePath() const
		{
			return this->m_filePath;
		};
		unsigned int line() const
		{
			return this->m_line;
		};
		std::string idString() const
		{
			return this->m_idString;
		};
		std::string defaultString() const
		{
			return this->m_defaultString;
		};

	private:
		std::string m_filePath;
		unsigned int m_line;
		std::string m_idString;
		std::string m_defaultString;


};

static std::string translationFunction = "tr";
static std::list<class String*> strings;

static void run(const char *filePath)
{
	std::fstream fstream(filePath, std::ios_base::in);
	static std::string searchedString = translationFunction + "(\"";
	std::list<std::string> lines;
	std::string line;

	for (unsigned int i = 0; std::getline(fstream, line); ++i)
	{
		std::string::size_type position = line.find(searchedString);

		if (position != std::string::npos)
		{
			std::string::size_type length = line.find("\")", position);

			if (position != std::string::npos)
			{
				length = length - position;
				position += 4;
				length -= 4;
				std::stringstream sstream;
				sstream << "STRING_" << strings.size();
				strings.push_back(new String(filePath, i, sstream.str(), line.substr(position, length)));
				line.replace(position, length, sstream.str());
			}
		}

		lines.push_back(line);
	}

	fstream.close();
	fstream.open(filePath, std::ios_base::out);

	for (std::list<std::string>::iterator iterator = lines.begin(); iterator != lines.end(); ++iterator)
	{
		fstream << *iterator << '\n';
	}

	fstream.close();
}

static void writeOutputFile()
{
	std::ofstream fstream("output.fdf");
	fstream << "StringList {" << std::endl;
	int i = 0;

	for (std::list<class String*>::iterator iterator = strings.begin(); iterator != strings.end(); ++iterator, ++i)
	{
		fstream << "\t" << (*iterator)->idString() << "\t\t\t\"\"";
		
		if (i != strings.size() - 1)
			fstream << ',';

		fstream << "\t/* " << (*iterator)->defaultString() << " */" << std::endl;
	}

	fstream << '}' << std::endl;
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
			for (int i = 0; i < argc; ++i)
				run(argv[i]);

			writeOutputFile();
		}
	}
	else
	{
		std::cout << "Missing arguments." << std::endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
