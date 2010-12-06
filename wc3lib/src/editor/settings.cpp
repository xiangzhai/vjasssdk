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

#include "settings.hpp"
#include "modelview.hpp"

namespace wc3lib
{

namespace editor
{

Settings::Settings(class ModelView *modelView, QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f), m_modelView(modelView)
{
	KTabWidget *tabWidget = new KTabWidget(this);
	setupUi(tabWidget);
	this->layout()->addWidget(tabWidget);
	KDialogButtonBox *dialogButtonBox = new KDialogButtonBox(this);
	dialogButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel);
	connect(dialogButtonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(apply()));
	this->layout()->addWidget(dialogButtonBox);
}

void Settings::apply()
{
	Ogre::RenderSystem *renderSystem = this->m_modelView->root()->getRenderSystem();

	if (!renderSystem)
		return;

	// apply new render system
	const Ogre::RenderSystemList &renderers = this->m_modelView->root()->getAvailableRenderers();

	this->m_modelView->root()->setRenderSystem(renderers[this->m_renderSystemComboBox->currentIndex()]);
}

void Settings::showEvent(QShowEvent *event)
{
	const Ogre::RenderSystemList &renderers = this->m_modelView->root()->getAvailableRenderers();

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
}

}

}
