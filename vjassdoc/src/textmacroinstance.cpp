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

#include <sstream>

#include "objects.h"
#include "internationalisation.h"

namespace vjassdoc
{

const char *TextMacroInstance::sqlTableName = "TextMacroInstances";
unsigned int TextMacroInstance::sqlColumns;
std::string TextMacroInstance::sqlColumnStatement;

void TextMacroInstance::initClass()
{
	TextMacroInstance::sqlColumns = TextMacro::sqlColumns + 1;
	TextMacroInstance::sqlColumnStatement = TextMacro::sqlColumnStatement +
	",TextMacro INT";
}

TextMacroInstance::TextMacroInstance(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, const std::string &arguments) : m_textMacro(0), TextMacro(identifier, sourceFile, line, docComment, arguments)
{
}

TextMacroInstance::TextMacroInstance(std::vector<const unsigned char*> &columnVector) : m_textMacro(0), TextMacro(columnVector)
{
}

void TextMacroInstance::init()
{
	TextMacro::init();
	
	//mustn't be empty
	this->m_textMacro = static_cast<class TextMacro*>(this->searchObjectInList(this->identifier(), Parser::TextMacros));
}

void TextMacroInstance::pageNavigation(std::ofstream &file) const
{
	//do not use TextMacro::pageNavigation() because there are listed all instances.
	file
	<< "\t\t\t<li><a href=\"#Description\">"	<< _("Description") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Source File\">"	<< _("Source File") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Arguments\">"		<< _("Arguments") << "</a></li>\n"
	<< "\t\t\t<li><a href=\"#Text Macro\">"		<< _("Text Macro") << "</a></li>\n"
	;
}

void TextMacroInstance::page(std::ofstream &file) const
{
	//do not use TextMacro::page() because there are listed all instances.
	file
	<< "\t\t<h2><a name=\"Description\">" << _("Description") << "</a></h2>\n"
	<< "\t\t<p>\n"
	<< "\t\t" << Object::objectPageLink(this->docComment()) << '\n'
	<< "\t\t</p>\n"
	<< "\t\t<h2><a name=\"Source File\">" << _("Source File") << "</a></h2>\n"
	<< "\t\t" << SourceFile::sourceFileLineLink(this) << '\n'
	<< "\t\t<h2><a name=\"Arguments\">" << _("Arguments") << "</a></h2>\n"
	<< "\t\t" << this->parameters() << '\n'
	<< "\t\t<h2><a name=\"Text Macro\">" << _("Text Macro") << "</a></h2>\n"
	<< "\t\t" << Object::objectPageLink(this->textMacro(), this->identifier()) << '\n'
	;
}

std::string TextMacroInstance::sqlStatement() const
{
	std::ostringstream sstream;
	sstream
	<< TextMacro::sqlStatement() << ", "
	<< "TextMacro=" << Object::objectId(this->textMacro());

	return sstream.str();
}

}
