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

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>

#include "StormLib/StormLib.h"
#include "mainwindow.h"

int main(int argc, char* argv[])
{
	KCmdLineArgs::init(argc, argv, mpqeditor::MainWindow::mpqeditorAboutData);
	KCmdLineOptions options;
	options.add("<mpq archives>", ki18n("irc:// URL or server hostname"), 0);

	KCmdLineArgs::addCmdLineOptions(options);
	KCmdLineArgs::addStdCmdLineOptions();

	KApplication app;
	mpqeditor::MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}
