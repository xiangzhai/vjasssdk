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

//#include <iostream> //debug
#include <sstream>

#include "objects.h"
#include "file.h"
#include "internationalisation.h"

namespace vjassdoc
{

const char *DocComment::sqlTableName = "DocComments";
unsigned int DocComment::sqlColumns;
std::string DocComment::sqlColumnStatement;
const char *DocComment::keyword[Parser::MaxLists] =
{
	"comment",
	"keyword",
	"textmacro",
	"textmacroinstance",
	"type",
	"local",
	"global",
	"member",
	"parameter",
	"functioninterface",
	"function",
	"method",
	"implementation",
	"interface",
	"struct",
	"module",
	"scope",
	"library",
	"sourcefile",
	"doccomment"
};

void DocComment::initClass()
{
	DocComment::sqlColumns = 5;
	DocComment::sqlColumnStatement =
	"Text VARCHAR(255),"
	"SourceFile INT,"
	"Line INT,"
	"FormattedText VARCHAR(255),"
	"Object INT";
}

DocComment::DocComment(const std::string &identifier, class SourceFile *sourceFile, unsigned int line) : m_object(0), Object(identifier, sourceFile, line, 0)
{
}

DocComment::DocComment(std::vector<const unsigned char*> &columnVector) : Object(columnVector)
{
}

/// @todo FIXME
void DocComment::init()
{
	//std::cout << "Init doc comment -----------------------------" << std::endl;
	//std::cout << "Identifier " << this->identifier() << std::endl;

	std::string result;
	unsigned int oldIndex = 0;
	unsigned int newIndex = this->identifier().find('@');

	while (newIndex != std::string::npos)
	{
		result += this->identifier().substr(oldIndex, newIndex - oldIndex);
		++newIndex;
		
		if (newIndex == this->identifier().length())
			break;
		
		std::string token = File::getToken(this->identifier(), newIndex);
		//std::cout << "Token: " << token << " and index " << newIndex << std::endl;
		bool found = false;
		
		for (int i = 0; i < Parser::MaxLists; ++i)
		{
			if (token == DocComment::keyword[i])
			{
				//std::cout << "Found keyword: " << DocComment::keyword[i] << " and has index " << newIndex << std::endl;
				found = true;
				token = File::getToken(this->identifier(), newIndex); //FIXME
				//std::cout << "New token: " << token << std::endl;
				
				if (token.empty())
					break;
				
				/// @todo Cut . , ? ! etc: This is @functioninterface Peter!
				int lastIndex = token.length() - 1;
				char lastChar = token[lastIndex];
				
				if (lastChar == '.' || lastChar == ',' || lastChar == '!' || lastChar == '?')
					token = token.substr(0, lastIndex);
				else
					lastChar = 0;
				
				//std::cout << "Searching in list " << i << " for token " << token << std::endl;
				class Object *object = this->searchObjectInList(token, Parser::List(i));
				result += Object::objectPageLink(object, token);
				
				if (lastChar != 0)
					result += lastChar;
				
				//if (object != 0)
					//std::cout << "Object " << object->identifier() << " is not null." << std::endl;

				break;
			}
		}
		
		
		if (!found)
			result += token;
		
		oldIndex = newIndex;
		
		if (newIndex >= this->identifier().length() - 1)
			break;
		
		newIndex = this->identifier().find('@', newIndex);
	}
	
	//std::cout << "Old index " << oldIndex << std::endl;
	
	if (oldIndex <  this->identifier().length() - 1)
	{
		//std::cout << "Old index " << oldIndex << std::endl;
		result += this->identifier().substr(oldIndex);
	}
	
	m_formattedText = result;
}

void DocComment::pageNavigation(std::ofstream &file) const
{
	file
	<< "\t\t\t<li><a href=\"#Text\">"		<< _("Text") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Source File\">"	<< _("Source File") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Object\">"		<< _("Object") << "</a></li>\n"
	;
}

void DocComment::page(std::ofstream &file) const
{
	file
	<< "\t\t<h2><a name=\"Text\">" << _("Text") << "</a></h2>\n"
	<< "\t\t<p>\n"
	<< "\t\t" << this->m_formattedText << "\n"
	<< "\t\t</p>\n"
	<< "\t\t<h2><a name=\"Source File\">" << _("Source File") << "</a></h2>\n"
	<< "\t\t" << SourceFile::sourceFileLineLink(this) << '\n'
	<< "\t\t<h2><a name=\"Object\">" << _("Object") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->object()) << '\n'
	;
}

std::string DocComment::sqlStatement() const
{
	std::ostringstream sstream;
	sstream
	<< "Text=\"" << Object::sqlFilteredString(this->identifier()) << "\", "
	<< "SourceFile=" << Object::objectId(this->sourceFile()) << ", "
	<< "Line=" << this->line() << ", "
	<< "Object=" << Object::objectId(this->object());
	
	return sstream.str();
}

}
