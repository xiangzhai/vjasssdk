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

#ifndef VJASSDOC_FILE_HPP
#define VJASSDOC_FILE_HPP

#include <istream>

#include "vjassdoc.hpp"

namespace vjassdoc
{

class Parser;
class SourceFile;

/**
* A Jass or vJass code file which is treated by a @class Parser instance.
* This class knows the whole Jass and vJass syntax and is able to parse it.
* Parsing runs automatically after creation of an instance in constructor.
*/
class File
{
	public:
		File();
		/**
		* @return Returns number of parsed lines.
		*/
		std::string::size_type parse(class Parser *parser, class SourceFile *sourceFile, std::istream &istream);
		
		class Parser* parser() const;
		class SourceFile* sourceFile() const;
		/**
		* Required by Bison parser.
		*/
		void setLanguage(enum Vjassdoc::Language language);
		/**
		* Returns the current interpreted language in file.
		*/
		enum Vjassdoc::Language language() const;
		/**
		* Required by Bison parser.
		*/
		void addLines(std::size_t lines);
		std::size_t lines() const;
		
	protected:
		class Parser *m_parser;
		class SourceFile *m_sourceFile;
		enum Vjassdoc::Language m_language;
		std::size_t m_lines;
};

inline class Parser* File::parser() const
{
	return this->m_parser;
}

inline class SourceFile* File::sourceFile() const
{
	return this->m_sourceFile;
}

inline void File::setLanguage(enum Vjassdoc::Language language)
{
	this->m_language = language;
}

inline enum Vjassdoc::Language File::language() const
{
	return this->m_language;
}

inline void File::addLines(std::size_t lines)
{
	this->m_lines += lines;
}

inline std::size_t File::lines() const
{
	return this->m_lines;
}

}

#endif
