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

#include <iostream>
#include <fstream>

#include "mdlx/mdlx.hpp"

using namespace wc3lib;

int main(int argc, char *argv[])
{
	std::fstream inputFstream;
	std::fstream outputFstream;

	for (int i = 1; i < argc; ++i)
	{
		mdlx::Mdlx mdlx;
		inputFstream.open(argv[i], std::ifstream::in | std::ifstream::binary);

		try
		{
			mdlx.readMdx(inputFstream);
		}
		catch (Exception &exception)
		{
			std::cerr << "Error while reading mdx file: " << exception.what() << std::endl;
		}

		inputFstream.close();
		outputFstream.open("test.mdl", std::ifstream::out | std::ifstream::binary);
		mdlx.writeMdl(outputFstream);
		outputFstream.close();
	}

	return 0;
}