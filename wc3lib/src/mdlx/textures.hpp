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

#ifndef WC3LIB_MDLX_TEXTURES_HPP
#define WC3LIB_MDLX_TEXTURES_HPP

#include <list>

#include "mdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;
class Texture;

/// TEXS // [Textures] (same as v800)
class Textures : public MdxBlock
{
	public:
		Textures(class Mdlx *mdlx);
		virtual ~Textures();

		class Mdlx* mdlx() const;
		const std::list<class Texture*>& textures() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		std::list<class Texture*> m_textures;
};

inline class Mdlx* Textures::mdlx() const
{
	return this->m_mdlx;
}

inline const std::list<class Texture*>& Textures::textures() const
{
	return this->m_textures;
}

}

}

#endif
