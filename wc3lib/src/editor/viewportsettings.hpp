/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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

#ifndef WC3LIB_EDITOR_VIEWPORTSETTINGS_HPP
#define WC3LIB_EDITOR_VIEWPORTSETTINGS_HPP

#include <Ogre.h>

#include "settingsinterface.hpp"

namespace wc3lib
{

namespace editor
{

/// Each view port includes one camera (and of course its settings).
class ViewPortSettings : public SettingsInterface
{
	public:
		ViewPortSettings(Ogre::Viewport *viewPort);
		virtual void read(const KConfigGroup &group);
		virtual void write(KConfigGroup &group) const;
		virtual QString groupName() const;

		void setViewPort(Ogre::Viewport *viewPort);
		Ogre::Viewport* viewPort() const;

	protected:
		Ogre::Viewport *m_viewPort;
};

inline void ViewPortSettings::setViewPort(Ogre::Viewport *viewPort)
{
	this->m_viewPort = viewPort;
}

inline Ogre::Viewport* ViewPortSettings::viewPort() const
{
	return this->m_viewPort;
}

}

}

#endif
