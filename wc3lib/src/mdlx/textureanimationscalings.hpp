/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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

#ifndef WC3LIB_MDLX_TEXTUREANIMATIONSCALINGS_HPP
#define WC3LIB_MDLX_TEXTUREANIMATIONSCALINGS_HPP

#include "mdlxscalings.hpp"

namespace wc3lib
{

namespace mdlx
{

/// tag KTAS
class TextureAnimationScalings : public MdlxScalings
{
	public:
		TextureAnimationScalings(class TextureAnimation *textureAnimation);
		virtual ~TextureAnimationScalings();

		class TextureAnimation* textureAnimation() const;
		const std::list<class TextureAnimationScaling*>& scalings() const;

		virtual std::streamsize readMdl(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(std::ostream &ostream) const throw (class Exception);

	protected:
		virtual class MdlxScaling* createNewMember();

		class TextureAnimation *m_textureAnimation;
};

inline class TextureAnimation* TextureAnimationScalings::textureAnimation() const
{
	return this->m_textureAnimation;
}

inline const std::list<class TextureAnimationScaling*>& TextureAnimationScalings::scalings() const
{
	return reinterpret_cast<const std::list<class TextureAnimationScaling*>&>(this->m_scalings);
}

}

}

#endif
