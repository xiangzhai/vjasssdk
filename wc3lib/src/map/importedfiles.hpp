/***************************************************************************
 *   Copyright (C) 2011 by Tamino Dauth                                    *
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

#ifndef WC3LIB_MAP_IMPORTEDFILES_HPP
#define WC3LIB_MAP_IMPORTEDFILES_HPP

#include <list>

#include "platform.hpp"

namespace wc3lib
{

namespace map
{

class ImportedFiles : public Format
{
	public:
		class Path : public Format
		{
			public:
				Path();

				virtual std::streamsize read(InputStream &istream) throw (class Exception);
				virtual std::streamsize write(OutputStream &ostream) const throw (class Exception);

				bool hasPrefix() const;
				const string& path() const;

			protected:
				bool m_hasPrefix; // 1byte: tells if the path is complete or needs "war3mapImported\" (5 or 8= standard path, 10 or 13: custom path) (thx PitzerMike)
				string m_path;
		};

		ImportedFiles();
		~ImportedFiles();

		virtual std::streamsize read(InputStream &istream) throw (class Exception);
		virtual std::streamsize write(OutputStream &ostream) const throw (class Exception);

		virtual int32 latestFileVersion() const;
		virtual const char8* fileName() const;

		virtual int32 version() const;
		std::list<class Path*>& paths();
		const std::list<class Path*>& paths() const;

		static const string mapPrefix;
		static const string campaignPrefix;

	protected:
		int32 m_version;
		std::list<class Path*> m_paths;
};

inline bool ImportedFiles::Path::hasPrefix() const
{
	return this->m_hasPrefix;
}

inline const string& ImportedFiles::Path::path() const
{
	return this->m_path;
}

inline int32 ImportedFiles::latestFileVersion() const
{
	return 1;
}

inline const char8* ImportedFiles::fileName() const
{
	return "war3map.imp";
}

inline int32 ImportedFiles::version() const
{
	return this->m_version;
}

inline std::list<class ImportedFiles::Path*>& ImportedFiles::paths()
{
	return this->m_paths;
}

inline const std::list<class ImportedFiles::Path*>& ImportedFiles::paths() const
{
	return this->m_paths;
}

}

}

#endif
