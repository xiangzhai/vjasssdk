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

#include "objects.h"

namespace vjassdoc
{

#ifdef SQLITE
const char *Key::sqlTableName = "Keys";
unsigned int Key::sqlColumns;
std::string Key::sqlColumnStatement;

void Key::initClass()
{
	Key::sqlColumns = Keyword::sqlColumns;
	Key::sqlColumnStatement = Keyword::sqlColumnStatement;
}
#endif

Key::Key(const std::string &identifier, class SourceFile *sourceFile, unsigned int line, class DocComment *docComment, class Library *library, class Scope *scope, bool isPrivate) : Keyword(identifier, sourceFile, line, docComment, library, scope, isPrivate)
{
}

#ifdef SQLITE
Key::Key(std::vector<const unsigned char*> &columnVector) : Keyword(columnVector)
{
}
#endif

}
