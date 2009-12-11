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

#ifndef VJASSDOC_EXCEPTION_HPP
#define VJASSDOC_EXCEPTION_HPP

#include <exception>
#include <string>

namespace vjassdoc
{

class Exception : public std::exception
{
	public:
		Exception(std::size_t errorCode, const std::string &message);
		Exception(const std::string &message);
		virtual ~Exception();
		std::size_t errorCode() const;
		const std::string& message() const;
		virtual const char* what() const;
		
	protected:
		std::size_t m_errorCode;
		std::string m_message;
};

inline std::size_t Exception::errorCode() const
{
	return this->m_errorCode;
}

inline const std::string& Exception::message() const
{
	return this->m_message;
}

inline const char* Exception::what() const
{
	return this->m_message.c_str();
}

}

#endif
