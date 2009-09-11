#include <kapplication.h>
#include <kcmdlineargs.h>

#include "application.hpp"
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
	KAboutData aboutData(
		"vjassdocgui",
		0,
		ki18n("vjassdocgui"),
		"0.1",
		ki18n("GUI for vjassdoc."),
		KAboutData::License_GPL_V2,
		ki18n("Copyright 2009, Tamino Dauth"),
		KLocalizedString(),
		"https://sourceforge.net/projects/vjasssdk/"
	);
	aboutData.addAuthor(ki18n("Tamino Dauth"), KLocalizedString(), "tamino@cdauth.de");
	aboutData.setOrganizationDomain("https://sourceforge.net/projects/vjasssdk/");

	KCmdLineArgs::init(argc, argv, &aboutData);

	KCmdLineOptions options;
	KCmdLineArgs::addCmdLineOptions(options);

	vjassdoc::gui::Application app;
	class vjassdoc::gui::MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}
