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

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>

#include <QtGui>

#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kmenu.h>
#include <kmenubar.h>
#include <kaction.h>
#include <kcolordialog.h>

#include "modeleditor.hpp"
#include "modeleditorview.hpp"
#include "editor.hpp"
#include "../utilities.hpp"
#include "resource.hpp"
#include "modeleditorsettings.hpp"
#include "modeleditorsettingsdialog.hpp"
#include "renderstatswidget.hpp"

namespace wc3lib
{

namespace editor
{

ModelEditor::ModelEditor(class Editor *editor) : Module(editor), m_modelView(new ModelEditorView(this)), m_settingsDialog(0), m_recentUrl(""), m_viewMenu(0), m_renderStatsWidget(0), m_hitTest(true)
{
	Ui::ModelEditor::setupUi(this);
	Module::setupUi();
	this->m_modelView->setMinimumSize(QSize(640, 480));
	this->setAcceptDrops(true); // enable drag & drop
	this->m_horizontalLayout->addWidget(this->m_modelView);
	this->topLayout()->addLayout(this->m_horizontalLayout);
	/*
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(this->m_modelView);
	this->m_modelViewWidget->setLayout(mainLayout);
	*/
	/// @todo Model view should be bigger than modules buttons
	//QGridLayout *layout = new QGridLayout(this);
	//layout->addWidget(this->m_modelView);
	//this->topLayout()->addWidget(this->m_modelView);

	QStandardItemModel *standardItemModel = new QStandardItemModel(this);
	QModelIndex modelIndex;
	standardItemModel->insertRows(0, 10, modelIndex);
	standardItemModel->setData(modelIndex, tr("Standard-Modelle"), Qt::DisplayRole);
	standardItemModel->setData(modelIndex, QIcon(), Qt::DecorationRole);
	this->m_treeView->setModel(standardItemModel);
}

ModelEditor::~ModelEditor()
{
	closeAllFiles();
}

void ModelEditor::show()
{
	Module::show();
	this->m_modelView->show();
	/// @todo FIXME: Either create model view data in constructor or get the right position for this function call.
	//readSettings(); // read settings when model view is being shown since its render window is also being created at that moment
}

void ModelEditor::openFile()
{
	KUrl::List urls = KFileDialog::getOpenUrls(this->m_recentUrl, i18n("*.mdl|Blizzard Model (*.mdl)\n*.mdx|Compressed Blizzard Model (*.mdx)\n*|All files (*)"), this);

	if (urls.isEmpty())
		return;

	foreach (KUrl url, urls)
	{
		if (!url.isLocalFile())
		{
			KMessageBox::error(this, i18n("Unable to open file \"%1\".", url.toLocalFile()));

			continue;
		}

		this->openUrl(url);
	}
}

void ModelEditor::saveFile()
{
	if (this->m_models.empty())
	{
		KMessageBox::error(this, i18n("There is no model to export."));

		return;
	}

	KUrl url = KFileDialog::getSaveUrl(this->m_recentUrl, i18n("*.mdl|Blizzard Model (*.mdl)\n*.mdx|Compressed Blizzard Model (*.mdx)\n*|All files (*)"), this);

	if (url.isEmpty())
		return;

	if (!url.isLocalFile())
	{
		KMessageBox::error(this, i18n("Unable to save file \"%1\".", url.toLocalFile()));

		return;
	}

	try
	{
		this->m_models.left.begin()->second->saveAs(url);
	}
	catch (Exception exception)
	{
		KMessageBox::error(this, i18n("Unable to write file \"%1\".\nException \"%2\".", url.toLocalFile(), exception.what().c_str()));
	}
}

void ModelEditor::closeAllFiles()
{
	BOOST_FOREACH(Models::left_reference value, this->m_models.left)
		removeModel(value.second);
}

void ModelEditor::showSettings()
{
	/*
	if (this->m_settingsDialog == 0)
		this->m_settingsDialog = new ModelEditorSettingsDialog(this, tr("Model Editor Settings"), 0);

	this->m_settingsDialog.show();
	*/
}

void ModelEditor::centerView()
{
	this->modelView()->centerView();
}

void ModelEditor::setPolygonModePoints()
{
	this->modelView()->setPolygonModePoints();
}

void ModelEditor::setPolygonModeWireframe()
{
	this->modelView()->setPolygonModeWireframe();
}

void ModelEditor::setPolygonModeSolid()
{
	this->modelView()->setPolygonModeSolid();
}

void ModelEditor::showStats()
{
	if (this->m_renderStatsWidget == 0)
	{
		this->m_renderStatsWidget = new RenderStatsWidget(this->modelView(), this);
		m_horizontalLayout->addWidget(this->m_renderStatsWidget);
	}


	this->m_renderStatsWidget->setVisible(!this->m_renderStatsWidget->isVisible());
}

void ModelEditor::changeTeamColor()
{
	QColor color;
	
	if (KColorDialog::getColor(color, OgreMdlx::teamColor(this->m_models.left.begin()->second->teamColor()), this) == KColorDialog::Ok)
		this->m_models.left.begin()->second->setTeamColor(OgreMdlx::teamColor(color));
}

void ModelEditor::changeTeamGlow()
{
	QColor color;
	
	if (KColorDialog::getColor(color, OgreMdlx::teamColor(this->m_models.left.begin()->second->teamGlow()), this) == KColorDialog::Ok)
		this->m_models.left.begin()->second->setTeamGlow(OgreMdlx::teamColor(color));
}

void ModelEditor::viewCamera()
{
	QAction *action = boost::polymorphic_cast<QAction*>(QObject::sender());
	//this->m_modelView->setCamera(this->m_cameraActions.left.at(action));
	OgreMdlx::updateCamera(*this->m_cameraActions.left.at(action), this->m_modelView->camera()); // updates current camera to camera's position
}

void ModelEditor::dragEnterEvent(QDragEnterEvent *event)
{
	/// @todo If it's an MDLX file event->acceptProposedAction();
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void ModelEditor::dropEvent(QDropEvent *event)
{
	/// @todo If it's an MDLX file event->acceptProposedAction();
	if (event->mimeData()->hasUrls())
	{
		foreach (QUrl url, event->mimeData()->urls())
			this->openUrl(url);

		event->accept();
	}
}

bool ModelEditor::openUrl(const KUrl &url)
{
	std::ios_base::openmode openmode = std::ios_base::in;
	bool isMdx;

	if (boost::filesystem::path(url.toEncoded()).extension() == ".mdx")
	{
		qDebug() << "Is MDLX";
		isMdx = true;
		openmode |= std::ios_base::binary;
	}
	else
		isMdx = false;

	boost::filesystem::basic_ifstream<mdlx::byte> ifstream(url.path().toUtf8().constData(), openmode);
	
	class mdlx::Mdlx *model = new mdlx::Mdlx();

	try
	{
		std::streamsize size;
		
		if (isMdx)
			size = model->readMdx(ifstream);
		else
			size = model->readMdl(ifstream);

		model->textures()->textures().size(); // TEST
		KMessageBox::information(this, i18n("Read %1 file \"%2\" successfully.\nSize: %3.", isMdx ? i18n("MDX") : i18n("MDL"), url.toLocalFile(), sizeStringBinary(size).c_str()));
	}
	catch (class Exception &exception)
	{
		KMessageBox::error(this, i18n("Unable to read %1 file \"%2\".\nException \"%3\".", isMdx ? i18n("MDX") : i18n("MDL"), url.toLocalFile(), exception.what().c_str()));

		return false;
	}

	//const Ogre::Vector3 position(0.0, 0.0, 0.0);
	OgreMdlx *ogreModel = new OgreMdlx(url, *model, this->m_modelView);
	this->editor()->addResource(ogreModel); // add to get URL

	try
	{
		QTime ct = QTime::currentTime();
		ct.start();
		qDebug() << "Refresh";
		ogreModel->refresh();
		qDebug() << "After refresh. Duration " << ct.elapsed() << "ms";
	}
	catch (class std::exception &exception)
	{
		KMessageBox::error(this, i18n("Error during model refresh:\n%1", exception.what()));
		this->editor()->removeResource(ogreModel);
		delete ogreModel;
		delete model;

		return false;
	}

	this->m_models.left.insert(Models::left_value_type(model, ogreModel));
	this->m_modelView->root()->addFrameListener(ogreModel);
	addCameraActions(*ogreModel);

	return true;
}

void ModelEditor::removeModel(OgreMdlx *ogreModel)
{
	Models::right_iterator iterator = m_models.right.find(ogreModel);
	
	if (iterator != m_models.right.end())
	{
		this->m_modelView->root()->removeFrameListener(ogreModel);
		removeCameraActions(*ogreModel);
		delete ogreModel;
		m_models.right.erase(iterator);
	}
}

void ModelEditor::addCameraActions(const OgreMdlx &ogreModel)
{
	if (this->m_viewMenu != 0)
	{
		BOOST_FOREACH(OgreMdlx::Cameras::const_reference iterator, ogreModel.cameras())
		{
			KAction *action = new KAction(KIcon(":/actions/viewcamera.png"), i18n("Camera: %1", iterator.first->name()), this);
			connect(action, SIGNAL(triggered()), this, SLOT(viewCamera()));
			this->m_viewMenu->addAction(action);
			this->m_cameraActions.left.insert(CameraActions::left_value_type(action, iterator.first));
		}
	}
}

void ModelEditor::removeCameraActions(const OgreMdlx &ogreModel)
{
	if (this->m_viewMenu != 0)
	{
		BOOST_FOREACH(OgreMdlx::Cameras::const_reference camera, ogreModel.cameras())
		{
			CameraActions::right_iterator iterator = m_cameraActions.right.find(camera.first);
			
			if (iterator != m_cameraActions.right.end())
			{
				delete iterator->second; // delete action
				m_cameraActions.right.erase(iterator);
			}
		}
	}
}

void ModelEditor::createFileActions(class KMenu *menu)
{
	class KAction *action;

	action = new KAction(KIcon(":/actions/openmodel.png"), i18n("Open model"), this);
	action->setShortcut(KShortcut(i18n("Ctrl+O")));
	connect(action, SIGNAL(triggered()), this, SLOT(openFile()));
	menu->addAction(action);

	action = new KAction(KIcon(":/actions/savemoel.png"), i18n("Save model"), this);
	action->setShortcut(KShortcut(i18n("Ctrl+S")));
	connect(action, SIGNAL(triggered()), this, SLOT(saveFile()));
	menu->addAction(action);

	action = new KAction(KIcon(":/actions/settings.png"), i18n("Settings"), this);
	//action->setShortcut(KShortcut(i18n("Ctrl+O")));
	connect(action, SIGNAL(triggered()), this, SLOT(settings()));
	menu->addAction(action);
}

void ModelEditor::createEditActions(class KMenu *menu)
{
	qDebug() << "Edit actions";
}

void ModelEditor::createMenus(class KMenuBar *menuBar)
{
	KMenu *viewMenu = new KMenu(i18n("View"), this);
	this->m_viewMenu = viewMenu;
	menuBar->addMenu(viewMenu);

	// test actions for one single view port/camera

	KAction *action = new KAction(KIcon(":/actions/centerview.png"), i18n("Center View"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(centerView()));
	viewMenu->addAction(action);

	action = new KAction(KIcon(":/actions/polygonmodepoints.png"), i18n("Polygon Mode Points"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(setPolygonModePoints()));
	viewMenu->addAction(action);

	action = new KAction(KIcon(":/actions/polygonmodewireframe.png"), i18n("Polygon Mode Wireframe"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(setPolygonModeWireframe()));
	viewMenu->addAction(action);

	action = new KAction(KIcon(":/actions/polygonmodesolid.png"), i18n("Polygon Mode Solid"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(setPolygonModeSolid()));
	viewMenu->addAction(action);

	action = new KAction(KIcon(":/actions/showstats.png"), i18n("Show Stats"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(showStats()));
	viewMenu->addAction(action);
	
	action = new KAction(KIcon(":/actions/changeteamcolor.png"), i18n("Change team color"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(changeTeamColor()));
	viewMenu->addAction(action);
	
	action = new KAction(KIcon(":/actions/changeteamglow.png"), i18n("Change team glow"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(changeTeamGlow()));
	viewMenu->addAction(action);

	qDebug() << "Menus";
}

void ModelEditor::createWindowsActions(class KMenu *menu)
{
	qDebug() << "Windows";
}

void ModelEditor::createToolButtons(class KToolBar *toolBar)
{
	qDebug() << "Tool buttons";
}

class SettingsInterface* ModelEditor::settings()
{
	return new ModelEditorSettings(this);
}

#include "moc_modeleditor.cpp"

}

}
