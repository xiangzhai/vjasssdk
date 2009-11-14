/*
	This file is part of the ogre-qt project.

	Copyright (C) 2006  Lakin Wecker <lakin.wecker@gmail.com>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "QOgreWidget.h"

#include <OgreNoMemoryMacros.h>
#if defined(Q_WS_WIN)
#include <windows.h>	// needed for WindowFromDC()
#else
#include <Qt/qx11info_x11.h>
#include <X11/Xlib.h>
#endif
#include <OgreMemoryMacros.h>


Ogre::Root *QOgreWidget::theOgreRoot = 0;
std::vector <Ogre::RenderWindow *> QOgreWidget::theRenderWindowList = std::vector <Ogre::RenderWindow*>();

QOgreWidget::QOgreWidget (QWidget* parent, Qt::WFlags f) :
    QWidget (parent, f),
   m_RenderWindow (0)
{
    std::cout  << __FILE__ << ":" << __LINE__ << std::endl;
    showEvent (0);
}

QOgreWidget::~QOgreWidget()
{
    if (theOgreRoot) delete theOgreRoot;
}
void QOgreWidget::showEvent (QShowEvent *event)
{
	startTimer(60);
    if (m_renderWindow) return;

    Ogre::NameValuePairList params;

    QWidget *q_parent = dynamic_cast <QWidget *> (parent());
    assert (q_parent);

#if !defined(Q_WS_WIN)
    QX11Info xInfo = x11Info();

    /// TODO: Is this needed anymore?!
    char buf[64];
    sprintf(buf, "GLX_WINDOW=0x%lx", winId());
    putenv(buf);

    //params["externalWindowHandle"] =
    params["parentWindowHandle"] =
         Ogre::StringConverter::toString ((unsigned long)xInfo.display()) +
        ":" + Ogre::StringConverter::toString ((unsigned int)xInfo.screen()) +
        ":" + Ogre::StringConverter::toString ((unsigned long)q_parent->winId());
#else
    params["parentWindowHandle"] = Ogre::StringConverter::toString((unsigned long)q_parent->winId());
#endif
    if (!theOgreRoot) {
        theOgreRoot = new Ogre::Root();
        /// TODO: Does this really belong in the base widget?
        ///  and if it does, should I also faithfully include 
        ///  methods for setting up the scene, the camera, the viewport?
        std::cout  << __FILE__ << ":" << __LINE__ << std::endl;
        //setupResources();

        configure();
        theOgreRoot->initialise (false);
    }


    m_renderWindow = theOgreRoot->createRenderWindow (
                "View" + Ogre::StringConverter::toString((unsigned long)this), width(), height(), false, &params);
    theRenderWindowList.push_back (m_renderWindow);

    WId window_id;
#if !defined(Q_WS_WIN)
    m_renderWindow->getCustomAttribute ("GLXWINDOW", &window_id);
#else
    m_renderWindow->getCustomAttribute ("HWND", &window_id);
#endif
    // Take over the ogre created window.
    QWidget::create (window_id);

    m_renderWindow->reposition (x(),y());

}
void QOgreWidget::timerEvent( QTimerEvent * )
{
    std::cout << this << " timerEvent" << std::endl;
    update();
}

void QOgreWidget::update() {
    Ogre::Root::getSingletonPtr()->_fireFrameStarted();
    for (unsigned int i=0; i < theRenderWindowList.size(); ++i) {
        theRenderWindowList[i]->update();
    }
    Ogre::Root::getSingletonPtr()->_fireFrameEnded();
}
void QOgreWidget::paintEvent (QPaintEvent *e)
{
    update();
}


void QOgreWidget::configure (void) {
		if (!theOgreRoot->restoreConfig()) {
			theOgreRoot->showConfigDialog ();
		}
}
QSize QOgreWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize QOgreWidget::sizeHint() const
{
	return QSize(800, 600);
}
