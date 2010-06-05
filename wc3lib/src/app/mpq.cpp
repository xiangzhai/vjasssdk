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

#include <iostream>

#include "../mpq.hpp"
#include "../internationalisation.hpp"
#include "../exception.hpp"
#include "../utilities.hpp"

#include <getopt.h>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <boost/timer.hpp>
#ifdef DEBUG
//#include <StormLib/stormlib/StormLib.h>
#endif

using namespace wc3lib;
using namespace wc3lib::mpq;

template<typename T>
static std::string sizeString(T size, bool humanReadable, bool decimal)
{
	if (humanReadable)
	{
		if (decimal)
			return sizeStringDecimal<T>(size);
		else
			return sizeStringBinary<T>(size);
	}
	
	std::stringstream result;
	result << size;
	
	return result.str();
}

int main(int argc, char *argv[])
{
	// Set the current locale.
	setlocale(LC_ALL, "");
	// Set the text message domain.
	bindtextdomain("mpq", LOCALE_DIR);
	textdomain("mpq");
	
	static struct option options[] =
	{
		{"version",                 no_argument,             0, 'V'},
		{"help",                    no_argument,             0, 'H'},
		{"info",		    no_argument,             0, 'i'},
		{"human-readable",          no_argument,             0, 'h'},
		{"decimal",                 no_argument,             0, 'd'},
		{"list",                    no_argument,             0, 'l'},
		{"benchmark",               no_argument,             0,  'b'},
		{0, 0, 0, 0}
	};
	
	static const char *version = "0.1";
	std::list<boost::filesystem::path> filePaths;
	int optionShortcut;
	bool optionInfo = false;
	bool optionHumanreadable = false;
	bool optionDecimal = false;
	bool optionList = false;
	bool optionBenchmark = false;
	
	while (true)
	{
		int optionIndex = 0;
		optionShortcut = getopt_long(argc, argv, "VHihdlb", options, &optionIndex);

		if (optionShortcut == -1)
			break;
     
		switch (optionShortcut)
		{
			case 'V':
			{
				std::cout << boost::format(_(
				"mpq %1%.\n"
				"Copyright © 2010 Tamino Dauth\n"
				"License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law."
				)) % version << std::endl;
				
				return EXIT_SUCCESS;
			}
			
			case 'H':
			{
				std::cout <<
				boost::format(_("mpq %1%.\n\n")) % version <<
				_("Usage: mpq [Options] [MPQ files]\n\n") <<
				_("Options:\n") <<
				_("\t-V, --version               Shows the current version of vjassdoc.\n") <<
				_("\t-h, --help                  Shows this text.\n") <<
				_("\t-i, --info                  Shows some basic information about all read MPQ archives.\n") <<
				_("\t-h, --human-readable        Shows output sizes in an human-readable format.\n") <<
				_("\t-d, --decimal               Shows decimal sizes (factor 1000 not 1024)\n") <<
				_("\t-l, --list                  Lists all contained files of all read MPQ archives (requires --info).\n") <<
				_("\t-b, --benchmark             Compares various functionalities of wc3lib and StormLib (requires --info).\n") <<
				std::endl <<
				_("Several arguments has to be separated by using the : character.\n") <<
				_("\nReport bugs to tamino@cdauth.de or on http://sourceforge.net/projects/vjasssdk/") <<
				std::endl;
	
				return EXIT_SUCCESS;
			}
			
			case 'i':
			{
				optionInfo = true;
				
				break;
			}
			
			case 'h':
			{
				optionHumanreadable = true;
				
				break;
			}
			
			case 'd':
			{
				optionDecimal = true;
				
				break;
			}
			
			case 'l':
			{
				optionList = true;
				
				break;
			}

			case 'b':
			{
				optionBenchmark = true;

				break;
			}
		}
	}
		
	if (optind < argc)
	{
		for ( ; optind < argc; ++optind)
		{
			bool found = false;
			
			BOOST_FOREACH(const boost::filesystem::path &path, filePaths)
			{
				if (path == argv[optind])
				{
						std::cerr << boost::format(_("File path \"%1%\" has already been added to list.")) % argv[optind] << std::endl;
						found = true;
						
						break;
				}
			}
			
			if (!found)
				filePaths.push_back(argv[optind]);
		}

	}
	else
	{
		std::cerr << _("Missing file arguments.") << std::endl;
		
		return EXIT_FAILURE;
	}
	
	if (optionInfo)
	{
		BOOST_FOREACH(const boost::filesystem::path &path, filePaths)
		{
			if (!boost::filesystem::is_regular_file(path))
			{
				std::cerr << boost::format(_("File \"%1%\" does not seem to be a regular file and will be skipped.")) % path.string() << std::endl;

				continue;
			}

			class Mpq mpq;
			
			try
			{
				std::streamsize size = mpq.open(path);
			}
			catch (class wc3lib::Exception &exception)
			{
				std::cerr << boost::format(_("Error occured while opening file \"%1%\": \"%2%\"")) % path.string() % exception.what() << std::endl;
				
				continue;
			}
			
			std::cout
			<< mpq.path().string() << std::endl
			<< boost::format(_("Size: %1%")) % sizeString<std::size_t>(mpq.size(), optionHumanreadable, optionDecimal) << std::endl
			<< boost::format(_("Hashes: %1%")) % mpq.hashes().size() << std::endl
			<< boost::format(_("Blocks: %1%")) % mpq.blocks().size() << std::endl
			<< boost::format(_("Files: %1%")) % mpq.files().size() << std::endl
			<< boost::format(_("Format: %1%")) % mpq.format() << std::endl
			<< boost::format(_("Used space: %1%")) % sizeString<int64>(mpq.usedSpace(), optionHumanreadable, optionDecimal) << std::endl
			<< boost::format(_("Unused space: %1%")) % sizeString<int64>(mpq.unusedSpace(), optionHumanreadable, optionDecimal) << std::endl
			<< boost::format(_("Sector size: %1%")) % sizeString<int16>(mpq.sectorSize(), optionHumanreadable, optionDecimal) << std::endl
			<< boost::format(_("Entire block size: %1%")) % sizeString<int64>(mpq.entireBlockSize(), optionHumanreadable, optionDecimal) << std::endl
			<< boost::format(_("Entire file size: %1%")) % sizeString<int64>(mpq.entireFileSize(), optionHumanreadable, optionDecimal) << std::endl
			<< boost::format(_("Has strong digital signature: %1%")) % boolString(mpq.hasStrongDigitalSignature()) << std::endl
			<< boost::format(_("Contains listfile file: %1%")) % boolString(mpq.containsListfileFile()) << std::endl
			<< boost::format(_("Contains attributes file: %1%")) % boolString(mpq.containsAttributesFile()) << std::endl
			<< boost::format(_("Contains signature file: %1%")) % boolString(mpq.containsSignatureFile()) << std::endl
			;
				
			if (mpq.extendedAttributes() != Mpq::None)
			{
				std::cout << _("Extended attributes:") << std::endl;
				
				if (mpq.extendedAttributes() & Mpq::FileCrc32s)
					std::cout << _("* File CRC32s") << std::endl;
				
				if (mpq.extendedAttributes() & Mpq::FileTimeStamps)
					std::cout << _("* File time stamps") << std::endl;
				
				if (mpq.extendedAttributes() & Mpq::FileMd5s)
					std::cout << _("* File MD5s") << std::endl;
			}

			/// @todo TEST
			const MpqFile *mpqFile = const_cast<const Mpq*>(&mpq)->findFile("Detector.js", MpqFile::Neutral, MpqFile::Default);

			if (mpqFile == 0)
				std::cerr << _("Error while searching for file.") << std::endl;
			else
				std::cout << "Flags: " << std::hex << mpqFile->hash()->block()->flags() << std::dec << "\nFile data: " << *mpqFile << std::endl;

			std::size_t invalidFiles = 0;
			enum Block::Flags flags = Block::None;

			BOOST_FOREACH(const class MpqFile *mpqFile, mpq.files())
			{
				if (mpqFile->sectors().empty())
				{
					++invalidFiles;

					if (flags == Block::None || flags != mpqFile->block()->flags())
					{
						std::cout << "New flag." << std::endl;
						flags = mpqFile->block()->flags();
					}
				}
			}

			std::cout << "Invalid files: " << invalidFiles << " with flags " << std::hex << flags << std::dec << std::endl;

			if (optionList)
			{
				std::cout << _("Listing contained files:") << std::endl;

				std::size_t i = 1;

				BOOST_FOREACH(const class MpqFile *mpqFile, mpq.files())
				{
					std::cout << boost::format(_("* File %1%: (Size: %2%) - %3%")) % i % sizeString<std::size_t>(mpqFile->size(), optionHumanreadable, optionDecimal) % mpqFile->path() << std::endl;
					++i;
				}

				std::cout << boost::format(_("All in all %1% files (Size: %2%)")) % (i + 1) % sizeString<int64>(mpq.entireFileSize(), optionHumanreadable, optionDecimal) << std::endl;
			}

			if (optionBenchmark)
			{
				mpq.close();

				std::cout << _("Detected debug mode.\nStarting benchmark (wc3lib vs. StormLib)") << std::endl;
				std::cout << _("Opening MPQ archive (wc3lib):") << std::endl;
				boost::timer timer;
				mpq.open(path);
				std::cout << boost::format(_("Result: %1%s")) % timer.elapsed() << std::endl;
				std::cout << _("Closing MPQ archive (wc3lib):") << std::endl;
				timer.restart();
				mpq.close();
				std::cout << boost::format(_("Result: %1%s")) % timer.elapsed() << std::endl;
/// @todo Add runtime linking support by using LibraryLoader.
#ifdef DEBUG
				std::cout << _("Opening MPQ archive (StormLib):") << std::endl;
				HANDLE stormLibArchive;
				timer.restart();

				if (SFileCreateArchiveEx(
				 path.string().c_str(),           // Archive file name
				 MPQ_OPEN_EXISTING,      // How to open the archive
				 HASH_TABLE_SIZE_MIN,            // Size of hash table for new archives
				 &stormLibArchive                    // Pointer to result HANDLE
				 ))
				{
					std::cout << boost::format(_("Result: %1%s")) % timer.elapsed() << std::endl;
					std::cout << _("Closing MPQ archive (StormLib):") << std::endl;
					timer.restart();
					SFileCloseArchive(stormLibArchive);
				}
				else
					std::cerr << boost::format(_("Failed to open MPQ archive \"%1%\" by using StormLib.")) % path.string() << std::endl;

#else
				std::cerr << _("Since program has been compiled without debug option, StormLib cannot be used.") << std::endl;
#endif
			}
		}
	}
	
	return EXIT_SUCCESS;
}
