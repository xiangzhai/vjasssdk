#***************************************************************************
#*   Copyright (C) 2009 by Tamino Dauth                                    *
#*   tamino@cdauth.de                                                      *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License as published by  *
#*   the Free Software Foundation; either version 2 of the License, or     *
#*   (at your option) any later version.                                   *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU General Public License for more details.                          *
#*                                                                         *
#*   You should have received a copy of the GNU General Public License     *
#*   along with this program; if not, write to the                         *
#*   Free Software Foundation, Inc.,                                       *
#*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
#***************************************************************************/

#import sys, dl
import sys
#import DLFCN as dl
# This is needed to ensure that dynamic_cast and RTTI works inside kdelibs.
#sys.setdlopenflags(dl.RTLD_NOW|dl.RTLD_GLOBAL)

from PyKDE4.kdecore import *
from PyKDE4.kdeui import *

class MainWindow(KMainWindow):

	openUrl = KUrl();

	def __init__(self, parent=0, f=0):
		self.fileMenu = KMenu("File", self)
		self.menuBar().addMenu(self.fileMenu)
		self.fileMenu.addAction(KStandardAction.openNew(self, fileNew(), self))
		super(self, parent, f)

	def fileNew(self):
		print "New file"

	def fileOpen(self):
		print "Open file"
		fileNames = KFileDialog.getOpenFileNames(self.openUrl, "*.w3x *.W3X|Warcraft 3 The Frozen Throne maps\n*.w3m *.W3M|Warcraft 3 Maps", self, "Open file") #QStringList 	getOpenFileNames (KUrl startDir=KUrl(), QString filter=QString(), QWidget parent=0, QString caption=QString())

	def fileClose(self):
		print "Close file"


mainWindow = MainWindow()
mainWindow.show()
