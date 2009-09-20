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

#ifndef WC3LIB_MAP_W3M_HPP
#define WC3LIB_MAP_W3M_HPP

#include <fstream>

#include "../exception.hpp"

namespace wc3lib
{

namespace mpq
{

class Mpq;

}

namespace blp
{

class Blp;

}

namespace map
{

class W3m
{
	public:
		enum Flags
		{
			HideMinimap = 0x0001,  //: 1=hide minimap in preview screens
			ModifyAllyPriorities = 0x0002, //: 1=modify ally priorities
			MeleeMap = 0x0004, //: 1=melee map
			LargePlayableMapSize = 0x0008, //: 1=playable map size was large and has never been reduced to medium
			PartiallyVisible = 0x0010, //: 1=masked area are partially visible
			FixedCustomForces = 0x0020, //: 1=fixed player setting for custom forces
			CustomForeces = 0x0040, //: 1=use custom forces
			CustomTechtree = 0x0080, //: 1=use custom techtree
			CustomAbilities = 0x0100, //: 1=use custom abilities
			CustomUpgrades = 0x0200, //: 1=use custom upgrades
			OpenedMapPropertiesMenu = 0x0400, //: 1=map properties menu opened at least once since map creation
			ShowWaterWavesOnCliffShores = 0x0800, //: 1=show water waves on cliff shores
			ShowWaterWavesOnRollingShores = 0x1000 //: 1=show water waves on rolling shores
		};

		virtual void read(class mpq::Mpq *mpq) throw (Exception);
		virtual void read(std::fstream &fstream) throw (Exception);

		/**
		* Creates an MPQ archive with map header and all required files.
		*/
		virtual void write(std::fstream &fstream) throw (std::exception);

	private:
		std::string m_name;
		int m_flags;
		int m_maxPlayers;
	
		class Environment *m_environment;
		class Shadow *m_shadow;
		class PathMap *m_pathMap;
		class Trees *m_trees;
		class Units *m_units;
		class Info *m_info;
		class Strings *m_strings;
		class blp::Blp *m_minimap;
		class MenuMinimap *m_menuMinimap;
		class CustomUnits *m_customUnits;
		class Triggers *m_triggers;
		class Cameras *m_cameras;
		class Rects *m_rects;
		class Sounds *m_sounds;
		class CustomTextTriggers *m_customTextTriggers;
		class ImportedFiles *m_importedFiles;
/*
w3x
		class ArtificialIntelligence *m_artificialIntelligence;
		class Misc *m_misc;
		class Skin *m_skin;
		class Extra *m_extra;
*/
};

}

}

#endif
