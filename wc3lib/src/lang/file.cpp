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
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <list>

#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include "file.hpp"
#include "parser.hpp"
#include "language.hpp"
#include "syntaxerror.hpp"
#include "../internationalisation.hpp"
#include "../utilities.hpp"

namespace wc3lib
{
	
namespace lang
{

File::File() : yyFlexLexer(), m_parser(0), m_language(0), m_lines(0), m_docComment(0)
{
}

std::size_t File::parse(class Parser *parser, class SourceFile *sourceFile, std::istream &istream)
{
	this->m_parser = parser;
	this->m_language = parser->m_currentLanguage;
	
	istream.seekg(0, std::ios_base::end);
	std::streampos position = ifstream.tellg();
	struct yy_buffer_state *bufferState = this->yy_create_buffer(&istream, position + 1);
	
	if (bufferState == 0)
		throw Exception(_("Error while creating buffer."));
	
	this->yy_switch_to_buffer(bufferState);
	
	/// @todo Parse
	
	while (true)
	{
		if (this->yylex() == 0)
		{
			this->yy_delete_buffer(bufferState);
			
			throw Exception(_("Parsing error!"));
		}
	}
	
	this->yy_delete_buffer(bufferState);
	bufferState = 0;
	
	std::size_t lines = this->m_lines;
	
	// reset members
	this->m_parser = 0;
	this->m_sourceFile = 0;
	this->m_language = Language::jass();
	this->m_lines = 0;
	this->m_docComment = 0;
	
	return lines;
}

}

}
