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

#ifndef WC3LIB_MDLX_GANIMATION_HPP
#define WC3LIB_MDLX_GANIMATION_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Geoset;

class Ganimation
{
	public:
		Ganimation(class Geoset *geoset);
		virtual ~Ganimation();

		class Geoset* geoset() const;
		float32 boundsRadius() const;
		float32 minExtentX() const;
		float32 minExtentY() const;
		float32 minExtentZ() const;
		float32 maxExtentX() const;
		float32 maxExtentY() const;
		float32 maxExtentZ() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Geoset *m_geoset;
		float32 m_boundsRadius;
		float32 m_minExtentX, m_minExtentY, m_minExtentZ;
		float32 m_maxExtentX, m_maxExtentY, m_maxExtentZ;
};

class Geoset* Ganimation::geoset() const
{
	return this->m_geoset;
}

float32 Ganimation::boundsRadius() const
{
	return this->m_boundsRadius;
}

float32 Ganimation::minExtentX() const
{
	return this->m_minExtentX;
}

float32 Ganimation::minExtentY() const
{
	return this->m_minExtentY;
}

float32 Ganimation::minExtentZ() const
{
	return this->m_minExtentZ;
}

float32 Ganimation::maxExtentX() const
{
	return this->m_maxExtentX;
}

float32 Ganimation::maxExtentY() const
{
	return this->m_maxExtentY;
}

float32 Ganimation::maxExtentZ() const
{
	return this->m_maxExtentZ;
}

}

}

#endif
