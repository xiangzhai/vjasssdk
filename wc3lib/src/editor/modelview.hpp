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

#ifndef WC3LIB_EDITOR_MODELVIEW_HPP
#define WC3LIB_EDITOR_MODELVIEW_HPP

#include <QGLWidget>

#include <Ogre.h>

namespace wc3lib
{

namespace editor
{

/**
* Model views are usual widgets which can render 3d graphics.
* They are required to provide a simple display for MDLX files.
* Therefore well known rendering engine OGRE is used in this class.
* The rendering viewport should be scaled correctly automatically since Qt GUI events are implemented.
* MDLX files can be converted into OGRE entities by creating an OgreMdlx instance which manages an OGRE mesh and sub mesh instance.
* @todo Since each model view widget uses its own OGRE root object there should be a possibility to assign plugins.cfg file path.
* @link http://qt-apps.org/content/show.php/QtOgre+Framework?content=92912, http://www.ogre3d.org/tikiwiki/QtOgre
* @see Mdlx, OgreMdlx
*/
class ModelView : public QGLWidget
{
	public:
		/**
		* @param ogreSceneType OGRE scene type which will be set for the scene manager of the widget. Should be changed for terrain (ST_EXTERIOR_FAR, ST_EXTERIOR_REAL_FAR).
		* @param ogreParameters OGRE window parameters.
		*/
		ModelView(class Editor *editor, QWidget *parent = 0, const QGLWidget *shareWidget = 0, Qt::WFlags f = 0, Ogre::SceneType ogreSceneType = Ogre::ST_EXTERIOR_CLOSE, const Ogre::NameValuePairList *ogreParameters = 0);
		virtual ~ModelView();

		//virtual void show();

		class Editor* editor() const;
		Ogre::Root* root() const;
		Ogre::SceneManager* sceneManager() const;

	protected:
		//virtual void paintEvent(QPaintEvent* event);
		virtual void resizeEvent(QResizeEvent *event);

		// GL events
		// Renders the OpenGL scene. Gets called whenever the widget needs to be updated
		virtual void paintGL();
		// Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been resized (and also when it is shown for the first time because all newly created widgets get a resize event automatically).
		virtual void resizeGL(int width, int height);
		// Sets up the OpenGL rendering context, defines display lists, etc. Gets called once before the first time resizeGL() or paintGL() is called.
		virtual  void initializeGL();

		virtual void setupResources();
		virtual void configure();


		// key events
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void keyReleaseEvent(QKeyEvent *event);
		virtual void wheelEvent(QWheelEvent *event);

		// load file events
		virtual void dragEnterEvent(QDragEnterEvent *event);
		virtual void dropEvent(QDropEvent *event);

		class Editor *m_editor;
		const Ogre::NameValuePairList *m_parameters;
		Ogre::Root *m_root;
		Ogre::RenderWindow *m_renderWindow;

		// default scene
		Ogre::SceneType m_sceneType;
		Ogre::SceneManager *m_sceneManager;
		Ogre::Camera *m_camera;
		Ogre::Viewport *m_viewPort;

		//Ogre::ExampleFrameListener *m_frameListener;

		// event values
		bool m_changeFarClip;
};

inline class Editor* ModelView::editor() const
{
	return this->m_editor;
}

inline Ogre::Root* ModelView::root() const
{
	return this->m_root;
}

inline Ogre::SceneManager* ModelView::sceneManager() const
{
	return this->m_sceneManager;
}

}

}

#endif
