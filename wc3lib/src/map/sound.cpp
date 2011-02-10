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

#include "sound.hpp"
#include "../utilities.hpp"

namespace wc3lib
{

namespace map
{

std::streamsize Sound::read(std::basic_istream<byte> &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::readString(istream, this->m_name, size);
	wc3lib::readString(istream, this->m_file, size);
	string eax;
	wc3lib::readString(istream, eax, size);

	if (eax == "DefaultEAXON")
		this->m_eaxEffects = Default;
	else if (eax == "CombatSoundsEAX")
		this->m_eaxEffects = Combat;
	else if (eax == "KotoDrumsEAX")
		this->m_eaxEffects = Drums;
	else if (eax == "SpellsEAX")
		this->m_eaxEffects = Spells;
	else if (eax == "MissilesEAX")
		this->m_eaxEffects = Missiles;
	else if (eax == "HeroAcksEAX")
		this->m_eaxEffects = HeroSpeech;
	else if (eax == "DoodadsEAX")
		this->m_eaxEffects = Doodads;
	else
		throw Exception(boost::format(_("Sound: Unknown EAX effect \"%1%\".")) % eax);

	int32 flags;
	wc3lib::read(istream, flags, size);
	this->m_flags = (Flags)flags;
	wc3lib::read(istream, this->m_fadeInRate, size);
	wc3lib::read(istream, this->m_fadeOutRate, size);
	wc3lib::read(istream, this->m_volume, size);
	wc3lib::read(istream, this->m_pitch, size);
	wc3lib::read(istream, this->m_unknown0, size);
	int32 channel;
	wc3lib::read(istream, channel, size);
	this->m_channel = (Channel)channel;
	wc3lib::read(istream, this->m_minDistance, size);
	wc3lib::read(istream, this->m_maxDistance, size);
	wc3lib::read(istream, this->m_distanceCutoff, size);
	wc3lib::read(istream, this->m_unknown2, size);
	wc3lib::read(istream, this->m_unknown3, size);
	wc3lib::read(istream, this->m_unknown4, size);
	wc3lib::read(istream, this->m_unknown5, size);
	wc3lib::read(istream, this->m_unknown6, size);
	wc3lib::read(istream, this->m_unknown7, size);

	return size;
}

std::streamsize Sound::write(std::basic_ostream<byte> &ostream) const throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::writeString(ostream, this->m_name, size);
	wc3lib::writeString(ostream, this->m_file, size);
	string eax;

	switch (this->m_eaxEffects)
	{
		case Default:
			eax = "DefaultEAXON";

			break;

		case Combat:
			eax = "CombatSoundsEAX";

			break;

		case Drums:
			eax = "KotoDrumsEAX";

			break;

		case Spells:
			eax = "SpellsEAX";

			break;

		case Missiles:
			eax = "MissilesEAX";

			break;

		case HeroSpeech:
			eax = "HeroAcksEAX";

			break;

		case Doodads:
			eax = "DoodadsEAX";

			break;
	}

	wc3lib::writeString(ostream, eax, size);
	wc3lib::write<int32>(ostream, this->m_flags, size);
	wc3lib::write(ostream, this->m_fadeInRate, size);
	wc3lib::write(ostream, this->m_fadeOutRate, size);
	wc3lib::write(ostream, this->m_volume, size);
	wc3lib::write(ostream, this->m_pitch, size);
	wc3lib::write(ostream, this->m_unknown0, size);
	wc3lib::write<int32>(ostream, this->m_channel, size);
	wc3lib::write(ostream, this->m_minDistance, size);
	wc3lib::write(ostream, this->m_maxDistance, size);
	wc3lib::write(ostream, this->m_distanceCutoff, size);
	wc3lib::write(ostream, this->m_unknown2, size);
	wc3lib::write(ostream, this->m_unknown3, size);
	wc3lib::write(ostream, this->m_unknown4, size);
	wc3lib::write(ostream, this->m_unknown5, size);
	wc3lib::write(ostream, this->m_unknown6, size);
	wc3lib::write(ostream, this->m_unknown7, size);

	return size;
}

}

}
