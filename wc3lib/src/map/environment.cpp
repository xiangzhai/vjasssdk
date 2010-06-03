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

#include <iostream>
#include <cstring>

#include <boost/format.hpp>

#include "environment.hpp"
#include "tilepoint.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{
	
namespace map
{
	
Environment::Environment(class W3m *w3m) : m_w3m(w3m)
{
}
	
std::streamsize Environment::read(std::istream &istream) throw (class Exception)
{
	char8 fileId[4];
	istream.read(fileId, sizeof(fileId));
	std::streamsize bytes = istream.gcount();
	
	if (memcmp(fileId, "W3E!", sizeof(fileId)) != 0)
		throw Exception(boost::str(boost::format(_("Environment: Expected \"W3E!\" identifier. Got unknown \"%1%\".")) % fileId));
	
	istream.read(reinterpret_cast<char*>(&this->m_version), sizeof(this->m_version));
	bytes += istream.gcount();
	
	if (this->m_version != Environment::currentVersion)
		std::cout << boost::str(boost::format(_("Environment: Expected version %1%. Got unknown %2%.")) % Environment::currentVersion % this->m_version) << std::endl;
	
	char8 mainTileset;
	istream.get(mainTileset);
	bytes += istream.gcount();
	this->m_mainTileset = Environment::convertCharToMainTileset(mainTileset);
	
	int32 customTilesetsFlag;
	istream.read(reinterpret_cast<char*>(&customTilesetsFlag), sizeof (customTilesetsFlag));
	bytes += istream.gcount();
	this->m_customized = customTilesetsFlag;
	
	int32 groundTilesetsNumber;
	istream.read(reinterpret_cast<char*>(&groundTilesetsNumber), sizeof(groundTilesetsNumber));
	bytes += istream.gcount();
	
	if (groundTilesetsNumber > Environment::maxTilesets)
		throw Exception(boost::str(boost::format(_("Environment: Ground tilesets are limited to %1% however %2% are used.")) % Environment::maxTilesets % groundTilesetsNumber));
	
	for (std::size_t i = 0; i < groundTilesetsNumber; ++i)
	{
		int32 groundTilesetId;
		istream.read(reinterpret_cast<char*>(&groundTilesetId), sizeof(groundTilesetId));
		bytes += istream.gcount();
		this->m_groundTilesetsIds.push_back(groundTilesetId);
	}
	
	int32 cliffTilesetsNumber;
	istream.read(reinterpret_cast<char*>(&cliffTilesetsNumber), sizeof(cliffTilesetsNumber));
	bytes += istream.gcount();
	
	if (cliffTilesetsNumber > Environment::maxTilesets)
		throw Exception(boost::str(boost::format(_("Environment: Cliff tilesets are limited to %1% however %2% are used.")) % Environment::maxTilesets % cliffTilesetsNumber));
	
	for (std::size_t i = 0; i < cliffTilesetsNumber; ++i)
	{
		int32 cliffTilesetId;
		istream.read(reinterpret_cast<char*>(&cliffTilesetId), sizeof(cliffTilesetId));
		bytes += istream.gcount();
		this->m_cliffTilesetsIds.push_back(cliffTilesetId);
	}
	
	istream.read(reinterpret_cast<char*>(&this->m_maxX), sizeof(this->m_maxX));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_maxY), sizeof(this->m_maxY));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_centerOffsetX), sizeof(this->m_centerOffsetX));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_centerOffsetY), sizeof(this->m_centerOffsetY));
	bytes += istream.gcount();
	
	// The first tilepoint defined in the file stands for the lower left corner of the map when looking from the top, then it goes line by line (horizontal).
	for (int32 y = 0; y < this->m_maxY; ++y)
	{
		for (int32 x = 0; x < this->m_maxX; ++x)
		{
			class Tilepoint *tilepoint = new Tilepoint(this);
			tilepoint->m_position = std::make_pair(x, y);
			bytes += tilepoint->read(istream);
			this->m_tilepoints[tilepoint->m_position] = tilepoint;
		}
	}
	
	return bytes;
}

std::streamsize Environment::write(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

enum Environment::MainTileset Environment::convertCharToMainTileset(char value) throw (class Exception)
{
	switch (value)
	{
		case 'A':
			return Environment::Ashenvale;

		case 'B':
			return Environment::Barrens;
			
		case 'C':
			return Environment::Felwood;

		case 'D':
			return Environment::Dungeon;
			
		case 'F':
			return Environment::LordaeronFall;
			
		case 'G':
			return Environment::Underground;
			
		case 'L':
			return Environment::LordaeronSummer;

		case 'N':
			return Environment::Northrend;
			
		case 'Q':
			return Environment::VillageFall;

		case 'V':
			return Environment::Village;
			
		case 'W':
			return Environment::LordaeronWinter;

		case 'X':
			return Environment::Dalaran;

		case 'Y':
			return Environment::Cityscape;

		case 'Z':
			return Environment::SunkenRuins;

		case 'I':
			return Environment::Icecrown;

		case 'J':
			return Environment::DalaranRuins;

		case 'O':
			return Environment::Outland;
			
		case 'K':
			return Environment::BlackCitadel;
	}
	
	throw Exception(boost::str(boost::format(_("Environment: Character \'%1%\' does not refer to any main tileset.")) % value));
	
	//return Environment::Ashenvale;
}
	
}

}
