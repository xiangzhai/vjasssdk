
%{
%}

/* Require bison 2.4 or later */
%require "2.4"

/* add debug output code to generated parser. disable this for release
 * versions. */
%debug

/* start symbol is named "ScopeContent" */
%start ScopeContent

/* write out a header file containing the token defines */
%defines

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
/* %name-prefix="example" */

/* keep track of the current position within the input */
%locations

%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &driver.streamname;
};

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class Driver& driver }

/* verbose error messages */
%error-verbose

// GRAMMAR DEFINITIONS START HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
%union {
    class Object *object;
}


//%token PP

%{

#include "driver.hpp"
#include "scanner.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex

%}


%%


IncreaseLine				: NewLine
					{
						Vjassdoc::parser()->currentFile()->addLines(1);
					}
					;

DocComment				: LineDocComment IncreaseLine
					| BlockDocComment /* increase lines please */
					{
					}
					;

%%
