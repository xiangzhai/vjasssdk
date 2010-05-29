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

#ifndef WC3LIB_MDLX_RIBBONEMITTER_HPP
#define WC3LIB_MDLX_RIBBONEMITTER_HPP

#include <istream>
#include <ostream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class RibbonEmitters;
class Translation1s;
class Rotation0s;
class Scaling0s;
class RibbonEmitterVisibilities;
class RibbonEmitterHeightsAbove;
class RibbonEmitterHeightsBelow;

class RibbonEmitter
{
	public:
		RibbonEmitter(class RibbonEmitters *ribbonEmitters);
		virtual ~RibbonEmitter();

		class RibbonEmitters* ribbonEmitters() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class RibbonEmitters *m_ribbonEmitters;
		//long nbytesi;
		//long nbytesikg; // inclusive bytecount including KGXXs
		ascii m_name[0x50]; //(0x50 bytes)
		long32 m_objectId;
		long32 m_parent; //(0xFFFFFFFF if none)
		long32 m_flags; //(0x00400000)
		class Translation1s *m_translations; //(KGTR)
		class Rotation0s *m_rotations; //(KGRT)
		class Scaling0s *m_scalings; //(KGSC)
		float32 m_heightAboveValue;
		float32 m_heightBelowValue;
		float32 m_alpha;
		float32 m_colorRed, m_colorGreen, m_colorBlue;
		float32 m_lifeSpan;
		long32 m_unknown0; //(0)
		long32 m_emissionRate;
		long32 m_rows;
		long32 m_columns;
		long32 m_materialId;
		float32 m_gravity;
		class RibbonEmitterVisibilities *m_visibilities; //(KRVS)
		class RibbonEmitterHeightsAbove *m_heightsAbove; //(KRHA)
		class RibbonEmitterHeightsBelow *m_heightsBelow; //(KRHB)
};

inline class RibbonEmitters* RibbonEmitter::ribbonEmitters() const
{
	return this->m_ribbonEmitters;
}

}

}

#endif
