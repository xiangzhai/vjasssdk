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
#include <functional>
#include <sstream>
#include <list>

#include <boost/filesystem.hpp>

namespace vjasstrans
{

class String
{
	public:
		struct UsageData
		{
			boost::filesystem::path &filePath;
			std::size_t line;
		};

		struct IdComparator : public std::binary_function<class String*, class String*, bool>
		{
			bool operator()(class String *first, class String *second) const
			{
				return first->id() > second->id();
			};
		};

		static std::size_t id(const String &string)
		{
			std::stringstream sstream(string.m_idString.substr(7));
			std::size_t result;
			sstream >> result;

			return result;
		}

		String(const std::string &idString, const std::string &defaultString, const std::string &valueString) : m_filePath(filePath), m_line(line), m_idString(idString), m_defaultString(defaultString), m_valueString(valueString), m_usageData(std::list<struct UsageData*>())
		{
		};

		~String()
		{
			BOOST_FOREACH(struct UsageData *usageData, this->m_usageData)
				delete usageData;
		}

		void addUsage(const boost::filesystem::path &path, std::size_t line)
		{
			struct UsageData *usageData = new UsageData;
			usageData->filePath = path;
			usageData->line = line;
			this->m_usageData.push_back(usageData);
		}

		boost::filesystem::path filePath() const
		{
			return this->m_filePath;
		};
		std::size_t line() const
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
		std::string valueString() const
		{
			return this->m_valueString;
		};
		std::size_t id() const
		{
			return id(*this);
		};
		const std::list<struct UsageData*>& usageData() const
		{
			return this->m_usgeData;
		}

	private:
		std::string m_idString;
		std::string m_defaultString;
		std::string m_valueString;
		std::list<struct UsageData*> m_usageData;
};

}

#endif
