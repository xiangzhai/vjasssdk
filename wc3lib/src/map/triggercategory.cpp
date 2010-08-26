
namespace wc3lib
{

namespace map
{

TriggerCategory::TriggerCategory(class Triggers *triggers) : m_triggers(triggers), m_name(), m_index(0)
{
}

std::streamsize TriggerCategory::read(std::istream &istream) throw (class Exception)
{
	std::streamsize size;
	read<int32>(istream, this->m_index, size);
	readString(istream, this->m_name, size);

	return size;
}

std::streamsize TriggerCategory::write(std::ostream &ostream) const throw (class Exception)
{
	std::streamsize size;
	write<int32>(ostream, this->m_index, size);
	writeString(ostream, this->m_name, size);

	return size;
}

}

}
