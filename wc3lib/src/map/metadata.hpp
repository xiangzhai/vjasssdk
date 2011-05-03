/***************************************************************************
 *   Copyright (C) 2011 by Tamino Dauth                                    *
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

#ifndef WC3LIB_MAP_METADATA_HPP
#define WC3LIB_MAP_METADATA_HPP

#include <boost/bimap.hpp>
#include <boost/detail/scoped_enum_emulation.hpp>

#include "platform.hpp"
#include "object.hpp"

namespace wc3lib
{

namespace map
{

BOOST_SCOPED_ENUM_START(Slk)
{
	Profile,
	AbilityData
};
BOOST_SCOPED_ENUM_END

BOOST_SCOPED_ENUM_START(Section)
{
	Misc
};
BOOST_SCOPED_ENUM_END

BOOST_SCOPED_ENUM_START(Category)
{
	Text,
	Stats,
	Art,
	Tech,
	Data
};
BOOST_SCOPED_ENUM_END

BOOST_SCOPED_ENUM_START(ImportType)
{
	None,
	Image
};
BOOST_SCOPED_ENUM_END

/**
 * Provides one single meta data entry.
 * \sa Data
 */
class MetaData : public Object
{
	public:
		bool operator<(const MetaData &metaData) const
		{
			return index() < metaData.index();
		}
		
		int32 index() const
		{
			return m_index;
		}
		
		const string& displayName() const
		{
			return m_displayName;
		}
		
		const struct Value& minValue() const
		{
			return m_minValue;
		}
		
		const struct Value& maxValue() const
		{
			return m_maxValue;
		}
		
		enum Value::Type type() const
		{
			return m_type;
		}
	
	protected:
		string m_field;
		enum Slk::enum_t m_slk;
		enum Section::enum_t m_section;
		int32 m_index;
		string m_displayName;
		enum Value::Type m_type;
		enum ImportType::enum_t m_importType;
		bool m_stringExt;
		bool m_caseSensitive;
		bool m_canBeEmpty;
		struct Value m_minValue;
		struct Value m_maxValue;
		bool m_forceNonNegative;
		int32 m_version;
		bool m_inBeta;
		int32 m_changeFlags;
		enum Category::enum_t m_category;
};

/**
 * Meta data entry from file "UI/MiscMetaData.slk".
 */
class MiscMetaData : public MetaData
{
	protected:
		bool m_sort;
		enum Value::Type m_type;
		enum ImportType::enum_t m_importType;
		bool m_stringExt;
		bool m_caseSensitive;
		bool m_canBeEmpty;
		struct Value m_minValue;
		struct Value m_maxValue;
		bool m_forceNonNegative;
		int32 m_version;
		bool m_inBeta;
		int32 m_changeFlags;
		enum Category::enum_t m_category;
};

/**
 * Meta data entry from file "UI/AbilityMetaData.slk".
 */
class AbilityMetaData : public MetaData
{
	protected:
		string m_field;
		enum Slk::enum_t m_slk;
		int32 m_index;
		bool m_repeat;
		enum Category::enum_t m_category;
		string m_displayName;
		string m_sort;
		enum Value::Type m_type;
		string m_changeFlags;
		enum ImportType::enum_t m_importType;
		bool m_stringExt;
		bool m_caseSensitive;
		bool m_canBeEmpty;
		struct Value m_minValue;
		struct Value m_maxValue;
		bool m_forceNonNegative;
		bool m_useUnit;
		bool m_useHero;
		bool m_useItem;
		bool m_useCreep;
		bool m_useSpecific;
		bool m_notSpecific;
		int32 m_version;
		enum Section::enum_t m_section;
};

class Data : public Object, public boost::bimap<class MetaData*, class Value> // one pointer per column entry
{
};

}

}

#endif
