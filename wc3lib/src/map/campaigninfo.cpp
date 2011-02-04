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

#include "campaigninfo.hpp"

namespace wc3lib
{

namespace map
{

CampaignInfo::MapTitle::MapTitle() : m_isVisibleFromTheBeginning(false)
{
}

std::streamsize CampaignInfo::MapTitle::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read<int32>(istream, this->m_isVisibleFromTheBeginning, size);
	readString(istream, this->m_chapterTitle, size);
	readString(istream, this->m_mapTitle, size);
	readString(istream, this->m_path, size);

	return size;
}

std::streamsize CampaignInfo::MapTitle::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	std::streamsize size = 0;
	write<int32>(ostream, this->m_isVisibleFromTheBeginning, size);
	writeString(ostream, this->m_chapterTitle, size);
	writeString(ostream, this->m_mapTitle, size);
	writeString(ostream, this->m_path, size);

	return size;
}

CampaignInfo::Map::Map()
{
}

std::streamsize CampaignInfo::Map::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = 0;
	readString(istream, this->m_unknown, size);
	readString(istream, this->m_path, size);

	return size;
}

std::streamsize CampaignInfo::Map::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	std::streamsize size = 0;
	writeString(ostream, this->m_unknown, size);
	writeString(ostream, this->m_path, size);

	return size;
}

CampaignInfo::CampaignInfo(class Campaign *campaign) : m_campaign(campaign)
{
}

CampaignInfo::~CampaignInfo()
{
	BOOST_FOREACH(class MapTitle *mapTitle, this->m_mapTitles);
		delete mapTitle;

	BOOST_FOREACH(class Map *map, this->m_maps);
		delete map;
}

std::streamsize CampaignInfo::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read(istream, this->m_version, size);
	wc3lib::read(istream, this->m_campaignVersion, size);
	wc3lib::read(istream, this->m_editorVersion, size);
	readString(istream, this->m_name, size);
	readString(istream, this->m_difficulty, size);
	readString(istream, this->m_author, size);
	readString(istream, this->m_description, size);
	in32 flag;
	wc3lib::read(istream, flag, size);

	switch (flag)
	{
		case 0: // =Fixed Diffculty, Only w3m maps
			this->m_difficultyType = Fixed;
			this->m_hasW3x = false;

			break;
		case 1: // =Variable Difficulty, Only w3m maps
			this->m_difficultyType = Variable;
			this->m_hasW3x = false;

			break;
		case 2: // =Fixed Difficulty, Contains w3x maps
			this->m_difficultyType = Fixed;
			this->m_hasW3x = true;

			break;
		case 3: // =Variable Difficulty, Contains w3x maps
			this->m_difficultyType = Variable;
			this->m_hasW3x = true;

			break;

		default:
			throw Exception(boost::format(_("Campaign info: Unknown flag: %1%.")) % flag);
	}

	wc3lib::read(istream, this->m_backgroundScreenIndex, size);
	readString(istream, this->m_backgroundScreen, size);
	readString(istream, this->m_minimapPicture, size);
	wc3lib::read(istream, this->m_ambientSoundIndex, size);
	readString(istream, this->m_ambientSound, size);
	wc3lib::read(istream, this->m_terrainFogIndex, size);
	wc3lib::read(istream, this->m_fogStartZHeight, size);
	wc3lib::read(istream, this->m_fogEndZHeight, size);
	wc3lib::read(istream, this->m_fogDensity, size);
	wc3lib::read(istream, this->m_fogColor, size);
	wc3lib::read<int32>(istream, this->m_race, size);

	int32 mapTitles;
	wc3lib::read(istream, mapTitles, size);
	int32 maps;
	wc3lib::read(istream, maps, size);

	while (mapTitles > 0)
	{
		class MapTitle *mapTitle = new MapTitle();
		mapTitle->read(istream);
		this->m_mapTitles.push_back(mapTitle);
		--mapTitles;
	}

	while (maps > 0)
	{
		class Map *map = new Map();
		map->read(istream);
		this->m_maps.push_back(map);
		--maps;
	}

	return size;
}

std::streamsize CampaignInfo::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	/// \todo Complete!
	return 0;
}

}

}
