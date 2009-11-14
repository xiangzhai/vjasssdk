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
	if (image->numColors() > 256)
		return false;
	
	if (this->m_blp != 0)
		delete this->m_blp;
	
	this->m_blp->setCompression(blp::Blp::Uncompressed);
	
	if (image->hasAlphaChannel())
		this->m_blp->setFlags(8);
	else
		this->m_blp->setFlags(0);
		
	this->m_blp->setWidth(image->width());
	this->m_blp->setHeight(image->height());
	
	if (image->hasAlphaChannel())
		this->m_blp->setPictureType(3);
	else
		this->m_blp->setPictureType(5);
	
	this->m_blp->setPictureSubType(1);
	int i = 0;
	
	foreach (QRgb rgb, image->colorTable())
	{
		this->m_blp->setPaletteColor(i, rgb);
		++i;
	}
	
	struct blp::Blp::MipMap *mipMap = new blp::Blp::MipMap;
	mipMap->m_width = image->width();
	mipMap->m_height = image->height();
	QImage alphaChannel = image->alphaChannel();
	
	for (int i = 0; i < image->width(); ++i)
	{
		for (int j = 0; j < image->height(); ++j)
		{
			mipMap->m_indexList.push_back(image->pixel(i, j));
			
			if (image->hasAlphaChannel())
				mipMap->m_alphaList.push_back(alphaChannel.pixel(i, j));
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
