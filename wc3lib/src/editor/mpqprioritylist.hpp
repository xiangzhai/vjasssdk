#ifndef WC3LIB_EDITOR_MPQPRIORITYLIST_HPP
#define WC3LIB_EDITOR_MPQPRIORITYLIST_HPP

namespace wc3lib
{

namespace editor
{

class MpqPriorityList : public std::map<std::size_t, const class mpq::Mpq*>
{
	public:
		void setLocale();
		Locale locale() const;

		void addMpq(const boost::filesystem::path &path)

		const class MpqFile* findFile(const std::string &fileName);
		std::streamsize writeContent(const std::string &fileName, std::ostream &ostream) throw (class Exception);
};

#endif
