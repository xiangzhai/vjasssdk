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

#ifndef WC3LIB_MDLX_GEOSETANIMATION_HPP
#define WC3LIB_MDLX_GEOSETANIMATION_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class GeosetAnimations;
class Alpha;
class Color;

class GeosetAnimation
{
	public:
		enum ColorAnimation
		{
			None = 0,
			DropShadow = 1,
			Color = 2,
			Both = 3
		};

		GeosetAnimation(class GeosetAnimations *geosetAnimations);
		virtual ~GeosetAnimation();

		class GeosetAnimations* geosetAnimations() const;
		float32 staticAlpha() const;
		long32 colorAnimation() const;
		float32 colorRed() const;
		float32 colorGreen() const;
		float32 colorBlue() const;
		long32 geosetId() const;
		class Alpha* alpha() const;
		class Color* color() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual long32 readMdx(std::fstream &fstream) throw (class Exception);
		virtual long32 writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class GeosetAnimations *m_geosetAnimations;
		float32 m_staticAlpha; //(1.0:use KGAO)
		long32 m_colorAnimation; //(0:none;1:DropShadow;2:Color;3:Both)
		float32 m_colorRed, m_colorGreen, m_colorBlue; //(default:1)
		long32 m_geosetId;
		class Alpha *m_alpha; //(KGAO)
		class Color *m_color; //(KGAC)
};

inline class GeosetAnimations* GeosetAnimation::geosetAnimations() const
{
	return this->m_geosetAnimations;
}

inline float32 GeosetAnimation::staticAlpha() const
{
	return this->m_staticAlpha;
}

inline long32 GeosetAnimation::colorAnimation() const
{
	return this->m_colorAnimation;
}

inline float32 GeosetAnimation::colorRed() const
{
	return this->m_colorRed;
}

inline float32 GeosetAnimation::colorGreen() const
{
	return this->m_colorGreen;
}

inline float32 GeosetAnimation::colorBlue() const
{
	return this->m_colorBlue;
}

inline long32 GeosetAnimation::geosetId() const
{
	return this->m_geosetId;
}

inline class Alpha* GeosetAnimation::alpha() const
{
	return this->m_alpha;
}

inline class Color* GeosetAnimation::color() const
{
	return this->m_color;
}

}

}

#endif
