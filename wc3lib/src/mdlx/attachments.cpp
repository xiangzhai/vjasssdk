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

#include "attachments.hpp"

namespace wc3lib
{

namespace mdlx
{

Attachment::Attachment(class Mdlx *mdlx) : MdxBlock("ATCH"), m_mdlx(mdlx)
{
}

Attachment::~Attachment()
{
}

void Attachment::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Attachment::readMdx(std::fstream &fstream) throw (class Exception)
{
}

void Attachment::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

void Attachment::writeMdx(std::fstream &fstream) throw (class Exception)
{
}

}

}
