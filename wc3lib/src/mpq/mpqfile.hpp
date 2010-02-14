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

#include <istream>
#include <ostream>
#include <list>

#include <boost/filesystem.hpp>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mpq
{
	
class Mpq;
class Hash;
class Sector;

/**
* @brief Abstract class for mpq file access. Combines hash and block table data of file.
*/
class MpqFile
{
	public:
		/// @todo Define all locales.
		enum Locale
		{
			Neutral
		};
		
		/// There seems to be only one platform.
		enum Platform
		{
			Default
		};

		void remove() throw (class Exception);
		void rename(const std::string &newName, bool overwriteExisting = false) throw (class Exception);
		void move(const boost::filesystem::path &newPath, bool overwriteExisting = false) throw (class Exception);
		/**
		* Reads the whole file data, not the header data.
		*/
		std::streamsize read(std::istream &istream) throw (class Exception);
		/**
		* Writes the whole file data, not the header data.
		*/
		std::streamsize write(std::ostream &ostream) const throw (class Exception);
		/// @return Returns file size in bytes.
		std::size_t size() const;
		/// @return Returns compressed file size in bytes.
		std::size_t compressedSize() const;
		enum Locale locale() const;
		/**
		* @return Returns the file path. Note that MPQ archives without list file don't have any information about the file paths.
		*/
		const boost::filesystem::path& path() const;
		const std::list<class Sector*>& sectors() const;

	protected:
		friend class Mpq;
		
		static int16 localeToInt(enum Locale locale);
		static enum Locale intToLocale(int16 value);
		static int8 platformToInt(enum Platform platform);
		static enum Platform intToPlatform(int8 value);
		
		/**
		* MPQ files are created by @class Mpq only.
		*/
		MpqFile(class Mpq *mpq, class Hash *hash);
		~MpqFile();
		
		class Mpq *m_mpq;
		class Hash *m_hash;
		std::size_t m_size; /// @todo  Get correct size type from MPQ specification
		std::size_t m_compressedSize;
		enum Locale m_locale;
		enum Platform m_platform;
		boost::filesystem::path m_path;
		std::list<class Sector*> m_sectors;
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

inline const boost::filesystem::path& MpqFile::path() const
{
	return this->m_path;
}

inline const std::list<class Sector*>& MpqFile::sectors() const
{
	return this->m_sectors;
}

inline int16 MpqFile::localeToInt(enum MpqFile::Locale locale)
{
	return int16(locale);
}

inline enum MpqFile::Locale MpqFile::intToLocale(int16 value)
{
	return MpqFile::Locale(value);
}

inline int8 MpqFile::platformToInt(enum MpqFile::Platform platform)
{
	return int8(platform);
}

inline enum MpqFile::Platform intToPlatform(int8 value)
{
	return MpqFile::Platform(value);
}

}

}

#endif
