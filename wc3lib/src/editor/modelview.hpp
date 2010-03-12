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

#ifndef WC3LIB_EDITOR_MODELVIEW_HPP
#define WC3LIB_EDITOR_MODELVIEW_HPP

#include <list>

#include "qogre/QOgreWidget.h"

namespace Ogre
{

class Vector3;
class Camera;
class Viewport;
class SceneManager;
class ExampleFrameListener;
	
}

namespace wc3lib
{
	
namespace mdlx
{

class OgreMdlx;
class Mdlx;

}
	
namespace editor
{
	
class ModelView : public QOgreWidget
{
	public:
		ModelView(QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~ModelView();
		
		virtual void show();
		
		void addOgreMdlx(class mdlx::OgreMdlx *ogreMdlx);
		Ogre::Entity* createOgreMdlxEntity(class mdlx::OgreMdlx *ogreMdlx, const Ogre::Vector3 &position);
		Ogre::Entity* createOgreMdlx(class mdlx::Mdlx *mdlx, const Ogre::Vector3 &position, class mdlx::OgreMdlx *&ogreMdlx);
		
	protected:
		virtual void resizeEvent(QResizeEvent *event);
		
		virtual void setupResources();
		virtual void configure();

		class Ogre::Camera *m_camera;
		class Ogre::Viewport *m_viewPort;
		class Ogre::SceneManager *m_sceneManager;
		class Ogre::ExampleFrameListener *m_frameListener;
		std::list<class mdlx::OgreMdlx*> m_models;
};

}

}

#endif
