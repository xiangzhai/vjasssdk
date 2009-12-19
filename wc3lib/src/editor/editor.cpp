/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.de                                                      *
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
#include <kmenubar.h>
#include <boost/foreach.hpp>

#include "editor.hpp"
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
#include "../mpq/mpq.hpp"
#include "../mpq/mpqfile.hpp"

namespace wc3lib
{

namespace editor
{

Editor::Editor(QWidget *parent, Qt::WindowFlags f) : KMainWindow(parent, f), m_terrainEditor(0), m_triggerEditor(0), m_soundEditor(0), m_objectEditor(0), m_campaignEditor(0), m_aiEditor(0), m_objectManager(0), m_importManager(0), m_mpqEditor(0), m_modelEditor(0), m_textureEditor(0), m_newMapDialog(0)
{
	class KMenu *fileMenu = new KMenu(tr("File"), this);
	class KAction *newMapAction = new KAction(KIcon(":/actions/newmap.png"), tr("&New map ..."), this);
	connect(newMapAction, SIGNAL(triggered()), this, SLOT(newMap()));
	this->menuBar()->addMenu(fileMenu);
}

Editor::~Editor()
{
	if (this->m_terrainEditor != 0)
		delete this->m_terrainEditor;
	
	if (this->m_triggerEditor != 0)
		delete this->m_triggerEditor;
	
	if (this->m_soundEditor != 0)
		delete this->m_soundEditor;
	
	if (this->m_objectEditor != 0)
		delete this->m_objectEditor;
	
	if (this->m_campaignEditor != 0)
		delete this->m_campaignEditor;
	
	if (this->m_aiEditor != 0)
		delete this->m_aiEditor;
	
	if (this->m_objectManager != 0)
		delete this->m_objectManager;
	
	if (this->m_importManager != 0)
		delete this->m_importManager;
	
	if (this->m_mpqEditor != 0)
		delete this->m_mpqEditor;
	
	if (this->m_modelEditor != 0)
		delete this->m_modelEditor;
	
	if (this->m_textureEditor != 0)
		delete this->m_textureEditor;
	
	if (this->m_newMapDialog != 0)
		delete this->m_newMapDialog;
}

void Editor::addMpq(class Mpq *mpq, std::size_t priority)
{
	std::list<class mpq::Mpq*>::const_iterator mpqIterator = this->m_mpqs.begin();
	std::list<std::size_t>::const_iterator priorityIterator = this->m_mpqsPriorities.begin();
	
	while (mpqIterator != this->m_mpqs.end())
	{
		if (*priorityIterator < priority)
		{
			this->m_mpqs.insert(mpqIterator,mpq);
			this->m_mpqsPriorities.insert(priorityIterator, priority);
			
			return;
		}
		
		++mpqIterator;
		++priorityIterator;
	}
	
	this->m_mpqs.push_back(mpq);
	this->m_mpqsPriorities.push_back(priority);
}

const class mpq::MpqFile* Editor::loadMpqFile(const boost::filesystem::path &path)
{
	const class mpq::MpqFile *result = 0;
	
	BOOST_FOREACH(const class mpq::Mpq *mpq, this->m_mpqs)
	{
		result = mpq->findFile(path);
		
		if (result != 0)
			break;
	}
	
	return result;
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
}

void Editor::showTextureEditor()
{
	if (this->m_textureEditor == 0)
		this->m_textureEditor = new TextureEditor(this);
	
	this->m_textureEditor->show();
}

void Editor::newMap()
{
	if (this->m_newMapDialog == 0)
		this->m_newMapDialog = new NewMapDialog(this);
	
	this->m_newMapDialog->show();
}
	

}

}
