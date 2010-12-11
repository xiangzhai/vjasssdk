/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#ifndef WC3LIB_EDITOR_SETTINGS_HPP
#define WC3LIB_EDITOR_SETTINGS_HPP

#include <QWidget>

#include "ui/ui_settings.hpp"

namespace wc3lib
{

namespace editor
{

/**
* Settings widget for model view settings.
* Allows you to configure OGRE rendering settings (e. g. resolution, renderer, lighting etc.).
* @see ModelView, ModelEditor, TerrainEditor
*/
class Settings : public QWidget, private Ui::SettingsTabWidget
{
	Q_OBJECT

	public:
		Settings(class ModelView *modelView, QWidget *parent = 0, Qt::WindowFlags f = 0);

	public slots:
		void apply();

	protected:
		bool hasToReinitialiseRenderer() const;

		virtual void showEvent(QShowEvent *event);


		class ModelView *m_modelView;
};

}

}

#endif
