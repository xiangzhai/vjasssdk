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

#ifndef WC3LIB_MDLX_MDLXROTATION_HPP
#define WC3LIB_MDLX_MDLXROTATION_HPP

#include <istream>
#include <ostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class MdlxRotation
{
	public:
		MdlxRotation(class MdlxRotations *rotations);
		virtual ~MdlxRotation();

		class MdlxRotations* rotations() const;
		float32 frame() const;
		const struct QuaternionData& quaternionData() const;
		const struct InterpolationRotationData& interpolationData() const;

		virtual std::streamsize readMdl(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class MdlxRotations *m_rotations;
		long32 m_frame;
		struct QuaternionData m_quaternionData;
		//if (LineType > 1)
		//{
		struct InterpolationRotationData m_interpolationData;
		//}
};

inline class MdlxRotations* MdlxRotation::rotations() const
{
	return this->m_rotations;
}

inline float32 MdlxRotation::frame() const
{
	return this->m_frame;
}

inline const struct QuaternionData& MdlxRotation::quaternionData() const
{
	return this->m_quaternionData;
}

inline const struct InterpolationRotationData&  MdlxRotation::interpolationData() const
{
	return this->m_interpolationData;
}

}

}

#endif
