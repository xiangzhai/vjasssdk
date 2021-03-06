# -*- coding: utf-8 -*-
#***************************************************************************
#*   Copyright (C) 2009 by Tamino Dauth                                    *
#*   tamino@cdauth.eu                                                      *
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

from PyQt4 import QtGui
from PyKDE4.kdecore import *
from PyKDE4.kdeui import *
from PyKDE4.kio import *

class MainWindow(KMainWindow):

	openUrl = KUrl();

	def __init__(self, parent = 0, f = 0):
		KMainWindow.__init__(self)
		fileMenu = KMenu("File", self)
		self.menuBar().addMenu(fileMenu)
		fileMenu.addAction(KStandardAction.openNew(self.fileNew, self))
		fileMenu.addAction(KStandardAction.open(self.fileOpen, self))
		fileMenu.addAction(KStandardAction.close(self.fileClose, self))
		fileMenu.addSeparator()
		fileMenu.addAction(KStandardAction.save(self.fileSave, self))
		fileMenu.addAction(KStandardAction.saveAs(self.fileSaveAs, self))
		fileMenu.addAction(self.tr("Calculate shadows and save map"), self.fileCalculateShadowsAndSave)
		fileMenu.addSeparator()
		fileMenu.addAction(self.tr("Export script ..."), self.fileExportScript)
		fileMenu.addAction(self.tr("Export minimap ..."), self.fileExportMinimap)
		fileMenu.addAction(self.tr("Export strings ..."), self.fileExportStrings)
		fileMenu.addAction(self.tr("Import strings ..."), self.fileImportStrings)
		fileMenu.addSeparator()
		#recent files
		fileMenu.addSeparator()
		fileMenu.addAction(self.tr("Presettings ..."), self.filePresettings)
		fileMenu.addAction(KStandardAction.configureNotifications(self.fileConfigureNotifications, self))
		fileMenu.addSeparator()
		fileMenu.addAction(self.tr("Test map"), self.fileTestMap)
		fileMenu.addSeparator()
		fileMenu.addAction(KStandardAction.quit(self.close, self))

		helpMenu = KHelpMenu(self)
		self.menuBar().addMenu(helpMenu.menu())

	def fileNew(self):
		print "New file"

	def fileOpen(self):
		print "Open file"
		fileNames = KFileDialog.getOpenFileNames(self.openUrl, "*.w3x *.W3X|Warcraft 3 The Frozen Throne maps\n*.w3m *.W3M|Warcraft 3 Maps", self, "Open file") #QStringList 	getOpenFileNames (KUrl startDir=KUrl(), QString filter=QString(), QWidget parent=0, QString caption=QString())

	def fileClose(self):
		print "Close file"

	def fileSave(self):
		print "Save file"

	def fileSaveAs(self):
		print "Save file as"

	def fileCalculateShadowsAndSave(self):
		print "Calculate shadows and save file"

	def fileExportScript(self):
		print "Export file script"

	def fileExportMinimap(self):
		print "Export file minimap"

	def fileExportStrings(self):
		print "Export file strings"

	def fileImportStrings(self):
		print "Import file strings"

	def filePresettings(self):
		print "Presettings"

	def fileConfigureNotifications(self):
		print "Notifications"

	def fileTestMap(self):
		print "Test map"

appName     = "editor"
catalog     = ""
programName = ki18n("editor")
version     = "0.1"
description = ki18n("GUI editor for Warcraft 3 The Frozen Throne formats.")
license     = KAboutData.License_GPL_V2
copyright   = ki18n("(c) 2009 Tamino Dauth")
text        = ki18n("none")
homePage    = "http://sourceforge.net/projects/vjasssdk/"
bugEmail    = "barade.barade@web.de"

aboutData   = KAboutData(appName, catalog, programName, version, description,
                        license, copyright, text, homePage, bugEmail)
KCmdLineArgs.init(sys.argv, aboutData)
app = KApplication()
mainWindow = MainWindow()
mainWindow.show()
sys.exit(app.exec_())