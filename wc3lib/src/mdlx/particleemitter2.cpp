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
#include <boost/foreach.hpp>

#include "particleemitter2.hpp"
#include "particleemitter2s.hpp"
#include "mdlxtranslations.hpp"
#include "mdlxrotations.hpp"
#include "mdlxscalings.hpp"
#include "segmentcolor.hpp"
#include "particleemitter2speeds.hpp"
#include "particleemitter2latitudes.hpp"
#include "emissionrates.hpp"
#include "particleemitter2visibilities.hpp"
#include "lengths.hpp"
#include "particleemitter2widths.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace mdlx
{

ParticleEmitter2::ParticleEmitter2(class ParticleEmitter2s *particleEmitters) : Node(particleEmitters->mdlx()), m_particleEmitters(particleEmitters), m_speeds(new ParticleEmitter2Speeds(this)), m_latitudes(new ParticleEmitter2Latitudes(this)), m_emissionRates(new EmissionRates(particleEmitters->mdlx())), m_visibilities(new ParticleEmitter2Visibilities(this)), m_numbers(new Lengths(this)), m_widths(new ParticleEmitter2Widths(this))
{
}

ParticleEmitter2::~ParticleEmitter2()
{
	BOOST_FOREACH(class SegmentColor *segmentColor, this->m_segmentColors)
		delete segmentColor;

	delete this->m_speeds;
	delete this->m_latitudes;
	delete this->m_emissionRates;
	delete this->m_visibilities;
	delete this->m_numbers;
	delete this->m_widths;
}

std::streamsize ParticleEmitter2::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize ParticleEmitter2::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize ParticleEmitter2::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = 0;
	long32 nbytesi;
	wc3lib::read(istream, nbytesi, size);
	long32 nbytesikg;
	wc3lib::read(istream, nbytesikg, size);
	wc3lib::read(istream, this->m_name, size);
	size += Node::readMdx(istream);
	wc3lib::read(istream, this->m_speed, size);
	wc3lib::read(istream, this->m_variation, size);
	wc3lib::read(istream, this->m_latitidue, size);
	wc3lib::read(istream, this->m_gravity, size);
	wc3lib::read(istream, this->m_lifespan, size);
	wc3lib::read(istream, this->m_emissionRate, size);
	wc3lib::read(istream, this->m_length, size);
	wc3lib::read(istream, this->m_width, size);
	wc3lib::read(istream, *reinterpret_cast<long32*>(&this->m_filterMode), size);
	wc3lib::read(istream, this->m_rows, size);
	wc3lib::read(istream, this->m_columns, size);
	wc3lib::read(istream, *reinterpret_cast<long32*>(&this->m_flags), size);
	wc3lib::read(istream, this->m_tailLength, size);
	wc3lib::read(istream, this->m_time, size);

	for (std::size_t i = 0; i < 3; ++i) /// @todo Usually 3 segment colors?!
	{
		class SegmentColor *segmentColor = new SegmentColor(this);
		size += segmentColor->readMdx(istream);
		this->m_segmentColors.push_back(segmentColor);
	}

	wc3lib::read(istream, this->m_alpha1, size);
	wc3lib::read(istream, this->m_alpha2, size);
	wc3lib::read(istream, this->m_alpha3, size);
	wc3lib::read(istream, this->m_scalingX, size);
	wc3lib::read(istream, this->m_scalingY, size);
	wc3lib::read(istream, this->m_scalingZ, size);
	wc3lib::read(istream, this->m_lifeSpanUvAnim1, size);
	wc3lib::read(istream, this->m_lifeSpanUvAnim2, size);
	wc3lib::read(istream, this->m_lifeSpanUvAnim3, size);
	wc3lib::read(istream, this->m_decayUvAnim1, size);
	wc3lib::read(istream, this->m_decayUvAnim2, size);
	wc3lib::read(istream, this->m_decayUvAnim3, size);
	wc3lib::read(istream, this->m_tailUvAnim1, size);
	wc3lib::read(istream, this->m_tailUvAnim2, size);
	wc3lib::read(istream, this->m_tailUvAnim3, size);
	wc3lib::read(istream, this->m_tailDecayUvAnim1, size);
	wc3lib::read(istream, this->m_tailDecayUvAnim2, size);
	wc3lib::read(istream, this->m_tailDecayUvAnim3, size);
	wc3lib::read(istream, this->m_textureId, size);
	wc3lib::read(istream, this->m_squirt, size);
	wc3lib::read(istream, this->m_priorityPlane, size);
	wc3lib::read(istream, this->m_replaceableId, size);
	size += this->m_speeds->readMdx(istream);
	size += this->m_latitudes->readMdx(istream);
	size += this->m_emissionRates->readMdx(istream);
	size += this->m_visibilities->readMdx(istream);
	size += this->m_numbers->readMdx(istream);
	size += this->m_widths->readMdx(istream);

	return size;
}

std::streamsize ParticleEmitter2::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	/*
	std::streampos position = istream.tellg();
	istream.seekg(2 * sizeof(long32), std::ios_base::dasdsadsd); /// @todo Skip!, nbytesi and nbytesikg
	*/

	return 0;
}

}

}
