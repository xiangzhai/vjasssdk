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

#ifndef WC3LIB_EDITOR_MODELEDITOR_HPP
#define WC3LIB_EDITOR_MODELEDITOR_HPP

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

#include <kurl.h>

#include <Ogre.h>

#include "module.hpp"
#include "ui/ui_modeleditor.h"
#include "ogremdlx.hpp"

namespace wc3lib
{

namespace editor
{

/**
* We have model SLK entries listet at the tree view.
* @todo Should use a customized version of model view for selection and editing models.
*/
class ModelEditor : public Module, protected Ui::ModelEditor
{
	Q_OBJECT

	public:
		typedef boost::bimap<class mdlx::Mdlx*, class OgreMdlx*> Models;
		typedef boost::bimap<QAction*, const mdlx::Camera*> CameraActions;
		
		ModelEditor(class Editor *editor);
		virtual ~ModelEditor();

		virtual void show();

		class ModelEditorView* modelView() const;
		const Models& models() const;
		const CameraActions& cameraACtions() const;
		void setHitTest(bool hitTest);
		bool hitTest() const;

	public slots:
		void openFile();
		void saveFile();
		void closeAllFiles();
		void showSettings();
		// test actions for one single view port/camera
		void centerView();
		void setPolygonModePoints();
		void setPolygonModeWireframe();
		void setPolygonModeSolid();
		void showStats();
		void changeTeamColor();
		void changeTeamGlow();

	protected slots:
		void viewCamera();

	protected:
		friend class ModelEditorSettings;

		virtual void createFileActions(class KMenu *menu);
		virtual void createEditActions(class KMenu *menu);
		virtual void createMenus(class KMenuBar *menuBar);
		virtual void createWindowsActions(class KMenu *menu);
		virtual void createToolButtons(class KToolBar *toolBar);
		virtual class SettingsInterface* settings();

		// load file events
		virtual void dragEnterEvent(QDragEnterEvent *event);
		virtual void dropEvent(QDropEvent *event);

		bool openUrl(const KUrl &url);
		void removeModel(OgreMdlx *ogreModel);

		void addCameraActions(const OgreMdlx &ogreModel);
		void removeCameraActions(const OgreMdlx &ogreModel);

		class ModelEditorView *m_modelView;
		class ModelEditorSettingsDialog *m_settingsDialog;
		KUrl m_recentUrl;
		Models m_models;
		CameraActions m_cameraActions;

		class KMenu *m_viewMenu;
		class RenderStatsWidget *m_renderStatsWidget;
		
		bool m_hitTest;
};

inline class ModelEditorView* ModelEditor::modelView() const
{
	return this->m_modelView;
}

inline const ModelEditor::Models& ModelEditor::models() const
{
	return m_models;
}

inline const ModelEditor::CameraActions& ModelEditor::cameraACtions() const
{
	return m_cameraActions;
}

inline void ModelEditor::setHitTest(bool hitTest)
{
	this->m_hitTest = hitTest;
}

inline bool ModelEditor::hitTest() const
{
	return this->m_hitTest;
}

}

}

#endif
