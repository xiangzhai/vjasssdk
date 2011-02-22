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

#include <kmenu.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>

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
#include "settings.hpp"
#include "../internationalisation.hpp"
#include "../blp.hpp"

namespace wc3lib
{

namespace editor
{

KAboutData Editor::m_aboutData = KAboutData("editor", "", ki18n("World Editor"), "0.1", ki18n("Clone of Blizzard's Warcraft 3 The Frozen Throne World Editor."), KAboutData::License_GPL_V2, ki18n("Copyright (C) 2009 by Tamino Dauth <tamino@cdauth.eu>"), ki18n("Other"), "http://sourceforge.net/projects/vjasssdk/", "tamino@cdauth.eu")
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
	for (int i = 0; i < OgreMdlx::MaxTeamColors; ++i)
		this->m_teamColorImages[(enum OgreMdlx::TeamColor)i] = 0;

	for (int i = 0; i < OgreMdlx::MaxTeamColors; ++i)
		this->m_teamGlowImages[(enum OgreMdlx::TeamColor)i] = 0;

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

	this->readSettings();

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


	showModelEditor(); // test
}

Editor::~Editor()
{
	this->writeSettings();
	// do not delete allocated sub widgets (parent system of Qt already considers)
}

Ogre::Image* Editor::blpToOgre(const KUrl &url, const QString &format) const throw (class Exception)
{
	qDebug() << "New URL " << findFile(url).toLocalFile();
	QFile file(findFile(url).toLocalFile());

	if (!file.open(QIODevice::ReadOnly))
		throw Exception(boost::format(_("Unable to open texture image \"%1%\".")) % url.toLocalFile().toAscii().data());

	QImage qImage;

	if (!qImage.load(&file, 0))
		throw Exception(boost::format(_("Unable to load texture image \"%1%\".")) % url.toLocalFile().toAscii().data());

	file.close();
	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	qImage.save(&buffer, format.toAscii().data());

	// TEST (writing buffer on disk)
	/*
	KUrl fileUrl = KFileDialog::getSaveUrl(KUrl(""), i18n("*.png|PNG"));

	if (!fileUrl.isEmpty() && fileUrl.isLocalFile())
	{
		QFile newFile(fileUrl.toLocalFile());
		newFile.open(QIODevice::WriteOnly);
		newFile.write(ba.data(), ba.size());
		newFile.close();

		KMessageBox::information(0, i18n("Hat geklappt!"));
	}
	*/
	// END TEST
	// bis hierher klappt alles, die PNG-Bilder sehen korrekt aus, wenn sie auf die Festplatte geschrieben werden

	// load for OGRE
	//void *memory[ba.size()];
	//memcpy(memory, (const void*)(ba), ba.size());
	Ogre::DataStreamPtr dsPtr(new Ogre::MemoryDataStream(static_cast<void*>(ba.data()), ba.size()));
	//ba.clear();
	//dsPtr.bind(dynamic_cast<Ogre::DataStream*>(ms));
	Ogre::Image *image = new Ogre::Image();

	try
	{
		image->load(dsPtr);
	}
	catch (Ogre::Exception &exception)
	{
		throw Exception(boost::format(_("Unable to open texture image \"%1%\".\nOGRE error: \"%2%\"")) % url.toLocalFile().toAscii().data() % exception.what());
	}

	return image;
}

void Editor::showTerrainEditor()
{
	this->terrainEditor()->show();
}

void Editor::showTriggerEditor()
{
	this->triggerEditor()->show();
}

void Editor::showSoundEditor()
{
	this->soundEditor()->show();
}

void Editor::showObjectEditor()
{
	this->objectEditor()->show();
}

void Editor::showCampaignEditor()
{
	this->campaignEditor()->show();
}

void Editor::showAiEditor()
{
	this->aiEditor()->show();
}

void Editor::showObjectManager()
{
	this->objectManager()->show();
}

void Editor::showImportManager()
{
	this->importManager()->show();
}

void Editor::showMpqEditor()
{
	this->mpqEditor()->show();
}

void Editor::showModelEditor()
{
	this->modelEditor()->show();
}

void Editor::showTextureEditor()
{
	this->textureEditor()->show();
}

void Editor::newMap()
{
	this->newMapDialog()->show();
}

void Editor::openMap(const KUrl &url)
{
	/*
	Map map;
	this->addResource(map);
	*/
}

void Editor::switchToMap(const class Map *map)
{
}

void Editor::closeMap(const class Map *map)
{
}

void Editor::setMapActionsEnabled(bool enabled)
{
	this->m_actionCollection->action("closemap")->setEnabled(enabled);
	this->m_actionCollection->action("savemap")->setEnabled(enabled);
	this->m_actionCollection->action("savemapas")->setEnabled(enabled);
	this->m_actionCollection->action("savemapshadows")->setEnabled(enabled);
}

void Editor::readSettings()
{
	/// @todo Actions should get the same entry names and shortcuts as in the original World Editor?
	this->m_actionCollection->setConfigGroup("Shortcuts");
	this->m_actionCollection->readSettings(); // load shortcuts after setting default

	Settings settings(this);
	settings.read(KGlobal::config()->group("Settings"));
	/*
	TODO create from component data?!
	KConfig config(this->com
	Settings settings(this);
	settings.read(settings.groupName()
	*/
}

void Editor::writeSettings()
{
	this->m_actionCollection->setConfigGroup("Shortcuts");
	this->m_actionCollection->writeSettings();

	Settings settings(this);
	KConfigGroup settingsConfigGroup = KGlobal::config()->group("Settings");
	settings.write(settingsConfigGroup);

	/*
	TODO create from component data?!
	KConfig config
	Settings settings(this);
	settings.write(settings.groupName());
	*/
}

#include "moc_editor.cpp"

}

}
