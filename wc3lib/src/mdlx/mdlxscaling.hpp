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

#ifndef WC3LIB_MDLX_MDLXSCALING_HPP
#define WC3LIB_MDLX_MDLXSCALING_HPP

#include <istream>
#include <ostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class MdlxScaling
{
	public:
		MdlxScaling(class MdlxScalings *scalings);
		virtual ~MdlxScaling();

		std::streamsize readMdl(std::istream &istream) throw (class Exception);
		std::streamsize writeMdl(std::ostream &ostream) const throw (class Exception);
		std::streamsize readMdx(std::istream &istream) throw (class Exception);
		std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

		long32 frame() const;
		const struct VertexData& vertexData() const;
		const struct InterpolationData& interpolationData() const;

	protected:
		class MdlxScalings *m_scalings;
		long32 m_frame;
		struct VertexData m_vertexData;
		//if (LineType > 1) {
		struct InterpolationData m_interpolationData;
		//}
};

inline long32 MdlxScaling::frame() const
{
	return this->m_frame;
}

inline const struct VertexData& MdlxScaling::vertexData() const
{
	return this->m_vertexData;
}

inline const struct InterpolationData& MdlxScaling::interpolationData() const
{
	return this->m_interpolationData;
}

}

}

#endif
