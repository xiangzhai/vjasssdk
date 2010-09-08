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

#include "light.hpp"
#include "lights.hpp"
#include "intensities.hpp"
#include "lightambientvisibilities.hpp"
#include "lightambientcolors.hpp"
#include "ambientcolors.hpp"
#include "ambientintensities.hpp"

namespace wc3lib
{

namespace mdlx
{

Light::Light(class Lights *lights) : Object(lights->mdlx()), m_lights(lights), m_intensities(new Intensities(this)), m_visibilities(new LightAmbientVisibilities(this)), m_colors(new LightAmbientColors(this)), m_ambientColors(new AmbientColors(this)), m_ambientIntensities(new AmbientIntensities(this))
{
}

Light::~Light()
{
	delete this->m_intensities;
	delete this->m_visibilities;
	delete this->m_colors;
	delete this->m_ambientColors;
	delete this->m_ambientIntensities;
}

std::streamsize Light::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Light::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

std::streamsize Light::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytesi;
	istream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	std::streamsize bytes = istream.gcount();
	bytes += Object::readMdx(istream);
	istream.read(reinterpret_cast<char*>(&this->m_type), sizeof(this->m_type));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_attStart), sizeof(this->m_attStart));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_attEnd), sizeof(this->m_attEnd));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_colorRed), sizeof(this->m_colorRed));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_colorGreen), sizeof(this->m_colorGreen));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_colorBlue), sizeof(this->m_colorBlue));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_intensity), sizeof(this->m_intensity));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_ambColorRed), sizeof(this->m_ambColorRed));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_ambColorGreen), sizeof(this->m_ambColorGreen));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_ambColorBlue), sizeof(this->m_ambColorBlue));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_ambIntensity), sizeof(this->m_ambIntensity));
	bytes += istream.gcount();
	bytes += this->m_intensities->readMdx(istream);
	bytes += this->m_visibilities->readMdx(istream);
	bytes += this->m_colors->readMdx(istream);
	bytes += this->m_ambientColors->readMdx(istream);
	bytes += this->m_ambientIntensities->readMdx(istream);

	return bytes;
}

std::streamsize Light::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
