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

#ifndef WC3LIB_MDLX_PARTICLEEMITTER2VISIBILITY_HPP
#define WC3LIB_MDLX_PARTICLEEMITTER2VISIBILITY_HPP

#include "mdlxalpha.hpp"
#include "particleemitter2visibilities.hpp"

namespace wc3lib
{

namespace mdlx
{

class ParticleEmitter2Visibility : public MdlxAlpha
{
	public:
		ParticleEmitter2Visibility(class ParticleEmitter2Visibilities *visibilities);
		
		class ParticleEmitter2Visibilities* visibilities() const;
};

inline class ParticleEmitter2Visibilities* ParticleEmitter2Visibility::visibilities() const
{
	return dynamic_cast<class ParticleEmitter2Visibilities*>(this->mdlxAlphas());
}

}

}

#endif
