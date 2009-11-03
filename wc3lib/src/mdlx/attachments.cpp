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
#include "attachment.hpp"

namespace wc3lib
{

namespace mdlx
{

Attachments::Attachments(class Mdlx *mdlx) : MdxBlock("ATCH"), m_mdlx(mdlx)
{
}

Attachments::~Attachments()
{
	for (std::list<class Attachment*>::iterator iterator = this->m_attachments.begin(); iterator != this->m_attachments.end(); ++iterator)
		delete *iterator;
}

void Attachments::readMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Attachments::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes;
	fstream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += fstream.gcount();
	
	while (nbytes > 0)
	{
		class Attachment *attachment = new Attachment(this);
		long32 readBytes = attachment->readMdx(fstream);
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_attachments.push_back(attachment);
	}
	
	return bytes;
}

void Attachments::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Attachments::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
