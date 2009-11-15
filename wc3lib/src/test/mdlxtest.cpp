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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <list>
#include <fstream>

 #include <getopt.h>

#include "../internationalisation.hpp"
#include "../blp/blp.hpp"
#include "../blp/platform.hpp"
#include "../mdlx/mdlx.hpp"
#include "../mdlx/platform.hpp"

using namespace wc3lib;

static const char *version = "0.1";

enum Format
{
	Blp,
#ifdef JPEG
	Jpeg,
#endif
#ifdef TGA
	Tga,
#endif
#ifdef PNG
	Png,
#endif
	Mdl,
	Mdx,
#ifdef BLEND
	Blend,
#endif
#ifdef MAX
	Max,
#endif
	MaxFormats,
	InvalidFormat
};

static const char *formatExpression[MaxFormats] =
{
	"blp",
#ifdef JPEG
	"jpeg",
#endif
#ifdef TGA
	"tga",
#endif
#ifdef PNG
	"png",
#endif
	"mdl",
	"mdx",
#ifdef BLEND
	"blend"
#endif
#ifdef MAX
	, "3ds"
#endif
};

static const char *formatExtension[MaxFormats] =
{
	"blp",
#ifdef JPEG
	"jpeg",
#endif
#ifdef TGA
	"tga",
#endif
#ifdef PNG
	"png",
#endif
	"mdl",
	"mdx",
#ifdef BLEND
	"blend"
#endif
#ifdef MAX	
	, "3ds"
#endif
};	

static const bool formatIsBinary[MaxFormats] =
{
	true,
#ifdef JPEG
	true,
#endif
#ifdef TGA
	true,
#endif
#ifdef PNG
	true,
#endif
	false,
	true,
#ifdef BLEND
	true,	
#endif
#ifdef MAX
	true
#endif
};

static const bool formatConvertibility[MaxFormats][MaxFormats] =
{
	// blp   jpeg   tga    png    mdl    mdx,   blend  3ds
	{
		true,
#ifdef JPEG
		true,
#endif
#ifdef TGA
		true,
#endif
#ifdef PNG
		true,
#endif
		false,
		false,
#ifdef BLEND
		false
#endif
#ifdef MAX
		,false
#endif
	},
#ifdef JPEG
	{
		true,
#ifdef JPEG
		true,
#endif
#ifdef TGA
		true,
#endif
#ifdef PNG
		true,
#endif
		false,
		false,
#ifdef BLEND
		false
#endif
#ifdef MAX
		,false
#endif
	},
#endif
#ifdef TGA
	{
		true,
#ifdef JPEG
		true,
#endif
#ifdef TGA
		true,
#endif
#ifdef PNG
		true,
#endif
		false,
		false,
#ifdef BLEND
		false
#endif
#ifdef MAX
		,false
#endif
	},
#endif
#ifdef PNG
	{
		true,
#ifdef JPEG
		true,
#endif
#ifdef TGA
		true,
#endif
#ifdef PNG
		true,
#endif
		false,
		false,
#ifdef BLEND
		false
#endif
#ifdef MAX
		,false
#endif
	},
#endif
	// mdl
	{
		false,
#ifdef JPEG
		false,
#endif
#ifdef TGA
		false,
#endif
#ifdef PNG
		false,
#endif
		true,
		true,
#ifdef BLEND
		true
#endif
#ifdef MAX
		,true
#endif
	},
	// mdx
	{
		false,
#ifdef JPEG
		false,
#endif
#ifdef TGA
		false,
#endif
#ifdef PNG
		false,
#endif
		true,
		true,
#ifdef BLEND
		true
#endif
#ifdef MAX
		,true
#endif
	},
#ifdef BLEND
	{
		false,
#ifdef JPEG
		false,
#endif
#ifdef TGA
		false,
#endif
#ifdef PNG
		false,
#endif
		true,
		true,
#ifdef BLEND
		true
#endif
#ifdef MAX
		,true
#endif
	}
#endif
#ifdef MAX
	,{
		false,
#ifdef JPEG
		false,
#endif
#ifdef TGA
		false,
#endif
#ifdef PNG
		false,
#endif
		true,
		true,
#ifdef BLEND
		true
#endif
#ifdef MAX
		,true
#endif
	}
#endif
};

static inline std::string getFormatExpression(enum Format format)
{
	return formatExpression[format];
}

static inline enum Format getFormatByExpression(const char *formatString)
{
	for (int i = 0; i < MaxFormats; ++i)
	{
		if (strcmp(formatString, formatExpression[i]) == 0)
			return Format(i);
	}
	
	return InvalidFormat;
}

static inline std::string getFormatExtension(enum Format format)
{
	return formatExtension[format];
}

static inline bool isFormatBinary(enum Format format)
{
	return formatIsBinary[format];
}

static inline bool checkFormatConvertibility(enum Format format0, enum Format format1)
{
	return formatConvertibility[format0][format1];
}

int main(int argc, char *argv[])
{
	// Set the current locale.
	setlocale(LC_ALL, "");
	// Set the text message domain.
	bindtextdomain("wc3lib", LOCALE_DIR);
	textdomain("wc3lib");
	
	static struct option options[] =
	{
		{"version",  no_argument,             0, 'v'},
		{"help",     no_argument,             0, 'h'},
		{"iformat",  required_argument,       0, 'i'},
		{"oformat",  required_argument,       0, 'o'},
		{0, 0, 0, 0}
	};
	
	enum Format optionIformat = InvalidFormat;
	enum Format optionOformat = InvalidFormat;
	std::list<std::string> optionFiles;
	int optionShortcut;
	
	while (true)
	{
		int optionIndex = 0;
		optionShortcut = getopt_long(argc, argv, "vhi:o:", options, &optionIndex);

		if (optionShortcut == -1)
			break;
     
		switch (optionShortcut)
		{
			case 'v':
			{
				printf("mdlxtest %s.\n", version);
				std::cout << _(
				"Copyright © 2009 Tamino Dauth\n"
				"License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law."
				) << std::endl;
				
				break;
			}
			
			case 'h':
			{
				std::cout <<
				_("mdlxtest\n") <<
				_("\nUsage:\n") <<
				_("\tmdlxtest [-io] <input files>\n") <<
				_("\nOptions:\n") <<
				_("\t-v, --version             Shows the current version of mdlxtest.\n") <<
				_("\t-h, --help                Shows this text.\n") <<
				_("\t-i, --iformat <arg>       <arg> has to be replaced by input files format.\n") <<
				_("\t-o, --oformat <arg>       <arg> has to be replaced by output files format.\n") <<
				_("\nReport bugs to tamino@cdauth.de or on http://sourceforge.net/projects/vjasssdk/") <<
				std::endl;
				
				break;
			}
			
			case 'i':
			{
				if (getFormatByExpression(optarg) == InvalidFormat)
				{
					fprintf(stderr, _("Invalid format \"%s\".\n"), optarg);
					
					return EXIT_FAILURE;
				}
				
				optionIformat = getFormatByExpression(optarg);
				
				break;
			}
			
			case 'o':
			{
				if (getFormatByExpression(optarg) == InvalidFormat)
				{
					fprintf(stderr, _("Invalid format \"%s\".\n"), optarg);
					
					return EXIT_FAILURE;
				}
				
				optionOformat = getFormatByExpression(optarg);
				
				break;
			}
		}
	}
	
	if (optind < argc)
	{
		while (optind < argc)
			optionFiles.push_back(argv[optind++]);

	}
	else
	{
		std::cerr << _("Missing file arguments.") << std::endl;
		
		return EXIT_FAILURE;
	}
	
	if (optionIformat == InvalidFormat)
	{
		std::cerr << _("Input format was not defined.") << std::endl;
		
		return EXIT_FAILURE;
	}
	
	if (optionOformat == InvalidFormat)
	{
		std::cerr << _("Output format was not defined.") << std::endl;
		
		return EXIT_FAILURE;
	}
	
	if (!checkFormatConvertibility(optionIformat, optionOformat))
	{
		fprintf(stderr, _("Format \"%s\" can not be converted into format \"%s\"."), getFormatExpression(optionIformat).c_str(), getFormatExpression(optionOformat).c_str());
		
		return EXIT_FAILURE;
	}
	
	if (optionFiles.empty())
	{
		std::cerr << _("No input files were chosen.") << std::endl;
		
		return EXIT_FAILURE;
	}
	

	for (std::list<std::string>::iterator iterator = optionFiles.begin(); iterator != optionFiles.end(); ++iterator)
	{
		std::cout << "Loop" << std::endl;
		
		std::ios_base::openmode openMode = std::ifstream::in;
		
		if (isFormatBinary(optionIformat))
			openMode |= std::ifstream::binary;
		
		std::ifstream ifstream((*iterator).c_str(), openMode);
		
		if (!ifstream)
		{
			fprintf(stderr, _("Error while opening file \"%s\". Continuing with next one.\n"), (*iterator).c_str());
			
			continue;
		}
		
		class mdlx::Mdlx mdlx;
		class blp::Blp blp;
		
		try
		{
			switch (optionIformat)
			{
				case Blp:
				{
					blp::dword bytes = blp.readBlp(ifstream);
					printf(_("Read BLP file successfully. %d Bytes.\n"), bytes);
				
					break;
				}		
#ifdef JPEG
				case Jpeg:
				{
					blp::dword bytes = blp.readJpeg(ifstream);
					printf(_("Read JPEG file successfully. %d Bytes.\n"), bytes);
				
					break;
				}
#endif
#ifdef TGA
				case Tga:
				{
					blp::dword bytes = blp.readTga(ifstream);
					printf(_("Read TGA file successfully. %d Bytes.\n"), bytes);
				
					break;
				}
#endif
#ifdef PNG
				case Png:
				{
					blp::dword bytes = blp.readPng(ifstream);
					printf(_("Read PNG file successfully. %d Bytes.\n"), bytes);
				
					break;
				}
#endif
				case Mdl:
					mdlx.readMdl(ifstream);
					std::cout << _("Read MDL file successfully.") << std::endl;
					
					break;
				
				case Mdx:
				{
					mdlx::long32 bytes = mdlx.readMdx(ifstream);
					printf(_("Read MDX file successfully. %d Bytes.\n"), bytes);
					
					break;
				}
#ifdef BLEND					
				case Blend:
				{
					mdlx::long32 bytes = mdlx.readBlend(ifstream);
					printf(_("Read Blender file successfully. %d Bytes.\n"), bytes);
					
					break;
				}
#endif
#ifdef MAX					
				case Max:
				{
					mdlx::long32 bytes = mdlx.readMax(ifstream);
					printf(_("Read 3ds Max file successfully. %d Bytes.\n"), bytes);
					
					break;
				}
#endif			
			}			
		}
		catch (class Exception &exception)
		{
			fprintf(stderr, _("Error while reading file \"%s\":\n \"%s\"\n"), (*iterator).c_str(), exception.what());
			std::cerr << _("Skiping file.") << std::endl;
			
			continue;
		}
		
		ifstream.close();
		
		openMode = std::ofstream::out;
		
		if (isFormatBinary(optionOformat))
			openMode |= std::ofstream::binary;
		
		std::string extension = getFormatExtension(optionOformat);
		std::string filePath = *iterator;
		std::string::size_type index = filePath.find_last_of('.');
		
		// remove old extension
		if (index != std::string::npos && filePath.substr(index).length() <= 3)
			filePath.erase(index);
		
		filePath += extension;
		std::ofstream ofstream(filePath.c_str(), openMode);
		
		if (!ofstream)
		{
			fprintf(stderr, _("Error while opening file \"%s\". Continuing with next one.\n"), filePath.c_str());
			
			continue;
		}
		
		try
		{
			
			switch (optionOformat)
			{
				case Blp:
				{
					blp::dword bytes = blp.writeBlp(ofstream);
					printf(_("Wrote BLP file successfully. %d Bytes.\n"), bytes);
				
					break;
				}
#ifdef JPEG					
				case Jpeg:
				{
					blp::dword bytes = blp.writeJpeg(ofstream);
					printf(_("Wrote JPEG file successfully. %d Bytes.\n"), bytes);
				
					break;
				}
#endif
#ifdef TGA					
				case Tga:
				{
					blp::dword bytes = blp.writeTga(ofstream);
					printf(_("Wrote TGA file successfully. %d Bytes.\n"), bytes);
				
					break;
				}
#endif
#ifdef PNG					
				case Png:
				{
					blp::dword bytes = blp.writePng(ofstream);
					printf(_("Wrote PNG file successfully. %d Bytes.\n"), bytes);
				
					break;
				}
#endif				
				case Mdl:
					mdlx.writeMdl(ofstream);
					std::cout << _("Wrote MDL file successfully.") << std::endl;
					
					break;
				
				case Mdx:
					mdlx::long32 bytes = mdlx.writeMdx(ofstream);
					printf(_("Wrote MDX file successfully. %d Bytes.\n"), bytes);
				
					break;
#ifdef BLEND					
				case Blend:
				{
					mdlx::long32 bytes = mdlx.writeBlend(ofstream);
					printf(_("Wrote Blender file successfully. %d Bytes.\n"), bytes);
				
					break;
				}
#endif
#ifdef MAX					
				case Max:
				{
					mdlx::long32 bytes = mdlx.writeMax(ofstream);
					printf(_("Wrote 3ds Max file successfully. %d Bytes.\n"), bytes);
				
					break;
				}
#endif
			}
		}
		catch (class Exception &exception)
		{
			fprintf(stderr, _("Error while writing file \"%s\":\n \"%s\"\n"), filePath.c_str(), exception.what());
			std::cerr << _("Skiping file.") << std::endl;
			
			continue;
		}
		
		ofstream.close();
	}

	return EXIT_SUCCESS;
}
