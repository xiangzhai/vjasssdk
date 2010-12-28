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

#ifndef WC3LIB_EDITOR_MODULE_HPP
#define WC3LIB_EDITOR_MODULE_HPP

#include <QWidget>

namespace wc3lib
{

namespace editor
{

/**
* @brief Abstract class for module implementation such as model or terrain editors.
* Implement the pure virtual functions to customize your module's menu, actions and tool buttons.
*/
class Module : public QWidget
{
	public:
		Module(class Editor *editor);
		virtual ~Module();
		class Editor* editor() const;
		class KMenuBar* menuBar() const;

	protected:
		virtual void setupUi();

		virtual void createFileActions(class KMenu *menu) = 0;
		virtual void createEditActions(class KMenu *menu) = 0;
		virtual void createMenus(class KMenuBar *menuBar) = 0;
		virtual void createWindowsActions(class KMenu *menu) = 0;
		virtual void createToolButtons(class KToolBar *toolBar) = 0;
		virtual class SettingsInterface* settings() = 0;

		class QVBoxLayout* topLayout() const;

		void readSettings();
		void writeSettings();

	private:
		class Editor *m_editor;
		class KMenuBar *m_menuBar;

		class QVBoxLayout *m_topLayout;
};

inline class Editor* Module::editor() const
{
	return this->m_editor;
}

inline class QVBoxLayout* Module::topLayout() const
{
	return this->m_topLayout;
}

}

}

#endif
