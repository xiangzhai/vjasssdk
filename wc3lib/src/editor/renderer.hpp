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

#ifndef WC3LIB_RENDERER_HPP
#define WC3LIB_RENDERER_HPP

namespace Ogre
{

class Root;
class RenderWindow;
class SceneManager;
class Camera;
class Viewport;
	
}

namespace wc3lib
{

namespace editor
{

/**
* Provides simple Warcraft 3 editor-like render window and manager. Is able to render MDX, MDL and BLP files.
* @todo Add methods like addMdx(<coordinates>), addMdl(<coordinates>).
*/
class Renderer
{
	public:
		void start();
	
	private:
		class Ogre::Root *m_root;
		class Ogre::RenderWindow *m_renderWindow;
		class Ogre::SceneManager *m_sceneManager; //class Ogre::DefaultSceneManager *m_defaultSceneManager;
		class Ogre::Camera *m_camera; // main camera
		class Ogre::Viewport *m_viewport;
};

}

}

#endif
