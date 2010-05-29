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

#include "particleemitter.hpp"
#include "particleemitters.hpp"
#include "translation1s.hpp"
#include "rotation0s.hpp"
#include "scaling0s.hpp"
#include "particleemittervisibilities.hpp"

namespace wc3lib
{

namespace mdlx
{

ParticleEmitter::ParticleEmitter(class ParticleEmitters *particleEmitters) : m_particleEmitters(particleEmitters), m_translations(new Translation1s(particleEmitters->mdlx())),  m_rotations(new Rotation0s(particleEmitters->mdlx())), m_scalings(new Scaling0s(particleEmitters->mdlx())), m_visibilities(new ParticleEmitterVisibilities(this))
{
}

ParticleEmitter::~ParticleEmitter()
{
	delete this->m_translations;
	delete this->m_rotations;
	delete this->m_scalings;
	delete this->m_visibilities;
}

void ParticleEmitter::readMdl(std::istream &istream) throw (class Exception)
{
}

void ParticleEmitter::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize ParticleEmitter::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytesi;
	istream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	std::streamsize bytes = istream.gcount();
	long32 nbytesikg; // inclusive bytecount including KGXXs
	istream.read(reinterpret_cast<char*>(&nbytesikg), sizeof(nbytesikg));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(this->m_name), sizeof(this->m_name));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_objectId), sizeof(this->m_objectId));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_parent), sizeof(this->m_parent));
	bytes += istream.gcount(); //(0xFFFFFFFF if none)
	istream.read(reinterpret_cast<char*>(&this->m_flags), sizeof(this->m_flags));
	bytes += istream.gcount();
	bytes += this->m_translations->readMdx(istream); //(KGTR)
	bytes += this->m_rotations->readMdx(istream);
	bytes += this->m_scalings->readMdx(istream);
	istream.read(reinterpret_cast<char*>(&this->m_emissionRate), sizeof(this->m_emissionRate));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_gravity), sizeof(this->m_gravity));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_longitude), sizeof(this->m_longitude));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_latitidue), sizeof(this->m_latitidue));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(this->m_modelPath), sizeof(this->m_modelPath));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_unknown0), sizeof(this->m_unknown0));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_lifeSpan), sizeof(this->m_lifeSpan));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_initVelocity), sizeof(this->m_initVelocity));
	bytes += istream.gcount();
	bytes += this->m_visibilities->readMdx(istream);
	
	return bytes;
}

std::streamsize ParticleEmitter::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
