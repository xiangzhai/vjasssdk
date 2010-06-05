#ifndef WC3LIB_FORMAT_HPP
#define WC3LIB_FORMAT_HPP

#include <istream>
#include <ostream>

#include "exception.hpp"

namespace wc3lib
{

/// @brief Abstract class for formats.
class Format
{
	public:
		virtual std::streamsize read(std::istream &istream) throw (class Exception) = 0;
		virtual std::streamsize write(std::ostream &ostream) const throw (class Exception) = 0;
};

}

#endif
