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

#include <iostream> // debug

#include "layers.hpp"
#include "layer.hpp"
#include "material.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Layers::Layers(class Material *material) : MdxBlock("LAYS"), m_material(material)
{
}

Layers::~Layers()
{
}

void Layers::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Layers::writeMdl(std::fstream &fstream) throw (class Exception)
{
}


long32 Layers::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(fstream);
	
	if (bytes == 0)
		return 0;
	
	long32 nlays = 0;
	fstream.read(reinterpret_cast<char*>(&nlays), sizeof(nlays));
	bytes += fstream.gcount();
	
	for ( ; nlays > 0; --nlays)
	{
		std::cout << "Layer " << nlays - 1 << std::endl;
		class Layer *layer = new Layer(this);
		long32 readBytes = layer->readMdx(fstream);
		
		if (readBytes == 0)
			throw Exception(_("Layers: 0 byte layer."));
		
		bytes += readBytes;
		this->m_layers.push_back(layer);
	}
	
	return bytes;
}

long32 Layers::writeMdx(std::fstream &fstream) throw (class Exception)
{
	long32 bytes = 0;
	bytes += MdxBlock::readMdx(fstream);
	
	return bytes;
}

}

}
