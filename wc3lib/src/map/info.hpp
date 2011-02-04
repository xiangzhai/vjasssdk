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

#ifndef WC3LIB_MAP_INFO_HPP
#define WC3LIB_MAP_INFO_HPP

#include "../format.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace map
{

/// \todo Implement InfoX for expansion.
class Info : public Format<byte>
{
	public:
		std::streamsize read(std::basic_istream<byte> &istream) throw (class Exception);
		std::streamsize write(std::basic_ostream<byte> &ostream) const throw (class Exception);

		static const int32 currentFileVersion = 13;
		static const string fileName = "war3map.i";
	protected:
		int32 m_version;
		int32 m_mapVersion; // number of saves (map version)
		int32 m_editorVersion; // editor version (little endian)
		string m_name;
		string m_author;
		string m_description;
		string m_playersRecommended;
		float32 m_cameraBoundsJASS[8];
		int32 m_cameraBounds[4];
		int32 m_playableWidth;
		int32 m_playableHeight;
		enum MapFlags m_flags;
		byte m_mainGroundType;
		int32 m_campaignBackgroundIndex;
		string m_loadingScreenText;
		string m_loadingScreenTitle;
		string m_loadingScreenSubtitle;
		int32 m_loadingScreenIndex;
		string m_prologueScreenText;
		string m_prologueScreenTitle;
		string m_prologueScreenSubtitle;
		std::list<class Player*> m_players;
		std::list<class Force*> m_forces;
		std::list<class UpgradeAvailability*> m_upgradeAvailabilities;
		std::list<class TechAvailability*> m_techAvailabilities;
		std::list<class RandomUnitTable*> m_randomUnitTables;
};

}

}

#endif
