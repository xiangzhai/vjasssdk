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

#include "ribbonemitter.hpp"
#include "ribbonemitters.hpp"
#include "translation1s.hpp"
#include "rotation0s.hpp"
#include "scaling0s.hpp"
#include "ribbonemittervisibilities.hpp"
#include "ribbonemitterheightsabove.hpp"
#include "ribbonemitterheightsbelow.hpp"

namespace wc3lib
{

namespace mdlx
{

RibbonEmitter::RibbonEmitter(class RibbonEmitters *ribbonEmitters) : m_ribbonEmitters(ribbonEmitters), m_translations(new Translation1s(ribbonEmitters->mdlx())), m_rotations(new Rotation0s(ribbonEmitters->mdlx())), m_scalings(new Scaling0s(ribbonEmitters->mdlx())), m_visibilities(new RibbonEmitterVisibilities(this)), m_heightsAbove(new RibbonEmitterHeightsAbove(this)), m_heightsBelow(new RibbonEmitterHeightsBelow(this))
{
}

RibbonEmitter::~RibbonEmitter()
{
	delete this->m_translations;
	delete this->m_rotations;
	delete this->m_scalings;
	delete this->m_visibilities;
	delete this->m_heightsAbove;
	delete this->m_heightsBelow;
}

void RibbonEmitter::readMdl(std::istream &istream) throw (class Exception)
{
}

void RibbonEmitter::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 RibbonEmitter::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytesi = 0;
	istream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	long32 bytes = istream.gcount();
	long32 nbytesikg; // inclusive bytecount including KGXXs
	istream.read(reinterpret_cast<char*>(&nbytesikg), sizeof(nbytesikg));
	bytes += istream.gcount();
	istream.read(this->m_name, sizeof(this->m_name));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_objectId), sizeof(this->m_objectId));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_parent), sizeof(this->m_parent));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_flags), sizeof(this->m_flags));
	bytes += istream.gcount();
	bytes += this->m_translations->readMdx(istream);
	bytes += this->m_rotations->readMdx(istream);
	bytes += this->m_scalings->readMdx(istream);
	istream.read(reinterpret_cast<char*>(&this->m_heightAboveValue), sizeof(this->m_heightAboveValue));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_heightBelowValue), sizeof(this->m_heightBelowValue));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_alpha), sizeof(this->m_alpha));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_colorRed), sizeof(this->m_colorRed));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_colorGreen), sizeof(this->m_colorGreen));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_colorBlue), sizeof(this->m_colorBlue));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_lifeSpan), sizeof(this->m_lifeSpan));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_lifeSpan), sizeof(this->m_lifeSpan));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_unknown0), sizeof(this->m_unknown0));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_emissionRate), sizeof(this->m_emissionRate));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_rows), sizeof(this->m_rows));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_columns), sizeof(this->m_columns));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_materialId), sizeof(this->m_materialId));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_gravity), sizeof(this->m_gravity));
	bytes += fstream.gcount();
	bytes += this->m_visibilities->readMdx(istream);
	bytes += this->m_heightsAbove->readMdx(istream);
	bytes += this->m_heightsBelow->readMdx(istream);
	
	return bytes;
}

long32 RibbonEmitter::writeMdx(std::ostream &ostream) throw (class Exception)
{
	return 0;
}

}

}
