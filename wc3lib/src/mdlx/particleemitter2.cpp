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

#include <boost/format.hpp>

#include "particleemitter2.hpp"
#include "particleemitter2s.hpp"
#include "translation1s.hpp"
#include "rotation0s.hpp"
#include "scaling0s.hpp"
#include "segmentcolor.hpp"
#include "particleemitter2speeds.hpp"
#include "particleemitter2latitudes.hpp"
#include "emissionrates.hpp"
#include "visibility2s.hpp"
#include "lengths.hpp"
#include "particleemitter2widths.hpp"

namespace wc3lib
{

namespace mdlx
{

ParticleEmitter2::ParticleEmitter2(class ParticleEmitter2s *particleEmitters) : m_particleEmitters(particleEmitters), m_translations(new Translation1s(particleEmitters->mdlx())), m_rotations(new Rotation0s(particleEmitters->mdlx())), m_scalings(new Scaling0s(particleEmitters->mdlx())), m_speeds(new ParticleEmitter2Speeds(this)), m_latitudes(new ParticleEmitter2Latitudes(this)), m_emissionRates(new EmissionRates(particleEmitters->mdlx())), m_visibilities(new Visibility2s(particleEmitters->mdlx())), m_numbers(new Lengths(this)), m_widths(new ParticleEmitter2Widths(this))
{
}

ParticleEmitter2::~ParticleEmitter2()
{
	delete this->m_translations;
	delete this->m_rotations;
	delete this->m_scalings;
		
	for (std::list<class SegmentColor*>::iterator iterator = this->m_segmentColors.begin(); iterator != this->m_segmentColors.end(); ++iterator)
		delete *iterator;
				
	delete this->m_speeds;
	delete this->m_latitudes;
	delete this->m_emissionRates;
	delete this->m_visibilities;
	delete this->m_numbers;
	delete this->m_widths;
}

void ParticleEmitter2::readMdl(std::istream &istream) throw (class Exception)
{
}

void ParticleEmitter2::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 ParticleEmitter2::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytesi;
	istream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	long32 bytes = istream.gcount();
	long32 nbytesikg;
	istream.read(reinterpret_cast<char*>(&nbytesikg), sizeof(nbytesikg));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(this->m_name), sizeof(this->m_name));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_objectId), sizeof(this->m_objectId));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_parent), sizeof(this->m_parent));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_flags), sizeof(this->m_flags));
	bytes += istream.gcount();
	bytes += this->m_translations->readMdx(istream);
	/// @todo Missing rotations
	bytes += this->m_rotations->readMdx(istream);
	bytes += this->m_scalings->readMdx(istream);
	istream.read(reinterpret_cast<char*>(&this->m_speed), sizeof(this->m_speed));
	bytes += istream.gcount();	
	istream.read(reinterpret_cast<char*>(&this->m_variation), sizeof(this->m_variation));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_latitidue), sizeof(this->m_latitidue));
	bytes += istream.gcount();
	
	istream.read(reinterpret_cast<char*>(&this->m_gravity), sizeof(this->m_gravity));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_lifespan), sizeof(this->m_lifespan));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_emissionRate), sizeof(this->m_emissionRate));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_length), sizeof(this->m_length));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_width), sizeof(this->m_width));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_filterMode), sizeof(this->m_filterMode));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_rows), sizeof(this->m_rows));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_columns), sizeof(this->m_columns));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_flag2), sizeof(this->m_flag2));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_tailLength), sizeof(this->m_tailLength));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_time), sizeof(this->m_time));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_tailLength), sizeof(this->m_tailLength));
	bytes += istream.gcount();
	
	for (int i = 0; i < 3; ++i) /// @todo Usually 3 segment colors?!
	{
		class SegmentColor *segmentColor = new SegmentColor(this);
		bytes += segmentColor->readMdx(istream);
		this->m_segmentColors.push_back(segmentColor);
	}

	istream.read(reinterpret_cast<char*>(&this->m_alpha1), sizeof(this->m_alpha1));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_alpha2), sizeof(this->m_alpha2));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_alpha3), sizeof(this->m_alpha3));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_scalingX), sizeof(this->m_scalingX));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_scalingY), sizeof(this->m_scalingY));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_scalingZ), sizeof(this->m_scalingZ));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_lifeSpanUvAnim1), sizeof(this->m_lifeSpanUvAnim1));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_lifeSpanUvAnim2), sizeof(this->m_lifeSpanUvAnim2));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_decayUvAnim3), sizeof(this->m_decayUvAnim3));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_decayUvAnim1), sizeof(this->m_decayUvAnim1));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_decayUvAnim2), sizeof(this->m_decayUvAnim2));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_decayUvAnim3), sizeof(this->m_decayUvAnim3));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_tailUvAnim1), sizeof(this->m_tailUvAnim1));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_tailUvAnim2), sizeof(this->m_tailUvAnim2));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_tailUvAnim3), sizeof(this->m_tailUvAnim3));
	bytes += istream.gcount();
		
	istream.read(reinterpret_cast<char*>(&this->m_tailDecayUvAnim1), sizeof(this->m_tailDecayUvAnim1));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_tailDecayUvAnim2), sizeof(this->m_tailDecayUvAnim2));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_tailDecayUvAnim3), sizeof(this->m_tailDecayUvAnim3));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_textureId), sizeof(this->m_textureId));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_squirt), sizeof(this->m_squirt));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_tailUvAnim3), sizeof(this->m_tailUvAnim3));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_priorityPlane), sizeof(this->m_priorityPlane));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_replaceableId), sizeof(this->m_replaceableId));
	bytes += istream.gcount();
	bytes += this->m_speeds->readMdx(istream);
	bytes += this->m_latitudes->readMdx(istream);
	bytes += this->m_emissionRates->readMdx(istream);
	bytes += this->m_visibilities->readMdx(istream);
	bytes += this->m_numbers->readMdx(istream);
	bytes += this->m_widths->readMdx(istream);
	
	return bytes;
}

long32 ParticleEmitter2::writeMdx(std::ostream &ostream) throw (class Exception)
{
	return 0;
}

}

}
