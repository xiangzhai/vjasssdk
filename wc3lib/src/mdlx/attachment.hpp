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

#ifndef WC3LIB_MDLX_ATTACHMENT_HPP
#define WC3LIB_MDLX_ATTACHMENT_HPP

#include "object.hpp"

namespace wc3lib
{

namespace mdlx
{

class Attachments;
class Visibility0s;

class Attachment : public Object
{
	public:
		Attachment(class Attachments *attachments);
		virtual ~Attachment();
		
		class Attachments* attachments() const;
		const ascii* path() const;
		long32 unknown0() const;
		long32 attachmentId() const;
		class Visibility0s* visibilities() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

	protected:
		class Attachments *m_attachments;
		ascii m_path[0x100];
		long32 m_unknown0;
		long32 m_attachmentId;
		class Visibility0s *m_visibilities; //(KATV)
};

inline class Attachments* Attachment::attachments() const
{
	return this->m_attachments;
}

inline const ascii* Attachment::path() const
{
	return this->m_path;
}

inline long32 Attachment::unknown0() const
{
	return this->m_unknown0;
}

inline long32 Attachment::attachmentId() const
{
	return this->m_attachmentId;
}

inline class Visibility0s* Attachment::visibilities() const
{
	return this->m_visibilities;
}

}

}

#endif
