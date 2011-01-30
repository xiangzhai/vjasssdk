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

#include <map>

#include <kmainwindow.h>
#include <kaboutdata.h>
#include <kurl.h>

#include <boost/filesystem.hpp>

#include "mpqprioritylist.hpp"
#include "resource.hpp"
#include "terraineditor.hpp"
#include "triggereditor.hpp"
#include "soundeditor.hpp"
#include "objecteditor.hpp"
#include "campaigneditor.hpp"
#include "aieditor.hpp"
#include "objectmanager.hpp"
#include "importmanager.hpp"
#include "mpqeditor.hpp"
#include "modeleditor.hpp"
#include "textureeditor.hpp"
#include "newmapdialog.hpp"
#include "ogremdlx.hpp"

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
* Editor holds all resources and required MPQ archives since it inherits from MpqPriorityList.
* If a resource is required (by its URL) it could theoretically be cached if implemented in class Resource.
* Just use Editor#resources()[URL] to refer to your required resource.
* @todo Each Module has it's own tool bar with all other modules.
*/
class Editor : public KMainWindow, public MpqPriorityList
{
	Q_OBJECT

	signals:
		/**
		* This signal is being emitted when a map has been opened.
		*/
		void openedMap(const class Map *map);
		/**
		* Since one Editor instance allows you to edit multiple maps this signal is emitted when the currently opened map is changed.
		* This signal can be important to all editor's modules because they're oftenly showing map-related data
		*/
		void switchedToMap(const class Map *map);
		/**
		* Is emitted when a map is going to be closed.
		*/
		void aboutToCloseMap(const class Map *map);
		/**
		* Is emitted when a map has finally been closed.
		* Therefore there is no Map parameter since the object has already been released.
		* \p url URL of the corresponding map file.
		*/
		void closedMap(const KUrl &url);

	public:
		typedef Editor self;

		static const KAboutData& aboutData();
		static const KAboutData& wc3libAboutData();

		static KUrl teamColorUrl(enum OgreMdlx::TeamColor teamColor);
		static KUrl teamGlowUrl(enum OgreMdlx::TeamColor teamGlow);

		Editor(QWidget *parent = 0, Qt::WindowFlags f = Qt::Window);
		virtual ~Editor();

		class KActionCollection* actionCollection() const;
		class TerrainEditor* terrainEditor() const;
		class TriggerEditor* triggerEditor() const;
		class SoundEditor* soundEditor() const;
		class ObjectEditor* objectEditor() const;
		class CampaignEditor* campaignEditor() const;
		class AiEditor* aiEditor() const;
		class ObjectManager* objectManager() const;
		class ImportManager* importManager() const;
		class MpqEditor* mpqEditor() const;
		class ModelEditor* modelEditor() const;
		class TextureEditor* textureEditor() const;
		class NewMapDialog* newMapDialog() const;

		/**
		* All added resources will also be added to MPQ priority list automaticially.
		* Therefore there shouldn't occur any problems when you open an external MDL file and need its textures which are contained by the same directory.
		*/
		void addResource(class Resource *resource);
		bool removeResource(class Resource *resource);
		bool removeResource(const KUrl &url);
		const std::map<KUrl, class Resource*>& resources() const;

		/**
		* \todo Use KUrl and IO slaves (MPQ protocol)
		* Opens an external image file with the given URL \p url and reads it by using Qt which allows you to read BLP files as well and converts it into an OGRE image.
		* \param url URL of the image (can be modified by MPQ priority list internally).
		* \param format Has to be one of Qt's AND OGRE supported image formats.
		* \return Returns
		* \note OGRE has to be compiled and linked with FreeImage (PNG support).
		*/
		Ogre::Image* blpToOgre(const KUrl &url, const QString &format = "PNG") const throw (class Exception);
		/**
		* Once requested, the image is kept in memory until it's refreshed manually.
		*/
		const Ogre::Image& teamColorImage(enum OgreMdlx::TeamColor teamColor) const;
		/**
		* Once requested, the image is kept in memory until it's refreshed manually.
		*/
		const Ogre::Image& teamGlowImage(enum OgreMdlx::TeamColor teamGlow) const;

	public slots:
		void newMap();
		void openMap(const KUrl &url);
		void switchToMap(const class Map *map);
		void closeMap(const class Map *map);
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
		/**
		* Reads settings from configuration which usually are stored on the user's filesystem and applies them on their corresponding editor modules.
		* @note This does not necessarily use the same entries as the original World Editor created by Blizzard Entertainment.
		*/
		void readSettings();
		void writeSettings();

		/**
		* Template get element function for pointers which can be 0 and should be set/allocated when being get.
		*/
		template<class T>
		T* module(T* const &module) const;

		static KAboutData m_aboutData;
		static KAboutData m_wc3libAboutData;

		class KActionCollection *m_actionCollection;
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

		std::map<KUrl, class Resource*> m_resources;

		// team color and glow images
		mutable std::map<enum OgreMdlx::TeamColor, Ogre::Image*> m_teamColorImages;
		mutable std::map<enum OgreMdlx::TeamColor, Ogre::Image*> m_teamGlowImages;
};

inline KUrl Editor::teamColorUrl(enum OgreMdlx::TeamColor teamColor)
{
	QString number("%1");
	number.arg((int)teamColor);

	if (number.size() == 1)
		number.prepend('0');

	return KUrl("ReplaceableTextures/TeamColor/TeamColor" + number + ".blp");
}

inline KUrl Editor::teamGlowUrl(enum OgreMdlx::TeamColor teamGlow)
{
	QString number("%1");
	number.arg((int)teamGlow);

	if (number.size() == 1)
		number.prepend('0');

	return KUrl("ReplaceableTextures/TeamGlow/TeamGlow" + number + ".blp");
}

template<class T>
T* Editor::module(T* const &module) const
{
	if (module == 0)
		const_cast<T*&>(module) = new T(const_cast<self*>(this));

	return module;
}

inline class KActionCollection* Editor::actionCollection() const
{
	return this->m_actionCollection;
}

inline class TerrainEditor* Editor::terrainEditor() const
{
	return module(this->m_terrainEditor);
}

inline class TriggerEditor* Editor::triggerEditor() const
{
	return module(this->m_triggerEditor);
}

inline class SoundEditor* Editor::soundEditor() const
{
	return module(this->m_soundEditor);
}

inline class ObjectEditor* Editor::objectEditor() const
{
	return module(this->m_objectEditor);
}

inline class CampaignEditor* Editor::campaignEditor() const
{
	return module(this->m_campaignEditor);
}

inline class AiEditor* Editor::aiEditor() const
{
	return module(this->m_aiEditor);
}

inline class ObjectManager* Editor::objectManager() const
{
	return module(this->m_objectManager);
}

inline class ImportManager* Editor::importManager() const
{
	return module(this->m_importManager);
}

inline class MpqEditor* Editor::mpqEditor() const
{
	return module(this->m_mpqEditor);
}

inline class ModelEditor* Editor::modelEditor() const
{
	return module(this->m_modelEditor);
}

inline class TextureEditor* Editor::textureEditor() const
{
	return module(this->m_textureEditor);
}

inline class NewMapDialog* Editor::newMapDialog() const
{
	return module(this->m_newMapDialog);
}

inline void Editor::addResource(class Resource *resource)
{
	this->m_resources.insert(std::make_pair(resource->url(), resource));
	this->addEntry(resource->url());
}


inline bool Editor::removeResource(class Resource *resource)
{
	this->removeResource(resource->url());
}

inline bool Editor::removeResource(const KUrl &url)
{
	std::map<KUrl, class Resource*>::iterator iterator = this->m_resources.find(url);

	if (iterator == this->m_resources.end())
		return false;

	this->m_resources.erase(iterator);
	this->removeEntry(url);

	return true;
}

inline const std::map<KUrl, class Resource*>& Editor::resources() const
{
	return this->m_resources;
}

inline const Ogre::Image& Editor::teamColorImage(enum OgreMdlx::TeamColor teamColor) const
{
	if (this->m_teamColorImages[teamColor] == 0)
		this->m_teamColorImages[teamColor] = this->blpToOgre(teamColorUrl(teamColor));

	return *this->m_teamColorImages[teamColor];
}

inline const Ogre::Image& Editor::teamGlowImage(enum OgreMdlx::TeamColor teamGlow) const
{
	if (this->m_teamGlowImages[teamGlow] == 0)
		this->m_teamGlowImages[teamGlow] = this->blpToOgre(teamGlowUrl(teamGlow));

	return *this->m_teamGlowImages[teamGlow];
}

}

}

#endif
