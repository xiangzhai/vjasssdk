#ifndef DATAENTRY_HPP
#define DATAENTRY_HPP

namespace wc3lib
{
	
namespace slk
{
	
class Data;
class DataSubEntry;

class DataEntry
{
	public:
		DataEntry(class Data *data);
	    
	protected:
		class Data *m_data;
		std::string m_name;
		std::list<class DataSubEntry*> m_subEntries;
};

}

}

#endif
