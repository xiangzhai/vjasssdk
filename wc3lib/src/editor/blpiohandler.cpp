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

#include <boost/foreach.hpp>

#include <QtCore>
#include <QtGui>

#include "blpiohandler.hpp"
#include "../blp/blp.hpp"

namespace wc3lib
{

namespace editor
{

namespace
{

inline blp::color rgbaToColor(QRgb rgba)
{
	return (blp::color)(qAlpha(rgba)) << 32 + (rgba & 0x00FFFFFF);
}

inline QRgb colorToRgba(blp::color c)
{
	//return qRgba(c & 0xFF000000, c & 0x00FF0000, c & 0x0000FF00, c & 0x000000FF);
	/// @todo ARGB!
	return qRgba((c & 0x00FF0000) >> 16, (c & 0x0000FF00) >> 8, (c & 0x000000FF), 0xFF - ((c & 0xFF000000) >> 32));
	//return qRgba((c & 0xFF000000) >> 0xFFFFFF, (c & 0x00FF0000) >> 0xFFFF, (c & 0x0000FF00) >> 0xFF, (c & 0x000000FF));
}

}

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
	std::basic_istringstream<blp::byte> istream;
	istream.rdbuf()->pubsetbuf(reinterpret_cast<unsigned char*>(all.data()), all.size());
	class blp::Blp blpImage;

	try
	{
		blpImage.read(istream);
	}
	catch (class Exception &exception)
	{
		qDebug() << "BLP Input Exception: " << exception.what().c_str();

		return false;
	}

	if (blpImage.mipMaps().empty()) // no mip maps
		return false;

	// write blp data into image
	//if (this->m_blp->flags() == blp::Blp::Alpha)
		//image->
	blp::Blp::MipMap *mipMap = blpImage.mipMaps().front(); // first mip map has original size

	*image = QImage(mipMap->width(), mipMap->height(), blpImage.flags() == blp::Blp::Alpha ? QImage::Format_ARGB32 : QImage::Format_RGB32);

	typedef std::pair<const blp::Blp::MipMap::Coordinates&, const class blp::Blp::MipMap::Color&> MapType;

	foreach (blp::Blp::MipMap::MapEntryType mapEntry, mipMap->colors())
	{
		const blp::Blp::MipMap::Coordinates &coordinates = mapEntry.first;
		const struct blp::Blp::MipMap::Color &color = mapEntry.second;
		QRgb pixelColor = colorToRgba(color.rgba());
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
	/// @todo Fill palette if paletted!!!!!!
	class blp::Blp::MipMap *mipMap = blpImage.addInitialMipMap(image.width(), image.height());

	for (int width = 0; width < image.size().width(); ++width)
	{
		for (int height = 0; height < image.size().height(); ++height)
		{
			// set color
			QRgb rgb = image.pixel(width, height);
			int index = 0;

			if (blpImage.compression() == blp::Blp::Paletted)
				index = image.pixelIndex(width, height); // index has to be set because paletted compression can also be used

			blp::color rgba = rgbaToColor(rgb);
			mipMap->setColor(width, height, rgba, qAlpha(rgb), index);
		}
	}

	blpImage.generateMipMaps(mipMap);
	std::basic_ostringstream<blp::byte> ostream;

	try
	{
		blpImage.write(ostream);
	}
	catch (class Exception &exception)
	{
		qDebug() << "BLP Output Exception: " << exception.what().c_str();

		return false;
	}

	std::streamsize bufferSize = ostream.rdbuf()->in_avail();
	char buffer[bufferSize];
	ostream.rdbuf()->sgetn(reinterpret_cast<blp::byte*>(buffer), bufferSize);
	this->device()->write(buffer, bufferSize);

	/// @todo Recognize image IO handler options!

	return true;
}

}

}
