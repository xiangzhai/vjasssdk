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

#include <QtGui>
#include <list>
#include <string>

#include "mainwindow.hpp"
#include "vjassdoc.h"

namespace vjassdoc
{

namespace gui
{

const KAboutData *MainWindow::aboutData = new KAboutData("vjassdocgui",
"",
ki18n("vjassdocgui"),
"0.1",
ki18n("Program which rulez!!!!!!!!!!1111"),
KAboutData::License_GPL_V2,
ki18n("(C) 2009 Tamino Dauth"),
ki18n("Bla bla bla"),
"http://sourceforge.net/projects/vjasssdk/");

void MainWindow::run()
{
	bool parseObjectsOfList[vjassdoc::Parser::MaxLists];

	for (int i = 0; i < vjassdoc::Parser::MaxLists; ++i)
		parseObjectsOfList[i] = true;

	std::list<std::string> importDirs;

	foreach (QString iterator, importDirectoriesEditListBox->items())
		importDirs.push_back(iterator.toAscii().data()); //iterator.toStdString()

	std::list<std::string> filePaths;

	foreach (QString iterator, filesEditListBox->items())
		filePaths.push_back(iterator.toAscii().data()); //iterator.toStdString()

	std::list<std::string> databases;

	foreach (QString iterator, databasesEditListBox->items())
		databases.push_back(iterator.toAscii().data()); //iterator.toStdString()

/*
	vjassdoc::Vjassdoc::configure(
	optionJassCheckBox->isChecked(),
	optionDebugCheckBox->isChecked(),
	optionPrivateCheckBox->isChecked(),
	optionTextmacrosCheckBox->isChecked(),
	optionFunctionsCheckBox->isChecked(),
	optionHtmlCheckBox->isChecked(),
	optionPagesCheckBox->isChecked(),
	optionSpecialpagesCheckBox->isChecked(),
	optionSyntaxCheckBox->isChecked(),
	compilationDirectoryUrlRequester->url().path().toAscii().data(),
	optionDatabaseCheckBox->isChecked(),
	optionVerboseCheckBox->isChecked(),
	optionTimeCheckBox->isChecked(),
	optionAlphabeticalCheckBox->isChecked(),
	parseObjectsOfList,
	titleLineEdit->text().toAscii().data(),
	outputDirectoryUrlRequester->url().path().toAscii().data(),
	importDirs,
	filePaths,
	databases);
#ifdef SQLITE
	Vjassdoc::initClasses();
#endif
	Vjassdoc::run();
	Vjassdoc::clear();
*/
}

MainWindow::MainWindow(QWidget *parent) : KMainWindow(parent)
{
	setupUi(this);
}

MainWindow::~MainWindow()
{
}

void MainWindow::buttonClicked(KDialog::ButtonCode button)
{
	switch (button)
	{
		case KDialog::Ok:
			this->run();
			break;

		case KDialog::Cancel:
			this->close();
			break;

		default:
			break;
	}
}

}

}
