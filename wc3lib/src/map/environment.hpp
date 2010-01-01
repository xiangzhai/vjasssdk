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

#ifndef WC3LIB_MAP_ENVIRONMENT_HPP
#define WC3LIB_MAP_ENVIRONMENT_HPP

#include <istream>
#include <ostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace map
{

class W3m;
class Tilepoint;

class Environment
{
	public:
		static const int32 currentVersion = 11;
		static const int32 maxTilesets = 16;
	
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
		
		std::streamsize read(std::istream &istream) throw (class Exception);
		std::streamsize write(std::ostream &ostream) throw (class Exception);
		
		int32 mapWidth() const;
		int32 mapHeigth() const;
		
	protected:
		static enum MainTileset convertCharToMainTileset(char value);
		
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
		std::list<class Tilepoint*> m_tilepoints;
		
};

inline int32 Environment::mapWidth() const
{
	return this->m_maxX - 1;
}

inline int32 Environment::mapHeigth() const
{
	return this->m_maxY - 1;
}

}

}

#endif
