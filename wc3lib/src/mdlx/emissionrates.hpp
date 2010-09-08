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

#ifndef WC3LIB_MDLX_EMISSIONRATES_HPP
#define WC3LIB_MDLX_EMISSIONRATES_HPP

#include "mdxalphas.hpp"

namespace wc3lib
{

namespace mdlx
{

/// KP2E same as KMTA (MdxAlphas)
class EmissionRates : public MdxAlphas
{
	public:
		EmissionRates(class Mdlx *mdlx);
		virtual ~EmissionRates();

		class Mdlx* mdlx() const;

		const std::list<class EmissionRate*>& emissionRates() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
};

inline class Mdlx* EmissionRates::mdlx() const
{
	return this->m_mdlx;
}

inline const std::list<class EmissionRate*>& EmissionRates::emissionRates() const
{
	return reinterpret_cast<const std::list<class EmissionRate*>&>(*&this->m_alphas);
}

}

}

#endif
