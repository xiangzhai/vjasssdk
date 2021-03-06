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

#ifndef WC3LIB_EDITOR_MPQARCHIVE_HPP
#define WC3LIB_EDITOR_MPQARCHIVE_HPP

#include <karchive.h>

namespace wc3lib
{

namespace mpq
{

class Mpq;

}

namespace editor
{

/**
* Implementation for Blizzard's MPQ format which should be usable in KDE applications as normal archive format (I/O slave).
* @todo Finish and make installable as KDE plugin (such like qblp).
*/
class MpqArchive : public KArchive
{
	public:
		MpqArchive(QIODevice *dev);
		MpqArchive(const QString &fileName);
		virtual ~MpqArchive();

	protected:
		/*
		virtual bool closeArchive();
		virtual bool doFinishWriting(qint64 size);
		virtual bool doPrepareWriting(const QString &name, const QString &user, const QString &group, qint64 size, mode_t perm, time_t atime, time_t mtime, time_t ctime);
		virtual bool doWriteDir(const QString &name, const QString &user, const QString &group, mode_t perm, time_t atime, time_t mtime, time_t ctime);
		virtual bool doWriteSymLink(const QString &name, const QString &target, const QString &user, const QString &group, mode_t perm, time_t atime, time_t mtime, time_t ctime);
		KArchiveDirectory* findOrCreate(const QString &path);
		virtual bool openArchive(QIODevice::OpenMode mode);
		virtual KArchiveDirectory* rootDir();
		void setDevice(QIODevice *dev);
		void setRootDir(KArchiveDirectory *rootDir);
		virtual void virtual_hook(int id, void *data);
		*/

		class mpq::Mpq *m_mpq;
};

}

}

#endif
