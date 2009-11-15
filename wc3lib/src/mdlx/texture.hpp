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

#ifndef WC3LIB_MDLX_TEXTURE_HPP
#define WC3LIB_MDLX_TEXTURE_HPP

#include <iostream>

#include "../exception.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace mdlx
{

class Textures;

class Texture
{
	public:
		enum Wrapping
		{
			WrapWidth = 1,
			WrapHeight = 2,
			Both = 3
		};

		Texture(class Textures *textures);
		virtual ~Texture();

		class Textures* textures() const;
		long32 replaceableId() const;
		const ascii* texturePath() const;
		long32 unknown0() const;
		long32 wrapping() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

	protected:
		class Textures *m_textures;
		long32 m_replaceableId;
		ascii m_texturePath[0x100]; //(0x100 bytes)
		long32 m_unknown0; //(0)
		long32 m_wrapping; //(1:WrapWidth;2:WrapHeight;3:Both)
};

inline class Textures* Texture::textures() const
{
	return this->m_textures;
}

inline long32 Texture::replaceableId() const
{
	return this->m_replaceableId;
}

inline const char* Texture::texturePath() const
{
	return this->m_texturePath;
}

inline long32 Texture::unknown0() const
{
	return this->m_unknown0;
}

inline long32 Texture::wrapping() const
{
	return this->m_wrapping;
}

}

}

#endif
