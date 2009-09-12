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

#include "blpuncompressed2.hpp"
#include "blp.hpp"

namespace wc3lib
{

namespace blp
{

BlpUncompressed2::BlpUncompressed2(const class Blp &blp)
{
	for (int i = 0; i < 16; ++i)
		this->m_mipMaps[i].m_indexList = new byte[blp.width() * blp.height()];
}

BlpUncompressed2::~BlpUncompressed2()
{
	for (int i = 0; i < 16; ++i)
		delete[] this->m_mipMaps[i].m_indexList;
}

}

}