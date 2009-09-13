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

#include <fstream>

#include "../exception.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;

class Texture
{
	public:
		enum Wrapping
		{
			WrapWidth = 1,
			WrapHeight = 2,
			Both = 3
		};

		Texture(class Mdlx *mdlx);
		virtual ~Texture();

		class Mdlx* mdlx() const;
		long32 replaceableId() const;
		const ascii* texturePath() const;
		long32 unknown0() const;
		long32 wrapping() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		long32 m_replaceableId;
		ascii m_texturePath[0x100]; //(0x100 bytes)
		long32 m_unknown0; //(0)
		long32 m_wrapping; //(1:WrapWidth;2:WrapHeight;3:Both)
};

inline class Mdlx* Texture::mdlx() const
{
	return this->m_mdlx;
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
