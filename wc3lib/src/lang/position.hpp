#ifndef WC3LIB_LANG_POSITION_HPP
#define WC3LIB_LANG_POSITION_HPP

namespace wc3lib
{

namespace lang
{

class Position
{
	public:
		Position(class SourceFile *sourceFile, std::size_t line, std::size_t column);
		
	private:
		class SourceFile *m_sourceFile;
		std::size_t m_line;
		std::size_t m_column;
};


}

}
