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

#ifndef WC3LIB_LANG_FILE_HPP
#define WC3LIB_LANG_FILE_HPP

#include <istream>

#include <FlexLexer.h>

namespace wc3lib
{
	
namespace lang
{

class Parser;
class SourceFile;
class Language;
class DocComment;

/**
* A Code file which is treated by a @class Parser instance.
* This class is able to handle the whole scripting languages syntax and to parse it.
*/
class File : public yyFlexLexer
{
	public:
		File();
		/**
		* @return Returns number of parsed lines.
		*/
		std::string::size_type parse(class Parser *parser, class SourceFile *sourceFile, std::istream &istream);
		
		const class Parser* parser() const;
		const class SourceFile* sourceFile() const;
		/**
		* Required by Bison parser.
		*/
		void setLanguage(class Language *language);
		/**
		* Returns the current interpreted language in file.
		*/
		const class Language* language() const;
		/**
		* Required by Bison parser.
		*/
		void addLines(std::size_t lines);
		std::size_t lines() const;
		/**
		* Similar to File.lines.
		*/
		std::size_t line() const;
		/**
		* Since documentation comments are object related there should be a current
		* documentation comment which is related to the next added parsed object.
		*/
		void setDocComment(class DocComment *docComment);
		void clearDocComment();
		const class DocComment* docComment() const;
		
	protected:
		class Parser *m_parser;
		class SourceFile *m_sourceFile;
		class Language *m_language;
		std::size_t m_lines;
		class DocComment *m_docComment;
};

inline const class Parser* File::parser() const
{
	return this->m_parser;
}

inline const class SourceFile* File::sourceFile() const
{
	return this->m_sourceFile;
}

inline void File::setLanguage(class Language *language)
{
	this->m_language = language;
}

inline const class Language* File::language() const
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

inline std::size_t File::line() const
{
	return this->m_lines;
}

inline void File::setDocComment(class DocComment *docComment)
{
	this->m_docComment = docComment;
}

inline void File::clearDocComment()
{
	this->m_docComment = 0;
}

inline const class DocComment* File::docComment() const
{
	return this->m_docComment;
}

}

}

#endif
