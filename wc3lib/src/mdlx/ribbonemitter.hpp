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

#ifndef WC3LIB_MDLX_RIBBONEMITTER_HPP
#define WC3LIB_MDLX_RIBBONEMITTER_HPP

#include "node.hpp"

namespace wc3lib
{

namespace mdlx
{

class RibbonEmitter : public Node
{
	public:
		RibbonEmitter(class RibbonEmitters *ribbonEmitters);
		virtual ~RibbonEmitter();

		class RibbonEmitters* ribbonEmitters() const;

		virtual std::streamsize readMdl(istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(ostream &ostream) const throw (class Exception);

	protected:
		class RibbonEmitters *m_ribbonEmitters;
		//long nbytesi;
		//long nbytesikg; // inclusive bytecount including KGXXs
		class MdlxTranslations *m_translations; //(KGTR)
		class MdlxRotations *m_rotations; //(KGRT)
		class MdlxScalings *m_scalings; //(KGSC)
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
