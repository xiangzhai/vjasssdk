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

#include <iostream>

#include <OGRE/Ogre.h>	
//#include <OGRE/OgreErrorDialog.h>

//#include "../exception.hpp"
#include "renderer.hpp"

namespace wc3lib
{

namespace editor
{

void Renderer::start()
{
	/*
	try
	{
	*/
		this->m_root = new Ogre::Root("../../plugins.cfg", "../ogre.cfg", "ogre.log");
		
		if (!this->m_root->restoreConfig())
		{
			if (!this->m_root->showConfigDialog())
			{
				return;
			}
		}
		this->m_renderWindow = this->m_root->initialise(true, "Renderer Application");
		
		this->m_sceneManager = this->m_root->createSceneManager(Ogre::ST_GENERIC, "sceneManager");
		// create main camera (for editor)
		this->m_camera = this->m_sceneManager->createCamera("Camera");
		this->m_camera->setPosition(Ogre::Vector3(0.0f, 0.0f, 500.0f));
		this->m_camera->lookAt(Ogre::Vector3(0.0f, 0.0f, 0.0f));
		this->m_camera->setNearClipDistance(5.0f);
		this->m_camera->setFarClipDistance(5000.0f);
		
		this->m_viewport = this->m_renderWindow->addViewport(this->m_camera);
		this->m_viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
		this->m_camera->setAspectRatio(Ogre::Real(this->m_viewport->getActualWidth()) / Ogre::Real(this->m_viewport->getActualHeight()));
		
		this->m_root->startRendering();

	/*
	}
	catch (class Ogre::Exception &exception)
	{
	*/
		//std::cerr << exception.what() << std::endl;
		/*
		Ogre::ErrorDialog *dialog = Ogre::PlatformManager::getSingleton().createErrorDialog();
		dialog->display(exception.getFullDescription());
		delete dialog;	
		*/
	/*
	}
	*/
}

}

}
