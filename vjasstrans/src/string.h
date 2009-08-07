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

#ifndef VJASSTRANS_STRING_H
#define VJASSTRANS_STRING_H

#include <string>

namespace vjasstrans
{

class String
{
	public:
		String(const std::string &filePath, unsigned int line, const std::string &idString, const std::string &defaultString) : m_filePath(filePath), m_line(line), m_idString(idString), m_defaultString(defaultString)
		{
		};

		std::string filePath() const
		{
			return this->m_filePath;
		};
		unsigned int line() const
		{
			return this->m_line;
		};
		std::string idString() const
		{
			return this->m_idString;
		};
		std::string defaultString() const
		{
			return this->m_defaultString;
		};

	private:
		std::string m_filePath;
		unsigned int m_line;
		std::string m_idString;
		std::string m_defaultString;


};

}

#endif
