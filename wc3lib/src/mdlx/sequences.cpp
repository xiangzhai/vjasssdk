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
	for (std::list<class Sequence*>::iterator iterator = this->m_sequences.begin(); iterator != this->m_sequences.end(); ++iterator)
		delete *iterator;
}

void Sequences::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Sequences::writeMdl(std::fstream &fstream) throw (class Exception)
{
	fstream
	<< "Sequences " << this->m_sequences.size() << " {\n";

	for (std::list<class Sequence*>::iterator iterator = this->m_sequences.begin(); iterator != this->m_sequences.end(); ++iterator)
	{
		fstream
		<< "\tAnim " << (*iterator)->name() << " {\n"
		<< "\t\tInterval { " << (*iterator)->intervalStart() << ", " << (*iterator)->intervalEnd() << " },\t"
		;

		if ((*iterator)->noLooping() == 1)
			fstream << "\t\tNonLooping,\n";

		if ((*iterator)->moveSpeed() != 0.0)
			fstream << "\t\tMoveSpeed " << (*iterator)->moveSpeed() << ",\n";

		if ((*iterator)->rarity() != 0.0)
			fstream << "\t\tRarity " << (*iterator)->rarity() << ",\n";

		if ((*iterator)->minExtX() != 0.0 || (*iterator)->minExtY() != 0.0 || (*iterator)->minExtZ() != 0.0)
			fstream << "\t\tMinimumExtent { " << (*iterator)->minExtX() << ", " << (*iterator)->minExtY() << ", " << (*iterator)->minExtZ() << " },\n";

		if ((*iterator)->maxExtX() != 0.0 || (*iterator)->maxExtY() != 0.0 || (*iterator)->maxExtZ() != 0.0)
			fstream << "\t\tMaxmimumExtent { " << (*iterator)->maxExtX() << ", " << (*iterator)->maxExtY() << ", " << (*iterator)->maxExtZ() << " },\n";

		if ((*iterator)->boundsRadius() != 0.0)
			fstream << "\t\tBoundsRadius " << (*iterator)->boundsRadius() << ",\n";

		fstream << "\t}\n";
	}

	fstream << "}\n";
}

long32 Sequences::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes = 0;
	fstream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += fstream.gcount();
	std::cout << "Sequence bytes: " << nbytes << std::endl;
	
	while (nbytes > 0)
	{
		class Sequence *sequence = new Sequence(this);
		long32 readBytes = sequence->readMdx(fstream);
		
		if (readBytes == 0)
			throw Exception(_("Sequences: 0 byte sequence."));
		
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_sequences.push_back(sequence);
	}
	
	return bytes;
}

long32 Sequences::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(fstream);
	long32 nbytes = 0;
	std::fstream::pos_type position = fstream.tellg();
	
	for (std::list<class Sequence*>::iterator iterator = this->m_sequences.begin(); iterator != this->m_sequences.end(); ++iterator)
	{
		long32 writtenBytes = (*iterator)->writeMdx(fstream);
		
		if (writtenBytes == 0)
			throw Exception(_("Sequences: 0 byte sequence."));
		
		nbytes += writtenBytes;
		bytes += writtenBytes;
	}
	
	fstream.seekg(position);
	fstream.write(reinterpret_cast<const char*>(nbytes), sizeof(nbytes));
	bytes += sizeof(nbytes);
	
	return bytes;
}

}

}
