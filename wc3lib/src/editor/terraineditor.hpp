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

#ifndef WC3LIB_EDITOR_TERRAINEDITOR_HPP
#define WC3LIB_EDITOR_TERRAINEDITOR_HPP

#include "module.hpp"
#include "modelview.hpp"

namespace wc3lib
{
	
namespace editor
{

/**
* @todo Model view (Ogre view) should be splittable.
* @todo Maybe you should add a custom UI like for other sub editors.
*/
class TerrainEditor : public Module, public ModelView
{
	public:
		TerrainEditor(class Editor *editor, Qt::WFlags f = 0);
		virtual ~TerrainEditor();
		
		virtual void show();
};

}

}

#endif
