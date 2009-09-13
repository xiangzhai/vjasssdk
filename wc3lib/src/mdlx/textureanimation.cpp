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

#include "textureanimation.hpp"

namespace wc3lib
{

namespace mdlx
{

TextureAnimation::TextureAnimation(class Mdlx *mdlx) : m_mdlx(mdlx)
{
}

TextureAnimation::~TextureAnimation()
{
}

void TextureAnimation::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void TextureAnimation::readMdx(std::fstream &fstream) throw (class Exception)
{
}

void TextureAnimation::writeMdl(std::fstream &fstream) throw (class Exception)
{
	fstream << "\tTVertexAnim {\n";

	if ((*iterator)->m_translation != 0)
		fstream << "\t\t(Translation { " << (*iterator)->translation->x << ", " << (*iterator)->translation->y << ", " << (*iterator)->translation->y << " })\n";

	/// @todo InTan and OutTan only appear when Hermite or Bezier. GlobalSeqId only appears when its value is not 0xFFFFFFFF.
	if ((*iterator)->rotation != 0)
		fstream << "\t\t(Rotation { " << (*iterator)->rotation->a << ", " << (*iterator)->rotation->b << ", " << (*iterator)->rotation->c << ", " << (*iterator)->rotation->d << " })\n";

	if ((*iterator)->scaling != 0)
		fstream << "\t\t(Scaling { " << (*iterator)->scaling->x << ", " << (*iterator)->scaling->y << ", " << (*iterator)->scaling->y << " })\n";

	fstream << "\t}\n";
}

void TextureAnimation::writeMdx(std::fstream &fstream) throw (class Exception)
{
}

}

}
