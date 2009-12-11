/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
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

#ifndef VJASSDOC_CALL_HPP
#define VJASSDOC_CALL_HPP

#include "object.hpp"

namespace vjassdoc
{

class Call : public Object
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
#ifdef SQLITE
		static const std::size_t sqlMaxArguments;
#endif
		Call(const std::string &identifier, class SourceFile *sourceFile, std::size_t line, class DocComment *docComment, const std::string &functionIdentifier, std::list<std::string> *argumentIdentifiers, bool isExecuted, bool isEvaluated);
#ifdef SQLITE
		Call(std::vector<const unsigned char*> &columnVector);
#endif
		virtual ~Call();
		virtual void init();
#ifdef HTML
		virtual void writeHtmlPageNavigation(std::ostream &ostream) const;
		virtual void writeHtmlPageContent(std::ostream &ostream) const;
#endif
		
		std::string functionIdentifier() const;
		class Function* function() const;
		std::list<std::string>* argumentIdentifiers() const;
		std::list<class Object*>* arguments() const;
		bool isExecuted() const;
		bool isEvaluated() const;
		
	private:
		std::string m_functionIdentifier;
		class Function *m_function;
		std::list<std::string> *m_argumentIdentifiers;
		std::list<class Object*> *m_arguments;
		bool m_isExecuted;
		bool m_isEvaluated;
};

inline std::string Call::functionIdentifier() const
{
	return this->m_functionIdentifier;
}

inline class Function* Call::function() const
{
	return this->m_function;
}

inline std::list<std::string>* Call::argumentIdentifiers() const
{
	return this->m_argumentIdentifiers;
}

inline std::list<class Object*>* Call::arguments() const
{
	return this->m_arguments;
}

inline bool Call::isExecuted() const
{
	return this->m_isExecuted;
}

inline bool Call::isEvaluated() const
{
	return this->m_isEvaluated;
}

}

#endif
