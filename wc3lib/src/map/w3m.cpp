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

#include <boost/filesystem/fstream.hpp>

#include "w3m.hpp"
#include "shadow.hpp"
#include "triggers.hpp"
#include "mpq/mpq.hpp"
#include "mpq/mpqfile.hpp"
#include "blp/blp.hpp"
#include "../internationalisation.hpp"
#include "../utilities.hpp"

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

W3m::W3m() : m_environment(new Environment(this)), m_shadow(new Shadow(this)), m_triggers(new Triggers(this))
{
}

W3m::~W3m()
{
	delete this->m_environment;
	delete this->m_shadow;
	delete this->m_triggers;
}

std::streamsize W3m::read(class mpq::Mpq *mpq) throw (class Exception)
{
	return 0;
}

std::streamsize W3m::read(std::istream &istream) throw (class Exception)
{
	std::streamsize size = this->readHeader(istream);
/*
	class mpq::Mpq mpq;
	size += mpq.readMpq(istream); // starts reading after header's position
	const class mpq::MpqFile *mpqFile = const_cast<const class mpq::Mpq>(mpq).findFile(std::string(Environment::fileName));

	if (mpqFile == 0)
		throw Exception(_("W3m: Missing file \"war3map.w3e\"."));

	std::stringstream iostream;
	mpqFile->writeData(iostream);
	size += this->m_environment->read(iostream);
	iostream.flush();

	mpqFile = const_cast<const class mpq::Mpq*>(mpq)->findFile(std::string(Shadow::fileName));

	if (mpqFile == 0)
		throw Exception(_("W3m: Missing file \"war3map.shd\"."));

	mpqFile->writeData(iostream);
	size += this->m_shadow->read(iostream);
	iostream.flush();

	mpq.close();
*/
	size += this->readSignature(istream);

	return size;
}

std::streamsize W3m::read(std::istream &headerStream, const std::list<boost::filesystem::path> &paths) throw (class Exception)
{
	std::streamsize result = this->readHeader(headerStream);
	boost::filesystem::path path;

	if (findPath(paths, path, Shadow::fileName))
	{
		boost::filesystem::ifstream ifstream(path, std::ios_base::binary | std::ios_base::in);
		this->m_shadow->read(ifstream);
	}

	return result;
}

std::streamsize W3m::write(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize W3m::readHeader(std::istream &istream) throw (class Exception)
{
	std::streamsize size = 0;
	struct Header header;
	wc3lib::read(istream, header, size);
	this->m_name = header.name;
	this->m_flags = header.flags;
	this->m_maxPlayers = header.maxPlayers;
	std::size_t byteCount = 512 - istream.gcount(); // followed by 00 bytes until the 512 bytes of the header are filled.
	istream.ignore(byteCount);
	size += byteCount;

	return size;
}

std::streamsize W3m::readSignature(std::istream &istream) throw (class Exception)
{
	std::streamsize result = 0;
	this->m_hasSignature = false;

	if (!istream.eof())
	{
		char8 signId[4];
		istream.read(signId, sizeof(signId));
		result += istream.gcount();

		// footer is optional!
		if (memcmp(signId, "NGIS", sizeof(signId)) == 0)
		{
			istream.read(this->m_authentification, sizeof(this->m_authentification));
			result += istream.gcount();
			this->m_hasSignature = true;
		}
	}

	return result;
}

bool W3m::findPath(const std::list<boost::filesystem::path> paths, boost::filesystem::path &path, const std::string &fileName)
{
	BOOST_FOREACH(const boost::filesystem::path iteratorPath, paths)
	{
		if (iteratorPath.filename() == fileName)
		{
			path = iteratorPath;

			return true;
		}
	}

	return false;
}

}

}
