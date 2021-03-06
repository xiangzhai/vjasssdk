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

#ifndef WC3LIB_MDLX_MDLXALPHAS_HPP
#define WC3LIB_MDLX_MDLXALPHAS_HPP

#include "mdlxanimatedproperties.hpp"

namespace wc3lib
{

namespace mdlx
{

class MdlxAlphas : public MdlxAnimatedProperties
{
	public:
		MdlxAlphas(class Mdlx *mdlx);
		virtual ~MdlxAlphas();

		const std::list<class MdlxAlpha*>& mdlxAlphas() const;
		
	protected:
		MdlxAlphas(class Mdlx *mdlx, const byte mdxIdentifier[MdxBlock::mdxIdentifierSize]);
		
		virtual class MdlxAnimatedProperty* createAnimatedProperty();
};

inline const std::list<class MdlxAlpha*>& MdlxAlphas::mdlxAlphas() const
{
	return *reinterpret_cast<const std::list<class MdlxAlpha*>*>(&properties());
}

}

}

#endif
