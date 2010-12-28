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

#ifndef WC3LIB_MDLX_BONE_HPP
#define WC3LIB_MDLX_BONE_HPP

#include "object.hpp"

namespace wc3lib
{

namespace mdlx
{

/**
* Use two different bones for unit models: "bone_head" and "bone_chest". These two bones will also be used by Warcraft when lock body-part facing action is called.
* Use bone called "bone_turret" if you want a part of your model is rotated only when targeting another unit.
*/
class Bone : public Object
{
	public:
		Bone(class Bones *bones);
		virtual ~Bone();

		class Bones* bones() const;
		long32 geosetId() const;
		long32 geosetAnimationId() const;

		virtual std::streamsize readMdl(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class Bones *m_bones;
		long32 m_geosetId;
		long32 m_geosetAnimationId;
};

inline class Bones* Bone::bones() const
{
	return this->m_bones;
}

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
