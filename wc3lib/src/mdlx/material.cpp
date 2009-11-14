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

#include <cstdio>

#include "material.hpp"
#include "materials.hpp"
#include "layers.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Material::Material(class Materials *materials) : m_materials(materials), m_layers(new Layers(this))
{
}

Material::~Material()
{
	delete this->m_layers;
}

void Material::readMdl(std::istream &istream) throw (class Exception)
{
}

void Material::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 Material::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytesi = 0;
	istream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	long32 bytes = istream.gcount();
	
	if (nbytesi <= 0)
	{
		char message[50];
		sprintf(message, _("Material: Small byte count.\nBytes %d.\n"), nbytesi);
		
		throw Exception(message);
	}
	
	istream.read(reinterpret_cast<char*>(&this->m_priorityPlane), sizeof(this->m_priorityPlane));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_renderMode), sizeof(this->m_renderMode));
	bytes += istream.gcount();
	
	if (this->m_renderMode != 1 && this->m_renderMode != 16 && this->m_renderMode != 32)
		fprintf(stderr, _("Material: Warning, unknown render mode.\nRender mode %d.\n"), this->m_renderMode);
	
	bytes += this->m_layers->readMdx(istream);
	
	if (nbytesi != bytes)
		fprintf(stderr, _("Material: Real byte count is not equal to file byte count.\nReal byte count %d.\nFile byte count %d.\n"), bytes, nbytesi);
	
	return bytes;
}

long32 Material::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = 0;
	
	
	return bytes;
}

}

}

