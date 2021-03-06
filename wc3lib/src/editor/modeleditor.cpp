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

#include <KFileDialog>
#include <KMessageBox>
#include <KLocale>
#include <KMenu>
#include <KMenuBar>
#include <KAction>
#include <KColorDialog>

#include "modeleditor.hpp"
#include "modeleditorview.hpp"
#include "editor.hpp"
#include "../utilities.hpp"
#include "resource.hpp"
#include "modeleditorsettings.hpp"
#include "modeleditorsettingsdialog.hpp"
#include "renderstatswidget.hpp"
#include "teamcolordialog.hpp"

namespace wc3lib
{

namespace editor
{

ModelEditor::ModelEditor(class Editor *editor) : Module(editor), m_modelView(new ModelEditorView(this)), m_settingsDialog(0), m_recentUrl(""), m_models(), m_viewMenu(0), m_renderStatsWidget(0), m_showStatsAction(0), m_teamColorDialog(0), m_teamGlowDialog(0), m_showCollisionShapesAction(0), m_teamColor(OgreMdlx::Red), m_teamGlow(OgreMdlx::Red)
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
	hideCollisionShapes();
	closeAllFiles();
}

void ModelEditor::show()
{
	Module::show();
	this->m_modelView->show();
	/// @todo FIXME: Either create model view data in constructor or get the right position for this function call.
	//readSettings(); // read settings when model view is being shown since its render window is also being created at that moment
}

void ModelEditor::hideCollisionShapes()
{
	this->m_showCollisionShapesAction->setText(i18n("Show Collision SFhapes"));
	
	BOOST_FOREACH(CollisionShapeNodes::left_value_type value, m_collisionShapeNodes.left)
		delete value.second; // destroy node
	
	m_collisionShapeNodes.left.clear();
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
	if (this->m_models.left.empty())
	{
		KMessageBox::error(this, i18n("There is no model to export."));

		return;
	}

	KUrl url = KFileDialog::getSaveUrl(this->m_recentUrl, i18n("*.mdl|Blizzard Model (*.mdl)\n*.mdx|Compressed Blizzard Model (*.mdx)\n*.mesh|OGRE mesh (*.mesh)\n*|All files (*)"), this);

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
	
	if (this->m_renderStatsWidget->isVisible())
		m_showStatsAction->setText(i18n("Show Stats"));
	else
		m_showStatsAction->setText(i18n("Hide Stats"));


	this->m_renderStatsWidget->setVisible(!this->m_renderStatsWidget->isVisible());
}
/*
namespace
{

Ogre::ManualObject* createSphere(const std::string &strName, const float r, const int nRings = 16, const int nSegments = 16)
{
	using namespace Ogre;
	ManualObject * manual = new Ogre::ManualObject(strName);
	manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
		
	float fDeltaRingAngle = (Math::PI / nRings);
	float fDeltaSegAngle = (2 * Math::PI / nSegments);
	unsigned short wVerticeIndex = 0 ;

	// Generate the group of rings for the sphere
	for( int ring = 0; ring <= nRings; ring++ ) {
		float r0 = r * sinf (ring * fDeltaRingAngle);
		float y0 = r * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(int seg = 0; seg <= nSegments; seg++) {
		float x0 = r0 * sinf(seg * fDeltaSegAngle);
		float z0 = r0 * cosf(seg * fDeltaSegAngle);

		// Add one vertex to the strip which makes up the sphere
		manual->position( x0, y0, z0);
		manual->normal(Vector3(x0, y0, z0).normalisedCopy());
		manual->textureCoord((float) seg / (float) nSegments, (float) ring / (float) nRings);

		if (ring != nRings) {
			// each vertex (except the last) has six indicies pointing to it
			manual->index(wVerticeIndex + nSegments + 1);
			manual->index(wVerticeIndex);               
			manual->index(wVerticeIndex + nSegments);
			manual->index(wVerticeIndex + nSegments + 1);
			manual->index(wVerticeIndex + 1);
			manual->index(wVerticeIndex);
			wVerticeIndex ++;
			}
		}; // end for seg
	} // end for ring
	manual->end();
	MeshPtr mesh = manual->convertToMesh(name);
	mesh->_setBounds( AxisAlignedBox( Vector3(-r, -r, -r), Vector3(r, r, r) ), false );

	mesh->_setBoundingSphereRadius(r);
	unsigned short src, dest;
	
	if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
	{
		mesh->buildTangentVectors(VES_TANGENT, src, dest);
	}
	
	return manual;
}

	
}
*/

void ModelEditor::showCollisionShapes()
{
	if (collisionShapeNodes().left.empty())
	{
		this->m_showCollisionShapesAction->setText(i18n("Hide Collision Shapes"));
		qDebug() << "Showing collision shapes";
		
		BOOST_FOREACH(Models::left_const_reference value, m_models.left)
		{
			std::size_t i = 0;
			
			BOOST_FOREACH(OgreMdlx::CollisionShapes::const_reference collisionShape, value.second->collisionShapes())
			{
				std::string name = boost::str(boost::format("%1%.CollisionShape%2%") % value.second->namePrefix().toUtf8().constData() % i);
				Ogre::SceneNode *sceneNode = modelView()->sceneManager()->createSceneNode(name.c_str());
				qDebug() << "Showing shape of type " << collisionShape.second->shape;
				
				switch (collisionShape.second->shape)
				{
					case mdlx::CollisionShape::Box:
					{
						Ogre::ManualObject *manualObject = new Ogre::ManualObject(name.c_str());
						manualObject->setBoundingBox(*collisionShape.second->box);
						
						sceneNode->attachObject(manualObject);
						sceneNode->showBoundingBox(true);
						qDebug() << "Bounding box";
						
						break;
					}
					
					/// \todo Create sphere
					case mdlx::CollisionShape::Sphere:
					{
						//Ogre::ManualObject *manualObject = new Ogre::ManualObject(name.c_str());
						//Ogre::MeshPtr mesh = manualObject->convertToMesh(name.c_str());
						//mesh->_setBoundingSphereRadius(collisionShape.second->sphere->getRadius());
						
						//Ogre::ManualObject *manualObject = createSphere(name, collisionShape.second->sphere->getRadius());
						//manualObject->se
						//manualObject->_s
						
						
						Ogre::Entity *entity = modelView()->sceneManager()->createEntity(name.c_str(), Ogre::SceneManager::PT_SPHERE);
						
						sceneNode->attachObject(entity);
						sceneNode->setPosition(collisionShape.second->sphere->getCenter());
						sceneNode->setScale(Ogre::Vector3(collisionShape.second->sphere->getRadius())); // Radius, in theory.
						qDebug() << "Bounding sphere";
						
						break;
					}
				}
				
				m_collisionShapeNodes.left.insert(CollisionShapeNodes::left_value_type(collisionShape.second, sceneNode));
				
				++i;
			}
		}
	}
	else
		hideCollisionShapes();
}

void ModelEditor::changeTeamColor()
{
	if (m_teamColorDialog == 0)
		m_teamColorDialog = new TeamColorDialog(this);
	
	m_teamColorDialog->setTeamColor(teamColor());
	
	//m_teamColorDialog->show();
	
	if (m_teamColorDialog->exec() == TeamColorDialog::Accepted)
		setTeamColor(m_teamColorDialog->teamColor());
	
	/*
	QColor color;
	
	if (KColorDialog::getColor(color, OgreMdlx::teamColor(this->m_models.left.begin()->second->teamColor()), this) == KColorDialog::Ok)
		this->m_models.left.begin()->second->setTeamColor(OgreMdlx::teamColor(color));
	*/
}

void ModelEditor::changeTeamGlow()
{
	if (m_teamGlowDialog == 0)
		m_teamGlowDialog = new TeamColorDialog(this);
	
	m_teamGlowDialog->setTeamColor(teamGlow());
	
	//m_teamGlowDialog->show();
	
	if (m_teamGlowDialog->exec() == TeamColorDialog::Accepted)
		setTeamGlow(m_teamColorDialog->teamColor());
	
	/*
	QColor color;
	
	if (KColorDialog::getColor(color, OgreMdlx::teamColor(this->m_models.left.begin()->second->teamGlow()), this) == KColorDialog::Ok)
		this->m_models.left.begin()->second->setTeamGlow(OgreMdlx::teamColor(color));
	*/
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
		std::streamsize size = isMdx ? size = model->readMdx(ifstream) : size = model->readMdl(ifstream);
		
		//model->textures()->textures().size(); // TEST
		KMessageBox::information(this, i18n("Read %1 file \"%2\" successfully.\nSize: %3.", isMdx ? i18n("MDX") : i18n("MDL"), url.toLocalFile(), sizeStringBinary(size).c_str()));
	}
	catch (class Exception &exception)
	{
		KMessageBox::error(this, i18n("Unable to read %1 file \"%2\".\nException \"%3\".", isMdx ? i18n("MDX") : i18n("MDL"), url.toLocalFile(), exception.what().c_str()));
		
		delete model;

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
		this->editor()->removeResource(ogreModel); // ogreModel is deleted automatically
		
		delete model;

		return false;
	}

	this->m_models.left.insert(Models::left_value_type(model, ogreModel));
	this->m_modelView->root()->addFrameListener(ogreModel);
	addCameraActions(*ogreModel);
	ogreModel->setTeamColor(teamColor());
	ogreModel->setTeamGlow(teamGlow());

	return true;
}

void ModelEditor::removeModel(OgreMdlx *ogreModel)
{
	Models::right_iterator iterator = m_models.right.find(ogreModel);
	
	if (iterator != m_models.right.end())
	{
		this->m_modelView->root()->removeFrameListener(ogreModel);
		removeCameraActions(*ogreModel);
		this->editor()->removeResource(ogreModel); // delete ogreModel
		delete iterator->second; // delete corresponding MDLX model
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

	action = new KAction(KIcon(":/actions/savemodel.png"), i18n("Save model"), this);
	action->setShortcut(KShortcut(i18n("Ctrl+S")));
	connect(action, SIGNAL(triggered()), this, SLOT(saveFile()));
	menu->addAction(action);
	
	action = new KAction(KIcon(":/actions/closeallmodels.png"), i18n("Close all models"), this);
	//action->setShortcut(KShortcut(i18n("
	connect(action, SIGNAL(triggered()), this, SLOT(closeAllFiles()));
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
	m_showStatsAction = action;
	
	action = new KAction(KIcon(":/actions/showcollisionshapes.png"), i18n("Show Collision Shapes"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(showCollisionShapes()));
	viewMenu->addAction(action);
	m_showCollisionShapesAction = action;
	
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
