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

#include <boost/cast.hpp>

#include "helpers.hpp"
#include "helper.hpp"
#include "../utilities.hpp"

#include "mdlxrotations.hpp"
#include "mdlxtranslations.hpp"

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

std::streamsize Helpers::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Helpers::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize Helpers::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);

	if (size == 0)
		return 0;

	long32 nbytes = 0;
	wc3lib::read(istream, nbytes, size);
	std::cout << "Nbytes: " << nbytes << std::endl;

	while (nbytes > 0)
	{
		class Helper *helper = new Helper(this);
		std::streamsize readSize = helper->readMdx(istream);
		size += readSize;
		nbytes -= boost::numeric_cast<long32>(readSize);
		this->m_helpers.push_back(helper);
	}

	return size;
}

std::streamsize Helpers::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	if (!this->exists())
		return 0;

	std::streamsize bytes = MdxBlock::writeMdx(ostream);

	return bytes;
}

}

}
