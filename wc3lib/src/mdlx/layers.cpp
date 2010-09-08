/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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

#include <boost/cast.hpp>
#include <boost/foreach.hpp>

#include "layers.hpp"
#include "layer.hpp"
#include "material.hpp"
#include "../internationalisation.hpp"
#include "../utilities.hpp"

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

std::streamsize Layers::readMdl(std::istream &istream) throw (class Exception)
{
	return 0;
}

std::streamsize Layers::writeMdl(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}


std::streamsize Layers::readMdx(std::istream &istream) throw (class Exception)
{
	std::streamsize size = MdxBlock::readMdx(istream);

	if (size == 0)
		return 0;

	long32 number = 0;
	wc3lib::read(istream, number, size);

	for ( ; number > 0; --number)
	{
		class Layer *layer = new Layer(this);
		std::streamsize readSize = layer->readMdx(istream);

		if (readSize == 0)
			throw Exception(_("Layers: 0 byte layer."));

		size += readSize;
		this->m_layers.push_back(layer);
	}

	return size;
}

std::streamsize Layers::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size = MdxBlock::writeMdx(ostream);
	long32 number = boost::numeric_cast<long32>(this->m_layers.size());
	wc3lib::write(ostream, number, size);

	BOOST_FOREACH(const class Layer *layer, this->m_layers)
		size += layer->writeMdx(ostream);

	return size;
}

}

}
