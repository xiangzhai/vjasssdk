#ifndef WC3LIB_SLK_DATASUBENTRY_HPP
#define WC3LIB_SLK_DATASUBENTRY_HPP

namespace wc3lib
{
	
namespace slk
{

class DataEntry;
	
class DataSubEntry
{
	public:
		DataSubEntry(class DataEntry *dataEntry);
		
	protected:
		class DataEntry *m_dataEntry;
		std::string m_name;
		std::list<std::string> m_values;
};

}

}

#endif
