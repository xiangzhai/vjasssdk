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

#ifndef WC3LIB_MDLX_TRANSLATION0S_HPP
#define WC3LIB_MDLX_TRANSLATION0S_HPP

#include <istream>
#include <ostream>
#include <list>

#include "mdxscalings.hpp"

namespace wc3lib
{

namespace mdlx
{

class Camera;
class Translation0;

/**
* KCTR, like KGSC (Scalings)
* Camera translations.
*/
class Translation0s : public MdxScalings
{
	public:
		Translation0s(class Camera *camera);
		virtual ~Translation0s();

		class Camera* camera() const;
		
		const std::list<class Translation0*>& translations() const;

	protected:
		virtual class MdxScaling* createNewMember();
		
		class Camera *m_camera;
};

inline class Camera* Translation0s::camera() const
{
	return this->m_camera;
}

inline const std::list<class Translation0*>& Translation0s::translations() const
{
	return reinterpret_cast<const std::list<class Translation0*>&>(*&this->m_scalings);
}

}

}

#endif

