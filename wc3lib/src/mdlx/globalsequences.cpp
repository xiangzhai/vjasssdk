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
}

void GlobalSequences::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void GlobalSequences::writeMdl(std::fstream &fstream) throw (class Exception)
{
	fstream << "GlobalSequences " << this->globalSequences().size() << " {\n";

	for (std::list<class GlobalSequence*>::iterator iterator = this->globalSequences().begin(); iterator != this->globalSequences().end(); ++iterator)
		(*iterator)->writeMdl(fstream);

	fstream << "}\n";
}


long32 GlobalSequences::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes = 0; //nbytes
	fstream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += fstream.gcount();
	
	while (nbytes > 0)
	{
		class GlobalSequence *globalSequence = new GlobalSequence(this);
		long32 readBytes = globalSequence->readMdx(fstream);
		
		if (readBytes == 0)
			throw Exception(_("Global Sequences: 0 byte global sequence."));
		
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_globalSequences.push_back(globalSequence);
	}
	
	return bytes;
}

long32 GlobalSequences::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = 0;
	bytes += MdxBlock::readMdx(fstream);
	
	return bytes;
}

}

}
