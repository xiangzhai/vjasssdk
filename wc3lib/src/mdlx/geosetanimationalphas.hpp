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

#ifndef WC3LIB_MDLX_GEOSETANIMATIONALPHAS_HPP
#define WC3LIB_MDLX_GEOSETANIMATIONALPHAS_HPP

#include <list>

#include "mdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{
	
class GeosetAnimation;
class GeosetAnimationAlphas;

//KGAO
class GeosetAnimationAlphas : public MdxBlock
{
	public:
		GeosetAnimationAlphas(class GeosetAnimation *geosetAnimation);
		virtual ~GeosetAnimationAlphas();

		class GeosetAnimation* geosetAnimation() const;
		std::list<class GeosetAnimationAlpha*> geosetAnimationAlphas() const;
		long32 lineType() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual long32 readMdx(std::fstream &fstream) throw (class Exception);
		virtual long32 writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class GeosetAnimation *m_geosetAnimation;
		//long	nunks;
		long32 m_lineType; //(0:don't interp;1:linear;2:hermite;3:bezier)
		long32 m_globalSequenceId;
		std::list<class GeosetAnimationAlpha*> m_geosetAnimationAlphas;
};

inline class GeosetAnimation* GeosetAnimationAlphas::geosetAnimation() const
{
	return this->m_geosetAnimation;
}

inline long32 GeosetAnimationAlphas::lineType() const
{
	return this->m_lineType;
}

inline std::list<class GeosetAnimationAlpha*> GeosetAnimationAlphas::geosetAnimationAlphas() const
{
	return this->m_geosetAnimationAlphas;
}

}

}

#endif
