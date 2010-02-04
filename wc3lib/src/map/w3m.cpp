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

#include <cstring>
#include <sstream>

#include "w3m.hpp"
#include "shadow.hpp"
//#include "mpq/mpq.hpp"
//#include "mpq/mpqfile.hpp"
#include "blp/blp.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace map
{

struct Header
{
	char8 fileId[4]; //: file ID (should be "HM3W")
	int32 unknown; //: unknown
	char8 *name; //: map name
	int32 flags; //: map flags (these are exactly the same as the ones in the W3I file)
	int32 maxPlayers; //: max number of players
};

W3m::W3m() : m_environment(new Environment(this)), m_shadow(new Shadow(this))
{
}

W3m::~W3m()
{
	delete this->m_environment;
	delete this->m_shadow;
}

std::streamsize W3m::read(class mpq::Mpq *mpq) throw (class Exception)
{
	return 0;
}

std::streamsize W3m::read(std::istream &istream) throw (class Exception)
{/*
	std::streamsize bytes = 0;
	struct Header header;
	istream.read(reinterpret_cast<char*>(&header), sizeof(header));
	bytes += istream.gcount();
	this->m_name = header.name;
	this->m_flags = header.flags;
	this->m_maxPlayers = header.maxPlayers;
	std::size_t byteCount = 512 - istream.gcount(); //followed by 00 bytes until the 512 bytes of the header are filled.
	istream.ignore(byteCount);
	bytes += byteCount;
	
	class mpq::Mpq *mpq = new mpq::Mpq;
	bytes += mpq->readMpq(istream, mpq::Mpq::Read); // starts reading after header's position
	const class mpq::MpqFile *mpqFile = mpq->findFile("(listfile)");
	
	if (mpqFile == 0)
		std::cerr << _("W3m: Warning, file \"(listfile)\" was not found.") << std::endl;
	
	mpqFile = mpq->findFile("(signature)");
	
	if (mpqFile == 0)
		std::cerr << _("W3m: Warning, file \"(signature)\" was not found.") << std::endl;
	
	mpqFile = mpq->findFile("war3map.w3e");
	
	if (mpqFile == 0)
		throw Exception(_("W3m: Missing file \"war3map.w3e\"."));
	
	std::stringstream iostream;
	mpqFile->write(iostream);
	bytes += this->m_environment->read(iostream);
	iostream.flush();
	
	mpqFile = mpq->findFile("war3map.shd");
	
	if (mpqFile == 0)
		throw Exception(_("W3m: Missing file \"war3map.shd\"."));

	mpqFile->write(iostream);
	bytes += this->m_shadow->read(iostream);
	iostream.flush();
	
	delete mpq;
	mpq = 0;
	
	this->m_hasSignature = false;
	
	if (!iostream.eof())
	{
		char8 signId[4];
		istream.read(signId, sizeof(signId));
		bytes += istream.gcount();
		
		// footer is optional!
		if (memcmp(signId, "NGIS", sizeof(signId)) == 0)
		{
			istream.read(this->m_authentification, sizeof(this->m_authentification));
			bytes += istream.gcount();
			this->m_hasSignature = true;
		}
	}
	
	return bytes;
	*/
	
	return 0;
}

std::streamsize W3m::write(std::ostream &ostream) throw (class Exception)
{
	return 0;
}

}

}