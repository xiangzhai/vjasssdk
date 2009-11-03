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

#include "alphas.hpp"
#include "alpha.hpp"

namespace wc3lib
{

namespace mdlx
{

Alphas::Alphas(class Layer *layer) : MdxBlock("KMTA"), m_layer(layer)
{
}

Alphas::~Alphas()
{
	for (std::list<class Alpha*>::iterator iterator = this->m_alphas.begin(); iterator != this->m_alphas.end(); ++iterator)
		delete *iterator;
}

void Alphas::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Alphas::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Alphas::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nunks;
	fstream.read(reinterpret_cast<char*>(&nunks), sizeof(nunks));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_lineType), sizeof(this->m_lineType));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_globalSequenceId), sizeof(this->m_globalSequenceId));
	bytes += fstream.gcount();
	
	for ( ; nunks > 0; --nunks)
	{
		class Alpha *alpha = new Alpha(this);
		bytes += alpha->readMdx(fstream);
		this->m_alphas.push_back(alpha);
	}
	
	return bytes;
}

long32 Alphas::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
