/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#include "mpqprotocol.hpp"
#include "../mpq.hpp"

namespace wc3lib
{

namespace editor
{

/*
MpqArchive::MpqArchive(QIODevice *dev) : KArchive(dev), m_mpq(new mpq::Mpq())
{
}

MpqArchive::MpqArchive(const QString &fileName) : KArchive(fileName), m_mpq(new mpq::Mpq())
{

}

MpqArchive::~MpqArchive()
{
	delete this->m_mpq;
}

bool MpqArchive::closeArchive()
{
	this->m_mpq->close();

	return true;
}

bool MpqArchive::openArchive(QIODevice::OpenMode mode)
{
	switch (mode)
	{
		case QIODevice::WriteOnly:
		case QIODevice::ReadWrite:
			//this->m_mpq->readMpq(this->device()->)
			break;
	}
}


void MpqArchive::setDevice(QIODevice *dev)
{

}
*/

}

}
