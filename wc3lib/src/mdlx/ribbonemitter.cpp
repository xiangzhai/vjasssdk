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

void RibbonEmitter::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void RibbonEmitter::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 RibbonEmitter::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 nbytesi = 0;
	fstream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	long32 bytes = fstream.gcount();
	long32 nbytesikg; // inclusive bytecount including KGXXs
	fstream.read(reinterpret_cast<char*>(&nbytesikg), sizeof(nbytesikg));
	bytes += fstream.gcount();
	fstream.read(this->m_name, sizeof(this->m_name));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_objectId), sizeof(this->m_objectId));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_parent), sizeof(this->m_parent));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_flags), sizeof(this->m_flags));
	bytes += fstream.gcount();
	bytes += this->m_translations->readMdx(fstream);
	bytes += this->m_rotations->readMdx(fstream);
	bytes += this->m_scalings->readMdx(fstream);
	fstream.read(reinterpret_cast<char*>(&this->m_heightAboveValue), sizeof(this->m_heightAboveValue));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_heightBelowValue), sizeof(this->m_heightBelowValue));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_alpha), sizeof(this->m_alpha));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_colorRed), sizeof(this->m_colorRed));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_colorGreen), sizeof(this->m_colorGreen));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_colorBlue), sizeof(this->m_colorBlue));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_lifeSpan), sizeof(this->m_lifeSpan));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_lifeSpan), sizeof(this->m_lifeSpan));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_unknown0), sizeof(this->m_unknown0));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_emissionRate), sizeof(this->m_emissionRate));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_rows), sizeof(this->m_rows));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_columns), sizeof(this->m_columns));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_materialId), sizeof(this->m_materialId));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_gravity), sizeof(this->m_gravity));
	bytes += fstream.gcount();
	bytes +=this->m_visibilities->readMdx(fstream);
	bytes +=this->m_heightsAbove->readMdx(fstream);
	bytes +=this->m_heightsBelow->readMdx(fstream);
	
	return bytes;
}

long32 RibbonEmitter::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
