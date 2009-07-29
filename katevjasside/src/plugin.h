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

#ifndef KATEVJASSIDE_PLUGIN_H
#define KATEVJASSIDE_PLUGIN_H

#include <KPluginFactory>
#include <KPluginLoader>
#include <kaboutdata.h>
#include <kate/plugin.h>
#include <kate/pluginconfigpageinterface.h>
#include <kate/mainwindow.h>

namespace katevjasside
{

class Plugin;
class JasshelperPluginConfigPage;

class Plugin : public Kate::Plugin,  public Kate::PluginConfigPageInterface
{
	Q_OBJECT
	Q_INTERFACES(Kate::PluginConfigPageInterface)

	public:
		Plugin(QObject *parent, const QList<QVariant> &list);
		virtual Kate::PluginView *createView(Kate::MainWindow *mainWindow);
		virtual void readSessionConfig(KConfigBase *config, const QString &groupPrefix);
		virtual void writeSessionConfig(KConfigBase *config, const QString &groupPrefix);
		//interface
		virtual uint configPages () const;
		virtual Kate::PluginConfigPage* configPage(uint number = 0, QWidget *parent = 0, const char *name = 0);
		virtual QString configPageName(uint number = 0) const;
		virtual QString configPageFullName(uint number = 0) const;
		virtual KIcon configPageIcon(uint number = 0) const;
	
	private:
		KComponentData kcd;
		JasshelperPluginConfigPage *jasshelperConfigPage;
};

}

#endif
