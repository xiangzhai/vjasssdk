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

		void remove() = 0;
		std::iostream read() = 0;
		void rename(const std::string &newName) = 0;
		void extract(std::ofstream &fstream) = 0;
		/// @return Returns file size in bytes.
		unsigned int size() const = 0;
		/// @return Returns compressed file size in bytes.
		unsigned int compressedSize() const = 0;
		enum Locale locale() const = 0;
		std::string path() const = 0;
		std::string name() const = 0;
		bool isDir() const = 0;

	private:
		class Mpq *m_mpq;
};

}

}
