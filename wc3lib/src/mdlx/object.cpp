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

#include "object.hpp"
#include "translation1s.hpp"
#include "rotation0s.hpp"
#include "scaling0s.hpp"
#include "attachmentvisibilities.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Object::Object(class Mdlx *mdlx) : m_mdlx(mdlx), m_translations(new Translation1s(mdlx)), m_rotations(new Rotation0s(this->mdlx())), m_scalings(new Scaling0s(this->mdlx())), m_visibilities(new AttachmentVisibilities(this->mdlx()))
{
}

Object::~Object()
{
	delete this->m_translations;
	delete this->m_rotations;
	delete this->m_scalings;
	delete this->m_visibilities;
}

void Object::readMdl(std::istream &istream) throw (class Exception)
{
}

void Object::writeMdl(std::ostream &ostream) const throw (class Exception)
{
}

std::streamsize Object::readMdx(std::istream &istream) throw (class Exception)
{
	long32 nbytesi = 0;
	istream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	std::streamsize bytes = istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_name), sizeof(this->m_name));
	bytes += istream.gcount();
	std::cout << "Object name is " << this->m_name << " and nbytesi is " << nbytesi << std::endl;
	istream.read(reinterpret_cast<char*>(&this->m_objectId), sizeof(this->m_objectId));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_parent), sizeof(this->m_parent));
	bytes += istream.gcount();
	istream.read(reinterpret_cast<char*>(&this->m_type), sizeof(this->m_type));
	bytes += istream.gcount();
	
	/// @todo All those blocks seem to be optional, model Krieger.mdx doesn contain them for its bones.
	if (!this->inheritsTranslation())
		bytes += this->m_translations->readMdx(istream);
	
	if (!this->inheritsRotation())
		bytes += this->m_rotations->readMdx(istream);
	
	if (!this->inheritsScaling())
		bytes += this->m_scalings->readMdx(istream);
	
	bytes += this->m_visibilities->readMdx(istream);
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

std::streamsize Object::writeMdx(std::ostream &ostream) const throw (class Exception)
{
	return 0;
}

}

}
