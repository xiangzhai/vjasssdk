/***************************************************************************
 *   Copyright (C) 2008, 2009 by Tamino Dauth                              *
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

#ifndef WC3LIB_LANG_LANGUAGE_HPP
#define WC3LIB_LANG_LANGUAGE_HPP

#include <string>

#include "object.hpp"
#include "jass/jass.hpp"
#include "vjass/vjass.hpp"
#include "zinc/zinc.hpp"
#include "jasspp/jasspp.hpp"

namespace wc3lib
{
	
namespace lang
{

class Language
{
	public:
		static class jass::Jass* jass();
		static class vjass::Vjass* vjass();
		static class zinc::Zinc* zinc();
		static class jasspp::Jasspp* jasspp();
		
		Language();
		virtual ~Language();
		
		virtual const std::string& name() const = 0;
		virtual bool compatibleTo(const Language &language) const = 0;
		/**
		* Initializes all objects and their links to each other.
		*/
		virtual void initObjects() = 0;
		
	protected:
		static class jass::Jass *m_jass;
		static class vjass::Vjass *m_vjass;
		static class zinc::Zinc *m_zinc;
		static class jasspp::Jasspp *m_jasspp;
		
		class Object::List m_list;
};

inline class jass::Jass* Language::jass()
{
	if (Language::m_jass == 0)
		Language::m_jass = new jass::Jass;
	
	return Language::m_jass;
}

inline class vjass::Vjass* Language::vjass()
{
	if (Language::m_vjass == 0)
		Language::m_vjass = new vjass::Vjass;
				    
	return Language::m_vjass;
}


inline class zinc::Zinc* Language::zinc()
{
	if (Language::m_zinc == 0)
		Language::m_zinc = new zinc::Zinc;
	
	return Language::m_zinc;
}

inline class jasspp::Jasspp* Language::jasspp()
{
	if (Language::m_jasspp == 0)
		Language::m_jasspp = new jasspp::Jasspp;
	
	return Language::m_jasspp;
}

}

}

#endif
