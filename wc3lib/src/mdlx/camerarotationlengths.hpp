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

#ifndef WC3LIB_MDLX_CAMERAROTATIONLENGTHS_HPP
#define WC3LIB_MDLX_CAMERAROTATIONLENGTHS_HPP

#include "mdlxalphas.hpp"

namespace wc3lib
{

namespace mdlx
{

class CameraRotationLengths : public MdlxAlphas
{
	public:
		CameraRotationLengths(class Camera *camera);

		class Camera* camera() const;
		const std::list<class CameraRotationLength*>& lengths() const;

	private:
		virtual class MdlxAnimatedProperty* createAnimatedProperty();
		
		class Camera *m_camera;

};

inline class Camera* CameraRotationLengths::camera() const
{
	return this->m_camera;
}

inline const std::list<class CameraRotationLength*>& CameraRotationLengths::lengths() const
{
	return reinterpret_cast<const std::list<class CameraRotationLength*>&>(this->mdlxAlphas());
}

}

}

#endif
