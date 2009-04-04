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

#ifndef VJASSDOC_FUNCTION_H
#define VJASSDOC_FUNCTION_H

#include "functioninterface.h"

namespace vjassdoc
{

class Function : public FunctionInterface
{
	public:
		Function(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, class Library *library, class Scope *scope, bool isPrivate, std::list<std::string> *parameterTypeExpressions, std::list<std::string> *parameters, const std::string &returnTypeExpression, bool isPublic, bool isConstant, bool isNative);
		Function(std::vector<const unsigned char*> &columnVector);
		virtual void init();
		virtual void pageNavigation(std::ofstream &file) const;
		virtual void page(std::ofstream &file) const;
		virtual std::string sqlStatement() const;
		bool isPublic() const;
		bool isConstant() const;
		bool isNative() const;

	private:
		bool m_isPublic;
		bool m_isConstant;
		bool m_isNative;
};

inline bool Function::isPublic() const
{
	return this->m_isPublic;
}

inline bool Function::isConstant() const
{
	return this->m_isConstant;
}

inline bool Function::isNative() const
{
	return this->m_isNative;
}

}

#endif
