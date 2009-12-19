/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
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

#include "blpiohandler.hpp"

namespace wc3lib
{

namespace editor
{
	
BlpIOHandler::BlpIOHandler() : QImageIOHandler(), m_blp(new blp::Blp)
{
}

BlpIOHandler::~BlpIOHandler()
{
	delete this->m_blp;
}

bool BlpIOHandler::canRead() const
{
	char identifier[4];
	
	if (this->device() != 0 && this->device()->isReadable() && this->device()->peek(identifier, sizeof(identifier)) == sizeof(identifier) && memcmp(identifier, "BLP1", sizeof(identifier)))
		return true;

	return false;
}

bool BlpIOHandler::read(QImage *image)
{
	/*
	Using JPEG compression
	if (image->numColors() > 256)
		return false;
	*/
	this->m_blp->clear();
	this->m_blp->setCompression(blp::Blp::Jpeg);
	
	if (image->hasAlphaChannel())
		this->m_blp->setFlags(blp::Blp::Alpha);
	else
		this->m_blp->setFlags(blp::Blp::NoAlpha);
		
	this->m_blp->setWidth(image->width());
	this->m_blp->setHeight(image->height());
	this->m_blp->setPictureType(0);
	/*
	if (image->hasAlphaChannel())
		this->m_blp->setPictureType(3);
	else
		this->m_blp->setPictureType(5);
	*/
		
	this->m_blp->setPictureSubType(0); //1
	std::list<blp::color> palette;
	
	foreach (QRgb rgb, image->colorTable())
		palette.push_back(rgb);

	this->m_blp->setPalette(palette);
	struct blp::Blp::MipMap *mipMap = new blp::Blp::MipMap;
	mipMap->setWidth(image->width());
	mipMap->setHeight(image->height());
	QImage alphaChannel = image->alphaChannel();
	
	for (int i = 0; i < image->width(); ++i)
	{
		for (int j = 0; j < image->height(); ++j)
		{
			mipMap->addIndex(image->pixel(i, j));
			
			if (image->hasAlphaChannel())
				mipMap->addAlpha(alphaChannel.pixel(i, j));
		}
	}
	
	this->m_blp->generateMipMaps(mipMap);
	
	/// @todo Recognize image IO handler options!
	
	return true;
}

bool BlpIOHandler::supportsOption(ImageOption option) const
{
	switch (option)
	{
		case QImageIOHandler::Size:
			return false;
	}
	
	return false;
}

bool BlpIOHandler::write(const QImage &image)
{
	
	
	return true;
}

}

}
