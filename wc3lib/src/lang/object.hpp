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

#ifndef WC3LIB_LANG_OBJECT_HPP
#define WC3LIB_LANG_OBJECT_HPP

#include <string>
#include <list>
#include <ostream>

#include "parser.hpp"

namespace wc3lib
{
	
namespace lang
{

class SourceFile;

class Object
{
	public:
		class List : public std::list<class Object*>
		{
			public:
				List();
				virtual ~List();
				/**
				* Adds @param object to back of list.
				*/
				List& operator<<(class Object *object);
				/**
				* Extracts last object of list into @param object and removes it from list.
				*/
				List& operator>>(class Object* &object);
			
#ifdef HTML
				virtual std::string htmlCategoryName() const = 0;
				virtual std::string htmlFolderName() const = 0;
				void writeHtmlList(std::ostream &ostream);
				void writeHtmlPages() throw (std::exception);
#endif
#ifdef SQLITE
				/**
				* Writes all contained objects into database @param database.
				*/
				void writeSqlTable(sqlite3 *database);
#endif
				
			protected:
#ifdef SQLITE
				virtual std::string sqlTableName() const;
				virtual std::size_t sqlColumns() const;
				virtual std::string sqlColumnDataType(std::size_t column) const throw (std::exception);
				virtual std::string sqlColumnName(std::size_t column) const throw (std::exception);
#endif
				
			private:
				List(const List&);
		};

#ifdef SQLITE
		typedef const unsigned char* SqlValueDataType;
#endif
#ifdef HTML
		static std::string objectHtmlPageLink(const class Object *object, const std::string &identifier = "-"); //should use the id
#endif
	
		Object(const std::string &identifier, class SourceFile *sourceFile, std::size_t line);
#ifdef SQLITE		
		Object(std::vector<Object::SqlValueDataType> &columnVector);
#endif
		virtual ~Object();

		virtual void init() = 0; //Some Objects has to be initialized after finding all objects of all files.
#ifdef SQLITE
		virtual std::string sqlValue(std::size_t column) const = 0;
#endif
#ifdef HTML
		virtual void writeHtmlPageNavigation(std::ostream &ostream) const = 0;
		virtual void writeHtmlPageContent(std::ostream &ostream) const = 0;

		boost::filesystem::path htmlPagePath() const;
		std::string htmlPageLink() const;
#endif
		std::string identifier() const;
		const class SourceFile* sourceFile() const;
		std::size_t line() const;
#ifdef SQLITE
		/**
		* Ids are used when creating SQLite databases to identify each parsed object with an unique number.
		*/
		void setId(std::size_t id);
		std::size_t id() const;
#endif
		
		struct AlphabeticalComparator : public std::binary_function<const class Object*, const class Object*, bool>
		{
			bool operator()(const class Object *first, const class Object *second) const;
		};

	protected:
#ifdef HTML
		virtual std::string htmlPageName() const = 0;
#endif
		std::string m_identifier;
		class SourceFile *m_sourceFile;
		std::size_t m_line;
#ifdef SQLITE
		std::size_t m_id;
#endif

	private:
		Object(const Object&);
};

#ifdef HTML
inline std::string Object::objectHtmlPageLink(const class Object *object, const std::string &identifier)
{
	if (object == 0)
		return identifier;
	
	return object->htmlPageLink();
}

inline boost::filesystem::path Object::htmlPagePath() const
{
	return boost::filesystem::path(this->htmlFolderName()) / this->htmlPageName();
}

inline std::string Object::htmlPageLink() const
{
	std::ostringstream sstream;
	sstream << "<a href=\"" << this->htmlPagePath().string() << ".html\">";
	
	if (this->m_identifier.empty())
		sstream << _("Unnamed");
	else
		sstream << this->m_identifier;
	
	sstream << "</a>";
	
	return sstream.str();
}
#endif

inline std::string Object::identifier() const
{
	return this->m_identifier;
}

inline const class SourceFile* Object::sourceFile() const
{
	return this->m_sourceFile;
}

inline std::size_t Object::line() const
{
	return this->m_line;
}

#ifdef SQLITE
void Object::setId(std::size_t id)
{
	this->m_id = id;
}

std::size_t Object::id() const
{
	return this->m_id;
}
#endif

inline bool Object::AlphabeticalComparator::operator()(const class Object *first, const class Object *second) const
{
	return first->identifier().compare(second->identifier()) < 0;
}

}

}

#endif
