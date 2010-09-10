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

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#include <QX11Info>

//#include <qogre/ExampleFrameListener.h>

#include "modelview.hpp"
#include "../mdlx/ogremdlx.hpp"
#include "../mdlx/mdlx.hpp"
#include "../mdlx/model.hpp"

namespace wc3lib
{

namespace editor
{

ModelView::ModelView(QWidget *parent, const QGLWidget *shareWidget, Qt::WFlags f, Ogre::SceneType ogreSceneType, const Ogre::NameValuePairList *ogreParameters) : QGLWidget(parent, shareWidget, f), m_sceneType(ogreSceneType), m_parameters(ogreParameters), m_root(new Ogre::Root()), m_renderWindow(0), m_sceneManager(0), m_camera(0), m_viewPort(0), m_changeFarClip(false)
{
	// setup a renderer
	Ogre::RenderSystemList *renderers = this->m_root->getAvailableRenderers();
	assert(!renderers->empty()); // we need at least one renderer to do anything useful
	Ogre::RenderSystem *renderSystem;
	renderSystem = renderers->front();
	assert(renderSystem); // user might pass back a null renderer, which would be bad!
	this->m_root->setRenderSystem(renderSystem);
	QString dimensions = QString("%1x%2").arg(this->width()).arg(this->height());
	renderSystem->setConfigOption("Video Mode", dimensions.toAscii().data());
	// initialize without creating window
	this->m_root->getRenderSystem()->setConfigOption("Full Screen", "No");
	this->m_root->saveConfig();
	this->m_root->initialise(false); // don't create a window
}

ModelView::~ModelView()
{
	//if (this->m_frameListener)
		//delete this->m_frameListener;
}

//void ModelView::show()
//{
	/*
	static bool isInitialized;

	if (isInitialized)
		return;

	isInitialized = true;
	*/
        // Load resource paths from config file
        //class Ogre::ConfigFile configFile;
        //configFile.load("resources.cfg");

        // Go through all sections & settings in the file
        /*
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
	*/
	 // Create the camera
        //this->m_camera = this->m_sceneManager->createCamera("EditorCamera" + Ogre::StringConverter::toString((unsigned long)this));

        // Position it at 500 in Z direction
        //this->m_camera->setPosition(Ogre::Vector3(0, 0, 500));
        // Look back along -Z
        /*
        this->m_camera->lookAt(Ogre::Vector3(0, 0, -300));
        this->m_camera->setNearClipDistance(5);
	this->m_viewPort = this->m_renderWindow->addViewport(this->m_camera);
        this->m_viewPort->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	*/
        // Alter the camera aspect ratio to match the viewport
        //this->m_camera->setAspectRatio(Ogre::Real(this->m_viewPort->getActualWidth()) / Ogre::Real(this->m_viewPort->getActualHeight()));

	// Set default mipmap level (NB some APIs ignore this)
	//Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	//createResourceListener();
	// Load resources
	//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Create the scene
	// pure virtual - this has to be overridden
	//createScene();
	/*
	this->m_frameListener = new Ogre::ExampleFrameListener(this->m_renderWindow, this->m_camera);
        this->m_frameListener->showDebugOverlay(true);
        theOgreRoot->addFrameListener(this->m_frameListener);
	*/
//}

class Ogre::Entity* ModelView::createModel(const class mdlx::Mdlx &mdlx, const Ogre::Vector3 &position, class mdlx::OgreMdlx *&ogreMdlx)
{
	ogreMdlx = new mdlx::OgreMdlx(mdlx);

	return this->createModel(*ogreMdlx, position);
}

class Ogre::Entity* ModelView::createModel(const class mdlx::OgreMdlx &ogreMdlx, const Ogre::Vector3 &position)
{
	class Ogre::SceneNode *sceneNode = this->m_sceneManager->createSceneNode("Scene node");
	sceneNode->setPosition(position);
	Ogre::Entity *entity = this->m_sceneManager->createEntity("entity", ogreMdlx.mdlx()->model()->name());
	sceneNode->attachObject(entity);

	return entity;
}

void ModelView::resizeEvent(QResizeEvent *event)
{
	if(this->m_renderWindow)
	{
		this->m_renderWindow->resize(width(), height());
		this->m_renderWindow->windowMovedOrResized();

		for(int ct = 0; ct < this->m_renderWindow->getNumViewports(); ++ct)
		{
			Ogre::Viewport* viewport = this->m_renderWindow->getViewport(ct);
			Ogre::Camera* camera = viewport->getCamera();
			camera->setAspectRatio(static_cast<Ogre::Real>(viewport->getActualWidth()) / static_cast<Ogre::Real>(viewport->getActualHeight()));
		}
	}
}

void ModelView::paintGL()
{
	/// @todo Render window only?
	this->m_root->renderOneFrame();

	/*
	Ogre::Root::getSingleton()._fireFrameStarted();
	this->m_renderWindow->update();
	Ogre::Root::getSingleton()._fireFrameRenderingQueued();
	Ogre::Root::getSingleton()._fireFrameEnded();
	*/
}

void ModelView::resizeGL(int width, int height)
{
	if(this->m_renderWindow)
		this->m_renderWindow->windowMovedOrResized();
}

void ModelView::initializeGL()
{
	// Parameters to pass to Ogre::Root::createRenderWindow()
	Ogre::NameValuePairList params;

	// If the user passed in any parameters then be sure to copy them into our own parameter set.
	// NOTE: Many of the parameters the user can supply (left, top, border, etc) will be ignored
	// as they are overridden by Qt. Some are still useful (such as FSAA).
	if(this->m_parameters != 0)
		params.insert(this->m_parameters->begin(), this->m_parameters->end());

	// The external windows handle parameters are platform-specific
	Ogre::String externalWindowHandleParams;

	// Accept input focus
	setFocusPolicy(Qt::StrongFocus);

#if defined(Q_WS_WIN)
	// positive integer for W32 (HWND handle) - According to Ogre Docs
	externalWindowHandleParams = Ogre::StringConverter::toString((unsigned int)(winId()));
#endif

#if defined(Q_WS_X11)
	// poslong:posint:poslong:poslong (display*:screen:windowHandle:XVisualInfo*) for GLX - According to Ogre Docs
	QX11Info info = x11Info();
	externalWindowHandleParams  = Ogre::StringConverter::toString((unsigned long)(info.display()));
	externalWindowHandleParams += ":";
	externalWindowHandleParams += Ogre::StringConverter::toString((unsigned int)(info.screen()));
	externalWindowHandleParams += ":";
	qDebug() << "This is the WINDOW ID " << winId();
	externalWindowHandleParams += Ogre::StringConverter::toString((unsigned long)(winId()));
	//externalWindowHandleParams += ":";
	//externalWindowHandleParams += Ogre::StringConverter::toString((unsigned long)(info.visual()));
#endif

	// Add the external window handle parameters to the existing params set.
#if defined(Q_WS_WIN)
	params["externalWindowHandle"] = externalWindowHandleParams;
#endif

#if defined(Q_WS_X11)
	params["parentWindowHandle"] = externalWindowHandleParams;
#endif

	// Finally create our window.
	this->m_renderWindow = this->m_root->createRenderWindow("OgreWindow", width(), height(), false, &params);
	this->m_renderWindow->setActive(true);
	// old stuff, added stuff from QtOgreWidget
	//WId ogreWinId = 0x0;
	//this->m_renderWindow->getCustomAttribute( "WINDOW", &ogreWinId);
	//assert(ogreWinId);
/*
OLD!
#if defined(Q_WS_X11)
	WId ogreWinId = 0x0;
	this->m_renderWindow->getCustomAttribute("WINDOW", &ogreWinId);
	assert(ogreWinId);
	this->create(winId());
#endif
*/

	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NoBackground);

	//== Ogre Initialization ==//
	// default scene
	this->m_sceneManager = this->m_root->createSceneManager(this->m_sceneType);
	this->m_sceneManager->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	this->m_camera = this->m_sceneManager->createCamera("Widget_Cam");
	this->m_camera->setPosition(Ogre::Vector3(0, 1, 0));
	this->m_camera->lookAt(Ogre::Vector3(0, 0, 0));
	this->m_camera->setNearClipDistance(1.0);
	this->m_camera->setFarClipDistance(50000);
	this->m_viewPort = this->m_renderWindow->addViewport(this->m_camera);
	this->m_viewPort->setBackgroundColour(Ogre::ColourValue(0.8, 0.8, 1));
}


void ModelView::setupResources()
{
}

void ModelView::configure()
{
}

void ModelView::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_Control:
			this->m_changeFarClip = true;

			break;
	}

	event->accept();
}

void ModelView::keyReleaseEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_Control:
			this->m_changeFarClip = false;

			break;
	}

	event->accept();
}

void ModelView::wheelEvent(QWheelEvent *event)
{
	//QGLWidget::wheelEvent(event);

	// Ctrl is pressed, change far clip distance
	if (this->m_changeFarClip)
	{
		this->m_camera->setFarClipDistance(this->m_camera->getFarClipDistance() + event->delta());
	}
	// move camera
	else
	{
		this->m_camera->setDirection(Ogre::Vector3(event->x(), event->y(), this->m_camera->getDirection().z));
		this->m_camera->moveRelative(Ogre::Vector3(event->delta()));
	}

	event->accept();
}

}

}
