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

#include <QtGui>

#include <kmenu.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <kactioncollection.h>

#include "module.hpp"
#include "modulemenu.hpp"
#include "editor.hpp"

namespace wc3lib
{

namespace editor
{

Module::Module(class Editor *editor) : m_editor(editor), m_menuBar(0), m_topLayout(new QVBoxLayout(this))
{
}

KMenuBar* Module::menuBar() const
{
	return this->m_menuBar;
}

void Module::setupUi()
{
	this->m_menuBar = new KMenuBar(this);
	m_topLayout->addWidget(this->m_menuBar);

	KMenu *menu = new KMenu(tr("File"), this);
	this->menuBar()->addMenu(menu);

	// use actions from editor
	menu->addAction(this->editor()->actionCollection()->action("newmap"));
	menu->addAction(this->editor()->actionCollection()->action("openmap"));
	menu->addAction(this->editor()->actionCollection()->action("closemap"));
	menu->addSeparator();
	menu->addAction(this->editor()->actionCollection()->action("savemap"));
	menu->addAction(this->editor()->actionCollection()->action("savemapas"));
	menu->addAction(this->editor()->actionCollection()->action("savemapshadows"));
	menu->addSeparator();

	// create user-defined actions in file menu
	this->createFileActions(menu);

	// use actions from editor
	menu->addSeparator();
	menu->addAction(this->editor()->actionCollection()->action("testmap"));
	menu->addSeparator();
	menu->addAction(this->editor()->actionCollection()->action("closemodule"));

	menu = new KMenu(tr("Edit"), this);
	this->menuBar()->addMenu(menu);

	// create user-defined actions in edit menu
	this->createEditActions(menu);

	// module menu
	class ModuleMenu *moduleMenu = new ModuleMenu(this, this->editor());
	this->menuBar()->addMenu(moduleMenu);

	// create user-defined menus
	this->createMenus(this->menuBar());

	menu = new KMenu(tr("Windows"), this);
	this->menuBar()->addMenu(menu);

	// create user-defined actions in windows menu
	this->createWindowsActions(menu);

	// tool bar
	KToolBar *toolBar = new KToolBar(this);
	m_topLayout->addWidget(toolBar);
	toolBar->addSeparator();

	// user defined tool buttons
	this->createToolButtons(toolBar);

	toolBar->addSeparator();

	// modules tool buttons
	foreach (QAction *action, moduleMenu->actions())
		toolBar->addAction(action);

	// test map tool button
	toolBar->addAction(this->editor()->actionCollection()->action("testmap"));
}

}

}
