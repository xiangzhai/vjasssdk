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

#include "mpqfile.hpp"

namespace wc3lib
{

namespace mpq
{

void MpqFile::remove() throw (class Exception)
{
	this->m_hash->clear();
}
	
std::streamsize MpqFile::read(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize MpqFile::write(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

MpqFile::MpqFile(class Mpq *mpq, class Hash *hash) : m_mpq(mpq), m_hash(hash), m_size(0), m_compressedSize(0), m_locale(MpqFile::Neutral), m_platform(MpqFile::Default), m_path("")
{
}

MpqFile::~MpqFile()
{
}

}

}
