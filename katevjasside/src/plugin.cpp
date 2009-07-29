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

#include "plugin.h"
#include "pluginview.h"
#include "jasshelperpluginconfigpage.h"

namespace katevjasside
{

K_PLUGIN_FACTORY(PluginFactory, registerPlugin<katevjasside::Plugin>();)

static KAboutData createAboutData()
{
	KAboutData aboutData("katevjasside", "katevjasside", ki18n("katevjasside"), "0.1",
		ki18n("a description of the plugin"), KAboutData::License_GPL,
		ki18n("Copyright (C) 2009 Tamino Dauth"));
	aboutData.addAuthor(ki18n("Tamino Dauth"), ki18n("Maintainer"), "tamino@cdauth.de", "http://sourceforge.net/projects/vjasssdk");

	return aboutData;
}
K_EXPORT_PLUGIN(PluginFactory(createAboutData()))

Plugin::Plugin(QObject *parent, const QList<QVariant> &list) : kcd(PluginFactory::componentData())
{
}

Kate::PluginView* Plugin::createView(Kate::MainWindow *mainWindow)
{
	return new PluginView(mainWindow);
}

void Plugin::readSessionConfig(KConfigBase *config, const QString &groupPrefix)
{
}

void Plugin::writeSessionConfig(KConfigBase *config, const QString &groupPrefix)
{
}

//interface

uint Plugin::configPages() const
{
	return 3;
}

Kate::PluginConfigPage* Plugin::configPage(uint number, QWidget *parent, const char *name)
{
	switch (number)
	{
		case 0:
			return 0;

		case 1:
			if (this->jasshelperConfigPage == 0)
				return this->jasshelperConfigPage = new JasshelperPluginConfigPage(parent, name);

			return this->jasshelperConfigPage;

		case 2:
			return 0;

	}

	return 0;
}

QString Plugin::configPageName(uint number) const
{
	switch (number)
	{
		case 0:
			return i18n("PJass");

		case 1:
			return i18n("JassHelper");

		case 2:
			return i18n("vjassdoc");

	}

	return QString();
}

QString Plugin::configPageFullName(uint number) const
{
	switch (number)
	{
		case 0:
			return i18n("Configure PJass");

		case 1:
			return i18n("Configure JassHelper");

		case 2:
			return i18n("Configure vjassdoc");

	}

	return QString();
}

KIcon Plugin::configPageIcon(uint number) const
{
	switch (number)
	{
		case 0:
			return KIcon("jasshelper.png");

		case 1:
			return KIcon("jasshelper.png");

		case 2:
			return KIcon("jasshelper.png");

	}
	
	return KIcon("jasshelper.png");
}

}
