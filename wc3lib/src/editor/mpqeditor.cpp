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

#include "mpqeditor.hpp"

namespace wc3lib
{

namespace editor
{

MpqEditor::MpqEditor(class Editor *editor) : Module(editor)
{
}

void MpqEditor::newMpqArchive()
{
}

void MpqEditor::openMpqArchive()
{
}

void MpqEditor::closeMpqArchive()
{
}

void MpqEditor::addFiles()
{
}

void MpqEditor::extractFiles()
{
}

void MpqEditor::removeFiles()
{
}

void MpqEditor::createFileActions(class KMenu *menu)
{
}

void MpqEditor::createEditActions(class KMenu *menu)
{
}

void MpqEditor::createMenus(class KMenuBar *menuBar)
{
}

void MpqEditor::createWindowsActions(class KMenu *menu)
{
}

void MpqEditor::createToolButtons(class KToolBar *toolBar)
{
}

class SettingsInterface* MpqEditor::settings()
{
	/// @todo FIXME
	return 0;
}

#include "moc_mpqeditor.cpp"

}

}
