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

#ifndef WC3LIB_MPQ_HASH_HPP
#define WC3LIB_MPQ_HASH_HPP

#include <boost/filesystem.hpp>

#include "platform.hpp"
#include "../exception.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{
	
namespace mpq
{

class Hash : public Format
{
	public:
		Hash(class Mpq *mpq);
		
		std::streamsize read(istream &istream) throw (class Exception);
		std::streamsize write(ostream &ostream) const throw (class Exception) { throw Exception(_("Do not use write member function of class Hash.")); }
		virtual uint32_t version() const { return 0; }
		
		void clear();
		
		//bool isHash(const boost::filesystem::path &path, enum MpqFile::Locale locale, enum MpqFile::Platform platform) const;
		//bool isHash(int32 nameHashA, int32 nameHashB, enum MpqFile::Locale locale, enum MpqFile::Platform platform) const;
		bool isHash(int32 nameHashA, int32 nameHashB, int16 locale, int16 platform) const;

		/**
		 * Updates both hash values to new file path \p path.
		 * \todo Might be protected and only be used by class \ref MpqFile. Use \ref MpqFile::move for changing file path.
		 * \sa Hash::filePathHashA, Hash::filePathHashB
		 */
		void changePath(const boost::filesystem::path &path);

		bool check() const;
		/**
		 * \return Returns true if the hash table entry has been deleted.
		 * \sa Hash::empty
		 */
		bool deleted() const;
		/**
		 * Hash table entries are empty if they do not link to any block table entry.
		 * Consider that a hash table entry can not be empty when it's been deleted.
		 * \return Returns true if the hash table entry has never been used.
		 * \sa Hash::deleted
		*/
		bool empty() const;
		
		class Mpq* mpq() const;
		class MpqFile* mpqFile() const;
		int32 filePathHashA() const;
		int32 filePathHashB() const;
		int16 locale() const;
		int16 platform() const;
		class Block* block() const;
		
		/**
		* Compares both hash values of both hashes.
		* @return Returns true if hash values A and B are equal, otherwise false is being returned.
		*/
		bool operator==(const Hash &hash) const;
		
	protected:
		friend class Mpq;
		friend class MpqFile;
		
		static const int32 blockIndexDeleted = 0xFFFFFFFE;
		static const int32 blockIndexEmpty = 0xFFFFFFFF;
		
		class Mpq *m_mpq;
		class MpqFile *m_mpqFile;
		int32 m_filePathHashA;
		int32 m_filePathHashB;
		int16 m_locale;
		int16 m_platform;
		class Block *m_block; // if this value is 0 it has never been used
		bool m_deleted; // can not be true if m_block is 0
};

inline bool Hash::check() const
{
	return true;
}

inline bool Hash::deleted() const
{
	return this->m_deleted;
}

inline bool Hash::empty() const
{
	return !this->m_deleted && this->m_block == 0;
}

inline class Mpq* Hash::mpq() const
{
	return this->m_mpq;
}

inline class MpqFile* Hash::mpqFile() const
{
	return this->m_mpqFile;
}

inline int32 Hash::filePathHashA() const
{
	return this->m_filePathHashA;
}

inline int32 Hash::filePathHashB() const
{
	return this->m_filePathHashB;
}

inline int16 Hash::locale() const
{
	return this->m_locale;
}

inline int16 Hash::platform() const
{
	return this->m_platform;
}
		
inline class Block* Hash::block() const
{
	return this->m_block;
}

inline bool Hash::operator==(const Hash &hash) const
{
	return this->isHash(hash.m_filePathHashA, hash.m_filePathHashB, hash.m_locale, hash.m_platform);
}

}

}

#endif
