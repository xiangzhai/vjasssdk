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

#include <iostream> // debug

#include "helpers.hpp"
#include "helper.hpp"

namespace wc3lib
{

namespace mdlx
{

Helpers::Helpers(class Mdlx *mdlx) : MdxBlock("HELP"), m_mdlx(mdlx)
{
}

Helpers::~Helpers()
{
}

void Helpers::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Helpers::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Helpers::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes = 0;
	fstream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += fstream.gcount();
	
	while (nbytes > 0)
	{
		class Helper *helper = new Helper(this);
		long32 readBytes = helper->readMdx(fstream);
		std::cout << "Got " << readBytes << " bytes helper." << std::endl;
		bytes += readBytes;
		nbytes -= readBytes;
		this->m_helpers.push_back(helper);
	}
	
	return bytes;
}

long32 Helpers::writeMdx(std::fstream &fstream) throw (class Exception)
{
	if (!this->exists())
		return 0;
	
	long32 bytes = MdxBlock::writeMdx(fstream);
	
	return bytes;
}

}

}
