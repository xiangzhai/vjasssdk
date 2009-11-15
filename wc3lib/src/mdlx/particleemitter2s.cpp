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

#include "particleemitter2s.hpp"
#include "particleemitter2.hpp"

namespace wc3lib
{
	
namespace mdlx
{

ParticleEmitter2s::ParticleEmitter2s(class Mdlx *mdlx) : MdxBlock("PRE2"), m_mdlx(mdlx)
{
}

ParticleEmitter2s::~ParticleEmitter2s()
{
	for (std::list<class ParticleEmitter2*>::iterator iterator = this->m_particleEmitters.begin(); iterator != this->m_particleEmitters.end(); ++iterator)
		delete *iterator;
}

void ParticleEmitter2s::readMdl(std::istream &istream) throw (class Exception)
{
}

void ParticleEmitter2s::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 ParticleEmitter2s::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();
	
	while (nbytes > 0)
	{
		class ParticleEmitter2 *particleEmitter = new ParticleEmitter2(this);
		long32 readBytes = particleEmitter->readMdx(istream);
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_particleEmitters.push_back(particleEmitter);
		std::cout << "Got particle emitter with " << readBytes << " bytes and " << nbytes << " bytes left." << std::endl;
	}
	
	return bytes;
}

long32 ParticleEmitter2s::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	return bytes;
}

}

}
