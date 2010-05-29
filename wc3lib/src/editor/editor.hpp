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

#ifndef WC3LIB_EDITOR_EDITOR_HPP
#define WC3LIB_EDITOR_EDITOR_HPP

#include <list>

#include <kmainwindow.h>

#include <boost/filesystem.hpp>

namespace wc3lib
{
	
namespace mpq
{

class Mpq;
class MpqFile;	

}

namespace editor
{

class TerrainEditor;
class TriggerEditor;
class SoundEditor;
class ObjectEditor;
class CampaignEditor;
class AiEditor;
class ObjectManager;
class ImportManager;
class MpqEditor;
class ModelEditor;
class TextureEditor;
class NewMapDialog;

/**
* @todo Each @class SubEditor has it's own tool bar with all other sub editors.
*/
class Editor : public KMainWindow
{
	Q_OBJECT
	
	public:
		Editor(QWidget *parent = 0, Qt::WindowFlags f = Qt::Window);
		~Editor();
		
		/**
		* Each time a file has to searched for, all editor MPQ archives will be checked for in the ordering of their priority.
		* Higher priority means it will be searched through befor MPQ archives with less priority.
		* @return Returns the MPQ's position in editor MPQ list.
		*/
		std::size_t addMpq(const class Mpq *mpq, std::size_t priority);
		
		const class mpq::MpqFile* loadMpqFile(const boost::filesystem::path &path);
	
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
		void readSettings();

		std::list<class mpq::Mpq*> m_mpqs;
		std::list<std::size_t> m_mpqsPriorities;
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

}

}

#endif
