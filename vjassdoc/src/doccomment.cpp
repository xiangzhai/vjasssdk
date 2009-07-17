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

#include <iostream> //debug
#include <sstream>

#include "objects.h"
#include "file.h"
#include "internationalisation.h"

namespace vjassdoc
{

const char *DocComment::sqlTableName = "DocComments";
unsigned int DocComment::sqlColumns;
std::string DocComment::sqlColumnStatement;
const int DocComment::maxAuthors = 10;
const int DocComment::maxSeeObjects = 10;
const char *DocComment::keyword[DocComment::MaxKeywords] =
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
	"doccomment",
	//doxygen
	"arg",
	"attention",
	"author",
	"callgraph",
	"code",
	"dot",
	"else",
	"endcode",
	"endcond",
	"enddot",
	"endhtmlonly",
	"endif",
	"endlatexonly",
	"endlink",
	"endmanonly",
	"endverbatim",
	"endxmlonly",
	"f[",
	"f]",
	"f$",
	"hideinitializer",
	"htmlonly",
	"internal",
	"invariant",
	"~",
	"@",
	"$",
	"\\",
	"#",
	"latexonly",
	"li",
	"manonly",
	"n",
	"nosubgrouping",
	"note",
	"only",
	"post",
	"pre",
	"remarks",
	"return",
	"returns",
	"sa",
	"see",
	"showinitializer",
	"since",
	"test",
	"todo",
	"verbatim",
	"warning",
	"xmlonly",
	"addtogroup",
	"a",
	"anchor",
	"b",
	"c",
	"cond",
	"copydoc",
	"def",
	"dontinclude",
	"dotfile",
	"e",
	"elseif",
	"em",
	"example",
	"file",
	"htmlinclude",
	"if",
	"ifnot",
	"include",
	"link",
	"p",
	"package",
	"ref",
	"relatesalso",
	"relates",
	"retval",
	"verbinclude",
	"version",
	"xrefitem",
	"param",
	"image",
	"defgroup",
	"page",
	"paragraph",
	"section",
	"subsection",
	"subsubsection",
	"weakgroup",
	"addindex",
	"brief",
	"bug",
	"date",
	"deprecated",
	"fn",
	"ingroup",
	"line",
	"mainpage",
	"name",
	"overload",
	"par",
	"short",
	"skip",
	"skipline",
	"until",
	"var"
	/*
	"author",
	"todo",
	"param",
	"return",
	"state",
	"source"
	*/
};

void DocComment::initClass()
{
	DocComment::sqlColumns = 4 + DocComment::maxAuthors + DocComment::maxSeeObjects;
	DocComment::sqlColumnStatement =
	"Text VARCHAR(255),"
	"SourceFile INT,"
	"Line INT,"
	"BriefDescription VARCHAR(255),"
	"Object INT,";
	std::ostringstream sstream;
	
	for (int i = 0; i < DocComment::maxAuthors; ++i)
		sstream << "Author" << i << " VARCHAR(255),";
	
	for (int i = 0; i < DocComment::maxSeeObjects; ++i)
		sstream << "SeeObject" << i << " INT,";
	
	/// @todo Remove last ,.
	DocComment::sqlColumnStatement += sstream.str();
}

DocComment::DocComment(const std::string &identifier, class SourceFile *sourceFile, unsigned int line) : m_object(0), Object(identifier, sourceFile, line, 0)
{
}

DocComment::DocComment(std::vector<const unsigned char*> &columnVector) : m_object(0), Object(columnVector)
{
}

/// @todo FIXME
void DocComment::init()
{
	std::cout << "Documentation comment initialization" << std::endl;
	std::string result = this->identifier();

	// ab index suchen
	for (std::string::size_type index = result.find('@'); index != std::string::npos; index = result.find('@'))
	{
		std::string::size_type oldIndex = index; //position of identifier (after erasing): @author Peter, position of P
		std::string keyword = File::getToken(result, ++index);
		result.erase(oldIndex, index);
		std::cout << "Keyword " << keyword << std::endl;
		
		if (keyword.empty()) //@ only
			continue;
	
		for (unsigned int i = 0; i < DocComment::MaxKeywords; ++i)
		{
			if (keyword == DocComment::keyword[i])
			{
				switch (i)
				{
					case DocComment::AuthorKeyword:
					{
						std::string::size_type start = oldIndex;
						std::string::size_type length = result.find(oldIndex, '\n');
						
						if (length == std::string::npos)
							length = result.length() - oldIndex;
						else
							length -= oldIndex;
						
						if (length == 0)
							break;
						
						std::string author = result.substr(start, length);
						result.erase(start, length); /// @todo get the whole line or until next @ character.
						
						this->m_authors.push_back(author);
						break;
					}
				
					case DocComment::BriefKeyword:
					{
						std::string::size_type end = result.find('\n', oldIndex);
						std::string::size_type length = end != std::string::npos ? end - oldIndex : result.length() - oldIndex;
						
						this->m_briefDescription = result.substr(oldIndex, length); // get the line
						result.erase(oldIndex, length);
						break;
					}
					
					case DocComment::SeeKeyword:
					{
						std::string identifier = File::getToken(result, oldIndex);
						
						if (identifier.empty())
							break;
						
						class Object *object = 0;
						
						for (unsigned int i = 0; i < Parser::MaxLists && object == 0; ++i)
							object = Vjassdoc::getParser()->searchObjectInList(identifier, Parser::List(i));
					
						if (object != 0)
							this->m_seeObjects.push_back(object);
						
						break;
					}
					
					default:
					{
						if (i >= DocComment::CommentKeyword && i <= DocCommentKeyword)
						{
							std::string::size_type position = oldIndex;
							std::string identifier = File::getToken(result, oldIndex);
							
							std::cout << "Identifier " << identifier << std::endl;
							
							class Object *object = this->searchObjectInList(identifier, Parser::List(i), Parser::Unspecified);
							std::cout << "object " << object << std::endl;
							std::string link = Object::objectPageLink(object, identifier);
							std::cout << "link " << link << std::endl;
							
							result.replace(position, oldIndex - position, link);
						}
					}
				}
				
				break;
			}
		}
		
	}
	
	std::cout << "After initialization" << std::endl;
	this->setIdentifier(result);

/*
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
		
		for (int i = 0; i < DoComment::Author; ++i)
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
*/
}

void DocComment::pageNavigation(std::ofstream &file) const
{
	file
	<< "\t\t\t<li><a href=\"#Brief Description\">"	<< _("Brief Description") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Source File\">"	<< _("Source File") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Object\">"		<< _("Object") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Authors\">"		<< _("Authors") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#See Objects\">"	<< _("See Objects") << "</a></li>\n"
	;
}

void DocComment::page(std::ofstream &file) const
{
	file
	<< "\t\t<h2><a name=\"Brief Description\">" << _("Brief Description") << "</a></h2>\n"
	<< "\t\t<p>\n"
	<< "\t\t";
	
	if (!this->m_briefDescription.empty())
		file << this->m_briefDescription;
	else
		file << '-';
	file
	<< "\n"
	<< "\t\t</p>\n"
	<< "\t\t<h2><a name=\"Source File\">" << _("Source File") << "</a></h2>\n"
	<< "\t\t" << SourceFile::sourceFileLineLink(this) << '\n'
	<< "\t\t<h2><a name=\"Object\">" << _("Object") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->object()) << '\n'
	<< "\t\t<h2><a name=\"Authors\">" << _("Authors") << "</a></h2>\n"
	;
	
	std::cout << "Bla 1" << std::endl;
	
	if (!this->m_authors.empty())
	{
		std::cout << "Not empty" << std::endl;
	
		file << "\t\t<ul>\n";
	
		//for (std::vector<std::string>::iterator iterator = this->m_authors.begin(); iterator != this->m_authors.end(); ++iterator)
			//file << "\t\t\t<li>" << *iterator << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
	
	
	std::cout << "Bla 2" << std::endl;
	
	file
	<< "\t\t<h2><a name=\"See Objects\">" << _("See Objects") << "</a></h2>\n"
	;
	
	std::cout << "Bla 3" << std::endl;
	
	if (!this->m_seeObjects.empty())
	{
		file << "\t\t<ul>\n";
	
		//for (std::vector<class Object*>::iterator iterator = this->m_seeObjects.begin(); iterator != this->m_seeObjects.end(); ++iterator)
			//file << "\t\t\t<li>" << *iterator << "</li>\n";
		
		file << "\t\t</ul>\n";
	}
	else
		file << "\t\t-\n";
}

std::string DocComment::sqlStatement() const
{
	std::ostringstream sstream;
	sstream
	<< "Text=\"" << Object::sqlFilteredString(this->identifier()) << "\", "
	<< "SourceFile=" << Object::objectId(this->sourceFile()) << ", "
	<< "Line=" << this->line() << ", "
	<< "BriefDescription=" << this->briefDescription() << ", "
	<< "Object=" << Object::objectId(this->object()) << ", ";
	
	int i = 0;
	
	for (std::vector<std::string>::iterator iterator = this->authors().begin(); iterator != this->authors().end() && i < DocComment::maxAuthors; ++iterator, ++i)
		sstream << "Author" << i << "=\"" << Object::sqlFilteredString(*iterator) << "\", ";
	
	for ( ; i < DocComment::maxAuthors; ++i)
		sstream << "Author" << i << "=NULL, ";

	i = 0;
	
	for (std::vector<class Object*>::iterator iterator = this->seeObjects().begin(); iterator != this->seeObjects().end() && i < DocComment::maxSeeObjects; ++iterator, ++i)
		sstream << "SeeObject" << i << "=" << Object::objectId(*iterator) << ", ";
	
	for ( ; i < DocComment::maxSeeObjects; ++i)
		sstream << "SeeObject" << i << "=-1, ";
	
	/// @todo Remove last , character.
	
	return sstream.str();
}

}
