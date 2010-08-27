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

#include <sstream>
#include <utility>

#include <Qt/QtGui>

#include "blpiohandler.hpp"
#include "../blp/blp.hpp"

namespace wc3lib
{

namespace editor
{

BlpIOHandler::BlpIOHandler() : QImageIOHandler()
{
}

BlpIOHandler::~BlpIOHandler()
{
}

bool BlpIOHandler::canRead() const
{
	blp::dword identifier;

	if (this->device() != 0 && this->device()->isReadable() && this->device()->peek(reinterpret_cast<char*>(&identifier), sizeof(identifier)) == sizeof(identifier) && memcmp(reinterpret_cast<const void*>(&identifier), reinterpret_cast<const void*>(blp::Blp::identifier0), sizeof(identifier)))
		return true;

	return false;
}

bool BlpIOHandler::read(QImage *image)
{
	// read buffer into input stream
	QByteArray all = this->device()->readAll();
	std::istringstream isstream;
	isstream.rdbuf()->pubsetbuf(all.data(), all.size());
	class blp::Blp blpImage;
	blpImage.readBlp(isstream);

	if (blpImage.mipMaps().empty()) // no mip maps
		return false;

	// write blp data into image
	//if (this->m_blp->flags() == blp::Blp::Alpha)
		//image->
	typedef std::pair<blp::Blp::MipMap::Coordinates, struct blp::Blp::MipMap::Color> MapType;

	foreach (MapType mapEntry, blpImage.mipMaps().front()->colors())
	{
		const blp::Blp::MipMap::Coordinates coordinates = mapEntry.first;
		const struct blp::Blp::MipMap::Color color = mapEntry.second;
		QRgb pixelColor = (color.alpha << 3) + color.rgb; // QRgb starts with alpha
		image->setPixel(coordinates.first, coordinates.second, pixelColor);
	}

	return true;
}

/// @todo Where should we get the options from?!
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
	class blp::Blp blpImage;

	if (image.format() == QImage::Format_Indexed8)
		blpImage.setCompression(blp::Blp::Paletted);
	else
		blpImage.setCompression(blp::Blp::Jpeg);

	if (image.hasAlphaChannel())
		blpImage.setFlags(blp::Blp::Alpha);
	else
		blpImage.setFlags(blp::Blp::NoAlpha);

	blpImage.setWidth(image.width());
	blpImage.setHeight(image.height());
	blpImage.setPictureType(0);
	/*
	if (image.hasAlphaChannel())
		this->m_blp->setPictureType(3);
	else
		this->m_blp->setPictureType(5);
	*/

	// create mip map
	/// @todo Fill palett if paletted!!!!!!
	class blp::Blp::MipMap *mipMap = new blp::Blp::MipMap(image.width(), image.height());

	for (int width = 0; width < image.size().width(); ++width)
	{
		for (int height = 0; height < image.size().height(); ++height)
		{
			// set color
			QRgb rgb = image.pixel(width, height);
			int index = 0;

			if (blpImage.compression() == blp::Blp::Paletted)
				index = image.pixelIndex(width, height); // index has to be set because paletted compression can also be used

			mipMap->setColor(width, height, rgb << 1, qAlpha(rgb), index); // move rgb and discard alpha
		}
	}

	blpImage.generateMipMaps(mipMap);

	std::ostringstream osstream;
	blpImage.writeBlp(osstream);
	std::streamsize bufferSize = osstream.rdbuf()->in_avail();
	char buffer[bufferSize];
	osstream.rdbuf()->sgetn(buffer, bufferSize);
	this->device()->write(buffer, bufferSize);

	/// @todo Recognize image IO handler options!

	return true;
}

}

}
