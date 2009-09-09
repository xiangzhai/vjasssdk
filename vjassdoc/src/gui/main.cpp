#include <kapplication.h>
#include <kcmdlineargs.h>

#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
	KCmdLineArgs::init(argc, argv, vjassdoc::gui::MainWindow::aboutData);
	//KCmdLineOptions options;
	//options.add("<mpq archives>", ki18n("irc:// URL or server hostname"), 0);

	//KCmdLineArgs::addCmdLineOptions(options);
	KCmdLineArgs::addStdCmdLineOptions();

	KApplication app;
	class vjassdoc::gui::MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}
