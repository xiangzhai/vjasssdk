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

#include "object.hpp"
#include "translation1s.hpp"
#include "rotation0s.hpp"
#include "scaling0s.hpp"
#include "visibility0s.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Object::Object(class Mdlx *mdlx) : m_mdlx(mdlx), m_translations(new Translation1s(mdlx)), m_rotations(new Rotation0s(this->mdlx())), m_scalings(new Scaling0s(this->mdlx())), m_visibilities(new Visibility0s(this->mdlx()))
{
}

Object::~Object()
{
	delete this->m_translations;
	delete this->m_rotations;
	delete this->m_scalings;
	delete this->m_visibilities;
}

void Object::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Object::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

long32 Object::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 nbytesi = 0;
	fstream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	long32 bytes = fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_name), sizeof(this->m_name));
	bytes += fstream.gcount();
	std::cout << "Object name is " << this->m_name << " and nbytesi is " << nbytesi << std::endl;
	fstream.read(reinterpret_cast<char*>(&this->m_objectId), sizeof(this->m_objectId));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_parent), sizeof(this->m_parent));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_type), sizeof(this->m_type));
	bytes += fstream.gcount();
	/// @todo All those blocks seem to be optional, model Krieger.mdx doesn contain them for its bones.
	bytes += this->m_translations->readMdx(fstream);
	bytes += this->m_rotations->readMdx(fstream);
	bytes += this->m_scalings->readMdx(fstream);
	bytes += this->m_visibilities->readMdx(fstream);
	/*	
	if (bytes != nbytesi)
	{
		std::cout << "test 1 and bytes are " << bytes << std::endl;
		char message[50];
		sprintf(message, _("Object: File byte count is not equal to real byte count.\nFile byte count: %d.\nReal byte count: %d.\n"), nbytesi, bytes);
		std::cout << "test 2" << std::endl;
		
		throw Exception(message);
	}
	*/
	
	return bytes;
}

long32 Object::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
