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

#include "rotation1s.hpp"
#include "rotation1.hpp"

namespace wc3lib
{

namespace mdlx
{

Rotation1s::Rotation1s(class TextureAnimation *textureAnimation) : MdxScalings("KTAR", false), m_textureAnimation(textureAnimation)
{
}

Rotation1s::~Rotation1s()
{
}

void Rotation1s::readMdl(std::istream &istream) throw (class Exception)
{
}

void Rotation1s::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

class MdxScaling* Rotation1s::createNewMember()
{
	return new Rotation1(this);
}
	
}

}