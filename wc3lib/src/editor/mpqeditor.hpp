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

#ifndef WC3LIB_EDITOR_MPQEDITOR_HPP
#define WC3LIB_EDITOR_MPQEDITOR_HPP

#include <list>

#include "module.hpp"

namespace wc3lib
{

namespace mpq
{

class Mpq;

}

namespace editor
{

/**
* File-based MPQ editor which uses wc3lib's MPQ protocol plug-in (\ref MpqArchive) for MPQ
* file handling.
* Moreover it allows you to easily open Warcraft'3 default MPQ archives.
* MPQ archive list includes them, too and additionally all MPQ archives from editor's MPQ priority list (\ref MpqPriorityList) plus all opened maps and campaigns in editor.
* \sa MpqArchive, MpqPriorityList
*/
class MpqEditor : public Module
{
	Q_OBJECT

	public:
		MpqEditor(class Editor *editor);

	public slots:
		void newMpqArchive();
		void openMpqArchive();
		void closeMpqArchive();
		void optimizeMpqArchive();
		void optimizeAllMpqArchives();

		void addFiles();
		/**
		* Extracts all selected files (including directories) into a given target which is defined by user via file dialog.
		*/
		void extractFiles(); // selected files
		/**
		* Removes all selected files (including directories) from the given MPQ archive if possible (archive may be opened in read-only mode).
		*/
		void removeFiles();
		/**
		* Optimizes all selected files (including directories) which means that their compression and other flags will be defined trying to get the best compression results.
		* \note If archive is open in read-only mode this can not be used on any of its files.
		*/
		void optimizeFiles();
		/**
		* Creates a new directory with a user-defined name in the MPQ archive.
		*/
		void createDirectory();

		void openWar3();
		void openWar3X();
		void openWar3Patch();
		void openWar3XLocal();

	protected:
		virtual void createFileActions(class KMenu *menu);
		virtual void createEditActions(class KMenu *menu);
		virtual void createMenus(class KMenuBar *menuBar);
		virtual void createWindowsActions(class KMenu *menu);
		virtual void createToolButtons(class KToolBar *toolBar);
		virtual class SettingsInterface* settings();

		std::list<class mpq::Mpq*> m_mpqArchives;
};

}

}

#endif
