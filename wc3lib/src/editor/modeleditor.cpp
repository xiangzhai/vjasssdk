/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <QtGui>

#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kmenu.h>
#include <kaction.h>

#include "modeleditor.hpp"
#include "modelview.hpp"
#include "editor.hpp"
#include "../utilities.hpp"
#include "resource.hpp"
#include "modeleditorsettings.hpp"

namespace wc3lib
{

namespace editor
{

ModelEditor::ModelEditor(class Editor *editor) : Module(editor), m_modelView(new ModelView(editor, this, 0)), m_recentUrl("")
{
	Ui::ModelEditor::setupUi(this);
	Module::setupUi();
	/*
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(this->m_modelView);
	this->m_modelViewWidget->setLayout(mainLayout);
	*/
	/// @todo Model view should be bigger than modules buttons
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(this->m_modelView);
	this->topLayout()->addLayout(layout);
}

ModelEditor::~ModelEditor()
{
}

void ModelEditor::show()
{
	Module::show();
	this->m_modelView->show();
	/// @todo FIXME: Either create model view data in constructor or get the right position for this function call.
	//readSettings(); // read settings when model view is being shown since its render window is also being created at that moment
}

void ModelEditor::openFile()
{
	KUrl url = KFileDialog::getOpenUrl(this->m_recentUrl, i18n("*.mdl|Blizzard Model (*.mdl)\n*.mdx|Compressed Blizzard Model (*.mdx)\n*|All files (*)"), this);

	if (url.isEmpty())
		return;

	if (!url.isLocalFile())
	{
		KMessageBox::error(this, i18n("Unable to open file \"%1\".", url.toLocalFile()));

		return;
	}

	this->openUrl(url);
}

void ModelEditor::showSettings()
{
}

void ModelEditor::dragEnterEvent(QDragEnterEvent *event)
{
	/// @todo If it's an MDLX file event->acceptProposedAction();
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void ModelEditor::dropEvent(QDropEvent *event)
{
	/// @todo If it's an MDLX file event->acceptProposedAction();
	if (event->mimeData()->hasUrls())
	{
		foreach (QUrl url, event->mimeData()->urls())
			this->openUrl(url);

		event->accept();
	}
}

bool ModelEditor::openUrl(const KUrl &url)
{
	std::ios_base::openmode openmode = std::ios_base::in;
	bool isMdx;
	qDebug() << "Extension is " << QString(boost::filesystem::path(url.toEncoded()).extension().c_str());
	qDebug() << "Encoded URL " << url.path().toAscii();

	if (boost::filesystem::path(url.toEncoded()).extension() == ".mdx")
	{
		qDebug() << "Is MDLX";
		isMdx = true;
		openmode |= std::ios_base::binary;
	}
	else
		isMdx = false;

	std::ifstream ifstream(url.path().toAscii(), openmode);
	/// @todo Should be allocated on heap (has to be used permanently by OgreMdlx?).
	class mdlx::Mdlx model;

	try
	{
		std::streamsize size;
		if (isMdx)
			size = model.readMdx(ifstream);
		else
			size = model.readMdl(ifstream);

		KMessageBox::information(this, i18n("Read %1 file \"%2\" successfully.\nSize: %3.", isMdx ? i18n("MDX") : i18n("MDL"), url.toLocalFile(), sizeStringBinary(size).c_str()));
	}
	catch (class Exception &exception)
	{
		KMessageBox::error(this, i18n("Unable to read %1 file \"%2\".\nException \"%3\".", isMdx ? i18n("MDX") : i18n("MDL"), url.toLocalFile(), exception.what().c_str()));

		return false;
	}

	//const Ogre::Vector3 position(0.0, 0.0, 0.0);
	OgreMdlx *ogreModel = new OgreMdlx(url, model, this->m_modelView);
	this->editor()->addResource(ogreModel); // add to get URL

	try
	{
		QTime ct = QTime::currentTime();
		ct.start();
		qDebug() << "Refresh";
		ogreModel->refresh();
		qDebug() << "After refresh. Duration " << ct.elapsed() << "ms";
	}
	catch (class std::exception &exception)
	{
		KMessageBox::error(this, i18n("Error during model refresh:\n%1", exception.what()));
		this->editor()->removeResource(ogreModel);
		delete ogreModel;

		return false;
	}

	this->m_models.push_back(ogreModel);
	this->m_modelView->root()->addFrameListener(ogreModel);

	return true;
}

void ModelEditor::createFileActions(class KMenu *menu)
{
	class KAction *action;

	action = new KAction(KIcon(":/actions/openmodel.png"), i18n("Open model"), this);
	action->setShortcut(KShortcut(i18n("Ctrl+O")));
	connect(action, SIGNAL(triggered()), this, SLOT(openFile()));
	menu->addAction(action);

	action = new KAction(KIcon(":/actions/settings.png"), i18n("Settings"), this);
	//action->setShortcut(KShortcut(i18n("Ctrl+O")));
	connect(action, SIGNAL(triggered()), this, SLOT(settings()));
	menu->addAction(action);
}

void ModelEditor::createEditActions(class KMenu *menu)
{
	qDebug() << "Edit actions";
}

void ModelEditor::createMenus(class KMenuBar *menuBar)
{
	qDebug() << "Menus";
}

void ModelEditor::createWindowsActions(class KMenu *menu)
{
	qDebug() << "Windows";
}

void ModelEditor::createToolButtons(class KToolBar *toolBar)
{
	qDebug() << "Tool buttons";
}

class SettingsInterface* ModelEditor::settings()
{
	return new ModelEditorSettings(this);
}

#include "modeleditor.moc"
#include "moc_modeleditor.cpp"

}

}
