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

#include <cstring>

#include <boost/tokenizer.hpp>

#include "attachment.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

Attachment::Attachment(class Mdlx *mdlx) : Object(mdlx), m_visibility(0)
{
}

Attachment::~Attachment()
{
	if (this->m_visibility != 0)
		delete this->m_visibility;
}

void Attachment::readMdl(std::fstream &fstream) throw (Exception)
{
	std::string line;
	std::getline(fstream, line);
	boost::tokenizer<> tokenizer(line);
	boost::tokenizer<>::iterator iterator = tokenizer.begin();

	if ((*iterator) != "Attachment")
		throw Exception("");

	++iterator;

	if (iterator == tokenizer.end() || sizeof((*iterator).c_str()) > 0x100) /// @todo Name size is bigger than in object?! path length?
		throw Exception("");

	this->m_name = (*iterator).c_str();
	++iterator;

	if ((*iterator) != "{")
		throw Exception("");

	std::getline(fstream, line);
	tokenizer.assign(line);
	iterator = tokenizer.begin();

	if ((*iterator) != "ObjectId")
		throw Exception("");

	++iterator;
	std::stringstream sstream;
	sstream << (*iterator);
	sstream >> this->m_objectId;
}

void Attachment::readMdx(std::fstream &fstream) throw (Exception)
{
	long32 bytes; //nbytesi;
	fstream >> bytes;
	Object::readMdx(fstream);
	fstream >> this->m_path;
	fstream >> this->m_unknown0;
	fstream >> this->m_attachmentId;
	this->m_visibility = new Visibility(this->mdlx());
	this->m_visibility.readMdx(fstream);
}

void Attachment::writeMdl(std::fstream &fstream) throw (Exception)
{
	// Observe properties of an Object.
	// Path only appears if its length is greater than 0. 
	// Maximum size is 256 characters (0x100 bytes)
	// I am unsure as to how it is determined that AttachmentID be shown...
	// NightElfCampaign3D and UndeadCampaign3D.mdl are the only two MDLs
	// that utilize this attribute. Their only exclusive similarity is the
	// underscore prefixing their name string. "_Blah"
	fstream
	<< "Attachment " << this->name() << " {\n"
	<< "\tObjectId " << this->objectId() << ",\n"
	;

	if (this->parent() != 0xFFFFFFFF)
		fstream << "\tParent " << this->parent() << ",\n";

	if (this->type() & Object::BillboardedLockZ)
		fstream << "\tBillboardedLockZ,\n";

	if (this->type() & Object::BillboardedLockY)
		fstream << "\tBillboardedLockY,\n";
	
	if (this->type() & Object::BillboardedLockX)
		fstream << "\tBillboardedLockX,\n";
	
	if (this->type() & Object::Billboarded)
		fstream << "\tBillboarded,\n";
	
	if (this->type() & Object::CameraAnchored)
		fstream << "\tCameraAnchored,\n";

	if (this->type() & Object::DontInheritRotation)
		fstream << "\tDontInherit { Rotation },\n";
	else if (this->type() & Object::DontInheritTranslation)
		fstream << "\tDontInherit { Translation },\n";
	else if (this->type() & Object::DontInheritScaling)
		fstream << "\tDontInherit { Scaling },\n";

	fstream << "\tAttachmentID " << this->attachmentId() << ",\n";

	if (strlen(this->path()) > 0)
		fstream << "\tPath " << this->path() << ",\n";

	fstream << "\tTranslation { " << this->translation()->x() << ", " << this->translation()->y() << ", " << this->translation()->z() << " }\n";
	fstream << "\tRotation { " << this->rotation()->a() << ", " << this->rotation()->b() << ", " << this->rotation()->c() << ", " << this->rotation()->d() << " }\n";
	fstream << "\tScaling { " << this->scaling()->x() << ", " << this->scaling()->y() << ", " << this->scaling()->z() << " }\n";
	fstream << "\tVisibility " << this->visibility()->value() << '\n';
	fstream << "}\n";
}

void Attachment::writeMdx(std::fstream &fstream) throw (Exception)
{
}

}

}
