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

#ifndef WC3LIB_MDLX_LIGHT_HPP
#define WC3LIB_MDLX_LIGHT_HPP

#include "object.hpp"

namespace wc3lib
{

namespace mdlx
{

class Light : public Object
{
	public:
		enum Type
		{
			Omnidirectional = 0,
			Directional = 1,
			Ambient = 2
		};

		Light(class Mdlx *mdlx);
		virtual ~Light();

		long32 type() const;
		float32 attStart() const;
		float32 attEnd() const;
		float32 colorRed() const;
		float32 colorGreen() const;
		float32 colorBlue() const;
		float32 intensity() const;
		float32 ambColorRed() const;
		float32 ambColorGreen() const;
		float32 ambColorBlue() const;
		float32 ambIntensity() const;
		class Intensity* intensity() const;
		class Visibility* visibility() const;
		class Color* color() const;
		class AmbientColor* ambientColor() const;
		class AmbientIntensity* ambientIntensity() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		//long nbytesi;
		//OBJ
		long32 m_type; //(0:Omnidirectional;1:Directional;2:Ambient)
		float32 m_attStart, m_attEnd;
		float32 m_colorRed, m_colorGreen, m_colorBlue;
		float32 m_intensity;
		float32 m_ambColorRed, m_ambColorGreen, m_ambColorBlue;
		float32 m_ambIntensity;
		class Intensity *m_intensity; //(KLAI)
		class Visibility *m_visibility; //(KLAV)
		class Color *m_color; //(KLAC)
		class AmbientColor *m_ambientColor; //(KLBC)
		class AmbientIntensity *m_ambientIntensity; //(KLBI)
};

inline long32 Light::type() const
{
	return this->m_type;
}

inline float32 Light::attStart() const
{
	return this->m_attStart;
}

inline float32 Light::attEnd() const
{
	return this->m_attEnd;
}

inline float32 Light::colorRed() const
{
	return this->m_colorRed;
}

inline float32 Light::colorGreen() const
{
	return this->m_colorBlue;
}

inline float32 Light::colorBlue() const
{
	return this->m_colorBlue;
}

inline float32 Light::intensity() const
{
	return this->m_intensity;
}

inline float32 Light::ambColorRed() const
{
	return this->m_ambColorRed;
}

inline float32 Light::ambColorGreen() const
{
	return this->m_ambColorGreen;
}

inline float32 Light::ambColorBlue() const
{
	return this->m_ambColorBlue;
}

inline float32 Light::ambIntensity() const
{
	return this->m_ambIntensity;
}

inline class Intensity* Light::intensity() const
{
	return this->m_intensity;
}

inline class Visibility* Light::visibility() const
{
	return this->m_visibility;
}

inline class Color* Light::color() const
{
	return this->m_color;
}

inline class AmbientColor* Light::ambientColor() const
{
	return this->m_ambientColor;
}

inline class AmbientIntensity* Light::ambientIntensity() const
{
	return this->m_ambientIntensity;
}

#endif
