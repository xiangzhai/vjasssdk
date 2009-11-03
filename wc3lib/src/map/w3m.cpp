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

#include "w3m.hpp"
#include "mpq/mpq.hpp"
#include "mpq/mpqfile.hpp"
#include "blp/blp.hpp"
#include "../internationalisation.hpp"

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

std::streamsize W3m::read(class mpq::Mpq *mpq) throw (class Exception)
{
	return 0;
}

std::streamsize W3m::read(std::fstream &fstream) throw (class Exception)
{
	std::streamsize bytes = 0;
	struct Header header;
	fstream.read(reinterpret_cast<char*>(&header), sizeof(header));
	bytes += fstream.gcount();
	this->m_name = header.name;
	this->m_flags = header.flags;
	this->m_maxPlayers = header.maxPlayers;
	int byteCount = 512 - fstream.gcount(); //followed by 00 bytes until the 512 bytes of the header are filled.
	char *freeBytes = new char[byteCount];
	fstream.read(freeBytes, byteCount);
	bytes += fstream.gcount();
	delete[] freeBytes;
	freeBytes = 0;
	
	class mpq::Mpq *mpq = new mpq::Mpq;
	bytes += mpq->read(fstream, mpq::Mpq::Read); // starts reading after header's position
	const class mpq::MpqFile *mpqFile = mpq->findFile("(listfile)");
	
	if (mpqFile == 0)
	{
		std::cerr << _("W3m: Warning, file \"(listfile)\" was not found.") << std::endl;
	}
	
	mpqFile = mpq->findFile("(signature)");
	
	for (std::list<class mpq::MpqFile*>::const_iterator iterator = mpq->files().begin(); iterator != mpq->files().end(); ++iterator)
	{
		if ((*iterator)->path() == "(listfile)")
		{
		}
		//else if (*iterator->path() == "(signature)")
		//else if (*iterator->path() == "(signature)")
	}
	
	delete mpq;
	mpq = 0;
	
	//struct Footer footer;
	//fstream.read(reinterpret_cast<char*>(&footer), sizeof(footer));
	/*
	if ()
	{
		bytes += fstream.gcount();
	}
	*/
	return 0;
}

}

}