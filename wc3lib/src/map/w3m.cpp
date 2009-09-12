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

#include "w3m.hpp"

namespace wc3lib
{

namespace map
{

struct Header
{
	char fileId[4]; //: file ID (should be "HM3W")
	int unknown; //: unknown
	char *name; //: map name
	int flags; //: map flags (these are exactly the same as the ones in the W3I file)
	int maxPlayers; //: max number of players
};

//optional
struct Footer
{
	char signId[4]; //: footer sign ID (should be "NGIS" == 'sign' reversed)
	char authentification[256]; //: 256 data bytes for authentification. I don't know how they are used at the moment.
};

void W3m::read(class Mpq *mpq) throw (class Exception)
{
}

void W3m::read(std::fstream &fstream) throw (class Exception)
{
	struct Header header;
	fstream.read((char*)&header, sizeof(struct Header));
	this->m_name = header.name;
	this->m_flags = header.flags;
	this->m_maxPlayers = header.maxPlayers;
	int byteCount = 512 - fstream.gcount(); //followed by 00 bytes until the 512 bytes of the header are filled.
	char *bytes = new char[byteCount];
	fstream.read(bytes, byteCount);
	delete[] bytes;
	bytes = 0;

	//create mpq archive
}

}

}