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

#include "collisionshapes.hpp"

namespace wc3lib
{

namespace mdlx
{

CollisionShapes::CollisionShapes(class Mdlx *mdlx) : MdxBlock("CLID"), m_mdlx(mdlx)
{
}

CollisionShapes::~CollisionShapes()
{
}

void CollisionShapes::readMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 CollisionShapes::readMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

void CollisionShapes::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 CollisionShapes::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}