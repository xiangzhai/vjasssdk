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

#ifndef MPQEDITOR_MAINWINDOW_H
#define MPQEDITOR_MAINWINDOW_H

#include <QVector>

#include <kmainwindow.h>
#include <kaboutdata.h>
#include <kactioncollection.h>
#include <kmenu.h>
#include <kaction.h>
#include <khelpmenu.h>
#include <kurl.h>

#include "StormLib/StormLib.h"
#include "StormLib/StormPort.h"

namespace mpqeditor
{

class ArchiveCreationDialog;

class MainWindow : public KMainWindow
{
	Q_OBJECT

	public:
		static KAboutData *mpqeditorAboutData;
		static KAboutData *stormlibAboutData;
	
		MainWindow();
		
		bool openMpqArchive(const QString &filePath);

	private slots:
		void fileNew();
		void fileOpen();
		void fileOpenRecent();
		void fileClose();
		
		void editAdd();

	private:
		class KActionCollection *m_actionCollection;
	
		class KMenu *m_fileMenu;
		class KAction *m_fileNewAction;
		class KAction *m_fileOpenAction;
		class KAction *m_fileReopenAction;
		class KAction *m_fileOpenRecentAction;
		class KAction *m_fileCloseAction;
		class KAction *m_fileQuitAction;
		class KMenu *m_editMenu;
		class KAction *m_editAddAction;
		class KMenu *m_viewMenu;
		class KMenu *m_settingsMenu;
		class KHelpMenu *m_helpMenu;
		class KAction *m_helpAboutStormlibAction;
		
		class ArchiveCreationDialog *m_archiveCreationDialog;
		
		KUrl m_openFileUrl;
		
		QVector<HANDLE> m_mpqArchives;
		int m_selectedMpqArchiveIndex;
		
		void addFile(const SFILE_FIND_DATA &findData);
};

}

#endif
