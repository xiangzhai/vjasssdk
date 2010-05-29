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

#ifndef WC3LIB_MDLX_OGREMDLX_HPP
#define WC3LIB_MDLX_OGREMDLX_HPP

#include <OgreMesh.h>

#include "../exception.hpp"

namespace wc3lib
{
	
namespace mdlx
{
	
class Mdlx;

/**
* This class can be used to display MDLX models by using the OGRE 3d rendering engine.
*/
class OgreMdlx
{
	public:
		OgreMdlx(class Mdlx *mdlx);
		
		class Mdlx* mdlx() const;
		
		/**
		* Loads and analyses all data of corresponding MDLX model and refreshes displayed OGRE mesh.
		*/
		void refresh() throw (class Exception);
		
	protected:
		class Mdlx *m_mdlx;
		class Ogre::MeshPtr m_mesh;
};

inline class Mdlx* OgreMdlx::mdlx() const
{
	return this->m_mdlx;
}

}

}

#endif
