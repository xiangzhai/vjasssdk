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

#include <QtCore>

#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klocale.h>

#include "textureeditor.hpp"
#include "blpiohandler.hpp"

namespace wc3lib
{

namespace editor
{

TextureEditor::TextureEditor(class Editor *editor) : Module(editor)
{
	Ui::TextureEditor::setupUi(this);
	Module::setupUi();
}

void TextureEditor::openFile()
{
	/*
	QPluginLoader loader("libqblp.so");
	loader.load();

	if (!loader.isLoaded())
	{
		KMessageBox::error(this, i18n("Unable to load BLP plugin: \"%1\".", loader.errorString()));

		return;
	}
	*/

	KUrl url = KFileDialog::getOpenUrl(this->m_recentUrl, i18n("*.blp|BLP textures\n*.png|PNG images"), this);

	if (url.isEmpty())
		return;

	if (!url.isLocalFile())
	{
		KMessageBox::error(this, i18n("Unable to open file \"%1\".", url.toLocalFile()));

		return;
	}

	QFile file(url.toLocalFile());
	//BlpIOHandler handler;
	//handler.setDevice(&file);
	file.open(QIODevice::ReadOnly);

	/*if (!handler.canRead())
	{
		KMessageBox::error(this, i18n("Unable to detect any BLP format in file \"%1\".", url.toLocalFile()));

		return;
	}
	*/

	QImage image;

	if (!image.load(&file, 0))//(!handler.read(&image))
	{
		KMessageBox::error(this, i18n("Unable to read BLP image from file \"%1\".", url.toLocalFile()));

		return;
	}

	qDebug() << "Size is " << image.width() << " | " << image.height();

	if (image.isNull())
	{
		KMessageBox::error(this, i18n("Unable to read file \"%1\".", url.toLocalFile()));

		return;
	}

	this->m_imageLabel->setPixmap(QPixmap::fromImage(image));
	this->m_imageLabel->resize(this->m_imageLabel->pixmap()->size());
	qDebug() << "Set pixmap to label ";
	qDebug() << "Label size is " << this->m_imageLabel->width() << " | " << this->m_imageLabel->height();
}

void TextureEditor::createFileActions(class KMenu *menu)
{
}

void TextureEditor::createEditActions(class KMenu *menu)
{
}

void TextureEditor::createMenus(class KMenuBar *menuBar)
{
}

void TextureEditor::createWindowsActions(class KMenu *menu)
{
}

void TextureEditor::createToolButtons(class KToolBar *toolBar)
{
}

}

}
