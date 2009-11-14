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

#include "editor.hpp"
#include "terraineditor.hpp"
//#include "newmapdialog.hpp"

namespace wc3lib
{

namespace editor
{

Editor::Editor(QWidget *parent, Qt::WindowFlags f) : KMainWindow(parent, f), m_terrainEditor(0)
{
	class KMenu *fileMenu = new KMenu(tr("File"), this);
	class KAction *newMapAction = new KAction(KIcon(":/actions/newmap.png"), tr("&New map ..."), this);
	connect(newMapAction, SIGNAL(triggered()), this, SLOT(newMap()));
	this->menuBar()->addMenu(fileMenu);
}

void Editor::newMap()
{
//	NewMapDialog::showDialog();
}

void Editor::showTerrainEditor()
{
	if (this->m_terrainEditor == 0)
		this->m_terrainEditor = new TerrainEditor(this);
	
	this->m_terrainEditor->show();
}
	

}

}
