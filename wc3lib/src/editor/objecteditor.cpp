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

#include "objecteditor.hpp"
#include "../slk/data.hpp"
#include "../mpq/mpqfile.hpp"

namespace wc3lib
{
	
namespace editor
{
	
ObjectEditor::ObjectEditor(class Editor *editor) : SubEditor(editor), m_data(0)
{
	const class mpq::MpqFile *unitEditorDataFile = editor->loadMpqFile("UI/UnitEditorData.txt");
	
	if (unitEditorDataFile != 0)
	{
		this->m_data = new slk::Data;
		std::iostream iostream;
		unitEditorDataFile->write(iostream);
		this->m_data->read(iostream);
	}
}

ObjectEditor::~ObjectEditor()
{
	if (this->m_data != 0)
		delete this->m_data;
}
	
}

}
