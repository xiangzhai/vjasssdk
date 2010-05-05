/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
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

#include <dlfcn.h>

#include <boost/format.hpp>

#include "libraryloader.hpp"
#include "internationalisation.hpp"

namespace wc3lib
{

std::map<boost::filesystem::path, void*> LibraryLoader::m_libraries;

bool LibraryLoader::libraryHasBeenLoaded(const boost::filesystem::path &path)
{
	return self::m_libraries.find(path) != self::m_libraries.end();
}

void LibraryLoader::loadLibrary(const boost::filesystem::path &path) throw (class Exception)
{
	// Library has already been loaded.
	if (self::libraryHasBeenLoaded(path))
		return;

	void *handle = dlopen(path.string().c_str(), RTLD_NOW);

	if (handle == NULL)
		throw Exception(boost::format(_("Error while loading shared object \"%1%\": %2%")) % path.string() % dlerror());

	self::m_libraries.insert(std::make_pair(path, handle));
}

void LibraryLoader::unloadLibrary(const boost::filesystem::path &path) throw (class Exception)
{
	// Library hasn't already been loaded.
	if (dlclose(self::m_libraries[path]) != 0)
		throw Exception(boost::format(_("Error while unloading shared object \"%1%\". It has never been loaded.")) % path.string());
}

void* LibraryLoader::librarySymbol(const boost::filesystem::path &path, const std::string symbolName) throw (class Exception)
{
	if (!self::libraryHasBeenLoaded(path))
		throw Exception(boost::format(_("Error while loading symbol \"%1%\" from shared object \"%2%\": Shared object has never been loaded.")) % symbolName % path.string());

	dlerror(); // clean up errors

	// get symbol
	void *handle = dlsym(self::m_libraries[path], symbolName.c_str());

	// got error
	if (dlerror() != NULL)
		throw Exception(boost::format(_("Error while loading symbol \"%1%\" from shared object \"%2%\": %3%")) % symbolName % path.string() % dlerror());

	return handle;
}

LibraryLoader::LibraryLoader()
{
}

}
