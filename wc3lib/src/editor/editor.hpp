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

#include <kmainwindow.h>

namespace wc3lib
{

namespace editor
{

class TerrainEditor;
class TriggerEditor;
class ObjectEditor;
class AiEditor;
class CampaignEditor;
class SoundEditor;
class ImportEditor;
class MpqEditor; // new
class ModelEditor; // new
class TextureEditor; // new

class Editor : public KMainWindow
{
	Q_OBJECT
	
	public:
		Editor(QWidget *parent, Qt::WindowFlags f);
	
	public slots:
		void newMap();
		void showTerrainEditor();
		
	private:
		class TerrainEditor *m_terrainEditor;
		class TriggerEditor *m_triggerEditor;
		class ObjectEditor *m_objectEditor;
		class AiEditor *m_aiEditor;
		class CampaignEditor *m_campaignEditor;
		class SoundEditor *m_soundEditor;
		class ImportEditor *m_importEditor;
};

}

}

#endif // EDITOR_HPP
