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

#ifndef WC3LIB_WC3LIB_HPP
#define WC3LIB_WC3LIB_HPP

#if !defined(LINUX) && !defined(MAC) && !defined(WINDOWS)
#error Undefined platform.
#endif

/**
* \mainpage wc3lib
* \date Version date
* 2010-12-25
*
* \section introduction Introduction
* This is the API reference and documentation of the wc3lib project. wc3lib is an abbreviation and means "Warcraft 3 Library".
* It's a collection of varios libraries which allow programmers to use Blizzard's customly developed formats with an abstract C++ interface.
* All implementations are free and mainly under the GPLv2 license but there's also some external code which is mostly under some similar license.
* As mentioned above the wc3lib is splitted up into some different modules. Each module consists of a single library which supports one of Blizzard's formats (except modules "Applications" and "Editor" which are extensions of the default modules).
* To use the whole library you could simply include file \ref "wc3lib.hpp" and link your program with library "wc3lib".
* Another probably faster way is to link it with the required libraries only.
* Therefore you can include all header files of your required modules since each module has one.
* Besides you should bear in mind that each module has its own namespace.
* All namespaces and other declarations of the wc3lib belong to the global namespace \ref wc3lib.
*
* \section mpq MPQ module
* Include file \ref "mpq.hpp" and use namspace \ref mpq to use this module.
*
* @section blp BLP module
* Include file \ref "blp.hpp" and use namspace \ref blp to use this module.
*
* \section editor Editor module
* The editor module contains many classes to emulate the original World Editor created by Blizzard Entertainment.
* There are various Qt and KDE plug-ins which allow you to load and save BLP, MPQ and SLK files in an appreciated way.
* Include file \ref "editor.hpp" and use namspace \ref edtor to use this module.
*
* \section applications Applications module
* Since this module only provides some useful applications there is neither any namespace to use nor any header file to include.
*
* \namespace wc3lib
* All code elements of the wc3lib belong to this namespace!
*/
/// @todo Add all include files!

#include "exception.hpp"
#include "internationalisation.hpp"
#include "libraryloader.hpp"
#include "utilities.hpp"

#include "blp.hpp"

#ifdef EDITOR
#include "editor.hpp"
#endif
#include "mdlx.hpp"

#include "mpq.hpp"

#endif
