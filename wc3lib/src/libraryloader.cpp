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

class LibraryLoader::Handle* LibraryLoader::loadLibrary(const boost::filesystem::path &path) throw (class Exception)
{
	void *handle = dlopen(path.string().c_str(), RTLD_LAZY);

	if (handle == NULL)
		throw Exception(boost::format(_("Error while loading shared object \"%1%\": %2%")) % path.string() % dlerror());

	class Handle *result = new Handle;
	result->handle = handle;
	result->path = path;

	return result;
}

void LibraryLoader::unloadLibrary(class Handle *handle) throw (class Exception)
{
	if (handle == 0)
		throw Exception(_("Error while unloading shared object. Handle is 0."));

	// Library hasn't already been loaded.
	if (dlclose(handle->handle) != 0)
		throw Exception(boost::format(_("Error while unloading shared object \"%1%\". It has never been loaded.")) % handle->path.string());

	delete handle;
}

void* LibraryLoader::librarySymbol(const class Handle &handle, const std::string symbolName) throw (class Exception)
{
	dlerror(); // clean up errors

	// get symbol
	void *symbolHandle = dlsym(handle.handle, symbolName.c_str());

	// got error
	if (dlerror() != NULL)
		throw Exception(boost::format(_("Error while loading symbol \"%1%\" from shared object \"%2%\": %3%")) % symbolName % handle.path.string() % dlerror());

	return symbolHandle;
}

LibraryLoader::LibraryLoader()
{
}

}
