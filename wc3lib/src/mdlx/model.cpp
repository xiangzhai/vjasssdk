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

#include <iostream> //debug

#include "model.hpp"
/// @todo Maybe we should one single "objects" include file.
#include "mdlx.hpp"
#include "geosets.hpp"
#include "geosetanimations.hpp"
#include "helpers.hpp"
#include "lights.hpp"
#include "bones.hpp"
#include "attachments.hpp"
#include "particleemitters.hpp"
#include "particleemitter2s.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Model::Model(class Mdlx *mdlx) : MdxBlock("MODL"), m_mdlx(mdlx)
{
}

Model::~Model()
{
}

void Model::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Model::writeMdl(std::fstream &fstream) throw (class Exception)
{
	fstream << "Model \"" << this->m_name << "\" {\n";

/*
	mdlx stuff
	if (this->mdlx()->geosets()->geosets().size() > 0)
		fstream << "\tNumGeosets " << this->mdlx()->geosets()->geosets().size() << ",\n";

	if (this->mdlx()->geosetAnimations()->geosetAnimations().size() > 0)
		fstream << "\tNumGeosetAnims " << this->mdlx()->geosetAnimations()->geosetAnimations().size() << ",\n";

	if (this->mdlx()->helpers()->helpers().size() > 0)
		fstream << "\tNumHelpers " << this->mdlx()->helpers()->helpers().size() << ",\n";

	if (this->mdlx()->lights()->lights().size() > 0)
		fstream << "\tNumLights " << this->lights()->lights().size() << ",\n";

	if (this->mdlx()->bones()->bones().size() > 0)
		fstream << "\tNumBones " << this->mdlx()->bones()->bones().size() << ",\n";

	if (this->mdlx()->attachments()->attachments().size() > 0)
		fstream << "\tNumAttachements " << this->mdlx()->attachments()->attachments().size() << ",\n";

	if (this->mdlx()->particleEmitters()->particleEmitters().size() > 0)
		fstream << "\tNumParticleEmitters " << this->mdlx()->particleEmitters()->particleEmitters().size() << ",\n";

	if (this->mdlx()->particleEmitter2s()->particleEmitters().size() > 0)
		fstream << "\tNumParticleEmitters2 " << this->mdlx()->particleEmitter2s()->particleEmitters().size() << ",\n";

	if (this->mdlx()->ribbonEmitters->ribbonEmitters.size() > 0)
		fstream << "\tNumRibbonEmitters " << this->mdlx()->ribbonEmitters->ribbonEmitters.size() << ",\n";

	if (this->mdlx()->events()->events().size() > 0)
		fstream << "\tNumEvents " << this->mdlx()->events()->events().size() << ",\n";
*/
	
	fstream << "\tBlendTime " << this->blendTime() << ",\n";

	if (this->minExtX() != 0.0 || this->minExtY() != 0.0 || this->minExtZ() != 0.0)
		fstream << "MinimumExtent { " << this->minExtX() << ", " << this->minExtY() << ", " << this->minExtZ() << " },\n";

	if (this->maxExtX() != 0.0 || this->maxExtY() != 0.0 || this->maxExtZ() != 0.0)
		fstream << "MaxmimumExtent { " << this->maxExtX() << ", " << this->maxExtY() << ", " << this->maxExtZ() << " },\n";

	if (this->boundsRadius()!= 0.0)
		fstream << "BoundsRadius " << this->boundsRadius() << ",\n";

	fstream << "}\n";
}

long32 Model::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	long32 nbytes = 0;
	fstream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	
	if (nbytes == 0)
		throw Exception(_("Model: 0 byte model."));
	
	bytes += fstream.gcount();
	fstream.read(this->m_name, sizeof(this->m_name));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_unknown0), sizeof(this->m_unknown0));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_boundsRadius), sizeof(this->m_boundsRadius));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_minExtX), sizeof(this->m_minExtX));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_minExtY), sizeof(this->m_minExtY));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_minExtZ), sizeof(this->m_minExtZ));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_maxExtX), sizeof(this->m_maxExtX));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_maxExtY), sizeof(this->m_maxExtY));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_maxExtZ), sizeof(this->m_maxExtZ));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_blendTime), sizeof(this->m_blendTime));
	bytes += fstream.gcount();
	
	if (bytes - 4 - sizeof(nbytes) != nbytes) //- identifier length
	{
		/// @todo Exception required?
		fprintf(stderr, _("Model: Warning - Read byte count doesn't fit with real byte count.\nRead byte count: %d.\nReal byte count: %d.\n"), nbytes, bytes);
	}
	
	return bytes;
}

long32 Model::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = 0;
	bytes += MdxBlock::writeMdx(fstream);
	long32 nbytes = sizeof(*this); //nbytes, excluding byte count
	fstream.write(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += sizeof(nbytes);
	fstream.write(this->m_name, sizeof(this->m_name));
	bytes += sizeof(this->m_name);
	fstream.write(reinterpret_cast<char*>(&this->m_unknown0), sizeof(this->m_unknown0));
	bytes += sizeof(this->m_unknown0);
	fstream.write(reinterpret_cast<char*>(&this->m_boundsRadius), sizeof(this->m_boundsRadius));
	bytes += sizeof(this->m_boundsRadius);
	fstream.write(reinterpret_cast<char*>(&this->m_minExtX), sizeof(this->m_minExtX));
	bytes += sizeof(this->m_minExtX);
	fstream.write(reinterpret_cast<char*>(&this->m_minExtY), sizeof(this->m_minExtY));
	bytes += sizeof(this->m_minExtY);
	fstream.write(reinterpret_cast<char*>(&this->m_minExtZ), sizeof(this->m_minExtZ));
	bytes += sizeof(this->m_minExtZ);
	fstream.write(reinterpret_cast<char*>(&this->m_maxExtX), sizeof(this->m_maxExtX));
	bytes += sizeof(this->m_maxExtX);
	fstream.write(reinterpret_cast<char*>(&this->m_maxExtY), sizeof(this->m_maxExtY));
	bytes += sizeof(this->m_maxExtY);
	fstream.write(reinterpret_cast<char*>(&this->m_maxExtZ), sizeof(this->m_maxExtZ));
	bytes += sizeof(this->m_maxExtZ);
	fstream.write(reinterpret_cast<char*>(&this->m_blendTime), sizeof(this->m_blendTime));
	bytes += sizeof(this->m_blendTime);
	
	return bytes;
}

}

}
