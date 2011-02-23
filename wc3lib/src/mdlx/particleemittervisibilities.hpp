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

#ifndef WC3LIB_MDLX_PARTICLEEMITTERVISIBILITIES_HPP
#define WC3LIB_MDLX_PARTICLEEMITTERVISIBILITIES_HPP

#include "mdlxalphas.hpp"

namespace wc3lib
{

namespace mdlx
{

/// tag KPEV
class ParticleEmitterVisibilities : public MdlxAlphas
{
	public:
		ParticleEmitterVisibilities(class ParticleEmitter *particleEmitter);

		class ParticleEmitter* particleEmitter() const;
		const std::list<class ParticleEmitterVisibility*>& visibilities() const;

	protected:
		virtual class MdlxAnimatedProperty* createAnimatedProperty();

		class ParticleEmitter *m_particleEmitter;
};

inline class ParticleEmitter* ParticleEmitterVisibilities::particleEmitter() const
{
	return this->m_particleEmitter;
}

inline const std::list<class ParticleEmitterVisibility*>& ParticleEmitterVisibilities::visibilities() const
{
	return reinterpret_cast<const std::list<class ParticleEmitterVisibility*>&>(this->mdlxAlphas());
}

}

}

#endif
