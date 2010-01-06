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

#ifndef WC3LIB_LANG_SOURCEFILE_HPP
#define WC3LIB_LANG_SOURCEFILE_HPP

#include "object.hpp"

namespace wc3lib
{
	
namespace lang
{

class SourceFile : public Object
{
	public:
		class List : public Object::List
		{
			public:
				List(class Language *language);
				virtual ~List();			
#ifdef HTML
				virtual const std::string& htmlCategoryName() const;
				virtual const std::string& htmlFolderName() const;
#endif

				
			protected:
#ifdef SQLITE
				virtual const std::string& sqlTableName() const;
				virtual std::size_t sqlColumns() const;
				virtual const std::string& sqlColumnDataType(std::size_t column) const throw (class Exception);
				virtual const std::string& sqlColumnName(std::size_t column) const throw (class Exception);
#endif
		};

		SourceFile(const boost::filesystem::path &path);
#ifdef SQLITE		
		SourceFile(std::vector<Object::SqlValueDataType> &columnVector);
#endif
		virtual ~SourceFile();

		virtual void init();
#ifdef SQLITE
		virtual const std::string& sqlValue(std::size_t column) const;
#endif
#ifdef HTML
		virtual void writeHtmlPageNavigation(std::ostream &ostream) const;
		virtual void writeHtmlPageContent(std::ostream &ostream) const;
#endif

#ifdef SQLITE
		virtual const std::string& sqlStatement() const;
#endif
		const boost::filesystem::path& path() const;
#ifdef HTML
		const std::string& lineLink(const std::size_t &line, const std::string &text) const;
		
		/**
		* @param sourceFileName If this value is true, the name of source file will be shown as link. Otherwise the line will be shown.
		*/
		static const std::string& sourceFileLineLink(const class Object *object, const bool &sourceFileName = true, const std::string &identifier = "-");
#endif

	protected:
#ifdef HTML
		virtual const std::string& htmlPageName() const;
#endif
		
		// do not use
		class SourceFile* sourceFile() const;
		std::size_t line() const;

		boost::filesystem::path m_path;
};

inline const boost::filesystem::path& SourceFile::path() const
{
	return this->m_path;
}

#ifdef HTML
inline const std::string& SourceFile::lineLink(const std::size_t &line, const std::string &text) const
{
	std::ostringstream sstream;
	sstream << "<a href=\"" << this->htmlPageLink() << "#" << line << "\">" << text << "</a>";
	
	return sstream.str();
}

inline const std::string& SourceFile::sourceFileLineLink(const class Object *object, const bool &sourceFileName, const std::string &identifier)
{
	if (object->sourceFile() == 0)
		return identifier;
	
	if (sourceFileName)
		return object->sourceFile()->lineLink(object->line(), object->sourceFile()->identifier());
	
	std::ostringstream sstream;
	sstream << object->line();
	
	return object->sourceFile()->lineLink(object->line(), sstream.str());
}
#endif

}

}

#endif
