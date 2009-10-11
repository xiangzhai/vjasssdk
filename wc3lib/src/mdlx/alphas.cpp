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

namespace wc3lib
{

namespace mdlx
{

Alphas::Alphas(class Mdlx *mdlx) : MdxBlock("KMTA"), m_mdlx(mdlx)
{
}

Alphas::~Alphas()
{
}

void Alphas::readMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Alphas::readMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

void Alphas::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Alphas::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
