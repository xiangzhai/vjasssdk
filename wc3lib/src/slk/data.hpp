#ifndef WC3LIB_SLK_DATA_HPP
#define WC3LIB_SLK_DATA_HPP

#include <istream>
#include <ostream>

namespace wc3lib
{
	
namespace slk
{
	
class DataEntry;

/**
* Default class for reading <Prefix>Data.txt files like WorldEditData.txt or TriggerStrings.txt.
* @todo Should use a Bison file which defines the possible syntax for these files.
*/
class Data
{
	public:
		Data();
		~Data();

		std::streamsize read(std::istream &istream) throw (class Exception);
		std::streamsize write(std::ostream &ostream) throw (class Exception);

	protected:
		virtual class DataEntry* dataEntry() = 0;

		std::list<class DataEntry*> m_entries;
};

}

}

#endif
