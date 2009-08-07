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
//POSIX
#include <sys/stat.h>

#include "parser.h"
#include "string.h"

namespace vjasstrans
{

std::string Parser::translationFunction = "tr";

bool Parser::parse(const std::string &filePath, std::list<class String*> &strings, bool replace)
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
		std::string::size_type position = line.find(searchedString);

		if (position != std::string::npos)
		{
			std::string::size_type length = line.find("\")", position);

			if (position != std::string::npos)
			{
				length = length - position;
				position += 4;
				length -= 4;
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
					strings.push_back(new String(filePath, i, sstream.str(), defaultString));
					
					if (replace)
						line.replace(position, length, sstream.str());
				}
				else if (replace)
					line.replace(position, length, (*iterator)->idString());
			}
		}

		lines.push_back(line);
	}

	fstream.close();
	fstream.open(filePath.c_str(), std::ios_base::out);

	for (std::list<std::string>::iterator iterator = lines.begin(); iterator != lines.end(); ++iterator)
	{
		fstream << *iterator << '\n';
	}

	fstream.close();

	return true;
}

bool Parser::writeFdf(const std::string &filePath, const std::list<class String*> &strings)
{
	std::ofstream fstream(filePath.c_str());

	if (!fstream)
	{
		fprintf(stdout, "Error while opening file \"%s\".\n", filePath.c_str());
		return false;
	}

	fstream << "StringList {" << std::endl;
	int i = 0;

	for (std::list<class String*>::const_iterator iterator = strings.begin(); iterator != strings.end(); ++iterator, ++i)
	{
		fstream << "\t" << (*iterator)->idString() << "\t\t\t\"\"";
		
		if (i != strings.size() - 1)
			fstream << ',';

		fstream << "\t/* " << (*iterator)->defaultString() << " */" << std::endl;
	}

	fstream << '}' << std::endl;

	return true;
}

bool Parser::writeWts(const std::string &filePath, const std::string &existingFilePath, const std::list<class String*> &strings)
{
	struct stat fileInfo;
	bool exists = stat(existingFilePath.c_str(), &fileInfo) == 0;
	std::fstream fstream;
	unsigned int startId = 0;

	if (exists)
	{
		fstream.open(existingFilePath.c_str(), std::ios_base::in);
		
		if (!fstream)
		{
			fprintf(stderr, "Error while opening map strings file \"%s\".\n", filePath.c_str());
			return false;
		}

		std::list<std::string> lines;
		std::string line;

		for (unsigned int i = 0; std::getline(fstream, line); ++i)
		{
			std::string::size_type position = line.find("STRING");
			
			if (position != std::string::npos)
				startId = atoi(line.substr(position + 1).c_str()) + 1;
			
			lines.push_back(line);
		}

		fstream.close();
	}
	
	fstream.open(filePath.c_str(), std::ios_base::out);

	if (!fstream)
	{
		fprintf(stderr, "Error while creating wts file \"%s\".\n", filePath.c_str());
		return false;
	}

	for (std::list<class String*>::const_iterator iterator = strings.begin(); iterator != strings.end(); ++iterator, ++startId)
	{
		fstream << "STRING " << startId << '\n'
		<< "// " << (*iterator)->defaultString() << '\n'
		<< "{\n"
		<< (*iterator)->defaultString() << '\n'
		<< "}\n"
		<< std::endl;
	}

	fstream.close();

	return true;
}

}
