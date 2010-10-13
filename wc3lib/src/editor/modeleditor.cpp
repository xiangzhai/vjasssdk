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

#include "modeleditor.hpp"

namespace wc3lib
{

namespace editor
{

ModelEditor::ModelEditor(class Editor *editor) : Module(editor), m_modelView(this, 0), m_recentUrl("")
{
	Ui::ModelEditor::setupUi(this);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(&this->m_modelView);
	this->m_modelViewWidget->setLayout(mainLayout);
}

void ModelEditor::show()
{
	Module::show();
	this->m_modelView.show();
}

void ModelEditor::openFile()
{
	KUrl url = KFileDialog::getOpenUrl(this->m_recentUrl, i18n("*.mdl|Blizzard Model (*.mdl)\n*.mdx|Compressed Blizzard Model (*.mdx)"), this);

	if (url.isEmpty())
		return;

	if (!url.isLocalFile())
	{
		KMessageBox::error(this, i18n("Unable to open file \"%1\".", url.toLocalFile()));

		return;
	}

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
	class mdlx::Mdlx model;

	try
	{
		std::streamsize size;
		if (isMdx)
			size = model.readMdx(ifstream);
		else
			size = model.readMdl(ifstream);

		KMessageBox::information(this, i18n("Read %1 file \"%2\" successfully.\nSize %3.", isMdx ? i18n("MDX") : i18n("MDL"), url.toLocalFile(), size));
	}
	catch (class Exception &exception)
	{
		KMessageBox::error(this, i18n("Unable to read %1 file \"%2\".\nException \"%3\".", isMdx ? i18n("MDX") : i18n("MDL"), url.toLocalFile(), exception.what().c_str()));

		return;
	}

	const Ogre::Vector3 position(0.0, 0.0, 0.0);
	class mdlx::OgreMdlx *ogreModel;
	this->m_modelView.createModel(model, position, ogreModel);
	this->m_models.push_back(ogreModel);
	ogreModel->refresh();
}

}

}
