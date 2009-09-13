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

#ifndef WC3LIB_MDLX_PARTICLEEMITTERS_HPP
#define WC3LIB_MDLX_PARTICLEEMITTERS_HPP

#include <fstream>
#include <list>

#include "mdxblock.hpp"
#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;
class ParticleEmitter2;

class ParticleEmitter2s : public MdxBlock
{
	public:
		ParticleEmitter2s(class Mdlx *mdlx);
		virtual ~ParticleEmitter2s();

		class Mdlx* mdlx() const;
		std::list<class ParticleEmitter2*> particleEmitters() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		std::list<class ParticleEmitter2*> m_particleEmitters;
};

inline class Mdlx* ParticleEmitter2s::mdlx() const
{
	return this->m_mdlx;
}

inline std::list<class ParticleEmitter2*> ParticleEmitter2s::particleEmitters() const
{
	return this->m_particleEmitters;
}

}

}

#endif
