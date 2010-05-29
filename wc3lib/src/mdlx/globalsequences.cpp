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

#include <boost/foreach.hpp>

#include "globalsequences.hpp"
#include "globalsequence.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

GlobalSequences::GlobalSequences(class Mdlx *mdlx) : MdxBlock("GLBS"), m_mdlx(mdlx)
{
}

GlobalSequences::~GlobalSequences()
{
	BOOST_FOREACH(class GlobalSequence *globalSequence, this->m_globalSequences)
		delete globalSequence;
}

void GlobalSequences::readMdl(std::istream &istream) throw (class Exception)
{
}

void GlobalSequences::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	ostream << "GlobalSequences " << this->globalSequences().size() << " {\n";

	BOOST_FOREACH(class GlobalSequence *globalSequence, this->m_globalSequences)
		globalSequence->writeMdl(ostream);

	ostream << "}\n";
}


std::streamsize GlobalSequences::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes = 0; //nbytes
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();
	
	while (nbytes > 0)
	{
		class GlobalSequence *globalSequence = new GlobalSequence(this);
		long32 readBytes = globalSequence->readMdx(istream);
		
		if (readBytes == 0)
			throw Exception(_("Global Sequences: 0 byte global sequence."));
		
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_globalSequences.push_back(globalSequence);
	}
	
	return bytes;
}

std::streamsize GlobalSequences::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = MdxBlock::writeMdx(ostream);
	
	return bytes;
}

}

}
