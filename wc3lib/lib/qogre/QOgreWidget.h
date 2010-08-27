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

#ifndef QTOGREWIDGET_H
#define QTOGREWIDGET_H


//#include <OgreNoMemoryMacros.h>
#include <Qt/QtGui>
//#include <OgreMemoryMacros.h>
#include <vector>

#include <Ogre.h>


/** QWidget specialised for embedding an Ogre window. */
class QOgreWidget : public QWidget
{
	// TODO: Prevents from correct compilation
	//Q_OBJECT

	public:
		QOgreWidget (QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~QOgreWidget ();

		void update();

		QSize minimumSizeHint () const;
		QSize sizeHint () const;

	protected:
		void showEvent (QShowEvent *event);
		void timerEvent (QTimerEvent *);
		void paintEvent (QPaintEvent *);
/*    virtual void create (
            WId window = 0,
            bool initializeWindow = true,
            bool destroyOldWindow = true);*/

		virtual void setupResources(void) = 0;
		virtual void configure(void);

		static Ogre::Root *m_ogreRoot;
		static std::vector <Ogre::RenderWindow *> m_renderWindowList;

		Ogre::RenderWindow *m_renderWindow;

};


#endif
