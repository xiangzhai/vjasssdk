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

#include <iostream> /// @todo debug
#include <openjpeg.h>

#include "blpjpeg.hpp"
#include "blp.hpp"

namespace wc3lib
{

namespace blp
{
	
/**
* @author PitzerMike
*/
int BlpJpeg::requiredMipMaps(int width, int height)
{
	int mips = 0;
	
	while (width > 0 && height > 0)
	{
		++mips;
		width = width / 2;
		height = height / 2;
	}

	return mips;
}
	
BlpJpeg::BlpJpeg(class Blp *blp) : m_blp(blp)
{
}

BlpJpeg::~BlpJpeg()
{
	if (this->m_jpegHeader != 0)
		delete this->m_jpegHeader;
}

dword BlpJpeg::read(std::istream &istream) throw (class Exception)
{
	dword bytes = 0;
	istream.read(reinterpret_cast<char*>(&this->m_jpegHeaderSize), sizeof(dword));
	bytes += istream.gcount();
	std::cout << "JPEG header size is " << this->m_jpegHeaderSize << std::endl;
	this->m_jpegHeader = new byte[this->m_jpegHeaderSize];

	for (int i = 0; i < this->m_jpegHeaderSize; ++i)
	{
		std::cout << "Reading header data " << i << std::endl;
		istream.read(reinterpret_cast<char*>(&this->m_jpegHeader[i]), sizeof(byte));
		bytes += fstream.gcount();
	}

	int maxMipMaps = BlpJpeg::requiredMipMaps(this->blp()->width(), this->blp()->height());
	std::cout << "Max mip maps " << maxMipMaps << std::endl;

	for (int i = 0; i < 16; ++i)
	{
		std::cout << "Reading mipmap data " << i << std::endl;
		if (bytes != this->blp()->header()->m_mipMapOffset[i])
		{
			std::cout << "Reading unnecessary offset" << std::endl;
			char *tmp = new char[this->blp()->header()->m_mipMapOffset[i]];
			istream.read(tmp, sizeof(tmp));
			bytes += istream.gcount();
			delete tmp;
		}
		
		this->m_mipMaps[i].m_jpegData = new byte[maxMipMaps];
		
		for (int j = 0; j < maxMipMaps; ++j)
		{
			istream.read(reinterpret_cast<char*>(&this->m_mipMaps[i].m_jpegData[j]), sizeof(this->m_mipMaps[i].m_jpegData[j])); /// @todo Reading 156 bytes instead of one.
			bytes += istream.gcount();
			std::cout << "Reading mip map jpeg data " << fstream.gcount() << std::endl;
		}
	}
	
	/// @todo Check out if each mip map is one version of texture and there are only those mip maps and what's the JPEG data for.
	/// @todo Header jpeg data should be written into a better header structure or image should be convertable to such a structure.
	opj_common_ptr
	
	OPJ_API opj_cio_t* OPJ_CALLCONV opj_cio_open(opj_common_ptr cinfo, unsigned char *buffer, int length);	
	opj_cio_close(opj_cio_t *cio);
int OPJ_CALLCONV cio_tell(opj_cio_t *cio);
void OPJ_CALLCONV cio_seek(opj_cio_t *cio, int pos);
			
	// jpeglib
	j_decompress_ptr jpegPointer;
	
	if (jpeg_read_header(jpegPointer, true) != JPEG_HEADER_OK)
		throw Exception(_("Wrong JPEG header."));
	
	jpeg_start_decompress JPP((j_decompress_ptr cinfo));
	
	EXTERN(JDIMENSION) jpeg_read_scanlines JPP((j_decompress_ptr cinfo,
					    JSAMPARRAY scanlines,
					    JDIMENSION max_lines));
EXTERN(boolean) jpeg_finish_decompress JPP((j_decompress_ptr cinfo));

/* Replaces jpeg_read_scanlines when reading raw downsampled data. */
EXTERN(JDIMENSION) jpeg_read_raw_data JPP((j_decompress_ptr cinfo,
					   JSAMPIMAGE data,
					   JDIMENSION max_lines));

/* Additional entry points for buffered-image mode. */
EXTERN(boolean) jpeg_has_multiple_scans JPP((j_decompress_ptr cinfo));
EXTERN(boolean) jpeg_start_output JPP((j_decompress_ptr cinfo,
				       int scan_number));
EXTERN(boolean) jpeg_finish_output JPP((j_decompress_ptr cinfo));
EXTERN(boolean) jpeg_input_complete JPP((j_decompress_ptr cinfo));
EXTERN(void) jpeg_new_colormap JPP((j_decompress_ptr cinfo));
EXTERN(int) jpeg_consume_input JPP((j_decompress_ptr cinfo));
/* Return value is one of: */
/* #define JPEG_SUSPENDED	0    Suspended due to lack of input data */
#define JPEG_REACHED_SOS	1 /* Reached start of new scan */
#define JPEG_REACHED_EOI	2 /* Reached end of image */
#define JPEG_ROW_COMPLETED	3 /* Completed one iMCU row */
#define JPEG_SCAN_COMPLETED	4 /* Completed last iMCU row of a scan */

/* Precalculate output dimensions for current decompression parameters. */
EXTERN(void) jpeg_calc_output_dimensions JPP((j_decompress_ptr cinfo));

/* Control saving of COM and APPn markers into marker_list. */
EXTERN(void) jpeg_save_markers
	JPP((j_decompress_ptr cinfo, int marker_code,
	     unsigned int length_limit));

/* Install a special processing method for COM or APPn markers. */
EXTERN(void) jpeg_set_marker_processor
	JPP((j_decompress_ptr cinfo, int marker_code,
	     jpeg_marker_parser_method routine));

/* Read or write raw DCT coefficients --- useful for lossless transcoding. */
EXTERN(jvirt_barray_ptr *) jpeg_read_coefficients JPP((j_decompress_ptr cinfo));
EXTERN(void) jpeg_write_coefficients JPP((j_compress_ptr cinfo,
					  jvirt_barray_ptr * coef_arrays));
EXTERN(void) jpeg_copy_critical_parameters JPP((j_decompress_ptr srcinfo,
						j_compress_ptr dstinfo));

/* If you choose to abort compression or decompression before completing
 * jpeg_finish_(de)compress, then you need to clean up to release memory,
 * temporary files, etc.  You can just call jpeg_destroy_(de)compress
 * if you're done with the JPEG object, but if you want to clean it up and
 * reuse it, call this:
 */
EXTERN(void) jpeg_abort_compress JPP((j_compress_ptr cinfo));
EXTERN(void) jpeg_abort_decompress JPP((j_decompress_ptr cinfo));
	
	return bytes;
}

dword BlpJpeg::write(std::ostream &ostream) throw (class Exception)
{
	dword bytes = 0;
	ostream.write(reinterpret_cast<const char*>(&this->m_jpegHeaderSize), sizeof(dword));
	bytes += sizeof(dword);

	for (int i = 0; i < this->m_jpegHeaderSize; ++i)
	{
		std::cout << "Writing header data " << i << std::endl;
		ostream.write(reinterpret_cast<const char*>(&this->m_jpegHeader[i]), sizeof(byte));
		bytes += sizeof(byte);
	}

	int maxMipMaps = BlpJpeg::requiredMipMaps(this->blp()->width(), this->blp()->height());
	
	for (int i = 0; i < 16; ++i)
	{
		std::cout << "Writing mip map data " << i << std::endl;
		
		for (int j = 0; j < maxMipMaps; ++j)
		{
			ostream.write(reinterpret_cast<const char*>(&this->m_mipMaps[i].m_jpegData[j]), sizeof(byte));
			bytes += sizeof(byte);
		}
	}
	
	return bytes;
}

}

}
