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
#include "internationalisation.hpp"
#include "syntaxerror.hpp"
#include "utilities.hpp"

namespace wc3lib
{
	
namespace lang
{

File::File() : m_parser(0), m_language(0), m_lines(0), m_docComment(0)
{
}

std::size_t File::parse(class Parser *parser, class SourceFile *sourceFile, std::istream &istream)
{
	this->m_parser = parser;
	this->m_language = parser->m_currentLanguage;
	
	istream.seekg(0, std::ios_base::end);
	std::streampos position = ifstream.tellg();
	istream.seekg(0);
	char *bytes = new char[position + 3];
	istream.read(bytes, position + 1);
	bytes[position + 1] = YY_END_OF_BUFFER_CHAR;
	bytes[position + 2] = YY_END_OF_BUFFER_CHAR;
	YY_BUFFER_STATE buffer = yy_scan_bytes(bytes, position + 3);
	
	if (stateHandle == 0)
		return 0;
	
	yy_switch_to_buffer(buffer);
	//yy_delete_buffer(buffer);
	delete[] bytes;
	bytes = 0;
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
