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
#include "settingsinterface.hpp"

namespace wc3lib
{

namespace editor
{

Module::Module(class Editor *editor) : m_editor(editor), m_menuBar(0), m_topLayout(new QVBoxLayout(this))
{
}

Module::~Module()
{
}

void Module::setupUi()
{
	this->m_menuBar = new KMenuBar(this);
	m_topLayout->addWidget(this->m_menuBar);

	this->m_fileMenu = new KMenu(tr("File"), this);
	this->menuBar()->addMenu(this->m_fileMenu);
	connect(this->m_fileMenu, SIGNAL(triggered(QAction *)), this, SLOT(triggered(QAction*)));

	// use actions from editor
	this->m_fileMenu->addAction(this->editor()->actionCollection()->action("newmap"));
	this->m_fileMenu->addAction(this->editor()->actionCollection()->action("openmap"));
	this->m_fileMenu->addAction(this->editor()->actionCollection()->action("closemap"));
	this->m_fileMenu->addSeparator();
	this->m_fileMenu->addAction(this->editor()->actionCollection()->action("savemap"));
	this->m_fileMenu->addAction(this->editor()->actionCollection()->action("savemapas"));
	this->m_fileMenu->addAction(this->editor()->actionCollection()->action("savemapshadows"));
	this->m_fileMenu->addSeparator();

	// create user-defined actions in file menu
	this->createFileActions(this->m_fileMenu);

	// use actions from editor
	this->m_fileMenu->addSeparator();
	this->m_fileMenu->addAction(this->editor()->actionCollection()->action("testmap"));
	this->m_fileMenu->addSeparator();
	this->m_fileMenu->addAction(this->editor()->actionCollection()->action("closemodule"));

	this->m_editMenu = new KMenu(tr("Edit"), this);
	this->menuBar()->addMenu(this->m_editMenu);

	// create user-defined actions in edit menu
	this->createEditActions(this->m_editMenu);

	// module menu
	this->m_moduleMenu = new ModuleMenu(this, this->editor());
	this->menuBar()->addMenu(this->m_moduleMenu);

	// create user-defined menus
	this->createMenus(this->menuBar());

	this->m_windowsMenu = new KMenu(tr("Windows"), this);
	this->menuBar()->addMenu(windowsMenu());

	// create user-defined actions in windows menu
	this->createWindowsActions(windowsMenu());

	// tool bar
	this->m_toolBar = new KToolBar(this);
	m_topLayout->addWidget(toolBar());
	toolBar()->addSeparator();

	// user defined tool buttons
	this->createToolButtons(toolBar());

	toolBar()->addSeparator();

	// modules tool buttons
	foreach (QAction *action, moduleMenu()->actions())
		toolBar()->addAction(action);

	// test map tool button
	toolBar()->addAction(this->editor()->actionCollection()->action("testmap"));
}

void Module::readSettings()
{
	SettingsInterface *settings = this->settings();
	KConfigGroup configGroup(KGlobal::config(), settings->groupName());
	settings->read(configGroup);
	delete settings;
}

void Module::writeSettings()
{
	SettingsInterface *settings = this->settings();
	KConfigGroup configGroup(KGlobal::config(), settings->groupName());
	settings->write(configGroup);
	delete settings;
}

void Module::triggered(QAction *action)
{
	if (action == this->editor()->actionCollection()->action("closemodule"))
	{
		this->close();
	}
}

#include "moc_module.cpp"

}

}
