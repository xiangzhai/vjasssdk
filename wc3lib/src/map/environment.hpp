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

#include "../exception.hpp"

namespace wc3lib
{

namespace map
{
	
class Tilepoint;

class Environment
{
	public:
		static const int currentVersion = 11;
		static const int maxTilesets = 16;
	
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
		
		std::streamsize read(std::istream &istream) throw (class Exception);
		std::streamsize write(std::ostream &ostream) throw (class Exception);
		
	protected:
		static enum MainTileset convertCharToMainTileset(char value);
		
		int m_version;
		enum MainTileset m_mainTileset;
		bool m_customized;
		std::list<std::string> m_groundTilesets;
		std::list<std::string> m_cliffTilesets;
		int m_maxX;
		int m_maxY;
		float m_centerOffsetX;
		float m_centerOffsetY;
		std::list<class Tilepoint> m_tilepoints;
		
};

}

}

#endif
