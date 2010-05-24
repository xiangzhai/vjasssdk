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

#include <map>
#include <string>

#include <boost/filesystem.hpp>

namespace vjasstrans
{

class String;

class Parser
{
	public:
		typedef std::map<std::size_t, class String*> StringList; // key is string id
		typedef std::pair<std::size_t, class String*> StringListValue;
		typedef std::pair<std::size_t, const class String*> StringListValueConst;

		static bool parse(const boost::filesystem::path &filePath, StringList &strings, const bool replace = false, const std::string &translationFunction = "GetLocalizedString");
		static bool readFdf(const boost::filesystem::path &filePath, StringList &strings);
		static bool writeFdf(const boost::filesystem::path &filePath, const StringList &strings);
		static bool readWts(const boost::filesystem::path &filePath, StringList &strings);
		static bool writeWts(const boost::filesystem::path &filePath, const StringList &strings);
};

}

#endif
