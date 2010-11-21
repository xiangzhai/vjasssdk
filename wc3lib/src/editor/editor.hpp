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

#ifndef WC3LIB_EDITOR_EDITOR_HPP
#define WC3LIB_EDITOR_EDITOR_HPP

#include <list>

#include <kmainwindow.h>
#include <kaboutdata.h>
#include <kurl.h>

#include <boost/filesystem.hpp>

#include "mpqprioritylist.hpp"

namespace wc3lib
{

namespace mpq
{

class Mpq;
class MpqFile;

}

namespace editor
{

/**
* All modules are stored as pointers and created on request. Therefore their initial value is 0 and they're allocated when needed.
* @todo Each Module has it's own tool bar with all other modules.
* @todo Add class MpqPriorityList which is hold for loaded MPQ archives -> map specific?
*/
class Editor : public KMainWindow, public MpqPriorityList
{
	Q_OBJECT

	public:
		static const KAboutData& aboutData();
		static const KAboutData& wc3libAboutData();

		Editor(QWidget *parent = 0, Qt::WindowFlags f = Qt::Window);
		virtual ~Editor();

		class KActionCollection* actionCollection() const;

	public slots:
		void newMap();
		void showTerrainEditor();
		void showTriggerEditor();
		void showSoundEditor();
		void showObjectEditor();
		void showCampaignEditor();
		void showAiEditor();
		void showObjectManager();
		void showImportManager();
		void showMpqEditor();
		void showModelEditor();
		void showTextureEditor();

	protected:
		/// Enables or disables actions which can only be used with an opened map.
		void setMapActionsEnabled(bool enabled);

		static KAboutData m_aboutData;
		static KAboutData m_wc3libAboutData;

		class KActionCollection *m_actionCollection;
		class MpqPriorityList *m_mpqs;
		class TerrainEditor *m_terrainEditor;
		class TriggerEditor *m_triggerEditor;
		class SoundEditor *m_soundEditor;
		class ObjectEditor *m_objectEditor;
		class CampaignEditor *m_campaignEditor;
		class AiEditor *m_aiEditor;
		class ObjectManager *m_objectManager;
		class ImportManager *m_importManager;
		class MpqEditor *m_mpqEditor; // new
		class ModelEditor *m_modelEditor; // new
		class TextureEditor *m_textureEditor; // new
		class NewMapDialog *m_newMapDialog;
};

inline class KActionCollection* Editor::actionCollection() const
{
	return this->m_actionCollection;
}

}

}

#endif
