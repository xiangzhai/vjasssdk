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

#include "cameras.hpp"
#include "camera.hpp"

namespace wc3lib
{

namespace mdlx
{

Cameras::Cameras(class Mdlx *mdlx) : MdxBlock("CAMS"), m_mdlx(mdlx)
{
}

Cameras::~Cameras()
{
	for (std::list<class Camera*>::iterator iterator = this->m_cameras.begin(); iterator != this->m_cameras.end(); ++iterator)
		delete *iterator;
}

void Cameras::readMdl(std::istream &istream) throw (class Exception)
{
}

void Cameras::writeMdl(std::ostream &ostream) throw (class Exception)
{
}

long32 Cameras::readMdx(std::istream &istream) throw (class Exception)
{
	long32 bytes = MdxBlock::readMdx(istream);
	
	if (bytes == 0)
		return 0;
	
	long32 nbytes;
	istream.read(reinterpret_cast<char*>(&nbytes), sizeof(nbytes));
	bytes += istream.gcount();
	
	while (nbytes > 0)
	{
		class Camera *camera = new Camera(this);
		long32 readBytes = camera->readMdx(istream);
		nbytes -= readBytes;
		bytes += readBytes;
		this->m_cameras.push_back(camera);
	}
	
	return bytes;
}

long32 Cameras::writeMdx(std::ostream &ostream) throw (class Exception)
{
	long32 bytes = MdxBlock::writeMdx(ostream);
	
	if (bytes == 0)
		return 0;
	
	return bytes;
}

}

}
