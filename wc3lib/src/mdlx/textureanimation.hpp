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

#ifndef WC3LIB_MDLX_TEXTUREANIMATION_HPP
#define WC3LIB_MDLX_TEXTUREANIMATION_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;
class Translation;
class Rotation;
class Scaling;

class TextureAnimation
{
	public:
		TextureAnimation(class Mdlx *mdlx);
		virtual ~TextureAnimation();

		class Mdlx* mdlx() const;
		class Translation* translation() const;
		class Rotation* rotation() const;
		class Scaling* scaling() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		//long32 nbytesi;
		class Mdlx *m_mdlx;
		class Translation *m_translation; //(KTAT) // Might be optional
		class Rotation *m_rotation; //(KTAR)
		class Scaling *m_scaling; //(KTAS)
};

inline class Mdlx* TextureAnimation::mdlx() const
{
	return this->m_mdlx;
}

inline class Translation* TextureAnimation::translation() const
{
	return this->m_translation;
}

inline class Rotation* TextureAnimation::rotation() const
{
	return this->m_rotation;
}

inline class Scaling* TextureAnimation::scaling() const
{
	return this->m_scaling;
}

}

}

#endif
