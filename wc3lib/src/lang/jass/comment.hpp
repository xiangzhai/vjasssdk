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

#ifndef WC3LIB_LANG_JASS_COMMENT_HPP
#define WC3LIB_LANG_JASS_COMMENT_HPP

#include "object.hpp"

namespace wc3lib
{
	
namespace lang
{
	
namespace jass
{

class Comment : public Object
{
	public:
		class List : public Object::List
		{		
#ifdef HTML
				virtual std::string htmlCategoryName() const;
				virtual std::string htmlFolderName() const;
#endif

			protected:
#ifdef SQLITE
				virtual std::string sqlTableName() const;
				virtual std::size_t sqlColumns() const;
				virtual std::string sqlColumnDataType(std::size_t column) const throw (std::exception);
				virtual std::string sqlColumnName(std::size_t column) const throw (std::exception);
#endif
		};
	
		Comment(const std::string &identifier, class SourceFile *sourceFile, std::size_t line, class DocComment *docComment);
#ifdef SQLITE
		Comment(std::vector<Object::SqlValueDataType> &columnVector);
#endif
		virtual ~Comment();
		virtual void init();
#ifdef SQLITE
		virtual std::string sqlValue(std::size_t column) const;
#endif
#ifdef HTML
		virtual void writeHtmlPageNavigation(std::ostream &ostream) const;
		virtual void writeHtmlPageContent(std::ostream &ostream) const;
#endif
		
	protected:
		virtual std::string htmlPageName() const;
};

inline virtual std::string Comment::htmlPageName() const
{
	return this->m_identifier;
}

}

#endif
