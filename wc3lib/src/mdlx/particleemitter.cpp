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

namespace wc3lib
{

namespace mdlx
{

ParticleEmitter::ParticleEmitter(class Mdlx *mdlx) : m_mdlx(mdlx), m_translations(0),  m_rotations(0), m_scalings(0), m_visibilities(0)
{
}

ParticleEmitter::~ParticleEmitter()
{
}

void Normal::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Normal::readMdx(std::fstream &fstream) throw (class Exception)
{
}

void Normal::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

void Normal::writeMdx(std::fstream &fstream) throw (class Exception)
{
}

}

}
