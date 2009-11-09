#include <QtGui>

#include <kmessagebox.h>

#include "widget.hpp"
#include "mainwindow.hpp"
#include "vjassdoc.hpp"

namespace vjassdoc
{

namespace gui
{

void Widget::run()
{
	if (this->filesEditListBox->items().isEmpty())
	{
		KMessageBox::error(this, tr("Missing file paths."), tr("Error"));
		
		return;
	}
	
	if (!QFile::exists(this->outputDirectoryUrlRequester->url().path()))
	{
		KMessageBox::error(this, tr("Missing output directory \"%1\".").arg(this->outputDirectoryUrlRequester->url().path()), tr("Error"));
		
		return;
	}


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

	vjassdoc::Vjassdoc::configure(
	this->optionJassCheckBox->isChecked(),
	this->optionDebugCheckBox->isChecked(),
	this->optionPrivateCheckBox->isChecked(),
	this->optionTextmacrosCheckBox->isChecked(),
	this->optionFunctionsCheckBox->isChecked(),
	this->optionHtmlCheckBox->isChecked(),
	this->optionPagesCheckBox->isChecked(),
	this->optionSpecialpagesCheckBox->isChecked(),
	this->optionSyntaxCheckBox->isChecked(),
	this->compilationDirectoryUrlRequester->url().path().toAscii().data(),
	this->databaseDirectoryUrlRequester->url().path().toAscii().data(),
	this->optionVerboseCheckBox->isChecked(),
	this->optionTimeCheckBox->isChecked(),
	this->optionAlphabeticalCheckBox->isChecked(),
	parseObjectsOfList,
	this->titleLineEdit->text().toAscii().data(),
	this->outputDirectoryUrlRequester->url().path().toAscii().data(),
	importDirs,
	filePaths,
	databases);
#ifdef SQLITE
	Vjassdoc::initClasses();
#endif
	Vjassdoc::run();
	Vjassdoc::clear();
}

Widget::Widget(class MainWindow *parent) : QWidget(parent)
{
	setupUi(this);
	connect(this->dialogButtonBox, SIGNAL(accepted()), this, SLOT(run()));
	connect(this->dialogButtonBox, SIGNAL(rejected()), parent, SLOT(close()));

	//settings
	QSettings settings;
	settings.beginGroup("widget");
	titleLineEdit->setText(settings.value("title", tr("My API Documentation")).toString());
	QStringList files;
	int size = settings.beginReadArray("files");
	int i = 0;

	for ( ; i < size; ++i)
	{
		settings.setArrayIndex(i);
		files << settings.value("file").toString();
	}

	settings.endArray();
	this->filesEditListBox->setItems(files);
	this->optionJassCheckBox->setChecked(settings.value("jass", false).toBool());
	this->optionJassCheckBox->setChecked(settings.value("debug", true).toBool());
	this->optionJassCheckBox->setChecked(settings.value("private", false).toBool());
	this->optionJassCheckBox->setChecked(settings.value("textmacros", false).toBool());
	this->optionJassCheckBox->setChecked(settings.value("functions", false).toBool());
	this->optionJassCheckBox->setChecked(settings.value("html", true).toBool());
	this->optionJassCheckBox->setChecked(settings.value("pages", true).toBool());
	this->optionJassCheckBox->setChecked(settings.value("specialpages", true).toBool());
	this->optionJassCheckBox->setChecked(settings.value("syntax", false).toBool());
	this->optionJassCheckBox->setChecked(settings.value("verbose", false).toBool());
	this->optionJassCheckBox->setChecked(settings.value("time", true).toBool());
	this->optionJassCheckBox->setChecked(settings.value("alphabetical", true).toBool());
	this->outputDirectoryUrlRequester->setUrl(settings.value("dir").toString());
	this->compilationDirectoryUrlRequester->setUrl(settings.value("compile").toString());
	this->databaseDirectoryUrlRequester->setUrl(settings.value("database").toString());
	QStringList import;
	size = settings.beginReadArray("import");
	i = 0;

	for ( ; i < size; ++i)
	{
		settings.setArrayIndex(i);
		import << settings.value("file").toString();
	}

	settings.endArray();
	QStringList databases;
	size = settings.beginReadArray("databases");
	i = 0;

	for ( ; i < size; ++i)
	{
		settings.setArrayIndex(i);
		databases << settings.value("file").toString();
	}

	settings.endArray();
	settings.endGroup();
}

Widget::~Widget()
{
	//settings
	QSettings settings;
	settings.beginGroup("widget");
	settings.setValue("title", this->titleLineEdit->text());
	settings.beginWriteArray("files", this->filesEditListBox->items().size());
	int i = 0;

	foreach (QString iterator, this->filesEditListBox->items())
	{
		settings.setArrayIndex(i);
		settings.setValue("file", iterator);
		++i;
	}

	settings.endArray();
	settings.setValue("jass", this->optionJassCheckBox->isChecked());
	settings.setValue("debug", this->optionDebugCheckBox->isChecked());
	settings.setValue("private", this->optionPrivateCheckBox->isChecked());
	settings.setValue("textmacros", this->optionTextmacrosCheckBox->isChecked());
	settings.setValue("functions", this->optionFunctionsCheckBox->isChecked());
	settings.setValue("html", this->optionHtmlCheckBox->isChecked());
	settings.setValue("pages", this->optionPagesCheckBox->isChecked());
	settings.setValue("specialpages", this->optionSpecialpagesCheckBox->isChecked());
	settings.setValue("syntax", this->optionSyntaxCheckBox->isChecked());
	settings.setValue("verbose", this->optionVerboseCheckBox->isChecked());
	settings.setValue("time", this->optionTimeCheckBox->isChecked());
	settings.setValue("alphabetical", this->optionAlphabeticalCheckBox->isChecked());
	settings.setValue("dir", this->outputDirectoryUrlRequester->url().path());
	settings.setValue("compile", this->compilationDirectoryUrlRequester->url().path());
	settings.setValue("database", this->databaseDirectoryUrlRequester->url().path());
	settings.beginWriteArray("import", this->importDirectoriesEditListBox->items().size());
	i = 0;

	foreach (QString iterator, this->importDirectoriesEditListBox->items())
	{
		settings.setArrayIndex(i);
		settings.setValue("file", iterator);
		++i;
	}

	settings.endArray();
	settings.beginWriteArray("databases", this->databasesEditListBox->items().size());
	i = 0;

	foreach (QString iterator, this->databasesEditListBox->items())
	{
		settings.setArrayIndex(i);
		settings.setValue("file", iterator);
		++i;
	}

	settings.endArray();
	settings.endGroup();
}

}

}
