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

#ifndef WC3LIB_MAP_TILEPOINT_HPP
#define WC3LIB_MAP_TILEPOINT_HPP

namespace wc3lib
{

namespace map
{

class Tilepoint
{
	public:
		struct Block
		{
			short groundHeight;
			short waterLevel;
			short flags; //4 bit: flags*(see notes)
			short groundTextureType; //4bit: ground texture type (grass, dirt, rocks,...)
			char textureDetails; //1byte: texture details (of the tile of which the tilepoint is the bottom left corner) (rocks, holes, bones,...). It appears that only a part of this byte is used for details. It needs more investigations
			short cliffTextureType; //4bit: cliff texture type
			short layerHeight; //4bit: layer height
		};

};

}

}

#endif
