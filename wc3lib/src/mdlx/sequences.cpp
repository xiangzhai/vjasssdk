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

#include "sequences.hpp"

namespace wc3lib
{

namespace mdlx
{

Sequences::Sequences(class Mdlx *mdlx) : MdxBlock("SEQS"), m_mdlx(mdlx)
{
}

Sequences::~Sequences()
{
}

void Sequences::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Sequences::readMdx(std::fstream &fstream) throw (class Exception)
{
}

void Sequences::writeMdl(std::fstream &fstream) throw (class Exception)
{
	fstream
	<< "Sequences " << this->sequences().size() << " {\n";

	for (std::list<class Sequence*>::iterator iterator = this->sequences().begin(); iterator != this->sequences().end(); ++iterator)
	{
		fstream
		<< "\tAnim " << (*iterator)->name() << " {\n"
		<< "\t\tInterval { " << (*iterator)->intervalStart() << ", " << (*iterator)->intervalEnd() << " },\t"
		;

		if ((*iterator)->noLooping() == 1)
			fstream << "\t\tNonLooping,\n";

		if ((*iterator)->moveSpeed() != 0.0)
			fstream << "\t\tMoveSpeed " << (*iterator)->moveSpeed << ",\n";

		if ((*iterator)->rarity() != 0.0)
			fstream << "\t\tRarity " << (*iterator)->rarity << ",\n";

		if ((*iterator)->minExtX() != 0.0 || (*iterator)->minExtY() != 0.0 || (*iterator)->minExtZ != 0.0)
			fstream << "\t\tMinimumExtent { " << (*iterator)->minExtX() << ", " << (*iterator)->minExtY() << ", " << (*iterator)->minExtZ() << " },\n";

		if ((*iterator)->maxExtX() != 0.0 || (*iterator)->maxExtY() != 0.0 || (*iterator)->maxExtZ() != 0.0)
			fstream << "\t\tMaxmimumExtent { " << (*iterator)->maxExtX() << ", " << (*iterator)->maxExtY() << ", " << (*iterator)->maxExtZ() << " },\n";

		if ((*iterator)->boundsRadius() != 0.0)
			fstream << "\t\tBoundsRadius " << (*iterator)->boundsRadius() << ",\n";

		fstream << "\t}\n";
	}

	fstream << "}\n";
}

void Sequences::writeMdx(std::fstream &fstream) throw (class Exception)
{
}

}

}
