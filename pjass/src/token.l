/* Jass2 parser for bison/yacc */
/* by Rudi Cilibrasi */
/* Sun Jun  8 00:51:53 CEST 2003 */
/* thanks to Jeff Pang for the handy documentation that this was based */
/* on at http://jass.sourceforge.net */
/* Released under the BSD license */

%{
/* need this for the call to atof() below */
#undef yywrap
int yywrap()
{
	return 1;
}

#include "grammar.tab.h" 
#include "misc.h"

%}

COMMENT [/][/].*\r?\n
NEWLINE  \r?\n
DIGIT    [0-9]
HEXDIGIT [0-9a-fA-F]
ID       [a-zA-Z]([a-zA-Z0-9_]*[a-zA-Z0-9])?
SPACE    [ \t]
CSTART  [/][*]
CEND    [*][/]
STRINGSTART ["]
STILLSTRINGA [^"\\]
STILLSTRINGB [\\][btrn"\\]
STRINGDONE ["]
FSTRINGSTART ["]
FSTILLSTRINGA [^"\\]
FSTILLSTRINGB [\\].
FSTRINGDONE ["]
UNITTYPEINT ['][a-zA-Z0-9][a-zA-Z0-9]?[a-zA-Z0-9]?[a-zA-Z0-9]?[']

%%

{STRINGSTART}({STILLSTRINGA}|{STILLSTRINGB})*{STRINGDONE} { return STRINGLIT; }
{FSTRINGSTART}({FSTILLSTRINGA}|{FSTILLSTRINGB})*{FSTRINGDONE} { printf( "Non valid escape sequences in string\n" ); }
{COMMENT} lineno++; return NEWLINE;
{CSTART}.*{CEND} { }
{NEWLINE} lineno++; return NEWLINE;

{DIGIT}*"."{DIGIT}*  return REALLIT;

({DIGIT}+)|("0x"{HEXDIGIT}+)  return INTLIT;

"if"  return IF;
"not" return NOT;
"then" return THEN;
"type" return TYPE;
"extends" return EXTENDS;
"handle" return HANDLE;
"globals" return GLOBALS;
"endglobals" return ENDGLOBALS;
"constant" return CONSTANT;
"native" return NATIVE;
"takes" return TAKES;
"returns" return RETURNS;
"function" return FUNCTION;
"endfunction" return ENDFUNCTION;
"local" return LOCAL;
"array" return ARRAY;
"set" return SET;
"call" return CALL;
"else" return ELSE;
"elseif" return ELSEIF;
"endif" return ENDIF;
"loop" return LOOP;
"exitwhen" return EXITWHEN;
"return" return RETURN;
"debug" return DEBUG;
"endloop" return ENDLOOP;
"null" return TNULL;
"true" return TTRUE;
"false" return TFALSE;
"code" return CODE;
"string" return STRING;
"integer" return INTEGER;
"real" return REAL;
"boolean" return BOOLEAN;
"nothing" return NOTHING;
"and" return AND;
"or" return OR;
"," return COMMA;
"=" return EQUALS;
"*" return TIMES;
"/" return DIV;
"+" return PLUS;
"-" return MINUS;
"(" return LPAREN;
")" return RPAREN;
"[" return LBRACKET;
"]" return RBRACKET;
"<" return LESS;
">" return GREATER;
"==" return EQCOMP;
"<=" return LEQ;
">=" return GEQ;
"!=" return NEQ;

{ID}        return ID;
{UNITTYPEINT}        return UNITTYPEINT;

"<"|">"|"!"|"["|"]"|"("|")"|"+"|"-"|"*"|"/"|"."  return TNULL;

[ \t]+          /* eat up whitespace */

.           printf( "Unrecognized character: %s\n", yytext );

%%

