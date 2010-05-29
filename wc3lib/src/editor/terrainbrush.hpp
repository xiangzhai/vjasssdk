/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#ifndef WC3LIB_EDITOR_TERRAINBRUSH_HPP
#define WC3LIB_EDITOR_TERRAINBRUSH_HPP

#include "brush.hpp"
#include "../map/platform.hpp"
#include "../map/tilepoint.hpp"

namespace wc3lib
{

namespace editor
{

class TerrainBrush : public Brush
{
	public:
		TerrainBrush(class TerrainEditor *terrainEditor);

	protected:
		map::uint16_t m_groundHeight;
		bool m_boundary:1;
		map::uint16_t waterLevel:15;
		enum map::Tilepoint::Flags m_flags;
		map::uint16_t groundTextureType:4;
		map::uint8_t textureDetails;
		map::uint16_t m_cliffTextureType:4;
		map::uint16_t m_layerHeight:4;
};

}

}

#endif
