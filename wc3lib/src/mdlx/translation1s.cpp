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

#include "translation1s.hpp"
#include "translation1.hpp"

namespace wc3lib
{

namespace mdlx
{

Translation1s::Translation1s(class Mdlx *mdlx) : MdxBlock("KGTR"), m_mdlx(mdlx)
{
}

Translation1s::~Translation1s()
{
}

void Translation1s::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Translation1s::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Translation1s::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nunks = 0;
	fstream.read(reinterpret_cast<char*>(&nunks), sizeof(nunks));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_lineType), sizeof(this->m_lineType));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_globalSequenceId), sizeof(this->m_globalSequenceId));
	bytes += fstream.gcount();

	for ( ; nunks > 0; --nunks)
	{
		class Translation1 *translation = new Translation1(this);
		bytes += translation->readMdx(fstream);
		this->m_translations.push_back(translation);
	}
	
	return bytes;
}

long32 Translation1s::writeMdx(std::fstream &fstream) throw (class Exception)
{
	if (!this->exists())
		return 0;
	
	long32 bytes = MdxBlock::writeMdx(fstream);
	
	return bytes;
}

}

}