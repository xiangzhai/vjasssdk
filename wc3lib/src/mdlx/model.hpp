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

#ifndef WC3LIB_MDLX_MODEL_HPP
#define WC3LIB_MDLX_MODEL_HPP

#include "mdxblock.hpp"

namespace wc3lib
{

namespace mdlx
{

//MODL
class Model : public MdxBlock
{
	public:
		Model(class Mdlx *mdlx);
		virtual ~Model();

		class Mdlx* mdlx() const;
		const ascii* name() const;
		long32 unknown0() const;
		float32 boundsRadius() const;
		float32 minExtX() const;
		float32 minExtY() const;
		float32 minExtZ() const;
		float32 maxExtX() const;
		float32 maxExtY() const;
		float32 maxExtZ() const;
		long32 blendTime() const;

		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) throw (class Exception);
		virtual long32 readMdx(std::istream &istream) throw (class Exception);
		virtual long32 writeMdx(std::ostream &ostream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		//long nbytes;
		ascii m_name[0x150]; //(0x150 bytes)
		long32 m_unknown0; //(0)
		float32 m_boundsRadius;
		float32 m_minExtX, m_minExtY, m_minExtZ;
		float32 m_maxExtX, m_maxExtY, m_maxExtZ;
		long32 m_blendTime;
};

inline Mdlx* Model::mdlx() const
{
	return this->m_mdlx;
}

inline const ascii* Model::name() const
{
	return this->m_name;
}

inline long32 Model::unknown0() const
{
	return this->m_unknown0;
}

inline float32 Model::boundsRadius() const
{
	return this->m_boundsRadius;
}

inline float32 Model::minExtX() const
{
	return this->m_minExtX;
}

inline float32 Model::minExtY() const
{
	return this->m_minExtY;
}

inline float32 Model::minExtZ() const
{
	return this->m_minExtZ;
}

inline float32 Model::maxExtX() const
{
	return this->m_maxExtX;
}


inline float32 Model::maxExtY() const
{
	return this->m_maxExtY;
}

inline float32 Model::maxExtZ() const
{
	return this->m_maxExtZ;
}

inline long32 Model::blendTime() const
{
	return this->m_blendTime;
}

}

}

#endif
