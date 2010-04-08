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
#include "block.hpp"
#include "hash.hpp"

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

		/**
		* Reads the whole file data, not the header data.
		*/
		std::streamsize read(std::istream &istream) throw (class Exception);
		/**
		* Reads data from stream @param istream and appends it to the already existing file data.
		* @return Returns the number of read bytes.
		*/
		std::streamsize append(std::istream &istream) throw (class Exception);
		/**
		* Writes the whole file data, not the header data.
		*/
		std::streamsize write(std::ostream &ostream) const throw (class Exception);
		
		// hash attributes
		enum Locale locale() const;
		enum Platform platform() const;

		class Mpq* mpq() const;
		class Hash* hash() const;
		/**
		* @return Returns the file path. Note that MPQ archives without list file don't have any information about the file paths.
		*/
		const boost::filesystem::path& path() const;
		
		// block attributes
		uint32 fileKey() const;
		/// @return Returns file size in bytes.
		int32 size() const;
		/// @return Returns compressed file size in bytes.
		int32 compressedSize() const;
		bool isFile() const;
		bool isEncrypted() const;
		bool isCompressed() const;
		
		// extended attributes
		CRC32 crc32() const;
		void setFileTime(const time_t &time);
		const struct FILETIME& fileTime() const;
		bool fileTime(time_t &time) const;
		MD5 md5() const;
		
		const std::list<class Sector*>& sectors() const;

	protected:
		friend class Mpq;
		
		static int16 localeToInt(enum Locale locale);
		static enum Locale intToLocale(int16 value);
		static int16 platformToInt(enum Platform platform);
		static enum Platform intToPlatform(int16 value);
		
		/**
		* MPQ files are created by @class Mpq only.
		*/
		MpqFile(class Mpq *mpq, class Hash *hash);
		~MpqFile();
		
		void remove() throw (class Exception);
		void rename(const std::string &newName, bool overwriteExisting = false) throw (class Exception);
		void move(const boost::filesystem::path &newPath, bool overwriteExisting = false) throw (class Exception);
		
		class Mpq *m_mpq;
		class Hash *m_hash;
		boost::filesystem::path m_path;
		std::list<class Sector*> m_sectors;
};

inline enum MpqFile::Locale MpqFile::locale() const
{
	return MpqFile::intToLocale(this->m_hash->locale());
}

inline enum MpqFile::Platform MpqFile::platform() const
{
	return MpqFile::intToPlatform(this->m_hash->platform());
}

inline class Mpq* MpqFile::mpq() const
{
	return this->m_mpq;
}

inline class Hash* MpqFile::hash() const
{
	return this->m_hash;
}

inline const boost::filesystem::path& MpqFile::path() const
{
	return this->m_path;
}

inline uint32 MpqFile::fileKey() const
{
	return this->m_hash->block()->fileKey(this->path());
}

inline int32 MpqFile::size() const
{
	return this->m_hash->block()->fileSize();
}

/// @todo FIXME (return compressed and not real size)
inline int32 MpqFile::compressedSize() const
{
	return this->m_hash->block()->blockSize();
}

inline bool MpqFile::isFile() const
{
	return this->m_hash->block()->flags() & Block::IsFile;
}

inline bool MpqFile::isEncrypted() const
{
	return this->m_hash->block()->flags() & Block::IsEncrypted;
}

inline bool MpqFile::isCompressed() const
{
	return this->m_hash->block()->flags() & Block::IsCompressed;
}

inline CRC32 MpqFile::crc32() const
{
	return this->m_hash->block()->crc32();
}

inline void MpqFile::setFileTime(const time_t &time)
{
	this->m_hash->block()->setFileTime(time);
}

inline const struct FILETIME& MpqFile::fileTime() const
{
	return this->m_hash->block()->fileTime();
}

inline bool MpqFile::fileTime(time_t &time) const
{
	return this->m_hash->block()->fileTime(time);
}

inline MD5 MpqFile::md5() const
{
	return this->m_hash->block()->md5();
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

inline int16 MpqFile::platformToInt(enum MpqFile::Platform platform)
{
	return int16(platform);
}

inline enum MpqFile::Platform MpqFile::intToPlatform(int16 value)
{
	return MpqFile::Platform(value);
}

}

}

#endif
