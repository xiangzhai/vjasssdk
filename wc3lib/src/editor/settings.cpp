/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#include <kdialogbuttonbox.h>
#include <kmessagebox.h>
#include <klocale.h>

#include "settings.hpp"
#include "modelview.hpp"

namespace wc3lib
{

namespace editor
{

namespace
{

inline QColor ogreColourValueToQColor(const Ogre::ColourValue &value)
{
	return QColor(255 * value.r, 255 * value.g, 255 * value.b, 255 * value.a);
}

inline Ogre::ColourValue qColorTOOgreColourValue(const QColor &color)
{
	return Ogre::ColourValue((float)color.red() / 255, (float)color.green() / 255, (float)color.blue() / 255, (float)color.alpha() / 255);
}

}

Settings::Settings(class ModelView *modelView, QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f), m_modelView(modelView)
{
	KTabWidget *tabWidget = new KTabWidget(this);
	setupUi(tabWidget);

	KDialogButtonBox *dialogButtonBox = new KDialogButtonBox(this);
	dialogButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel);
	connect(dialogButtonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(apply()));
	connect(dialogButtonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(hide()));
	connect(dialogButtonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
	connect(dialogButtonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(hide()));

	QVBoxLayout *layout = new QVBoxLayout(this);
	this->setLayout(layout);
	this->layout()->addWidget(tabWidget);
	this->layout()->addWidget(dialogButtonBox);
}

/// @todo Add VSync, RTT (already added?), FSAA and display frequency options
void Settings::apply()
{
	qDebug() << "Applying new model view settings";
	Ogre::RenderSystem *renderSystem = this->m_modelView->root()->getRenderSystem();

	if (!renderSystem)
		return;

	bool reinitialiseRenderer = this->hasToReinitialiseRenderer();

	//this->m_modelView->root()->showConfigDialog();

	// ask user to continue
	if (reinitialiseRenderer)
	{
		if (KMessageBox::questionYesNo(this, tr("Applying new renderer settings requiring renderer reinitialisation.\nDo you want to continue?")) == KMessageBox::No)
			return;
	}

	// apply new render system
	Ogre::RenderSystem *renderer = this->m_modelView->root()->getAvailableRenderers()[this->m_renderSystemComboBox->currentIndex()];

	if (renderer != renderSystem)
		this->m_modelView->root()->setRenderSystem(renderer);

	// apply new resolution
	if (renderSystem->getConfigOptions()["Video Mode"].currentValue.c_str() != this->m_resolutionLineEdit->text())
		renderSystem->getConfigOptions()["Video Mode"].currentValue = this->m_resolutionLineEdit->text().toUtf8().data();

	if (reinitialiseRenderer)
	{
		renderSystem->reinitialise(); /// @todo Applies full screen, render windows?!
		//this->m_modelView->initRenderWindow(); Creates new render window -.-
	}

	this->m_modelView->root()->saveConfig();

	// view
	this->m_modelView->viewPort()->setBackgroundColour(qColorTOOgreColourValue(this->m_backgroundColorColorButton->color()));
	this->m_modelView->sceneManager()->setAmbientLight(qColorTOOgreColourValue(this->m_ambientLightColorButton->color()));
	this->m_modelView->camera()->setNearClipDistance(this->m_nearClipDistanceSpinBox->value());
	this->m_modelView->camera()->setFarClipDistance(this->m_farClipDistanceSpinBox->value());
	this->m_modelView->camera()->setPolygonMode((Ogre::PolygonMode)(this->m_polygonModeComboBox->currentIndex() + 1));

	// textures
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(this->m_mipMapsSpinBox->value());
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TextureFilterOptions(this->m_textureFilteringComboBox->currentIndex()));
	//Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(2);

	this->m_modelView->repaint(); // refresh everything
}

bool Settings::hasToReinitialiseRenderer() const
{
	Ogre::RenderSystem *renderSystem = this->m_modelView->root()->getRenderSystem();

	if (!renderSystem)
		return false;

	// apply new render system
	Ogre::RenderSystem *renderer = this->m_modelView->root()->getAvailableRenderers()[this->m_renderSystemComboBox->currentIndex()];

	return (renderer != renderSystem || renderSystem->getConfigOptions()["Video Mode"].currentValue.c_str() != this->m_resolutionLineEdit->text());
}

void Settings::showEvent(QShowEvent *event)
{
	const Ogre::RenderSystemList &renderers = this->m_modelView->root()->getAvailableRenderers();

	// render system
	/// @todo Fill combo boxes, disable not available things etc.
	this->m_renderSystemComboBox->clear();

	foreach (Ogre::RenderSystem * const renderSystem, renderers)
	{
		this->m_renderSystemComboBox->addItem(renderSystem->getName().c_str());

		if (this->m_modelView->root()->getRenderSystem() == renderSystem)
			this->m_renderSystemComboBox->setCurrentIndex(this->m_renderSystemComboBox->count() - 1);
	}

	QString resolution(this->m_modelView->root()->getRenderSystem()->getConfigOptions()["Video Mode"].currentValue.c_str());
	this->m_resolutionLineEdit->setText(resolution);

	/// @todo Get Shade type, convert to real render system
	//switch (this->m_modelView->root()->getRenderSystem()->)

	/// @todo Is lighting enabled, convert to real render system
	//this->m_dynamicLightingCheckBox->setChecked(this->m_modelView->root()->getRenderSystem()->has);

	// view
	this->m_backgroundColorColorButton->setColor(ogreColourValueToQColor(this->m_modelView->viewPort()->getBackgroundColour()));
	this->m_ambientLightColorButton->setColor(ogreColourValueToQColor(this->m_modelView->sceneManager()->getAmbientLight()));
	this->m_nearClipDistanceSpinBox->setValue(this->m_modelView->camera()->getNearClipDistance());
	this->m_farClipDistanceSpinBox->setValue(this->m_modelView->camera()->getFarClipDistance());
	this->m_polygonModeComboBox->setCurrentIndex(this->m_modelView->camera()->getPolygonMode() - 1);

	// textures
	this->m_mipMapsSpinBox->setValue(Ogre::TextureManager::getSingleton().getDefaultNumMipmaps());

	this->m_textureFilteringComboBox->setCurrentIndex(Ogre::MaterialManager::getSingleton().getDefaultTextureFiltering(Ogre::FT_MIP)); /// @todo get texture filtering
	//Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TextureFilterOptions(this->m_textureFilteringComboBox->currentIndex()));
}

}

}
