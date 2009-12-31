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

#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/thread.hpp>

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
	W3e,
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
	"blend",
#endif
#ifdef MAX
	"3ds",
#endif
	"w3e"
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
	"blend",
#endif
#ifdef MAX	
	"3ds",
#endif
	"w3e"
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
	true,
#endif
	true // w3e
};

static const bool formatConvertibility[MaxFormats][MaxFormats] =
{
	// blp   jpeg   tga    png    mdl    mdx,   blend,  3ds, w3e
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
		false,
#endif
#ifdef MAX
		false,
#endif
		false
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
		false,
#endif
#ifdef MAX
		false
#endif
		false
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
		false,
#endif
#ifdef MAX
		false,
#endif
		false
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
		false,
#endif
#ifdef MAX
		false,
#endif
		false
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
		true,
#endif
#ifdef MAX
		true,
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
		true,
#endif
#ifdef MAX
		true,
#endif
		false
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
		true,
#endif
#ifdef MAX
		true,
#endif
		false
	},
#endif
#ifdef MAX
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
		true,
#endif
#ifdef MAX
		true,
#endif
		false
	},
#endif
// w3e
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
		false,
		false,
#ifdef BLEND
		false,
#endif
#ifdef MAX
		false,
#endif
		true
	}
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

static inline std::string formatBytes(unsigned int bytes)
{
	if (bytes % 1024)
		return boost::str(boost::format(_("%1% KiBytes")) % double(bytes / 1024));

	return boost::str(boost::format(_("%1% bytes")) % bytes);
}

static inline bool expectInput()
{
	std::string input;
	std::cin >> input;
			
	while (input != _("y") && input != _("n")) 
	{
		std::cout << _("Wrong input. Expecting new input (y/n):") << std::endl;
		std::cin >> input;
	}
			
	if (input == _("y"))
		return true;
	else
		return false;
}

static bool addFilePath(const boost::filesystem::path &path, std::list<boost::filesystem::path> &filePaths, bool recursive, std::string extension)
{
	if (!boost::filesystem::exists(path))
	{
		std::cerr << boost::format(_("Error, path \"%1%\" does not exist.\nContinue process (y/n)?")) % path.string() << std::endl;
		
		return expectInput();
	}
	
	if (boost::filesystem::is_directory(path))
	{
		if (recursive)
		{
			boost::filesystem::directory_iterator endIterator;
			
			for (boost::filesystem::directory_iterator iterator(path); iterator != endIterator; ++iterator)
				addFilePath(iterator->path(), filePaths, recursive, extension);
		}
		else
		{
			std::cerr << boost::format(_("Error, path \"%1%\" seems to be a directory and option -R is not used.\nContinue process (ignore file, y/n)?")) % path.string() << std::endl;
			
			return expectInput();
		}
	}
	else if (extension != "*")
	{
		std::string::size_type index = path.string().find_last_of(extension);
		
		if (index == std::string::npos)	
			return false;
	}
	
	filePaths.push_back(path);
	
	return true;
}

static void convertFile(const boost::filesystem::path &path, const boost::filesystem::path &dirPath, enum Format inputFormat, enum Format outputFormat, bool verbose, bool readonly) throw (boost::thread_interrupted)
{
	std::ios_base::openmode openMode = std::ifstream::in;
		
	if (isFormatBinary(inputFormat))
		openMode |= std::ifstream::binary;
	
	if (verbose)
		std::cout << boost::format(_("Reading file \"%1%\".")) % path.string() << std::endl;
	
	std::ifstream ifstream(path.string().c_str(), openMode);
	
	if (!ifstream)
	{
		std::cerr << boost::format(_("Error while opening file \"%1%\". Continuing with next one.")) % path.string() << std::endl;
		
		throw boost::thread_interrupted();
	}
	
	class mdlx::Mdlx mdlx;
	class blp::Blp blp;
	
	try
	{
		switch (inputFormat)
		{
			case Blp:
			{
				blp::dword bytes = blp.readBlp(ifstream);
				std::cout << boost::format(_("Read BLP file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
				break;
			}		
#ifdef JPEG
			case Jpeg:
			{
				blp::dword bytes = blp.readJpeg(ifstream);
				std::cout << boost::format(_("Read JPEG file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
				break;
			}
#endif
#ifdef TGA
			case Tga:
			{
				blp::dword bytes = blp.readTga(ifstream);
				std::cout << boost::format(_("Read TGA file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
				break;
			}
#endif
#ifdef PNG
			case Png:
			{
				blp::dword bytes = blp.readPng(ifstream);
				std::cout << boost::format(_("Read PNG file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
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
				std::cout << boost::format(_("Read MDX file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
				
				break;
			}
#ifdef BLEND					
			case Blend:
			{
				mdlx::long32 bytes = mdlx.readBlend(ifstream);
				std::cout << boost::format(_("Read Blender file successfully. %1%.\n")) % formatBytes(bytes) << std::endl; bytes);
				
				break;
			}
#endif
#ifdef MAX					
			case Max:
			{
				mdlx::long32 bytes = mdlx.readMax(ifstream);
				std::cout << boost::format(_("Read 3ds Max file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
				
				break;
			}
#endif			
		}			
	}
	catch (class Exception &exception)
	{
		std::cerr << boost::format(_("Error while reading file \"%1%\":\n\"%2%\"")) % path.string() % exception.what() << std::endl;
		std::cerr << _("Skiping file.") << std::endl;
		
		throw boost::thread_interrupted();
	}
	
	ifstream.close();
	
	if (readonly)
		return;
	
	openMode = std::ofstream::out;
	
	if (isFormatBinary(outputFormat))
		openMode |= std::ofstream::binary;
	
	boost::filesystem::path filePath = path.string();

	if (!dirPath.string().empty())
		filePath = dirPath / filePath.filename();

	filePath.replace_extension(getFormatExtension(outputFormat));
	boost::filesystem::ofstream ofstream(filePath, openMode);
	
	if (!ofstream)
	{
		std::cerr << boost::format(_("Error while opening file \"%1%\". Continuing with next one.")) % filePath << std::endl;
		
		throw boost::thread_interrupted();
	}
	
	try
	{
		
		switch (outputFormat)
		{
			case Blp:
			{
				blp::dword bytes = blp.writeBlp(ofstream);
				std::cout << boost::format(_("Wrote BLP file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
				break;
			}
#ifdef JPEG					
			case Jpeg:
			{
				blp::dword bytes = blp.writeJpeg(ofstream);
				std::cout << boost::format(_("Wrote JPEG file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
				break;
			}
#endif
#ifdef TGA					
			case Tga:
			{
				blp::dword bytes = blp.writeTga(ofstream);
				std::cout << boost::format(_("Wrote TGA file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
				break;
			}
#endif
#ifdef PNG					
			case Png:
			{
				blp::dword bytes = blp.writePng(ofstream);
				std::cout << boost::format(_("Wrote PNG file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
				break;
			}
#endif				
			case Mdl:
				mdlx.writeMdl(ofstream);
				std::cout << _("Wrote MDL file successfully.") << std::endl;
				
				break;
			
			case Mdx:
				mdlx::long32 bytes = mdlx.writeMdx(ofstream);
				std::cout << boost::format(_("Wrote MDX file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
				break;
#ifdef BLEND					
			case Blend:
			{
				mdlx::long32 bytes = mdlx.writeBlend(ofstream);
				std::cout << boost::format(_("Wrote Blender file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
				break;
			}
#endif
#ifdef MAX					
			case Max:
			{
				mdlx::long32 bytes = mdlx.writeMax(ofstream);
				std::cout << boost::format(_("Wrote 3ds Max file successfully. %1%.\n")) % formatBytes(bytes) << std::endl;
			
				break;
			}
#endif
		}
	}
	catch (class Exception &exception)
	{
		std::cerr << boost::format(_("Error while writing file \"%1%\":\n\"%1%\"")) % filePath % exception.what() << std::endl;
		std::cerr << _("Skiping file.") << std::endl;
		
		throw boost::thread_interrupted();
	}
	
	ofstream.close();
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
		{"version",     no_argument,             0, 'v'},
		{"help",        no_argument,             0, 'h'},
		{"iformat",     required_argument,       0, 'i'},
		{"oformat",     required_argument,       0, 'o'},
		{"dir",         required_argument,       0, 'D'},
		{"recursive",   no_argument,             0, 'R'},
		{"verbose",     no_argument,             0, 'V'}, /// @todo Probably reserved for --version, too
		{"readonly",    no_argument,             0, 'O'},
		{0, 0, 0, 0}
	};
	
	enum Format optionIformat = InvalidFormat;
	enum Format optionOformat = InvalidFormat;
	boost::filesystem::path optionDir;
	bool optionRecursive = false;
	bool optionVerbose = false;
	bool optionReadonly = false;
	std::list<boost::filesystem::path> optionFiles;
	int optionShortcut;
	
	while (true)
	{
		int optionIndex = 0;
		optionShortcut = getopt_long(argc, argv, "vhi:o:D:RVO", options, &optionIndex);

		if (optionShortcut == -1)
			break;
     
		switch (optionShortcut)
		{
			case 'v':
			{
				std::cout <<
				boost::format(_("converter %1%.")) % version
				<< std::endl <<
				_(
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
				_("converter\n") <<
				_("\nUsage:\n") <<
				_("\tconverter [-vhioRV] <input files/directories>\n") <<
				_("\nOptions:\n") <<
				_("\t-v, --version             Shows the current version of mdlxtest.\n") <<
				_("\t-h, --help                Shows this text.\n") <<
				_("\t-i, --iformat <arg>       <arg> has to be replaced by input files format.\n") <<
				_("\t-o, --oformat <arg>       <arg> has to be replaced by output files format.\n") <<
				_("\t-D  --dir <arg>           <arg> has to be replaced by output directory.\n") <<
				_("\t-R, --recursive           If some of the input files are directories they will be iterated recursively and searched for other files with the input format extension.\n") <<
				_("\t-V  --verbose             Enables verbose mode and shows more detailed output information.\n") <<
				_("\t-O  --readonly            Enables read-only mode. Output format is not necessary in read-only mode. This mode can be useful if you just want to try some format reading.\n") <<
				_("\nReport bugs to tamino@cdauth.de or on http://sourceforge.net/projects/vjasssdk/") <<
				std::endl;
				
				break;
			}
			
			case 'i':
			{
				if (getFormatByExpression(optarg) == InvalidFormat)
				{
					std::cerr << boost::format(_("Invalid format \"%1%\".")) % optarg << std::endl;
					
					return EXIT_FAILURE;
				}
				
				optionIformat = getFormatByExpression(optarg);
				
				break;
			}
			
			case 'o':
			{
				if (getFormatByExpression(optarg) == InvalidFormat)
				{
					std::cerr << boost::format(_("Invalid format \"%1%\".")) % optarg << std::endl;
					
					return EXIT_FAILURE;
				}
				
				optionOformat = getFormatByExpression(optarg);
				
				break;
			}

			case 'D':
			{
				if (!boost::filesystem::is_directory(optarg))
				{
					std::cerr << boost::format(_("Invalid directory \"%1%\".")) % optarg << std::endl;

					return EXIT_FAILURE;
				}

				optionDir = optarg;

				break;
			}
			
			case 'R':
			{
				optionRecursive = true;
				
				break;
			}
			
			case 'V':
			{
				optionVerbose = true;
				
				break;
			}
			
			case 'O':
			{
				optionReadonly = true;
				
				break;
			}
		}
	}
	
	if (optind < argc)
	{
		while (optind < argc)
		{
			boost::filesystem::path path = argv[optind++];
			
			if (!addFilePath(path, optionFiles, optionRecursive, getFormatExtension(optionIformat)))
			{
				std::cerr << _("Canceled process.") << std::endl;
				
				return EXIT_FAILURE;
			}
		}

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
	
	if (optionOformat == InvalidFormat && !optionReadonly)
	{
		std::cerr << _("Output format was not defined.") << std::endl;
		
		return EXIT_FAILURE;
	}
	
	if (!optionReadonly && !checkFormatConvertibility(optionIformat, optionOformat))
	{
		std::cerr << boost::format(_("Format \"%1%\" can not be converted into format \"%2%\".")) % getFormatExpression(optionIformat) % getFormatExpression(optionOformat) << std::endl;
		
		return EXIT_FAILURE;
	}
	
	if (optionFiles.empty())
	{
		std::cerr << _("No input files were chosen.") << std::endl;
		
		return EXIT_FAILURE;
	}
	

	/// @todo If it's multithreaded make sure that there aren't doubled file paths
	BOOST_FOREACH(boost::filesystem::path path, optionFiles)
	{
		std::cout << "Loop" << std::endl;
		
		try
		{
			boost::thread thread = boost::thread(convertFile, path, optionDir, optionIformat, optionOformat, optionVerbose, optionReadonly);
		}
		catch (boost::thread_interrupted &threadInterrupted)
		{
			
		}
		catch (...)
		{
			std::cerr << _("Unknown exception. Canceling.") << std::endl;
			
			break;
		}
		
	}

	return EXIT_SUCCESS;
}
