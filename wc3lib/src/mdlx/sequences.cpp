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

#include <iostream> //debug

#include <boost/foreach.hpp>

#include "sequences.hpp"
#include "sequence.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Sequences::Sequences(class Mdlx *mdlx) : MdxBlock("SEQS"), m_mdlx(mdlx)
{
}

Sequences::~Sequences()
{
	BOOST_FOREACH(class Sequence *sequence, this->m_sequences)
		delete sequence;
}

void Sequences::readMdl(std::istream &istream) throw (class Exception)
{
}

void Sequences::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	ostream
	<< "Sequences " << this->m_sequences.size() << " {\n";

	BOOST_FOREACH(class Sequence *sequence, this->m_sequences)
		sequence->writeMdl(ostream);

	ostream << "}\n";
}

std::streamsize Sequences::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes = 0;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();
	std::cout << "Sequence bytes: " << nbytes << std::endl;
	
	while (nbytes > 0)
	{
		class Sequence *sequence = new Sequence(this);
		long32 readBytes = sequence->readMdx(istream);
		
		if (readBytes == 0)
			throw Exception(_("Sequences: 0 byte sequence."));
		
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_sequences.push_back(sequence);
	}
	
	return bytes;
}

std::streamsize Sequences::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	long32 nbytes = 0;
	std::ostream::pos_type position = ostream.tellp();
	
	BOOST_FOREACH(const class Sequence *sequence, this->m_sequences)
	{
		long32 writtenBytes = sequence->writeMdx(ostream);
		
		if (writtenBytes == 0)
			throw Exception(_("Sequences: 0 byte sequence."));
		
		nbytes += writtenBytes;
		bytes += writtenBytes;
	}
	
	ostream.seekp(position);
	ostream.write(reinterpret_cast<const char*>(&nbytes), sizeof(nbytes));
	bytes += sizeof(nbytes);
	
	return bytes;
}

}

}
