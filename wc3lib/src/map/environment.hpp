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

#ifndef WC3LIB_MAP_ENVIRONMENT_HPP
#define WC3LIB_MAP_ENVIRONMENT_HPP

#include <list>
#include <map>
#include <string>

#include "platform.hpp"
#include "../format.hpp"

namespace wc3lib
{

namespace map
{

class Environment : public Format<byte>
{
	public:
		static const int32 maxTilesets;

		enum MainTileset
		{
			Ashenvale,
			Barrens,
			Felwood,
			Dungeon,
			LordaeronFall,
			Underground,
			LordaeronSummer,
			Northrend,
			VillageFall,
			Village,
			LordaeronWinter,
			Dalaran,
			Cityscape,
			SunkenRuins,
			Icecrown,
			DalaranRuins,
			Outland,
			BlackCitadel
		};

		Environment(class W3m *w3m);

		std::streamsize read(std::basic_istream<byte> &istream) throw (class Exception);
		std::streamsize write(std::basic_ostream<byte> &ostream) const throw (class Exception);

		virtual int32 fileId() const;
		virtual const char* fileName() const;
		virtual int32 latestFileVersion() const;

		int32 mapWidth() const;
		int32 mapHeight() const;

		const class Tilepoint* tilepoint(const class Position &position) const;

		/**
		* @return Returns a newly allocated C string which you'll have to free.
		*/
		static char8* tilesetIdToCString(int32 tilesetId);
		static std::string tilesetIdToString(int32 tilesetId);

	protected:
		static enum MainTileset convertCharToMainTileset(char value) throw (class Exception);

		class W3m *m_w3m;
		int32 m_version;
		enum MainTileset m_mainTileset;
		bool m_customized;
		std::list<int32> m_groundTilesetsIds;
		std::list<int32> m_cliffTilesetsIds;
		int32 m_maxX;
		int32 m_maxY;
		float32 m_centerOffsetX;
		float32 m_centerOffsetY;
		std::map<class Position, class Tilepoint*> m_tilepoints;

};

inline int32 Environment::fileId() const
{
	return (int32)"W3E!";
}

inline const char* Environment::fileName() const
{
	return "war3map.w3e";
}

inline int32 Environment::latestFileVersion() const
{
	return 11;
}

inline int32 Environment::mapWidth() const
{
	return this->m_maxX - 1;
}

inline int32 Environment::mapHeight() const
{
	return this->m_maxY - 1;
}

inline const class Tilepoint* Environment::tilepoint(const class Position &position) const
{
	std::map<class Position, class Tilepoint*>::const_iterator iterator = this->m_tilepoints.find(position);

	if (iterator == this->m_tilepoints.end())
		return 0;

	return iterator->second;
}

inline char8* Environment::tilesetIdToCString(int32 tilesetId)
{
	char8 *output = new char[5];
	memcpy(reinterpret_cast<void*>(output), reinterpret_cast<const void*>(&tilesetId), 4);
	output[4] = '\0';

	return output;
}

inline std::string Environment::tilesetIdToString(int32 tilesetId)
{
	char8 *cString = Environment::tilesetIdToCString(tilesetId);
	std::string value(cString);
	delete[] cString;

	return value;
}


}

}

#endif
