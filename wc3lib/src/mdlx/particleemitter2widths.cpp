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

#include "particleemitter2widths.hpp"
#include "particleemitter2width.hpp"

namespace wc3lib
{
	
namespace mdlx
{

ParticleEmitter2Widths::ParticleEmitter2Widths(class ParticleEmitter2 *particleEmitter) : MdxAlphas("KP2W"), m_particleEmitter(particleEmitter)
{
}

ParticleEmitter2Widths::~ParticleEmitter2Widths()
{
}

void ParticleEmitter2Widths::readMdl(std::istream &istream) throw (class Exception)
{
}

void ParticleEmitter2Widths::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

class MdxAlpha* ParticleEmitter2Widths::createNewMember()
{
	return new ParticleEmitter2Width(this);
}

}

}