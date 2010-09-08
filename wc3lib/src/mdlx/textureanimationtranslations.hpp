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

#ifndef WC3LIB_MDLX_TEXTUREANIMATIONTRANSLATIONS_HPP
#define WC3LIB_MDLX_TEXTUREANIMATIONTRANSLATIONS_HPP

#include "mdlxscalings.hpp"

namespace wc3lib
{

namespace mdlx
{

/// KTAT, like KGSC (Scalings)
class TextureAnimationTranslations : public MdlxScalings
{
	public:
		TextureAnimationTranslations(class TextureAnimation *textureAnimation);
		virtual ~TextureAnimationTranslations();

		class TextureAnimation* textureAnimation() const;
		const std::list<class TextureAnimationTranslation*>& translations() const;

		virtual std::streamsize readMdl(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(std::ostream &ostream) const throw (class Exception);

	protected:
		virtual class MdlxScaling* createNewMember();

		class TextureAnimation *m_textureAnimation;
};

inline class TextureAnimation* TextureAnimationTranslations::textureAnimation() const
{
	return this->m_textureAnimation;
}

inline const std::list<class TextureAnimationTranslation*>& TextureAnimationTranslations::translations() const
{
	return reinterpret_cast<const std::list<class TextureAnimationTranslation*>&>(this->m_scalings);
}

}

}

#endif

