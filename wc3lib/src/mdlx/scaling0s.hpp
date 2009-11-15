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

#ifndef WC3LIB_MDLX_SCALING0S_HPP
#define WC3LIB_MDLX_SCALING0S_HPP

#include "mdxscalings.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;
class Scaling0;

//KGSC
class Scaling0s : public MdxScalings
{
	public:
		Scaling0s(class Mdlx *mdlx);
		virtual ~Scaling0s();

		class Mdlx* mdlx() const;
		const std::list<class Scaling0*>& scalings() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);

	protected:
		virtual class MdxScaling* createNewMember();
		
		class Mdlx *m_mdlx;
};

inline class Mdlx* Scaling0s::mdlx() const
{
	return this->m_mdlx;
}

inline const std::list<class Scaling0*>& Scaling0s::scalings() const
{
	return reinterpret_cast<const std::list<class Scaling0*>&>(this->m_scalings);
}

}

}

#endif
