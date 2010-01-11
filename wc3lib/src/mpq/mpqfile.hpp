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

#ifndef WC3LIB_MPQ_MPQFILE_HPP
#define WC3LIB_MPQ_MPQFILE_HPP

#include <iostream>

#include "../exception.hpp"

namespace wc3lib
{

namespace mpq
{

/**
* @brief Abstract class for mpq file handlers.
*/
class MpqFile
{
	public:
		enum Locale
		{
			Neutral
		};

		MpqFile(class Mpq *mpq);
		~MpqFile();

		void remove();
		void rename(const std::string &newName);
		std::streamsize read(std::istream &istream);
		std::streamsize write(std::ostream &ostream) const throw (class Exception); //extract
		/// @return Returns file size in bytes.
		std::size_t size() const;
		/// @return Returns compressed file size in bytes.
		std::size_t compressedSize() const;
		enum Locale locale() const;
		std::string path() const;
		std::string name() const;
		bool isDir() const;

	private:
		class Mpq *m_mpq;
		std::size_t m_size; /// @todo  Get correct size type from MPQ specification
		std::size_t m_compressedSize;
		enum Locale m_locale;
		std::string m_path;
		std::string m_name;
		bool m_isDir;
};

inline std::size_t MpqFile::size() const
{
	return this->m_size;
}

inline std::size_t MpqFile::compressedSize() const
{
	return this->m_compressedSize;
}

inline enum MpqFile::Locale MpqFile::locale() const
{
	return this->m_locale;
}

inline std::string MpqFile::path() const
{
	return this->m_path;
}

inline std::string MpqFile::name() const
{
	return this->m_name;
}

inline bool MpqFile::isDir() const
{
	return this->m_isDir;
}

}

}

#endif
