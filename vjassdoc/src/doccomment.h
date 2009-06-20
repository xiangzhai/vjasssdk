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

#ifndef VJASSDOC_DOCCOMMENT_H
#define VJASSDOC_DOCCOMMENT_H

#include "object.h"
#include "parser.h"

namespace vjassdoc
{

class DocComment : public Object
{
	public:
		static const char *sqlTableName;
		static unsigned int sqlColumns;
		static std::string sqlColumnStatement;

		static void initClass();
		DocComment(const std::string &identifier, class SourceFile *sourceFile, unsigned int line);
		DocComment(std::vector<const unsigned char*> &columnVector);
		virtual void init();
		virtual void pageNavigation(std::ofstream &file) const;
		virtual void page(std::ofstream &file) const;
		virtual std::string sqlStatement() const;
		void setObject(class Object *object); //Just used by the Object class.
		std::string formattedText() const;
		class Object *object() const;

	protected:
		static const char *keyword[Parser::MaxLists];
	
		class DocComment* docComment() const; //Do not use
		
		std::string m_formattedText;
		class Object *m_object;
};

inline void DocComment::setObject(class Object *object)
{
	this->m_object = object;
}

inline std::string DocComment::formattedText() const
{
	return this->m_formattedText;
}

inline class Object* DocComment::object() const
{
	return this->m_object;
}

}

#endif
