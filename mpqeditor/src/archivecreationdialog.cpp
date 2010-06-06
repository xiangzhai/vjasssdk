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
#include <kmessagebox.h>

#include "archivecreationdialog.h"
#include "StormLib/StormLib.h"
#include "StormLib/StormPort.h"
#include "mainwindow.h"

namespace mpqeditor
{

ArchiveCreationDialog::ArchiveCreationDialog(class QWidget *parent) : KDialog(parent)
{
	setupUi(this);
	
	connect(this, SIGNAL(okClicked()), this, SLOT(createArchive()));
	
	this->m_archivePathUrlRequester->setMode(KFile::File | KFile::LocalOnly);
	this->m_directoryPathUrlRequester->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);
}

void ArchiveCreationDialog::createArchive()
{
	if (QFile::exists(this->m_archivePathUrlRequester->url().toLocalFile()))
	{
		if (KMessageBox::questionYesNo(this, tr("File does already exist. Do you want to replace it?")) == KMessageBox::No)
			return;
	}
/// @todo Value of this parameter is corrected to be the nearest greater power of two in the range <0x2; 0x40000>
/*
	if (this->m_hashTableSizeSpinBox->value() % 16 != 0)
	{
		KMessageBox::error(this, tr("Attention"), tr("Bla"));
		return;
	}
*/	
	HANDLE mpqArchive;
	DWORD flags = CREATE_ALWAYS;
	
	if (this->m_attributesCheckBox->isChecked())
		flags |= MPQ_CREATE_ATTRIBUTES;
	
	if (this->m_version2CheckBox->isChecked())
		flags |= MPQ_CREATE_ARCHIVE_V2;
	else
		flags |= MPQ_CREATE_ARCHIVE_V1;
	
	QString filePath = this->m_archivePathUrlRequester->url().toLocalFile();
	
	if (SFileCreateArchiveEx(filePath.toAscii(),      // Archive file name
		flags, // Open or create
		this->m_hashTableSizeSpinBox->value(),       // Size of hash table
		&mpqArchive)) // Pointer to result HANDLE
	{
		static_cast<class MainWindow*>(this->parent())->openMpqArchive(filePath);
	}
	else
		KMessageBox::detailedError(this, tr("Error while creating archive \"%1\".").arg(filePath), tr("Error: \"%1\"").arg(ErrString(GetLastError())));
}

}
