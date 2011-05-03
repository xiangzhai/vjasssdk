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

#ifndef WC3LIB_MAP_PLATFORM_HPP
#define WC3LIB_MAP_PLATFORM_HPP

#include <string>
#include <sstream>
#include <list>

#include <boost/cstdint.hpp>
#include <boost/array.hpp>
#include <boost/variant.hpp>

#include "../core.hpp"

namespace wc3lib
{

namespace map
{

typedef char byte;

/**
* Integers
* Intergers are stored using 4 bytes in "Little Endian" order. It means that the first byte read is the lowest byte.
* They are just like the C++ "int" (signed) type. In some other documentation of this kind you may see them named "long".
* Size: 4 bytes
* Example: 1234 decimal = [00 00 04 D2]h will be stored in this order: [D2 04 00 00]h
*/
typedef uint32_t int32;

/// Versions (file versions) are usually being saved as int32 values.
//typedef int32 version;
typedef uint32_t id;

/**
* Short Integers
* Short Integers are stored using 2 bytes in "Little Endian" order.
* They are close to the C++ signed short but their range is from -16384 to 16383. It means the 2 highest bit are free of use for a flag for example.
* Size: 2 bytes
*/
typedef int16_t short16;

/**
* Floats
* Floats are using the standard IEEE 32bit float format. They are stored using 4 bytes and the "Little Endian" order.
* They are just like the C++ "float" type.
* Size: 4 bytes
* Example: 7654.32 decimal, this number can't be stored using this format so the system will take the closest value that can be represented using binary digits. The closest one is: 7654.319824 decimal = [45 EF 32 8F]h and will be stored as [8F 32 EF 45]h
*/
typedef float float32;

/**
* Chars and Array of Chars
* They are just stored like standard chars (1 char = 1 byte) and array of chars (no null terminating char needed).
* Size (chars): 1 byte
* Size (array of chars): usually 4 bytes
*/
typedef byte char8;
typedef boost::array<char8, 4> char8Array;

/**
* Trigger Strings and Strings
*
* Strings are just arrays of chars terminated with a null char (C++ '\0'). However Blizzard sometimes use special control codes to change the displayed color for the string. These codes are like "|cAABBGGRR" where "AA", "RR", "GG" and "BB" are hexadecimal values (using 2 digits each) for the alpha, red, the green and the blue values. If a string starts with "TRIGSTR_" (case sensitive), it's considered as a trigger string. A trigger string is kept in memory as is ("TRIGSTR_***") and is only changed when Warcraft 3 needs to display it. Instead of just writing "TRIGSTR_000" on the user screen, War3 will look in its trigger string table created when the map was loaded and display the corresponding trigger string instead. Trigger strings only work for files inside a w3m (Jass, w3i, ...) except for the WTS which is used to define the trigger string table itself. If the number following "TRIGSTR_" is negative the trigger string will refer to a null (empty) string, if "TRIGSTR_" is followed by text, it'll be considered as trigger string #0 ( = "TRIGSTR_000").
* "TRIGSTR_7", "TRIGSTR_07", "TRIGSTR_007" and "TRIGSTR_7abc" are all representing trigger string #7. "TRIGSTR_ab7", "TRIGSTR_abc" and "TRIGSTR_" refer to trigger string #0. "TRIGSTR_-7" is negative and will not refer to a trigger string; it'll be displayed as "". By convention, "TRIGSTR_" is followed by 3 digits and the null char that ends the string.
* Example 1: your got the string "blah |c00FF8000blah", War3 will display "blah blah" but the second "bla h" will be orange (red=FF + green=80 + blue=00 ==> orange)
 The alpha value doesn't really work btw.
Example 2: you got "TRIGSTR_025" and trigger string 25 is defined (in the .wts file) as "blah|c00FF8000blah", it'll display the same result as the previous example.
 Size (string): vary. String length + 1 (null terminating char)
Size (trigger string): 12 bytes

For its strings Warcraft uses a unicode format called UTF-8. They do this because the files need to be localized into many different languages. This format uses one byte for the most common characters which is equal to the character's ASCII value. For example A = 65 or 0x41. For the more unusual characters it can take from 2 to 6 bytes per character. For example the German letter Ä would be represented by 195 and 164 or 0xC3A4. The higher the first byte is, the more bytes are required to represent the character. Simple modulo calculations are enough to convert UTF to common unicode (UCS) and back. To convert UTF to UCS use this pattern:

If FirstByte <= 191 return FirstByte
 If 192 <= FirstByte <= 223 return (FirstByte - 192) * 64 + (SecondByte - 128)
 If 224 <= FirstByte <= 239 return (FirstByte - 224) * 4096 + (SecondByte - 128) * 64 + (ThirdByte - 128)
 If 240 <= FirstByte <= 247 return (FirstByte - 240) * 262144 + (SecondByte - 128) * 4096 + (ThirdByte - 128) * 64 + (FourthByte - 128)
 If 248 <= FirstByte <= 251 return (FirstByte - 248) * 16777216 + (SecondByte - 128) * 262144 + (ThridByte - 128) * 4096 + (FourthByte - 128) * 64 + (FifthByte - 128)
 If 252 <= FirstByte return (FirstByte - 252) * 1073741824 + (SecondByte - 128) * 16777216 + (ThirdByte - 128) * 262144 + (FourthByte - 128) * 4096 + (FifthByte - 128) * 64 + (SixthByte - 128)
To convert UCS back to UTF use this:
If ASCII <= 127 FirstByte = ASCII
 If 128 <= ASCII <= 2047 FirstByte = 192 + (ASCII \ 64) SecondByte = 128 + (ASCII Mod 64)
 If 2048 <= ASCII <= 65535 FirstByte = 224 + (ASCII \ 4096) SecondByte = 128 + ((ASCII \ 64) Mod 64) ThirdByte = 128 + (ASCII Mod 64)
 If 65536 <= ASCII <= 2097151 FirstByte = 240 + (ASCII \ 262144) SecondByte = 128 + ((ASCII \ 4096) Mod 64) ThirdByte = 128 + ((ASCII \ 64) Mod 64) FourthByte = 128 + (ASCII Mod 64)
 If 2097152 <= ASCII <= 67108863 FirstByte = 248 + (ASCII \ 16777216) SecondByte = 128 + ((ASCII \ 262144) Mod 64) ThirdByte = 128 + ((ASCII \ 4096) Mod 64) FourthByte = 128 + ((ASCII \ 64) Mod 64) FifthByte = 128 + (ASCII Mod 64)
 If 67108864 <= ASCII <= 2147483647 FirstByte = 252 + (ASCII \ 1073741824) SecondByte = 128 + ((ASCII \ 16777216) Mod 64) ThirdByte = 128 + ((ASCII \ 262144) Mod 64) FourthByte = 128 + ((ASCII \ 4096) Mod 64) FifthByte = 128 + ((ASCII \ 64) Mod 64) SixthByte = 128 + (ASCII Mod 64))
The conversion will only be needed if you want to display text in your application or write user input to files. For all other purposes you can internally treat the UTF-Strings just like ordinary strings.
*/
typedef std::basic_string<char8> string;
typedef std::basic_stringstream<char8> stringstream;
typedef std::basic_ostringstream<char8> ostringstream;
typedef std::basic_istringstream<char8> istringstream;

class Format : public wc3lib::Format<byte>
{
	public:
		/// Usually only some formats have a customized version (e. g. map shadow - "war3map.shd")
		virtual int32 version() const { return 0; }
};

typedef std::basic_istream<byte> istream;
typedef std::basic_ostream<byte> ostream;

/**
* Flags
*
* Flags are boolean values (true or false, 1 or 0). They can be stored using 4 bytes. Each bit is a flag (4 bytes = 32 bit = 32 flags). Blizzard uses integers to store its flags.
* Size: usually 4 bytes
*/
typedef int32_t flag;

/**
* Custom Types
*
* Sometimes, an integer and one or several flags can share bytes. This is the case in the W3E file format: the water level and 2 flags are using the same group of 4 bytes. How? the 2 highest bit are used for the flags, the rest is reserved for the water level (the value range is just smaller). Sometimes a byte can contain two or more different data.
*
*/

typedef std::list<string> List;
typedef boost::variant<
int32,
float32,
string,
bool,
char8,
List
> ValueBase;

/**
 * Template type for variant types for the same value.
 * Required for custom objects.
 * Each modification of a custom object requires a speific type which indicates value's size as well.
 * \sa CustomUnits::Modification, CustomObjects::Modification
 */
struct Value : public ValueBase
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
		
		Value() : ValueBase(int32(0)), m_type(Integer)
		{
		}
		
		Value(Type type) : ValueBase(), m_type(type)
		{
		}
		
		Value(int32 value) : ValueBase(value), m_type(Integer)
		{
		}
		
		Value(float32 value) : ValueBase(value), m_type(Real)
		{
		}
		
		Value(string value) : ValueBase(value), m_type(String)
		{
		}
		
		Value(bool value) : ValueBase(value), m_type(Boolean)
		{
		}
		
		Value(char8 value) : ValueBase(value), m_type(Character)
		{
		}
		
		Value(List value) : ValueBase(value), m_type(StringList)
		{
		}
		
		enum Type type() const
		{
			return m_type;
		}
		
		int32& toInteger() throw (class boost::bad_get)
		{
			if (type() != Integer)
				throw boost::bad_get();
			
			return boost::get<int32&>(*this);
		}
		
		int32 toInteger() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toInteger();
		}
		
		float32& toReal() throw (class boost::bad_get)
		{
			if (type() != Real)
				throw boost::bad_get();
			
			return boost::get<float32&>(*this);
		}
		
		float32 toReal() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toReal();
		}
		
		float32& toUnreal() throw (class boost::bad_get)
		{
			if (type() != Real)
				throw boost::bad_get();
			
			return boost::get<float32&>(*this);
		}
		
		float32 toUnreal() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toUnreal();
		}
		
		string& toString() throw (class boost::bad_get)
		{
			if (type() != String)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toString() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toString();
		}
		
		bool& toBoolean() throw (class boost::bad_get)
		{
			if (type() != Boolean)
				throw boost::bad_get();
			
			return boost::get<bool&>(*this);
		}
		
		bool toBoolean() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toBoolean();
		}
		
		char8& toCharacter() throw (class boost::bad_get)
		{
			if (type() != Character)
				throw boost::bad_get();
			
			return boost::get<char8&>(*this);
		}
		
		char8 toCharacter() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toCharacter();
		}
		
		List& toList() throw (class boost::bad_get)
		{
			if (type() != Character)
				throw boost::bad_get();
			
			return boost::get<List&>(*this);
		}
		
		const List& toList() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toList();
		}
		
		List& toUnitList() throw (class boost::bad_get)
		{
			if (type() != Character)
				throw boost::bad_get();
			
			return boost::get<List&>(*this);
		}
		
		const List& toUnitList() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toUnitList();
		}
		
		List& toItemList() throw (class boost::bad_get)
		{
			if (type() != Character)
				throw boost::bad_get();
			
			return boost::get<List&>(*this);
		}
		
		const List& toItemList() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toItemList();
		}
		
		
		string& toRegenerationType() throw (class boost::bad_get)
		{
			if (type() != Character)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toRegenerationType() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toRegenerationType();
		}
		
		string& toAttackType() throw (class boost::bad_get)
		{
			if (type() != Character)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toAttackType() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toAttackType();
		}
		
		string& toWeaponType() throw (class boost::bad_get)
		{
			if (type() != Character)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toWeaponType() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toWeaponType();
		}
		
		string& toTargetType() throw (class boost::bad_get)
		{
			if (type() != Character)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toTargetType() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toTargetType();
		}
		
		string& toMoveType() throw (class boost::bad_get)
		{
			if (type() != MoveType)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toMoveType() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toMoveType();
		}
		
		string& toDefenseType() throw (class boost::bad_get)
		{
			if (type() != DefenseType)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toDefenseType() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toDefenseType();
		}
		
		string& toPathingTexture() throw (class boost::bad_get)
		{
			if (type() != PathingTexture)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toPathingTexture() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toPathingTexture();
		}
		
		List& toUpgradeList() throw (class boost::bad_get)
		{
			if (type() != UpgradeList)
				throw boost::bad_get();
			
			return boost::get<List&>(*this);
		}
		
		const List& toUpgradeList() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toUpgradeList();
		}
		
		List& toStringList() throw (class boost::bad_get)
		{
			if (type() != StringList)
				throw boost::bad_get();
			
			return boost::get<List&>(*this);
		}
		
		const List& toStringList() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toStringList();
		}
		
		List& toAbilityList() throw (class boost::bad_get)
		{
			if (type() != AbilityList)
				throw boost::bad_get();
			
			return boost::get<List&>(*this);
		}
		
		const List& toAbilityList() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toAbilityList();
		}
		
		List& toHeroAbilityList() throw (class boost::bad_get)
		{
			if (type() != HeroAbilityList)
				throw boost::bad_get();
			
			return boost::get<List&>(*this);
		}
		
		const List& toHeroAbilityList() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toHeroAbilityList();
		}
		
		string& toMissileArt() throw (class boost::bad_get)
		{
			if (type() != MissileArt)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toMissileArt() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toMissileArt();
		}
		
		string& toAttributeType() throw (class boost::bad_get)
		{
			if (type() != AttributeType)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toAttributeType() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toAttributeType();
		}
		
		string& toAttackBits() throw (class boost::bad_get)
		{
			if (type() != AttackBits)
				throw boost::bad_get();
			
			return boost::get<string&>(*this);
		}
		
		const string& toAttackBits() const throw (class boost::bad_get)
		{
			return const_cast<class Value*>(this)->toAttackBits();
		}
		
		/// Less than operator required for ordered containers.
		bool operator<(const Value &value) const
		{
			return type() < value.type();
		}
		
		bool operator==(const Value &value) const
		{
			if (type() == value.type())
			{
				switch (type())
				{
					case Integer:
						return this->toInteger() == value.toInteger();
					
					case Real:
						return this->toReal() == value.toReal();
						
					case Unreal:
						return this->toUnreal() == value.toUnreal();
						
					case String:
						return this->toString() == value.toString();
						
					case Boolean:
						return this->toBoolean() == value.toBoolean();
						
					case Character:
						return this->toCharacter() == value.toCharacter();
						
					case UnitList:
						return this->toUnitList() == value.toUnitList();
						
					case ItemList:
						return this->toItemList() == value.toItemList();
						
					case RegenerationType:
						return this->toRegenerationType() == value.toRegenerationType();
						
					case AttackType:
						return this->toAttackType() == value.toAttackType();
						
					case WeaponType:
						return this->toWeaponType() == value.toWeaponType();
						
					case TargetType:
						return this->toTargetType() == value.toTargetType();
						
					case MoveType:
						return this->toMoveType() == value.toMoveType();
						
					case DefenseType:
						return this->toDefenseType() == value.toDefenseType();
						
					case PathingTexture:
						return this->toPathingTexture() == value.toPathingTexture();
						
					case UpgradeList:
						return this->toUpgradeList() == value.toUpgradeList();
						
					case StringList:
						return this->toStringList() == value.toStringList();
						
					case AbilityList:
						return this->toAbilityList() == value.toAbilityList();
						
					case HeroAbilityList:
						return this->toHeroAbilityList() == value.toHeroAbilityList();
						
					case MissileArt:
						return this->toMissileArt() == value.toMissileArt();
						
					case AttributeType:
						return this->toAttributeType() == value.toAttributeType();
						
					case AttackBits:
						return this->toAttackBits() == value.toAttackBits();
				}
			}
			
			return false;
		}
		
	protected:
		enum Type m_type;
};

/**
* Structures:
*
* Warcraft 3 also uses structured types of various size.
*/

struct Rgb
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct Rgba
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
};

class Position : public std::pair<int32, int32>
{
	public:
		Position(const std::pair<int32, int32> &pair) { this->first = pair.first; this->second = pair.second; };
		Position(int32 x, int32 y) { this->first = x; this->first = y; };
		int32 x() const { return this->first; };
		int32 y() const { return this->second; };
};

enum MapFlags
{
	HideMinimapInPreviewScreens = 0x0001,
	ModifyAllyPriorities = 0x0002,
	MeleeMap = 0x0004,
	PlayableMapSizeWasLargeAndHasNeverBeenReducedToMedium = 0x0008,
	MaskedAreaArePartiallyVisible = 0x0010,
	FixedPlayerSettingForCustomForces = 0x0020,
	UseCustomForces = 0x0040,
	UseCustomTechtree = 0x0080,
	UseCustomAbilities = 0x0100,
	UseCustomUpgrades = 0x0200,
	MapPropertiesMenuOpenedAtLeastOnceSinceMapCreation = 0x0400,
	ShowWaterWavesOnCliffShores = 0x0800,
	ShowWaterWavesOnRollingShores = 0x1000
};

}

}

#endif
