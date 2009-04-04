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

TextMacroInstance::TextMacroInstance(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, const std::string &arguments) : m_textMacro(0), TextMacro(identifier, sourceFile, line, docComment, arguments)
{
}

TextMacroInstance::TextMacroInstance(std::vector<const unsigned char*> &columnVector) : TextMacro(columnVector)
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
	TextMacro::pageNavigation(file);
	file
	<< "\t\t\t<li><a href=\"#Text macro\">"	<< _("Text macro") << "</a></li>\n"
	;
}

void TextMacroInstance::page(std::ofstream &file) const
{
	TextMacro::page(file);
	file
	<< "\t\t<h2><a name=\"Text macro\">" << _("Text macro") << "</a></h2>\n"
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
