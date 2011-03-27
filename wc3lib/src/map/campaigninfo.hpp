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

#ifndef WC3LIB_MAP_CAMPAIGNINFO_HPP
#define WC3LIB_MAP_CAMPAIGNINFO_HPP

#include <list>

#include "../format.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace map
{

class CampaignInfo : public Format
{
	public:
		enum DifficultyType
		{
			Fixed,
			Variable
		};

		/// \todo Get missing race indices.
		enum Race
		{
			Human = 0
		};

		class MapTitle : public Format
		{
			public:
				MapTitle();
				virtual std::streamsize read(InputStream &istream) throw (class Exception);
				virtual std::streamsize write(OutputStream &ostream) const throw (class Exception);

			protected:
				bool m_isVisibleFromTheBeginning;
				string m_chapterTitle;
				string m_mapTitle;
				string m_path; // path of the map in the campaign archive
		};

		class Map : public Format
		{
			public:
				Map();
				virtual std::streamsize read(InputStream &istream) throw (class Exception);
				virtual std::streamsize write(OutputStream &ostream) const throw (class Exception);

			protected:
				string m_unknown; // unknown (always empty, might also be a single character)
				string m_path; // String: path of the map in the campaign archive
		};

		CampaignInfo(class Campaign *campaign);
		~CampaignInfo();
		virtual std::streamsize read(InputStream &istream) throw (class Exception);
		virtual std::streamsize write(OutputStream &ostream) const throw (class Exception);

		virtual int32 latestFileVersion() const;
		virtual const char8* fileName() const;

	protected:
		class Campaign *m_campaign;
		int32 m_version;
		int32 m_campaignVersion;
		int32 m_editorVersion;
		string m_name;
		string m_difficulty;
		string m_author;
		string m_description;
		enum DifficultyType m_difficultyType;
		bool m_hasW3x;
		int32 m_backgroundScreenIndex; // campaign background screen index (-1 = none or custom path)
		string m_backgroundScreen; // path of custom background screen (empty if none or preset)
		string m_minimapPicture; // path of minimap picture (empty = none)
		int32 m_ambientSoundIndex; // ambient sound index (-1 = imported file, 0 = none, greater 0 = preset index)
		string m_ambientSound; // path of custom imported ambient sound mp3
		int32 m_terrainFogIndex; // (0 = not used, greater 0 = index of terrain fog style dropdown box)
		float32 m_fogStartZHeight;
		float32 m_fogEndZHeight;
		float32 m_fogDensity;
		struct Rgba m_fogColor;
		enum Race m_race;
		std::list<class MapTitle*> m_mapTitles;
		std::list<class Map*> m_maps;
};

inline int32 CampaignInfo::latestFileVersion() const
{
	return 1;
}

inline const char8* CampaignInfo::fileName() const
{
	return "war3campaign.w3f";
}

}

}

#endif
