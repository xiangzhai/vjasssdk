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

#include <string>
#include <sstream>

#include <boost/tokenizer.hpp>

#include "globalsequence.hpp"
#include "globalsequences.hpp"

namespace wc3lib
{

namespace mdlx
{

GlobalSequence::GlobalSequence(class GlobalSequences *globalSequences) : m_globalSequences(globalSequences)
{
}

GlobalSequence::~GlobalSequence()
{
}

void GlobalSequence::readMdl(std::fstream &fstream) throw (class Exception)
{
	std::string line;
	std::getline(fstream, line);
	boost::tokenizer<> tokenizer(line);
	boost::tokenizer<>::iterator iterator = tokenizer.begin();

	if (iterator == tokenizer.end() || (*iterator) != "Duration")
		throw Exception("Global sequence: Missing keyword \"Duration\".");

	++iterator;

	if (iterator == tokenizer.end())
		throw Exception("Global sequence: Missing value.");
	else if (*((*iterator).end() - 1) != ',')
		throw Exception("Global sequence: Missing \",\" character.");

	line = (*iterator);
	line.erase(line.length() - 1);
	std::istringstream sstream(line);
	sstream >> this->m_duration;
}

long32 GlobalSequence::readMdx(std::fstream &fstream) throw (class Exception)
{
	fstream.read(reinterpret_cast<char*>(&this->m_duration), sizeof(this->m_duration));
	
	return fstream.gcount();
}

void GlobalSequence::writeMdl(std::fstream &fstream) throw (class Exception)
{
	fstream << "\tDuration " << this->duration() << ",\n";
}

void GlobalSequence::writeMdx(std::fstream &fstream) throw (class Exception)
{
	fstream << this->duration();
}

}

}
