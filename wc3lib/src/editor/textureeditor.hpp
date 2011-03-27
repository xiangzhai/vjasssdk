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

#ifndef WC3LIB_EDITOR_TEXTUREEDITOR_HPP
#define WC3LIB_EDITOR_TEXTUREEDITOR_HPP

#include <kurl.h>
#include <kaction.h>

#include "module.hpp"
#include "ui/ui_textureeditor.h"

namespace wc3lib
{

namespace editor
{

/// \todo Needs item list from object editor (skin meta data).
class TextureEditor : public Module, protected Ui::TextureEditor
{
	Q_OBJECT

	public:
		TextureEditor(class Editor *editor);
		virtual ~TextureEditor();
		
		bool showsAlphaChannel() const;
		bool showsTransparency() const;
		qreal factor() const;

	public slots:
		void openFile();
		void saveFile();
		void showSettings();

		void editColorPalette();
		void makeActive();
		void makePassive();
		void makeAutocast();
		void makeInfocardNormal();
		void makeInfoardLevel();

		void showAlphaChannel();
		void showTransparency();
		void actualSize();
		void zoomToFit();
		void zoomIn();
		void zoomOut();

		void massConverter();

	protected:
		void refreshImage();
		
		virtual void createFileActions(class KMenu *menu);
		virtual void createEditActions(class KMenu *menu);
		virtual void createMenus(class KMenuBar *menuBar);
		virtual void createWindowsActions(class KMenu *menu);
		virtual void createToolButtons(class KToolBar *toolBar);
		virtual class SettingsInterface* settings();

		QImage *m_image;
		bool m_showsAlphaChannel;
		bool m_showsTransparency;
		qreal m_factor;
		KUrl m_recentUrl;
		
		KAction *m_showAlphaChannelAction;
		KAction *m_showTransparencyAction;
};

inline bool TextureEditor::showsAlphaChannel() const
{
	return m_showsAlphaChannel;
}

inline bool TextureEditor::showsTransparency() const
{
	return m_showsTransparency;
}

inline qreal TextureEditor::factor() const
{
	return m_factor;
}

}

}

#endif
