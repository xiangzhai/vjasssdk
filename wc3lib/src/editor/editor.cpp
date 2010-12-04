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

#include <QtCore/QSettings>

#include <kmenu.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kmenubar.h>
#include <klocale.h>

#include <boost/foreach.hpp>

#include "editor.hpp"
#include "modulemenu.hpp"
#include "terraineditor.hpp"
#include "triggereditor.hpp"
#include "soundeditor.hpp"
#include "objecteditor.hpp"
#include "campaigneditor.hpp"
#include "aieditor.hpp"
#include "objectmanager.hpp"
#include "importmanager.hpp"
#include "mpqeditor.hpp"
#include "modeleditor.hpp"
#include "textureeditor.hpp"
#include "newmapdialog.hpp"
#include "resource.hpp"
#include "../mpq/mpq.hpp"
#include "../mpq/mpqfile.hpp"

namespace wc3lib
{

namespace editor
{

KAboutData Editor::m_aboutData = KAboutData("editor", "", ki18n("World Editor"), "0.1", ki18n("Clone of Blizzard's Warcraft 3 TFT World Editor."), KAboutData::License_GPL_V2, ki18n("Copyright (C) 2009 by Tamino Dauth <tamino@cdauth.eu>"), ki18n("Other"), "http://sourceforge.net/projects/vjasssdk/", "tamino@cdauth.eu")
.addAuthor(ki18n("Tamino Dauth"), ki18n("Maintainer"), "tamino@cdauth.eu", "http://tdauth.cdauth.eu/")
;
KAboutData Editor::m_wc3libAboutData = KAboutData("wc3lib", "", ki18n("Warcraft 3 Library"), "0.1", ki18n("Library which supports some of Blizzard's file formats used in Warcraft 3 TFT."), KAboutData::License_GPL_V2, ki18n("Copyright (C) 2009 by Tamino Dauth <tamino@cdauth.eu>"), ki18n("Other"), "http://sourceforge.net/projects/vjasssdk/", "tamino@cdauth.eu")
.addAuthor(ki18n("Tamino Dauth"), ki18n("Maintainer"), "tamino@cdauth.eu", "http://tdauth.cdauth.eu/")
;

const KAboutData& Editor::aboutData()
{
	return Editor::m_aboutData;
}


const KAboutData& Editor::wc3libAboutData()
{
	return Editor::m_wc3libAboutData;
}

Editor::Editor(QWidget *parent, Qt::WindowFlags f) : KMainWindow(parent, f), m_actionCollection(new KActionCollection(this)), m_terrainEditor(0), m_triggerEditor(0), m_soundEditor(0), m_objectEditor(0), m_campaignEditor(0), m_aiEditor(0), m_objectManager(0), m_importManager(0), m_mpqEditor(0), m_modelEditor(0), m_textureEditor(0), m_newMapDialog(0)
{
	/// @todo Actions should get the same entry names and shortcuts as in the original World Editor
	this->m_actionCollection->setConfigGroup("Shortcuts");

	class KAction *action = new KAction(KIcon(":/actions/newmap.png"), i18n("New map ..."), this);
	action->setShortcut(KShortcut(i18n("Ctrl+N")));
	connect(action, SIGNAL(triggered()), this, SLOT(newMap()));
	this->m_actionCollection->addAction("newmap", action);

	action = new KAction(KIcon(":/actions/openmap.png"), i18n("Open map ..."), this);
	action->setShortcut(KShortcut(i18n("Ctrl+O")));
	connect(action, SIGNAL(triggered()), this, SLOT(openMap()));
	this->m_actionCollection->addAction("openmap", action);

	action = new KAction(KIcon(":/actions/closemap.png"), i18n("Close map"), this);
	action->setShortcut(KShortcut(i18n("Strg+W")));
	connect(action, SIGNAL(triggered()), this, SLOT(closeMap()));
	this->m_actionCollection->addAction("closemap", action);

	// --

	action = new KAction(KIcon(":/actions/savemap.png"), i18n("Save map"), this);
	action->setShortcut(KShortcut(i18n("Ctrl+S")));
	connect(action, SIGNAL(triggered()), this, SLOT(saveMap()));
	this->m_actionCollection->addAction("savemap", action);

	action = new KAction(KIcon(":/actions/savemapas.png"), i18n("Save map as ..."), this);
	//action->setShortcut(KShortcut(i18n("Strg+S")));
	connect(action, SIGNAL(triggered()), this, SLOT(saveMapAs()));
	this->m_actionCollection->addAction("savemapas", action);

	action = new KAction(KIcon(":/actions/savemapshadows.png"), i18n("Calculate shadows and save map ..."), this);
	//action->setShortcut(KShortcut(i18n("Strg+S")));
	connect(action, SIGNAL(triggered()), this, SLOT(saveMapShadow()));
	this->m_actionCollection->addAction("savemapshadows", action);

	// --

	action = new KAction(KIcon(":/actions/testmap.png"), i18n("Test map"), this);
	action->setShortcut(KShortcut(i18n("Ctrl+F9")));
	connect(action, SIGNAL(triggered()), this, SLOT(testMap()));
	this->m_actionCollection->addAction("testmap", action);

	// --

	action = new KAction(KIcon(":/actions/closemodule.png"), i18n("Close module"), this);
	action->setShortcut(KShortcut(i18n("Ctrl+Shift+W")));
	connect(action, SIGNAL(triggered()), this, SLOT(closeModule()));
	this->m_actionCollection->addAction("closemodule", action);

	// module actions

	action = new KAction(KIcon(":/actions/terraineditor.png"), i18n("Terrain Editor"), this);
	action->setShortcut(KShortcut(i18n("F3")));
	connect(action, SIGNAL(triggered()), this, SLOT(showTerrainEditor()));
	this->m_actionCollection->addAction("terraineditor", action);

	action = new KAction(KIcon(":/actions/triggereditor.png"), i18n("Trigger Editor"), this);
	action->setShortcut(KShortcut(i18n("F4")));
	connect(action, SIGNAL(triggered()), this, SLOT(showTriggerEditor()));
	this->m_actionCollection->addAction("triggereditor", action);

	action = new KAction(KIcon(":/actions/soundeditor.png"), i18n("Sound Editor"), this);
	action->setShortcut(KShortcut(i18n("F5")));
	connect(action, SIGNAL(triggered()), this, SLOT(showSoundEditor()));
	this->m_actionCollection->addAction("soundeditor", action);

	action = new KAction(KIcon(":/actions/objecteditor.png"), i18n("Object Editor"), this);
	action->setShortcut(KShortcut(i18n("F6")));
	connect(action, SIGNAL(triggered()), this, SLOT(showObjectEditor()));
	this->m_actionCollection->addAction("objecteditor", action);

	action = new KAction(KIcon(":/actions/campaigneditor.png"), i18n("Campaign Editor"), this);
	action->setShortcut(KShortcut(i18n("F7")));
	connect(action, SIGNAL(triggered()), this, SLOT(showCampaignEditor()));
	this->m_actionCollection->addAction("campaigneditor", action);

	action = new KAction(KIcon(":/actions/aieditor.png"), i18n("AI Editor"), this);
	action->setShortcut(KShortcut(i18n("F8")));
	connect(action, SIGNAL(triggered()), this, SLOT(showAiEditor()));
	this->m_actionCollection->addAction("aieditor", action);

	action = new KAction(KIcon(":/actions/modeleditor.png"), i18n("Model Editor"), this);
	action->setShortcut(KShortcut(i18n("F9")));
	connect(action, SIGNAL(triggered()), this, SLOT(showModelEditor()));
	this->m_actionCollection->addAction("modeleditor", action);

	action = new KAction(KIcon(":/actions/textureeditor.png"), i18n("Texture Editor"), this);
	action->setShortcut(KShortcut(i18n("F10")));
	connect(action, SIGNAL(triggered()), this, SLOT(showTextureEditor()));
	this->m_actionCollection->addAction("textureeditor", action);

	action = new KAction(KIcon(":/actions/objectmanager.png"), i18n("Object Manager"), this);
	action->setShortcut(KShortcut(i18n("F11")));
	connect(action, SIGNAL(triggered()), this, SLOT(showObjectManager()));
	this->m_actionCollection->addAction("objectmanager", action);

	action = new KAction(KIcon(":/actions/importmanager.png"), i18n("Import Manager"), this);
	action->setShortcut(KShortcut(i18n("F12")));
	connect(action, SIGNAL(triggered()), this, SLOT(showImportManager()));
	this->m_actionCollection->addAction("importmanager", action);

	this->setMapActionsEnabled(false);

	this->m_actionCollection->readSettings(); // load shortcuts after setting default

	/*
	QSettings settings("Blizzard Entertainment", "WorldEdit", this);
	settings.beginGroup("shortcuts");
	// Read shortcuts
	newMapAction->setShortcut(settings.value("newmap", KShortcut(i18n("Strg+N"))).toString());
	*/
	/*
	showTerrainEditor(); // test
	this->m_terrainEditor->resize(QSize(300, 300));
	*/
	showTextureEditor();
	this->m_textureEditor->openFile();
	//showModelEditor();
	//this->m_modelEditor->openFile();
}

Editor::~Editor()
{
	// do not delete allocated sub widgets (parent system of Qt already considers)
}

void Editor::showTerrainEditor()
{
	if (this->m_terrainEditor == 0)
		this->m_terrainEditor = new TerrainEditor(this);

	this->m_terrainEditor->show();
}

void Editor::showTriggerEditor()
{
	if (this->m_triggerEditor == 0)
		this->m_triggerEditor = new TriggerEditor(this);

	this->m_triggerEditor->show();
}

void Editor::showSoundEditor()
{
	if (this->m_soundEditor == 0)
		this->m_soundEditor = new SoundEditor(this);

	this->m_soundEditor->show();
}

void Editor::showObjectEditor()
{
	if (this->m_objectEditor == 0)
		this->m_objectEditor = new ObjectEditor(this);

	this->m_objectEditor->show();
}

void Editor::showCampaignEditor()
{
	if (this->m_campaignEditor == 0)
		this->m_campaignEditor = new CampaignEditor(this);

	this->m_campaignEditor->show();
}

void Editor::showAiEditor()
{
	if (this->m_aiEditor == 0)
		this->m_aiEditor = new AiEditor(this);

	this->m_aiEditor->show();
}

void Editor::showObjectManager()
{
	if (this->m_objectManager == 0)
		this->m_objectManager = new ObjectManager(this);

	this->m_objectManager->show();
}

void Editor::showImportManager()
{
	if (this->m_importManager == 0)
		this->m_importManager = new ImportManager(this);

	this->m_importManager->show();
}

void Editor::showMpqEditor()
{
	if (this->m_mpqEditor == 0)
		this->m_mpqEditor = new MpqEditor(this);

	this->m_mpqEditor->show();
}

void Editor::showModelEditor()
{
	if (this->m_modelEditor == 0)
		this->m_modelEditor = new ModelEditor(this);

	this->m_modelEditor->show();
	this->m_modelEditor->openFile(); // TODO test
}

void Editor::showTextureEditor()
{
	if (this->m_textureEditor == 0)
		this->m_textureEditor = new TextureEditor(this);

	this->m_textureEditor->show();
}

void Editor::addResource(Resource *resource)
{
	assert(resource);

	this->m_resources.push_back(resource);
	this->addEntry(resource->url(), 0);
}

bool Editor::removeResource(Resource *resource)
{
	assert(resource);

	std::list<Resource*>::iterator iterator = std::find(this->m_resources.begin(), this->m_resources.end(), resource);

	if (iterator == this->m_resources.end())
		return false;

	this->m_resources.erase(iterator);
	this->removeEntry(resource->url()); // remove from MPQ priority list
}

void Editor::newMap()
{
	if (this->m_newMapDialog == 0)
		this->m_newMapDialog = new NewMapDialog(this);

	this->m_newMapDialog->show();
}

void Editor::setMapActionsEnabled(bool enabled)
{
	this->m_actionCollection->action("closemap")->setEnabled(enabled);
	this->m_actionCollection->action("savemap")->setEnabled(enabled);
	this->m_actionCollection->action("savemapas")->setEnabled(enabled);
	this->m_actionCollection->action("savemapshadows")->setEnabled(enabled);
}

}

}
