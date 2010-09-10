/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#include <kmenu.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kmenubar.h>
#include <klocale.h>

#include <QtGui>

#include "modulemenu.hpp"
#include "editor.hpp"

namespace wc3lib
{

namespace editor
{

ModuleMenu::ModuleMenu(QWidget *widget, class Editor *editor) : KMenu(tr("Module"), widget), m_actionCollection(new KActionCollection(widget))
{
	class KAction *action = new KAction(KIcon(":/actions/terraineditor.png"), i18n("Terrain Editor"), this);
	action->setShortcut(KShortcut(i18n("F3")));
	connect(action, SIGNAL(triggered()), editor, SLOT(showTerrainEditor()));
	this->m_actionCollection->addAction("terraineditor", action);
	this->addAction(action);

	action = new KAction(KIcon(":/actions/triggereditor.png"), i18n("Trigger Editor"), this);
	action->setShortcut(KShortcut(i18n("F4")));
	connect(action, SIGNAL(triggered()), editor, SLOT(showTriggerEditor()));
	this->m_actionCollection->addAction("triggereditor", action);
	this->addAction(action);

	action = new KAction(KIcon(":/actions/soundeditor.png"), i18n("Sound Editor"), this);
	action->setShortcut(KShortcut(i18n("F5")));
	connect(action, SIGNAL(triggered()), editor, SLOT(showSoundEditor()));
	this->m_actionCollection->addAction("soundeditor", action);
	this->addAction(action);

	action = new KAction(KIcon(":/actions/objecteditor.png"), i18n("Object Editor"), this);
	action->setShortcut(KShortcut(i18n("F6")));
	connect(action, SIGNAL(triggered()), editor, SLOT(showObjectEditor()));
	this->m_actionCollection->addAction("objecteditor", action);
	this->addAction(action);

	this->addSeparator();

	action = new KAction(KIcon(":/actions/campaigneditor.png"), i18n("Campaign Editor"), this);
	action->setShortcut(KShortcut(i18n("F7")));
	connect(action, SIGNAL(triggered()), editor, SLOT(showCampaignEditor()));
	this->m_actionCollection->addAction("campaigneditor", action);
	this->addAction(action);

	action = new KAction(KIcon(":/actions/aieditor.png"), i18n("AI Editor"), this);
	action->setShortcut(KShortcut(i18n("F8")));
	connect(action, SIGNAL(triggered()), editor, SLOT(showAiEditor()));
	this->m_actionCollection->addAction("aieditor", action);
	this->addAction(action);

	this->addSeparator();

	action = new KAction(KIcon(":/actions/modeleditor.png"), i18n("Model Editor"), this);
	action->setShortcut(KShortcut(i18n("F9")));
	connect(action, SIGNAL(triggered()), editor, SLOT(showModelEditor()));
	this->m_actionCollection->addAction("modeleditor", action);
	this->addAction(action);

	action = new KAction(KIcon(":/actions/textureeditor.png"), i18n("Texture Editor"), this);
	action->setShortcut(KShortcut(i18n("F10")));
	connect(action, SIGNAL(triggered()), editor, SLOT(showTextureEditor()));
	this->m_actionCollection->addAction("textureeditor", action);
	this->addAction(action);

	this->addSeparator();

	action = new KAction(KIcon(":/actions/objectmanager.png"), i18n("Object Manager"), this);
	action->setShortcut(KShortcut(i18n("F11")));
	connect(action, SIGNAL(triggered()), editor, SLOT(showObjectManager()));
	this->m_actionCollection->addAction("objectmanager", action);
	this->addAction(action);

	action = new KAction(KIcon(":/actions/importmanager.png"), i18n("Import Manager"), this);
	action->setShortcut(KShortcut(i18n("F12")));
	connect(action, SIGNAL(triggered()), editor, SLOT(showImportManager()));
	this->m_actionCollection->addAction("importmanager", action);
	this->addAction(action);
}

}

}
