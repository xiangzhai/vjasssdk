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

#ifndef VJASSTRANS_PARSER_H
#define VJASSTRANS_PARSER_H

#include <list>
#include <string>

namespace vjasstrans
{

class String;

class Parser
{
	public:
		bool parse(const std::string &filePath, std::list<class String*> &strings, bool replace = false);
		bool writeFdf(const std::string &filePath, const std::list<class String*> &strings);
		bool writeWts(const std::string &filePath, const std::string &existingFilePath, const std::list<class String*> &strings);

	private:
		static std::string translationFunction;
};

}

#endif