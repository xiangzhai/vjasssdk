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

#ifndef WC3LIB_MDLX_PARTICLEEMITTER2SPEED_HPP
#define WC3LIB_MDLX_PARTICLEEMITTER2SPEED_HPP

#include "mdxalpha.hpp"
#include "particleemitter2speeds.hpp"

namespace wc3lib
{
	
namespace mdlx
{

class ParticleEmitter2Speeds;
	
class ParticleEmitter2Speed : public MdxAlpha
{
	public:
		ParticleEmitter2Speed(class ParticleEmitter2Speeds *speeds);
		virtual ~ParticleEmitter2Speed();
		
		class ParticleEmitter2Speeds* speeds() const;
		
		virtual void readMdl(std::istream &istream) throw (class Exception);
		virtual void writeMdl(std::ostream &ostream) const throw (class Exception);
};

inline class ParticleEmitter2Speeds* ParticleEmitter2Speed::speeds() const
{
	return dynamic_cast<class ParticleEmitter2Speeds*>(this->m_alphas);
}

}

}

#endif
