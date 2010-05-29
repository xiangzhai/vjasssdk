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

#include <boost/format.hpp>

#include "textureanimation.hpp"
#include "textureanimations.hpp"
#include "textureanimationtranslations.hpp"
#include "textureanimationrotations.hpp"
#include "textureanimationscalings.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

TextureAnimation::TextureAnimation(class TextureAnimations *textureAnimations) : m_textureAnimations(textureAnimations), m_translations(new TextureAnimationTranslations(this)), m_rotations(new TextureAnimationRotations(this)), m_scalings(new TextureAnimationScalings(this))
{
}

TextureAnimation::~TextureAnimation()
{
	delete this->m_translations;
	delete this->m_rotations;
	delete this->m_scalings;
}

void TextureAnimation::readMdl(std::istream &istream) throw (class Exception)
{
}

void TextureAnimation::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	/*
	fstream << "\tTVertexAnim {\n";

	if (this->m_translations != 0)
	{
		class Translation2 *translation = *this->m_translations->translations().begin();
		fstream << "\t\t(Translation { " << translation->x() << ", " << translation->y() << ", " << translation->z() << " })\n";
	}

	/// @todo InTan and OutTan only appear when Hermite or Bezier. GlobalSeqId only appears when its value is not 0xFFFFFFFF.
	if (this->m_rotations != 0)
	{
		class Rotation1 *rotation = *this->m_rotations->rotations().begin();
		/// @todo @class Rotation1 inherits from Scaling0 which does not have members a, b, c and d. @class Rotation0 does have these values but isn't the right data type considering the specification.
		//fstream << "\t\t(Rotation { " << rotation->a() << ", " << rotation->b() << ", " << rotation->c() << ", " << rotation->d() << " })\n";
	}
		
	if (this->m_scalings != 0)
	{
		class Scaling1 *scaling = *this->m_scalings->scalings().begin();
		fstream << "\t\t(Scaling { " << scaling->x() << ", " << scaling->y() << ", " << scaling->z() << " })\n";
	}

	fstream << "\t}\n";
	*/
}


std::streamsize TextureAnimation::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytesi = 0;
	istream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	std::streamsize bytes = istream.gcount();
	bytes += this->m_translations->readMdx(istream);
	bytes += this->m_rotations->readMdx(istream);
	bytes += this->m_scalings->readMdx(istream);
	
	if (nbytesi != bytes)
		throw Exception(boost::str(boost::format(_("Texture Animation: Error, file byte count and real byte count aren't equal.\nFile byte count: %1% bytes.\nReal byte count: %2%.")) % nbytesi % bytes));
	
	return bytes;
}

std::streamsize TextureAnimation::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
