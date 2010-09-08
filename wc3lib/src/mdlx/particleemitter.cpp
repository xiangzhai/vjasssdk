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
#include "mdlxtranslations.hpp"
#include "mdlxrotations.hpp"
#include "mdlxscalings.hpp"
#include "particleemittervisibilities.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

ParticleEmitter::ParticleEmitter(class ParticleEmitters *particleEmitters) : Node(particleEmitters->mdlx()), m_particleEmitters(particleEmitters), m_visibilities(new ParticleEmitterVisibilities(this))
{
}

ParticleEmitter::~ParticleEmitter()
{
	delete this->m_visibilities;
}

std::streamsize ParticleEmitter::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize ParticleEmitter::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize ParticleEmitter::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytesi;
	std::streamsize size = 0;
	wc3lib::read(istream, nbytesi, size);
	long32 nbytesikg; // inclusive bytecount including KGXXs
	wc3lib::read(istream, nbytesikg, size);
	size += Node::readMdx(istream);
	wc3lib::read(istream, this->m_emissionRate, size);
	wc3lib::read(istream, this->m_gravity, size);
	wc3lib::read(istream, this->m_longitude, size);
	wc3lib::read(istream, this->m_latitidue, size);
	wc3lib::read(istream, this->m_modelPath, size);
	wc3lib::read(istream, this->m_unknown0, size);
	wc3lib::read(istream, this->m_lifeSpan, size);
	wc3lib::read(istream, this->m_initVelocity, size);
	size += this->m_visibilities->readMdx(istream);

	return size;
}

std::streamsize ParticleEmitter::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streampos position = ostream.tellp();
	ostream.seekp(2 * sizeof(long32), std::ios_base::cur);

	std::streamsize size = Node::writeMdx(ostream);
	wc3lib::write(ostream, this->m_emissionRate, size);
	wc3lib::write(ostream, this->m_gravity, size);
	wc3lib::write(ostream, this->m_longitude, size);
	wc3lib::write(ostream, this->m_latitidue, size);
	wc3lib::write(ostream, this->m_modelPath, size);
	wc3lib::write(ostream, this->m_unknown0, size);
	wc3lib::write(ostream, this->m_lifeSpan, size);
	wc3lib::write(ostream, this->m_initVelocity, size);
	size += this->m_visibilities->writeMdx(ostream);

	long32 nbytesi = size + sizeof(long32) * 2;
	long32 nbytesikg = size + sizeof(long32); /// @todo Same as nbytesi without its size? (inclusive bytecount including KGXXs)
	std::streampos currentPosition = ostream.tellp();
	ostream.seekp(position);
	wc3lib::write(ostream, nbytesi, size);
	wc3lib::write(ostream, nbytesikg, size);
	ostream.seekp(currentPosition);

	return size;
}

}

}
