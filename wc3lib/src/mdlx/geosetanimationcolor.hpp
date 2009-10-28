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

#ifndef WC3LIB_MDLX_GEOSETANIMATIONCOLOR_HPP
#define WC3LIB_MDLX_GEOSETANIMATIONCOLOR_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace mdlx
{

class GeosetAnimationColors;

class GeosetAnimationColor
{
	public:
		GeosetAnimationColor(class GeosetAnimationColors *geosetAnimationColors);
		
		class GeosetAnimationColors* geosetAnimationColors() const;
		
		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual long32 readMdx(std::fstream &fstream) throw (class Exception);
		virtual long32 writeMdx(std::fstream &fstream) throw (class Exception);
		
	private:
		class GeosetAnimationColors *m_geosetAnimationColors;
		long32 m_frame;
		float32	m_x, m_y, m_z;
		//if (LineType > 1) {
		float32 m_inTanX, m_inTanY, m_inTanZ;
		float32	m_outTanX, m_outTanY, m_outTanZ;
		//}
};

}

}

#endif
