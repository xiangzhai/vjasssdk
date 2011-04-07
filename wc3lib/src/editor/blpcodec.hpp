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

#ifndef WC3LIB_EDITOR_BLPCODEC_HPP
#define WC3LIB_EDITOR_BLPCODEC_HPP

#include <Ogre.h>
#include <OgreImageCodec.h>

namespace wc3lib
{

namespace editor
{

/**
 * \todo Add BLP support.
 */
class BlpCodec : public Ogre::ImageCodec
{
	public:
		virtual ~BlpCodec();
		Ogre::String getDataType() const;
			// Returns the type of the data that supported by this codec as a String.
			
		virtual Ogre::DataStreamPtr code(Ogre::MemoryDataStreamPtr &input, CodecDataPtr &pData) const;
			//Codes the data in the input stream and saves the result in the output stream.
		virtual void codeToFile(Ogre::MemoryDataStreamPtr &input, const Ogre::String &outFileName, CodecDataPtr &pData) const;
			//Codes the data in the input chunk and saves the result in the output filename provided.
		virtual DecodeResult decode( Ogre::DataStreamPtr &input) const;
			// Codes the data from the input chunk into the output chunk.
		virtual Ogre::String getType () const;
			//Returns the type of the codec as a String.
		virtual bool magicNumberMatch(const char *magicNumberPtr, size_t maxbytes) const;
			// Returns whether a magic number header matches this codec.
		virtual Ogre::String magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const;
			// Maps a magic number header to a file extension, if this codec recognises it. 
};

}

}

#endif
