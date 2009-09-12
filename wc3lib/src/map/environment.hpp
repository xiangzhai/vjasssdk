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

namespace wc3lib
{

namespace map
{

class Environment
{
	public:
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
			int maxX //: width of the map + 1 = Mx
			int maxY; //: height of the map + 1 = My
			float centerOffsetX; //: center offeset of the map X
			float centerOffsetY; //: center offeset of the map Y
		};
};

}

}

#endif
