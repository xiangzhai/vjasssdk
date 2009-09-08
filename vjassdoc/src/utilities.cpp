/***************************************************************************
 *   Copyright (C) 2008, 2009 by Tamino Dauth                              *
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

#include <string>
#include <sys/stat.h>

namespace vjassdoc
{

#ifdef UNIX
const char *dirSeparator = "/";
#elif defined WIN32
const char *dirSeparator = "\\\\";
#else
#error You have to define UNIX or WIN32.
#endif

std::string getToken(const std::string &line, unsigned int &index, bool endOfLine)
{
	if (index >= line.length() || line.empty()) //important
		return std::string();

	while (index < line.length() && (line[index] == ' ' || line[index] == '\t'))
		++index;

	int position = index;
	int length = 1;

	if (!endOfLine)
	{
		do
		{
			++index;
		}
		while (index <= line.length() && line[index] != ' ' && line[index] != '\t');

		length = index - position;
	}
	else
	{
		index = line.length();
		length = index;
	}

	return line.substr(position, length);
}

void cutFilePath(std::string &filePath)
{
	std::size_t position = filePath.find_last_of(dirSeparator);
	
	if (position != std::string::npos)
		filePath.erase(0, position + 1);
}


bool fileExists(const std::string &fileName)
{
	struct stat fileInfo; 
	int state;
	state = stat(fileName.c_str(), &fileInfo);
	
	if (state == 0)
		return true;
	
	return false;
}

}
