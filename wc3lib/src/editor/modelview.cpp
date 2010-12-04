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

#include <QtCore>
#include <QtGui>

#if defined(Q_WS_X11)
#include <QX11Info>
#endif

#include <klocale.h>
#include <kmessagebox.h>

//#include <qogre/ExampleFrameListener.h>

#include "modelview.hpp"

namespace wc3lib
{

namespace editor
{

ModelView::ModelView(class Editor *editor, QWidget *parent, Qt::WFlags f, Ogre::SceneType ogreSceneType, const Ogre::NameValuePairList *ogreParameters) : QWidget(parent, f), m_editor(editor), m_sceneType(ogreSceneType), m_parameters(ogreParameters), m_root(new Ogre::Root()), m_renderWindow(0), m_sceneManager(0), m_camera(0), m_viewPort(0), m_changeFarClip(false)
{
	// setup a renderer
	const Ogre::RenderSystemList &renderers = this->m_root->getAvailableRenderers();
	assert(!renderers.empty()); // we need at least one renderer to do anything useful
	Ogre::RenderSystem *renderSystem = renderers.front();
	assert(renderSystem); // user might pass back a null renderer, which would be bad!
	QString dimensions = QString("%1x%2").arg(this->width()).arg(this->height());
	qDebug() << "Dimensions " << dimensions.toAscii().data();
	renderSystem->setConfigOption("Video Mode", dimensions.toAscii().data());
	this->m_root->setRenderSystem(renderSystem);
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

/// @todo Window initialization is too early? Wrong window id -> exception!!!
void ModelView::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event); // show first since we need window handle id
}

void ModelView::resizeEvent(QResizeEvent *event)
{
	if(this->m_renderWindow)
	{
		qDebug() << QString("Resize to %1|%2").arg(width()).arg(height());
		this->m_renderWindow->resize(width(), height());
		this->m_renderWindow->windowMovedOrResized();

		for(int ct = 0; ct < this->m_renderWindow->getNumViewports(); ++ct)
		{
			Ogre::Viewport* viewport = this->m_renderWindow->getViewport(ct);
			Ogre::Camera* camera = viewport->getCamera();
			camera->setAspectRatio(static_cast<Ogre::Real>(viewport->getActualWidth()) / static_cast<Ogre::Real>(viewport->getActualHeight()));
		}
	}

	QWidget::resizeEvent(event);
}

void ModelView::paintEvent(QPaintEvent *event)
{
	qDebug() << "Render";

	this->m_root->_fireFrameStarted();

	if (this->m_renderWindow)
		this->m_renderWindow->update();
	else
		initRenderWindow();

	this->m_root->_fireFrameRenderingQueued();
	this->m_root->_fireFrameEnded();

	QWidget::paintEvent(event);
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
		qDebug() << "Changing far clip distance to: " << this->m_camera->getFarClipDistance();
	}
	// move camera
	else
	{
		this->moveCamera(Ogre::Vector3(event->x(), event->y(), this->m_camera->getDirection().z), Ogre::Vector3(event->delta()));
		qDebug() << "Moving camera to: (" << this->m_camera->getPosition().x << "|" << this->m_camera->getPosition().y << "|" << this->m_camera->getPosition().z << ")";
	}

	event->accept();
}

void ModelView::mouseMoveEvent(QMouseEvent *event)
{
	/// @todo If right mouse button is being pressed
	if (true)
	{
		// moves camera along to x- and y-axis
		this->moveCamera(Ogre::Vector3(event->x(), event->y(), 0));
		event->accept();
	}
	/// @todo If middle mouse button is being pressed
	else if (true)
	{
		// rotates camera around its current position
		//this->m_camera->rotate();
		event->accept();
	}
}

void ModelView::mousePressEvent(QMouseEvent *event)
{
	/// @todo When right button is being pressed mouse movement is possible
	/// @todo When left button is being pressed selection is possible (in editor only, not in view)
	/// @todo When middle button is being pressed camera rotation is possible
}

void ModelView::mouseReleaseEvent(QMouseEvent *event)
{
}

void ModelView::initRenderWindow()
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
	const QX11Info &info = x11Info();
	externalWindowHandleParams = Ogre::StringConverter::toString((unsigned long)(info.display()));
	externalWindowHandleParams += ":";
	externalWindowHandleParams += Ogre::StringConverter::toString((unsigned int)(info.screen()));
	externalWindowHandleParams += ":";
	qDebug() << QString("Display: %1\nScreen: %2\nWindow: %3\nVisual: %4").arg((unsigned long)(info.display())).arg((unsigned long)(info.screen())).arg((unsigned long)(winId())).arg((unsigned long)(info.visual()));
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
	try
	{
		this->m_renderWindow = this->m_root->createRenderWindow("OgreWindow", width(), height(), false, &params);
	}
	catch (const Ogre::RenderingAPIException &exception) // cancel
	{
		KMessageBox::detailedError(this, i18n("Fehler bei der Erzeugung eines Fensters mit der Rendering Engine OGRE"), exception.what());

		return;
	}

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
	this->m_camera->setPosition(Ogre::Vector3(0, 0, 0));
	this->m_camera->lookAt(Ogre::Vector3(0, 0, 0));
	this->m_camera->setNearClipDistance(1.0);
	this->m_camera->setFarClipDistance(50000);
	this->m_viewPort = this->m_renderWindow->addViewport(this->m_camera);
	this->m_viewPort->setBackgroundColour(Ogre::ColourValue(0.8, 0.8, 1));
	this->m_camera->setAspectRatio(Ogre::Real(this->m_viewPort->getActualWidth()) / Ogre::Real(this->m_viewPort->getActualHeight()));
	Ogre::Light *light = this->m_sceneManager->createLight("Light1");
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(Ogre::Vector3(250, 150, 250));
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue::White);

	// default graphics settings
	// VSync
	// Resolution
	/// @todo Should be configurable for the whole editor (using singleton)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(2);

	// if this function is called background becomes blue but program hangs up
	// this->m_root->startRendering(); /// @todo Error!
}

void ModelView::moveCamera(const Ogre::Vector3 &direction, const Ogre::Vector3 &delta)
{
	if (!this->m_camera)
		return;

	this->m_camera->setDirection(direction);
	this->m_camera->moveRelative(delta);
}

void ModelView::moveCamera(const Ogre::Vector3 &delta)
{
	this->m_camera->moveRelative(delta);
}

void ModelView::rotateCamera(const Ogre::Radian &angle)
{
	this->m_camera->rotate(this->m_camera->getPosition(), angle);
}

}

}
