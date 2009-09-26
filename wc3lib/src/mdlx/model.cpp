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

void Model::readMdx(std::fstream &fstream) throw (class Exception)
{
	MdxBlock::readMdx(fstream);
	long32 bytes;
	fstream >> bytes;
	fstream >> this->m_name;
	fstream >> this->m_unkown0;
	fstream >> this->m_boundsRadius;
	fstream >> this->m_minExtX;
	fstream >> this->m_minExtY;
	fstream >> this->m_minExtZ;
	fstream >> this->m_maxExtX;
	fstream >> this->m_maxExtY;
	fstream >> this->m_maxExtZ;
	fstream >> this->m_blendTime;
}

void Model::writeMdx(std::fstream &fstream) throw (class Exception)
{
	MdxBlock::writeMdx(fstream);
}

}

}
