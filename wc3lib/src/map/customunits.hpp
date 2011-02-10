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

#ifndef WC3LIB_MAP_CUSTOMUNITS_HPP
#define WC3LIB_MAP_CUSTOMUNITS_HPP

#include "../format.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace map
{

/**
* Warcraft 3 only allows you to customize unit data.
* Later, in Frozen Throne Blizzard introduced general object data modification (\ref CustomObjects).
*/
class CustomUnits : public Format<byte>
{
	public:
		class Unit : public Format<byte>
		{
			public:
				Unit();
				~Unit();

				std::streamsize read(std::basic_istream<byte> &istream) throw (class Exception);
				std::streamsize write(std::basic_ostream<byte> &ostream) const throw (class Exception);

				bool isOriginal() { return m_customId == 0; };

			protected:
				virtual Modification* createModification() const;

				id m_originalId;
				id m_customId;
				std::list<class Modification*> m_modifications;
		};

		class Modification : public Format<byte>
		{
			public:
				enum Type
				{
					Integer = 0,
					Real = 1,
					Unreal = 2,
					String = 3,
					Boolean = 4,
					Character = 5,
					UnitList = 6,
					ItemList = 7,
					RegenerationType = 8,
					AttackType = 9,
					WeaponType = 10,
					TargetType = 11,
					MoveType = 12,
					DefenseType = 13,
					PathingTexture = 14,
					UpgradeList = 15,
					StringList = 16,
					AbilityList = 17,
					HeroAbilityList = 18,
					MissileArt = 19,
					AttributeType = 20,
					AttackBits = 21
				};

				Modification();
				~Modification();

				std::streamsize read(std::basic_istream<byte> &istream) throw (class Exception);
				std::streamsize write(std::basic_ostream<byte> &ostream) const throw (class Exception);

			protected:
				std::streamsize readData(std::basic_istream<byte> &istream) throw (class Exception);
				std::streamsize writeData(std::basic_ostream<byte> &ostream) const throw (class Exception);

				id m_id; // from "Units\UnitMetaData.slk"
				enum Type m_type;
				union
				{
					int32 Integer;
					float32 Real;
					string String;
				} m_value;
		};

		CustomUnits();
		~CustomUnits();

		std::streamsize read(std::basic_istream<byte> &istream) throw (class Exception);
		std::streamsize write(std::basic_ostream<byte> &ostream) const throw (class Exception);

		virtual const char* fileName() const;
		virtual int32 latestFileVersion() const;

	protected:
		virtual Unit* createUnit() const;

		int32 m_version;
		std::list<class Unit*> m_originalTable;
		std::list<class Unit*> m_customTable;
};

inline const char* CustomUnits::fileName() const
{
	return "war3map.w3u";
}

inline int32 CustomUnits::latestFileVersion() const
{
	return 1;
}

}

}

#endif
