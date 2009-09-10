#include <QtGui>

#include "widget.hpp"
#include "mainwindow.hpp"
#include "vjassdoc.h"

namespace vjassdoc
{

namespace gui
{

Widget::Widget(class MainWindow *parent) : QWidget(parent)
{
	setupUi(this);
	connect(this->dialog, SIGNAL(buttonClicked(KDialog::ButtonCode)), this, SLOT(buttonClicked(KDialog::ButtonCode)));
}

void Widget::run()
{
	bool parseObjectsOfList[vjassdoc::Parser::MaxLists];

	for (int i = 0; i < vjassdoc::Parser::MaxLists; ++i)
		parseObjectsOfList[i] = true;

	std::list<std::string> importDirs;

	foreach (QString iterator, this->importDirectoriesEditListBox->items())
		importDirs.push_back(iterator.toAscii().data()); //iterator.toStdString()

	std::list<std::string> filePaths;

	foreach (QString iterator, this->filesEditListBox->items())
		filePaths.push_back(iterator.toAscii().data()); //iterator.toStdString()

	std::list<std::string> databases;

	foreach (QString iterator, this->databasesEditListBox->items())
		databases.push_back(iterator.toAscii().data()); //iterator.toStdString()

/*
	vjassdoc::Vjassdoc::configure(
	this->m_widget->optionJassCheckBox->isChecked(),
	this->m_widget->optionDebugCheckBox->isChecked(),
	this->m_widget->optionPrivateCheckBox->isChecked(),
	this->m_widget->optionTextmacrosCheckBox->isChecked(),
	this->m_widget->optionFunctionsCheckBox->isChecked(),
	this->m_widget->optionHtmlCheckBox->isChecked(),
	this->m_widget->optionPagesCheckBox->isChecked(),
	this->m_widget->optionSpecialpagesCheckBox->isChecked(),
	this->m_widget->optionSyntaxCheckBox->isChecked(),
	this->m_widget->compilationDirectoryUrlRequester->url().path().toAscii().data(),
	this->m_widget->optionDatabaseCheckBox->isChecked(),
	this->m_widget->optionVerboseCheckBox->isChecked(),
	this->m_widget->optionTimeCheckBox->isChecked(),
	this->m_widget->optionAlphabeticalCheckBox->isChecked(),
	parseObjectsOfList,
	this->m_widget->titleLineEdit->text().toAscii().data(),
	this->m_widget->outputDirectoryUrlRequester->url().path().toAscii().data(),
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

void Widget::buttonClicked(KDialog::ButtonCode button)
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
