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

#ifndef WC3LIB_MPQ_MPQ_HPP
#define WC3LIB_MPQ_MPQ_HPP

#include <istream>
#include <ostream>
#include <list>
#include <string>

#include "../exception.hpp"

namespace wc3lib
{

namespace mpq
{

class MpqFile;

class Mpq
{
	public:
		enum Mode
		{
			Read,
			Write
		};

		enum Format
		{
			Mpq1,
			Mpq2
		};

		Mpq();
		~Mpq();

		/**
		* @return Returns MPQ's size in bytes.
		*/
		std::streamsize read(std::istream &istream, enum Mode mode) throw (class Exception);
		
		/**
		* @return Returns MPQ's size in bytes.
		*/
		std::streamsize write(std::ostream &ostream, enum Format format) const;
		
		const class MpqFile* findFile(const std::string &path) const;
		const class MpqFile* findFileByName(const std::string &name) const;
		
		std::streamsize size() const;
		std::string path() const;
		const std::list<class MpqFile*>& files() const;

	private:
		std::list<class MpqFile*> m_files;
};

inline const std::list<class MpqFile*>& Mpq::files() const
{
	return this->m_files;
}

}

}

#endif
