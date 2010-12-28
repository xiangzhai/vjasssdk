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

#ifndef WC3LIB_EDITOR_MPQPRIORITYLIST_HPP
#define WC3LIB_EDITOR_MPQPRIORITYLIST_HPP

#include <list>
#include <algorithm>

#include <boost/operators.hpp>
#include <boost/foreach.hpp>

#include <kurl.h>

#include "../mpq.hpp"

namespace wc3lib
{

namespace editor
{

/**
* @brief Entry of an MPQ priority list.
* An entry can either be an MPQ archive or a URL of a directory.
* Each entry has its own priority. Entries with higher priority will be returned more likely than those with less priority.
*/
class MpqPriorityListEntry //: public boost::operators<MpqPriorityListEntry>
{
	public:
		typedef MpqPriorityListEntry self;
		typedef std::size_t Priority;

		/**
		* If url is the URL of an archive it will be set with "mpq:" protocol.
		* Otherwise it has to be a directory.
		*/
		MpqPriorityListEntry(const KUrl &url, Priority priority);

		Priority priority() const;
		bool isDirectory() const;
		bool isValid() const;
		const KUrl& url() const;

		bool operator<(const self& other) const;
		bool operator==(const self& other) const;
	protected:
		Priority m_priority;
		bool m_isDirectory;
		bool m_isValid;
		KUrl m_url;
};

inline MpqPriorityListEntry::Priority MpqPriorityListEntry::priority() const
{
	return this->m_priority;
}

inline bool MpqPriorityListEntry::isDirectory() const
{
	return this->m_isDirectory;
}

inline bool MpqPriorityListEntry::isValid() const
{
	return this->m_isValid;
}

inline const KUrl& MpqPriorityListEntry::url() const
{
	return this->m_url;
}

inline bool MpqPriorityListEntry::operator<(const self& other) const
{
	return this->priority() < other.priority();
}

inline bool MpqPriorityListEntry::operator==(const self& other) const
{
	return this->priority() == other.priority();
}

class MpqPriorityList : public std::list<MpqPriorityListEntry*>
{
	public:
		typedef std::list<MpqPriorityListEntry*> base;
		typedef MpqPriorityList self;

		void setLocale(mpq::MpqFile::Locale locale);
		mpq::MpqFile::Locale locale() const;

		/**
		* @return Returns entries list sorted by the entries priorities.
		*/
		base sortedValids() const;

		/**
		* Several URLs should never be the same.
		* @return Returns true if the URL has been added to the list (this doesn't happen if there already is an entry with the given URL).
		*/
		bool addEntry(const KUrl &url, MpqPriorityListEntry::Priority priority = 0);
		/**
		* Removes an entry by its corresponding URL.
		* @return Returns true if URL corresponds to some entry and that entry has been removed properly.
		*/
		bool removeEntry(const KUrl &url);

		/**
		* Checks the whole priority list in the specified order for the given path.
		*/
		KUrl findFile(const KUrl &url) const;
		/**
		* @copydoc MpqPriorityList::findFile
		* @note Does not use the list's internal locale!
		*/
		KUrl findFile(const KUrl &url, mpq::MpqFile::Locale locale) const;

	protected:
		mpq::MpqFile::Locale m_locale;
};

inline void MpqPriorityList::setLocale(mpq::MpqFile::Locale locale)
{
	this->m_locale = locale;
}

inline mpq::MpqFile::Locale MpqPriorityList::locale() const
{
	return this->m_locale;
}

inline MpqPriorityList::base MpqPriorityList::sortedValids() const
{
	MpqPriorityList::base result;

	BOOST_FOREACH(MpqPriorityListEntry *entry, *this)
	{
		if (entry->isValid())
			result.push_back(entry);
	}

	result.sort();

	return result;
}

}

}

#endif
