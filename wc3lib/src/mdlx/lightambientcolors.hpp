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

#ifndef WC3LIB_MDLX_LIGHTAMBIENTCOLORS_HPP
#define WC3LIB_MDLX_LIGHTAMBIENTCOLORS_HPP

#include "mdlxscalings.hpp"

namespace wc3lib
{

namespace mdlx
{

class Light;
class LightAmbientColor;

/// KLAC
class LightAmbientColors : public MdlxScalings
{
	public:
		LightAmbientColors(class Light *light);
		virtual ~LightAmbientColors();

		class Light* light() const;
		const std::list<class LightAmbientColor*>& colors() const;

		virtual std::streamsize readMdl(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(std::ostream &ostream) const throw (class Exception);

	protected:
		virtual class MdlxScaling* createNewMember();

		class Light *m_light;
};

inline class Light* LightAmbientColors::light() const
{
	return this->m_light;
}

inline const std::list<class LightAmbientColor*>& LightAmbientColors::colors() const
{
	return reinterpret_cast<const std::list<class LightAmbientColor*>&>(*&this->m_scalings);
}

}

}

#endif
