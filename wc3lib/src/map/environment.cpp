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

#include <boost/format.hpp>

#include "environment.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{
	
namespace map
{
	
struct Header
{
	char fileId[4]; //: file ID = "W3E!"
	int formatVersion; //: w3e format version [0B 00 00 00]h = version 11
	char mainTileset; //: main tileset [TS]
	int customTilesetsFlag; //: custom tilesets flag (1 = using cutom, 0 = not using custom tilesets)
	int groundTilesetsNumber; //: number a of ground tilesets used (Little Endian) (note: should not be greater than 16 because of tilesets indexing in tilepoints definition)
	char groundTilesetsIds[4][]; //[4][a]: ground tilesets IDs (tilesets table)
	int cliffTilesetsNumber; //: number b of cliff tilesets used (Little Endian) (note: should not be greater than 16 because of tilesets indexing in tilepoints definition)
	char cliffTilesetsIds[4][]; //[4][b]: cliff tilesets IDs (cliff tilesets table)
	int maxX; //: width of the map + 1 = Mx
	int maxY; //: height of the map + 1 = My
	float centerOffsetX; //: center offeset of the map X
	float centerOffsetY; //: center offeset of the map Y
};
	
std::streamsize Environment::read(std::istream &istream) throw (class Exception)
{
	char fileId[4];
	istream.read(fileId, sizeof(fileId));
	std::streamsize bytes = istream.gcount();
	
	if (memcmp(fileId, "W3E!", sizeof(fileId)) != 0)
		throw Exception(boost::str(boost::format(_("Environment: Expected \"W3E!\" identifier. Got unknown \"%1%\".")) % fileId));
	
	istream.read(reinterpret_cast<char*>(&this->m_version), sizeof(this->m_version));
	bytes += istream.gcount();
	
	if (this->m_version != Environment::currentVersion)
		std::cout << boost::str(boost::format(_("Environment: Expected version %1%. Got unknown %2%.")) % Environment::currentVersion % this->m_version) << std::endl;
	
	char mainTileset;
	istream.get(mainTileset);
	this->m_mainTileset = Environment::convertCharToMainTileset(mainTileset);
	
	/// @todo Continue development.
	
}

std::streamsize Environment::write(std::ostream &ostream) throw (class Exception)
{
}

enum MainTileset Environment::convertCharToMainTileset(char value)
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
	
	std::cout << boost::str(boost::format(_("Environment: Character \'%1%\' does not refer to any main tileset.")) % value) << std::endl;
	
	return Environment::Ashenvale;
}
	
}

}
