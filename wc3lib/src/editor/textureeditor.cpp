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

#include <QtGui>

#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <ktoolbar.h>
#include <kmenu.h>
#include <kaction.h>
#include <kmenubar.h>
#include <kstandardaction.h>

#include "textureeditor.hpp"
#include "blpiohandler.hpp"

namespace wc3lib
{

namespace editor
{

TextureEditor::TextureEditor(class Editor *editor) : Module(editor), m_image(new QImage()), m_showsAlphaChannel(false), m_showsTransparency(false), m_factor(1.0)
{
	Ui::TextureEditor::setupUi(this);
	Module::setupUi();

	topLayout()->addLayout(gridLayout_2);
}

TextureEditor::~TextureEditor()
{
	delete this->m_image;
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

	if (!this->m_image->load(&file, 0))//(!handler.read(&image))
	{
		KMessageBox::error(this, i18n("Unable to read BLP image from file \"%1\".", url.toLocalFile()));

		return;
	}

	qDebug() << "Size is " << this->m_image->width() << " | " << this->m_image->height();

	if (this->m_image->isNull())
	{
		KMessageBox::error(this, i18n("Unable to read file \"%1\".", url.toLocalFile()));

		return;
	}

	/// \todo Image format (e. g. Format_Indexed8 is not stored) which leads to another format when saving image somewhere.
	refreshImage();
	qDebug() << "Set pixmap to label ";
	qDebug() << "Label size is " << this->m_imageLabel->width() << " | " << this->m_imageLabel->height();
}

void TextureEditor::saveFile()
{
	if (this->m_image->isNull())
	{
		KMessageBox::error(this, i18n("No open image file."));

		return;
	}

	KUrl url = KFileDialog::getSaveUrl(this->m_recentUrl, i18n("*.blp|BLP textures\n*.png|PNG images"), this);

	if (url.isEmpty())
		return;

	if (!url.isLocalFile())
	{
		KMessageBox::error(this, i18n("Unable to save file \"%1\".", url.toLocalFile()));

		return;
	}

	QFile file(url.toLocalFile());
	file.open(QIODevice::WriteOnly);

	if (!this->m_image->save(&file)) /// \todo Guess correct format by file extension?
	{
		KMessageBox::error(this, i18n("Unable to save image to file \"%1\".", url.toLocalFile()));

		return;
	}
}

void TextureEditor::showSettings()
{
}

void TextureEditor::editColorPalette()
{
}

void TextureEditor::makeActive()
{
}

void TextureEditor::makePassive()
{
}

void TextureEditor::makeAutocast()
{
}

void TextureEditor::makeInfocardNormal()
{
}

void TextureEditor::makeInfoardLevel()
{
}

void TextureEditor::showAlphaChannel()
{
	if (!this->m_image->hasAlphaChannel())
	{
		KMessageBox::error(this, i18n("Image doesn't have alpha channel."));
		
		return;
	}
		
	if (showsAlphaChannel())
	{
		m_showAlphaChannelAction->setText(i18n("Show alpha channel"));
		m_showsAlphaChannel = false;
	}
	else
	{
		m_showAlphaChannelAction->setText(i18n("Hide alpha channel"));
		m_showsAlphaChannel = true;
	}
	
	refreshImage();
}

void TextureEditor::showTransparency()
{
	/// \todo According to documentation checking for mask is an expensive operation.
	
	if (showsTransparency())
	{
		m_showTransparencyAction->setText(i18n("Show transparency"));
		m_showsTransparency = false;
	}
	else
	{
		m_showTransparencyAction->setText(i18n("Hide transparency"));
		m_showsTransparency = true;
	}
	
	refreshImage();
}

void TextureEditor::actualSize()
{
	this->m_factor = 1.0;
	refreshImage();
}

void TextureEditor::zoomToFit()
{
	refreshImage();
}

void TextureEditor::zoomIn()
{
	this->m_factor += 0.20;
	refreshImage();
}

void TextureEditor::zoomOut()
{
	this->m_factor -= 0.20;
	refreshImage();
}

void TextureEditor::massConverter()
{
}

void TextureEditor::refreshImage()
{
	QPixmap newPixmap;
	
	if (!showsAlphaChannel())
		newPixmap = QPixmap::fromImage(*this->m_image).scaled(this->m_image->size() * this->factor());
	else
		newPixmap = QPixmap::fromImage(this->m_image->createAlphaMask()).scaled(this->m_image->size() * this->factor());
	
	if (showsTransparency())
		newPixmap.setMask(this->m_imageLabel->pixmap()->createMaskFromColor(Qt::transparent));
	
	this->m_imageLabel->setPixmap(newPixmap);
	this->m_imageLabel->resize(this->m_imageLabel->pixmap()->size());
}

void TextureEditor::createFileActions(class KMenu *menu)
{
	class KAction *action;

	action = new KAction(KIcon(":/actions/opentexture.png"), i18n("Open texture"), this);
	action->setShortcut(KShortcut(i18n("Ctrl+O")));
	connect(action, SIGNAL(triggered()), this, SLOT(openFile()));
	menu->addAction(action);

	action = new KAction(KIcon(":/actions/savetexture.png"), i18n("Save texture"), this);
	action->setShortcut(KShortcut(i18n("Ctrl+S")));
	connect(action, SIGNAL(triggered()), this, SLOT(saveFile()));
	menu->addAction(action);

	action = new KAction(KIcon(":/actions/settings.png"), i18n("Settings"), this);
	//action->setShortcut(KShortcut(i18n("Ctrl+O")));
	connect(action, SIGNAL(triggered()), this, SLOT(showSettings()));
	menu->addAction(action);
}

void TextureEditor::createEditActions(class KMenu *menu)
{
	KAction *action = new KAction(KIcon(":/actions/editcolorpalette.png"), i18n("Edit color palette"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(editColorPalette()));
	menu->addAction(action);
}

void TextureEditor::createMenus(class KMenuBar *menuBar)
{
	KMenu *viewMenu = new KMenu(i18n("View"), this);
	//this->m_viewMenu = viewMenu;
	menuBar->addMenu(viewMenu);

	m_showAlphaChannelAction = new KAction(KIcon(":/actions/showalphachannel.png"), i18n("Show alpha channel"), this);
	connect(m_showAlphaChannelAction, SIGNAL(triggered()), this, SLOT(showAlphaChannel()));
	viewMenu->addAction(m_showAlphaChannelAction);

	m_showTransparencyAction = new KAction(KIcon(":/actions/showtransparency.png"), i18n("Show transparency"), this);
	connect(m_showTransparencyAction, SIGNAL(triggered()), this, SLOT(showTransparency()));
	viewMenu->addAction(m_showTransparencyAction);

	viewMenu->addAction(KStandardAction::actualSize(this, SLOT(actualSize()), this));

	KAction *action = new KAction(KIcon(":/actions/zoomtofit.png"), i18n("Zoom to fit"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(zoomToFit()));
	viewMenu->addAction(action);

	viewMenu->addAction(KStandardAction::zoomIn(this, SLOT(zoomIn()), this));
	viewMenu->addAction(KStandardAction::zoomOut(this, SLOT(zoomOut()), this));

	KMenu *toolsMenu = new KMenu(i18n("Tools"), this);
	//this->m_toolsMenu = toolsMenu;
	menuBar->addMenu(toolsMenu);

	// TODO add check buttons to mass converter widget which allow you to a) convert and b) generate info cards etc. and c) to remove old files.
	action = new KAction(KIcon(":/actions/massconverter.png"), i18n("Mass converter"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(massConverter()));
	toolsMenu->addAction(action);
}

void TextureEditor::createWindowsActions(class KMenu *menu)
{
}

void TextureEditor::createToolButtons(class KToolBar *toolBar)
{
}

class SettingsInterface* TextureEditor::settings()
{
	/// @todo FIXME
	return 0;
}

#include "moc_textureeditor.cpp"

}

}
