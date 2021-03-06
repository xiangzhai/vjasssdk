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

#ifndef WC3LIB_MAP_TILEPOINT_HPP
#define WC3LIB_MAP_TILEPOINT_HPP

#include "platform.hpp"

namespace wc3lib
{

namespace map
{

class Tilepoint : public Format
{
	public:
		enum Flags
		{
			ShadowBoundary = 0x4000,
			Ramp = 0x0010,
			Blight = 0x0020,
			Water = 0x0040,
			CameraBoundary = 0x0080
		};
	
		Tilepoint(class Environment *environment);
		
		std::streamsize read(InputStream &istream) throw (class Exception);
		std::streamsize write(OutputStream &ostream) const throw (class Exception);
		
		const class Position& position() const;
		int32 x() const;
		int32 y() const;

		short16 worldEditorHeight(short16 layer, short16 groundZeroLevel, short16 layerZeroLevel) const;
		float32 worldEditorWaterLevel(short16 groundZeroLevel, float32 waterZeroLevel) const;
		
	protected:
		friend class Environment;

		class Environment *m_environment;
		class Position m_position;
		short16 m_groundHeight;
		short16 m_waterLevel;
		enum Tilepoint::Flags m_flags;
		unsigned int m_groundTextureType:4;
		char8 m_textureDetails;
		unsigned int m_cliffTextureType:4;
		unsigned int m_layerHeight:4;

};

inline const class Position& Tilepoint::position() const
{
	return this->m_position;
}

inline int32 Tilepoint::x() const
{
	return this->m_position.first;
}

inline int32 Tilepoint::y() const
{
	return this->m_position.second;
}
	
inline short16 Tilepoint::worldEditorHeight(short16 layer, short16 groundZeroLevel, short16 layerZeroLevel) const
{
	return (this->m_groundHeight - groundZeroLevel + (layer - layerZeroLevel) * this->m_layerHeight) / 4;
}

inline float32 Tilepoint::worldEditorWaterLevel(short16 groundZeroLevel, float32 waterZeroLevel) const
{
	return (this->m_waterLevel - groundZeroLevel) / 4 + waterZeroLevel;
}

}

}

#endif
