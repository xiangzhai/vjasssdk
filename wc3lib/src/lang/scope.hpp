#ifndef WC3LIB_LANG_SCOPE_HPP
#define WC3LIB_LANG_SCOPE_HPP

namespace wc3lib
{

namespace lang
{

/**
* Scopes can be encapsulated. Therefore each scope instance has a queue which holds all tokens.
* The first one in queue is the outermost one.
*/
class Scope
{
	public:
		Scope(class Token *token);
		Scope(std::queue<class Token*> &tokens);
		/**
		* @return Returns the innermost token of token queue.
		*/
		const class Token* token() const;

	private:
		std::queue<class Token*> m_tokens;
};

}

}
