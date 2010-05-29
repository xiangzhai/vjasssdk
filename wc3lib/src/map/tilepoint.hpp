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

#ifndef WC3LIB_MAP_TILEPOINT_HPP
#define WC3LIB_MAP_TILEPOINT_HPP

#include <istream>
#include <ostream>
#include <bitset>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace map
{
	
class Environment;

class Tilepoint
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
		
		std::streamsize read(std::istream &istream) throw (class Exception);
		std::streamsize write(std::ostream &ostream) const throw (class Exception);
		
		short16 worldEditorHeight(short16 layer, short16 groundZeroLevel, short16 layerZeroLevel) const;
		float32 worldEditorWaterLevel(short16 groundZeroLevel, float32 waterZeroLevel) const;
		
	protected:
		class Environment *m_environment;
		short16 m_groundHeight;
		short16 m_waterLevel;
		enum Tilepoint::Flags m_flags;
		unsigned int m_groundTextureType:4;
		char8 m_textureDetails;
		unsigned int m_cliffTextureType:4;
		unsigned int m_layerHeight:4;

};
	
inline short16 Tilepoint::worldEditorHeight(short16 layer, short16 groundZeroLevel, short16 layerZeroLevel) const
{
	return (this->m_groundHeight - groundZeroLevel + (layer - layerZeroLevel) * this->m_layerHeight.to_ulong()) / 4;
}

inline float32 Tilepoint::worldEditorWaterLevel(short16 groundZeroLevel, float32 waterZeroLevel) const
{
	return (this->m_waterLevel - groundZeroLevel) / 4 + waterZeroLevel;
}

}

}

#endif
