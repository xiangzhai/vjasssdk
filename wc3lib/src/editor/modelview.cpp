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

#include <OGRE/Ogre.h>

#include "modelview.hpp"
#include "qogre/ExampleFrameListener.h"

namespace wc3lib
{
	
namespace editor
{

ModelView::ModelView(QWidget *parent, Qt::WFlags f) : QOgreWidget (parent, f)
{
}

ModelView::~ModelView()
{
	if (this->m_frameListener)
		delete this->m_frameListener;
}

void ModelView::show()
{
	static bool isInitialized;
	
	if (isInitialized)
		return;
	
	isInitialized = true;
	
        // Load resource paths from config file
        class Ogre::ConfigFile configFile;
        configFile.load("resources.cfg");

        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator sectionIterator = configFile.getSectionIterator();
        Ogre::String sectionName, typeName, archiveName;
	
        while (sectionIterator.hasMoreElements())
        {
            sectionName = sectionIterator.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator iterator;
            
	    for (iterator = settings->begin(); iterator != settings->end(); ++iterator)
            {
                typeName = iterator->first;
                archiveName = iterator->second;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archiveName, typeName, sectionName);
            }
        }
	
	this->m_sceneManager = theOgreRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance" + Ogre::StringConverter::toString((unsigned long)this));
	 // Create the camera
        this->m_camera = this->m_sceneManager->createCamera("EditorCamera" + Ogre::StringConverter::toString((unsigned long)this));

        // Position it at 500 in Z direction
        this->m_camera->setPosition(Ogre::Vector3(0, 0, 500));
        // Look back along -Z
        this->m_camera->lookAt(Ogre::Vector3(0, 0, -300));
        this->m_camera->setNearClipDistance(5);
	this->m_viewPort = this->m_renderWindow->addViewport(this->m_camera);
        this->m_viewPort->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

        // Alter the camera aspect ratio to match the viewport
        this->m_camera->setAspectRatio(Ogre::Real(this->m_viewPort->getActualWidth()) / Ogre::Real(this->m_viewPort->getActualHeight()));
	
	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	
	// Create any resource listeners (for loading screens)
	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	//createResourceListener();
	// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	// Create the scene
	// pure virtual - this has to be overridden
	//createScene();
	
	this->m_frameListener = new Ogre::ExampleFrameListener(this->m_renderWindow, this->m_camera);
        this->m_frameListener->showDebugOverlay(true);
        theOgreRoot->addFrameListener(this->m_frameListener);
}

void ModelView::addOgreMdlx(class mdlx::OgreMdlx *ogreMdlx, Ogre::Vector3 position)
{
	this->m_models.push_back(ogreMdlx);
	/// @todo Move to vector.
}

void ModelView::resizeEvent(QResizeEvent *event)
{
	if (this->m_renderWindow)
	{
		this->m_renderWindow->resize(this->width(), this->height());
		this->m_renderWindow->windowMovedOrResized();
	} 

	if (this->m_camera)
		this->m_camera->setAspectRatio(Ogre::Real(this->m_viewPort->getActualWidth()) / Ogre::Real(this->m_viewPort->getActualHeight()));
}

void ModelView::setupResources()
{
}

void ModelView::configure()
{
}

}

}
