/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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

#ifndef WC3LIB_MAP_STRING_H
#define WC3LIB_MAP_STRING_H

#include <string>
#include <functional>
#include <sstream>
#include <list>

#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>

namespace wc3lib
{

namespace map
{

class String
{
	public:
		typedef uint64_t IdType;

		struct UsageData
		{
			boost::filesystem::path &filePath;
			std::size_t line;
		};

		struct IdComparator : public std::binary_function<class String*, class String*, bool>
		{
			bool operator()(const class String *first, const class String *second) const
			{
				return first->id() > second->id();
			};
		};

		static IdType id(const class String &string)
		{
			std::stringstream sstream(string.m_idString.substr(7));
			IdType result;
			sstream >> result;

			return result;
		}

		String(const std::string &idString, const std::string &defaultString, const std::string &valueString) : m_idString(idString), m_defaultString(defaultString), m_valueString(valueString), m_usageData(std::list<struct UsageData*>())
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

		const std::string& idString() const
		{
			return this->m_idString;
		};
		const std::string& defaultString() const
		{
			return this->m_defaultString;
		};
		const std::string& valueString() const
		{
			return this->m_valueString;
		};
		IdType id() const
		{
			return String::id(*this);
		};
		const std::list<struct UsageData*>& usageData() const
		{
			return this->m_usageData;
		}

	protected:
		std::string m_idString;
		std::string m_defaultString;
		std::string m_valueString;
		std::list<struct UsageData*> m_usageData;
};

}

}

#endif
