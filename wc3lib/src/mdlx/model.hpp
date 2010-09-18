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
#include "bounds.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;

/// MODL
class Model : public MdxBlock, public Bounds
{
	public:
		Model(class Mdlx *mdlx);
		virtual ~Model();

		class Mdlx* mdlx() const;
		const ascii* name() const;
		const ascii* animationFileName() const;
		long32 blendTime() const;

		virtual std::streamsize readMdl(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(std::ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(std::istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(std::ostream &ostream) const throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		//long nbytes;
		ascii m_name[80];
		ascii m_animationFileName[260];
		long32 m_blendTime;
};

inline class Mdlx* Model::mdlx() const
{
	return this->m_mdlx;
}

inline const ascii* Model::name() const
{
	return this->m_name;
}

inline const ascii* Model::animationFileName() const
{
	return this->m_animationFileName;
}

inline long32 Model::blendTime() const
{
	return this->m_blendTime;
}

}

}

#endif
