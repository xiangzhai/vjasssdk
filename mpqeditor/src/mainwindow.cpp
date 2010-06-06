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

#include <QtGui>

#include <kfiledialog.h>
#include <kmessagebox.h>
#include <kmenubar.h>

#include "mainwindow.h"
#include "archivecreationdialog.h"
#include "archiveswidget.h"

namespace mpqeditor
{

KAboutData *MainWindow::mpqeditorAboutData = new KAboutData("mpqeditor",
	"",
	ki18n("mpqeditor"),
	"0.1",
	ki18n("An editor for MPQ archives."),
	KAboutData::License_GPL_V2,
	ki18n("(C) 2009 Tamino Dauth"),
	ki18n("Bla bla bla"),
	"http://sourceforge.net/projects/vjasssdk/");
	
KAboutData *MainWindow::stormlibAboutData = new KAboutData("StormLib",
	"",
	ki18n("StormLib"),
	QString("%1").arg(STORMLIB_VERSION).toAscii(),
	ki18n("An MPQ accessing library."),
	KAboutData::License_Custom,
	ki18n("(C) Ladislav Zezula 1999 - 2005"),
	ki18n("Bla bla bla"),
	"http://www.zezula.net/");

void MainWindow::fileNew()
{
	if (this->m_archiveCreationDialog == 0)
		m_archiveCreationDialog = new ArchiveCreationDialog(this->centralWidget());
	
	this->m_archiveCreationDialog->show();
}

void MainWindow::fileOpen()
{
	QStringList filePaths = KFileDialog::getOpenFileNames(this->m_openFileUrl, tr("*.mpq, *.MPQ|MPQ archives\n*.w3m;;*.W3M|Warcraft 3 Maps"), this, tr("Open MPQ Archive"));
	
	if (filePaths.isEmpty())
		return;
	
	foreach (QString iterator, filePaths)
		this->openMpqArchive(iterator);
}

void MainWindow::fileOpenRecent()
{
}

void MainWindow::fileClose()
{
	SFileCloseArchive(this->m_mpqArchives[this->m_selectedMpqArchiveIndex]);
	this->m_mpqArchives.remove(this->m_selectedMpqArchiveIndex);
}

void MainWindow::editAdd()
{
}

MainWindow::MainWindow() : m_archiveCreationDialog(0), m_selectedMpqArchiveIndex(-1)
{
	//central widget
	this->setCentralWidget(new ArchivesWidget(this));
	//actions
	this->m_actionCollection = new KActionCollection(this);
	
	this->m_fileMenu = new KMenu(tr("File"), this);
	this->m_fileNewAction = this->m_actionCollection->addAction(
	KStandardAction::New,
        this,
        SLOT(fileNew()));
	this->m_fileMenu->addAction(this->m_fileNewAction);
	this->m_fileOpenAction = this->m_actionCollection->addAction(
	KStandardAction::Open,
        this,
        SLOT(fileOpen()));
	this->m_fileMenu->addAction(this->m_fileOpenAction);
	this->m_fileOpenRecentAction = this->m_actionCollection->addAction(
	KStandardAction::OpenRecent,
        this,
        SLOT(fileOpenRecent()));
	this->m_fileMenu->addAction(this->m_fileOpenRecentAction);
	this->m_fileCloseAction = this->m_actionCollection->addAction(
	KStandardAction::Close,
        this,
        SLOT(fileClose()));
	this->m_fileMenu->addAction(this->m_fileCloseAction);
	this->m_fileQuitAction = this->m_actionCollection->addAction(
	KStandardAction::Quit,
        this,
        SLOT(close()));
	this->m_fileMenu->addAction(this->m_fileQuitAction);
	this->menuBar()->addMenu(this->m_fileMenu);
	
	this->m_editMenu = new KMenu(tr("Edit"), this);
	this->m_editAddAction = this->m_actionCollection->addAction(tr("Add"),
        this,
        SLOT(editAdd()));
	this->m_editMenu->addAction(this->m_editAddAction);
	this->menuBar()->addMenu(this->m_editMenu);
	this->m_helpMenu = new KHelpMenu(this, MainWindow::mpqeditorAboutData);
	this->m_helpAboutStormlibAction = this->m_actionCollection->addAction(tr("About StormLib"),
        this,
        SLOT(helpAboutStormLib()));
	this->m_helpMenu->menu()->insertAction(this->m_helpMenu->action(KHelpMenu::menuAboutApp), this->m_helpAboutStormlibAction);
	//this->m_helpMenu->menu()->addTitle(tr("About StormLib"), this->m_helpMenu->action(KHelpMenu::menuAboutApp));
	this->menuBar()->addMenu(this->m_helpMenu->menu());
	
}

bool MainWindow::openMpqArchive(const QString &filePath)
{
	qDebug() << "Try to open " << filePath;

	HANDLE mpqArchive;
		
	if (!SFileOpenArchive(filePath.toAscii(), // Archive file name
		0,       // Archive priority
		0,          // Open flags, SFILE_OPEN_HARD_DISK_FILE
		&mpqArchive))          // Pointer to result HANDLE
	{
		KMessageBox::detailedError(this, tr("Error while opening archive \"%1\".").arg(filePath), tr("Error: \"%1\"").arg(ErrString(GetLastError())));
		return false;
	}
	
	this->m_mpqArchives << mpqArchive;
	
	SFILE_FIND_DATA findData;

	HANDLE file = SFileFindFirstFile(
		mpqArchive,                      // Handle of open archive
		"*",              // Wildcard mask
		&findData, // Struct with search result
		NULL          // File list name
		);
	
	if (file == NULL)
	{
		KMessageBox::information(this, tr("Archive is empty."));
		return false;
	}
	
	this->addFile(findData);
	
	while (SFileFindNextFile(
	file,                    // Search handle
	&findData // Struct with search result
	))
	{
		this->addFile(findData);
	}

	SFileFindClose(file);
	
	return true;
}

void MainWindow::addFile(const SFILE_FIND_DATA &findData)
{
	qDebug() << "Adding file with data:\n"
	<< findData.cFileName << '\n'
	<< findData.szPlainName << '\n'
	<< findData.lcLocale << '\n'
	<< findData.dwFileSize << '\n'
	<< findData.dwFileFlags << '\n'
	;
}

}
