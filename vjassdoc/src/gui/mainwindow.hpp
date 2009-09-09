/***************************************************************************
 *   Copyright (C) 2008, 2009 by Tamino Dauth                              *
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

#ifndef VJASSDOC_GUI_MAINWINDOW_H
#define VJASSDOC_GUI_MAINWINDOW_H

#include <kmainwindow.h>
#include <kaboutdata.h>

#include "ui/ui_widget.hpp"

namespace vjassdoc
{

namespace gui
{

class MainWindow : public KMainWindow, public Ui::Widget
{
	//Q_OBJECT

	public slots:
		void run();

	public:
		static const KAboutData *aboutData;

		MainWindow(QWidget *parent = 0);
		virtual ~MainWindow();

	private slots:
		void buttonClicked(KDialog::ButtonCode button);

	private:
		//Q_DISABLE_COPY(MainWindow)
};

}

}

#endif
