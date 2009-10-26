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

#include <sstream>
#include <cstring>

#include <boost/tokenizer.hpp>

#include "attachment.hpp"
#include "attachments.hpp"
#include "visibility0s.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

Attachment::Attachment(class Attachments *attachments) : Object(attachments->mdlx()), m_attachments(attachments), m_visibilities(new Visibility0s(attachments->mdlx()))
{
}

Attachment::~Attachment()
{
	delete this->m_visibilities;
}

void Attachment::readMdl(std::fstream &fstream) throw (class Exception)
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

	for (std::size_t i = 0; i < sizeof(this->m_name); ++i)
		this->m_name[i] = (*iterator).c_str()[i];

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

void Attachment::writeMdl(std::fstream &fstream) throw (class Exception)
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

	//fstream << "\tTranslation { " << this->translations()->x() << ", " << this->translations()->y() << ", " << this->translations()->z() << " }\n";
	//fstream << "\tRotation { " << this->rotations()->a() << ", " << this->rotations()->b() << ", " << this->rotations()->c() << ", " << this->rotations()->d() << " }\n";
	//fstream << "\tScaling { " << this->scalings()->x() << ", " << this->scalings()->y() << ", " << this->scalings()->z() << " }\n";
	//fstream << "\tVisibility " << this->visibilities()->value() << '\n';
	fstream << "}\n";
}


long32 Attachment::readMdx(std::fstream &fstream) throw (class Exception)
{
	long32 nbytesi = 0;
	fstream.read(reinterpret_cast<char*>(&nbytesi), sizeof(nbytesi));
	long32 bytes = fstream.gcount();
	bytes += Object::readMdx(fstream);
	fstream.read(reinterpret_cast<char*>(&this->m_path), sizeof(this->m_path));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_unknown0), sizeof(this->m_unknown0));
	bytes += fstream.gcount();
	fstream.read(reinterpret_cast<char*>(&this->m_attachmentId), sizeof(this->m_attachmentId));
	bytes += fstream.gcount();
	bytes += this->m_visibilities->readMdx(fstream);
	
	if (bytes != nbytesi)
	{
		char message[50];
		sprintf(message, _("Attachment: File byte count is not equal to real byte count.\nFile byte count: %d.\nReal byte count: %d.\n"), nbytesi, bytes);
		
		throw Exception(message);
	}
	
	return bytes;
}

long32 Attachment::writeMdx(std::fstream &fstream) throw (class Exception)
{
	return 0;
}

}

}
