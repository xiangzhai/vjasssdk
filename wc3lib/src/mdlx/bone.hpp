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

#ifndef WC3LIB_MDLX_BONE_HPP
#define WC3LIB_MDLX_BONE_HPP

#include "object.hpp"

namespace wc3lib
{

namespace mdlx
{

class Bone : public Object
{
	public:
		Bone(class Mdlx *mdlx);
		virtual ~Bone();
		inline long32 geosetId() const;
		inline long32 geosetAnimationId() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		long32 m_geosetId;
		long32 m_geosetAnimationId;
};

inline long32 Bone::geosetId() const
{
	return this->m_geosetId;
}

inline long32 Bone::geosetAnimationId() const
{
	return this->m_geosetAnimationId;
}

}

}
#endif
