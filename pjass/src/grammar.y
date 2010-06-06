// Jass2 parser for bison/yacc
// by Rudi Cilibrasi
// Sun Jun  8 00:51:53 CEST 2003
// thanks to Jeff Pang for the handy documentation that this was based
// on at http://jass.sourceforge.net
%{

#include <stdio.h>
#include <string.h>
#include "misc.h"

/*
preperation for gettext
Added by Tamino Dauth
*/
#include <libintl.h>
#include <locale.h>
#define _(string) gettext(string)

int yyerrorex (int errorlevel, char *s)
{
  if (showerrorlevel[errorlevel]) {
    haderrors++;
    printf ("%s:%d: %s\n", curfile, lineno, s);
    return 0;
  }
  else
    ignorederrors++;
}

int yyerror (char *s)  /* Called by yyparse on error */
{
  yyerrorex(0, s);
}

int main(int argc, char **argv)
{
  init(argc, argv);
  if (1)  {
		doparse(argc, argv);
  }
  else {
    for (;;) {
      int result = yylex();
      if (result == 0) break;
      printf(_("Got result %d, %s.\n"), result, yytext);
    }
  }
  if (!haderrors && didparse) {
		printf(_("Parse successful: %8d lines: %s.\n"), totlines, "<total>");
    if (ignorederrors)
	printf(_("%d errors ignored."), ignorederrors);

    return 0;
  }
  else {
		if (haderrors)
		{
			if (haderrors == 1)
				printf(_("Parse failed: 1 error total.\n"));
			else
				printf(_("Parse failed: %d errors total.\n"), haderrors);
		}
		else
			printf(_("Parse failed.\n"));
		if (ignorederrors)
		  printf(_("%d errors ignored."), ignorederrors);
    return 1;
	}
}

#define YYSTYPE union node

%}

%token IF
%token THEN
%token TYPE
%token EXTENDS
%token HANDLE
%token NEWLINE
%token GLOBALS
%token ENDGLOBALS
%token CONSTANT
%token NATIVE
%token TAKES
%token RETURNS
%token FUNCTION
%token ENDFUNCTION
%token LOCAL
%token ARRAY
%token SET
%token CALL
%token ELSE
%token ELSEIF
%token ENDIF
%token LOOP
%token EXITWHEN
%token RETURN
%token DEBUG
%token ENDLOOP
%token NOT
%token TNULL
%token TTRUE
%token TFALSE
%token CODE
%token STRING
%token INTEGER
%token REAL
%token BOOLEAN
%token NOTHING
%token ID
%token COMMENT
%token COMMA
%token AND
%token OR
%token NOT
%token COMMA
%token EQUALS
%token TIMES
%token DIV
%token PLUS
%token MINUS
%token LPAREN
%token RPAREN
%token LBRACKET
%token RBRACKET
%token LESS
%token GREATER
%token LEQ
%token GEQ
%token EQCOMP
%token NEQ
%token STRINGLIT
%token INTLIT
%token REALLIT
%token UNITTYPEINT
%token EQUALS

%right EQUALS
%left AND OR
%left LESS GREATER EQCOMP NEQ LEQ GEQ
%left NOT
%left MINUS PLUS
%left TIMES DIV

%%

program: topscopes globdefs topscopes funcdefns
;

topscopes: topscope
       | topscopes topscope
;

topscope: typedefs  
       | funcdecls
;

funcdefns: /* empty */
       | funcdefns funcdefn
;

globdefs: /* empty */
         | GLOBALS vardecls ENDGLOBALS endglobalsmarker
;

endglobalsmarker: /* empty */  {afterendglobals = 1}
;

vardecls: /* empty */
         | vd vardecls
;

vd:      NEWLINE
       | vardecl
;

funcdecls: /* empty */
         | fd funcdecls
;

fd:      NEWLINE
       | funcdecl
;

typedefs:  /* empty */
         | td typedefs
;

td:      NEWLINE
       | typedef
;

// Returns a typenode
expr: intexpr      { $$.ty = gInteger; }
      | realexpr   { $$.ty = gReal; }
      | stringexpr { $$.ty = gString; }
      | boolexpr   { $$.ty = gBoolean; }
      | FUNCTION rid { if (lookup(&functions, $2.str) == NULL)
                         yyerrorex(3, "Undefined function.");
                       $$.ty = gCode;
                     }
      | TNULL { $$.ty = gNull; }
      | expr LEQ expr { checkcomparison($1.ty, $3.ty); $$.ty = gBoolean; }
      | expr GEQ expr { checkcomparison($1.ty, $3.ty); $$.ty = gBoolean; }
      | expr LESS expr { checkcomparison($1.ty, $3.ty); $$.ty = gBoolean; }
      | expr GREATER expr { checkcomparison($1.ty, $3.ty); $$.ty = gBoolean; }
      | expr EQCOMP expr { checkeqtest($1.ty, $3.ty); $$.ty = gBoolean; }
      | expr NEQ expr { checkeqtest($1.ty, $3.ty); $$.ty = gBoolean; }
      | expr AND expr { canconvert($1.ty, gBoolean); canconvert($3.ty, gBoolean); $$.ty = gBoolean; }
      | expr OR expr { canconvert($1.ty, gBoolean); canconvert($3.ty, gBoolean); $$.ty = gBoolean; }
      | NOT expr { canconvert($2.ty, gBoolean); $$.ty = gBoolean; }
      | expr TIMES expr { $$.ty = binop($1.ty, $3.ty); }
      | expr DIV expr { $$.ty = binop($1.ty, $3.ty); }
      | expr MINUS expr { $$.ty = binop($1.ty, $3.ty); }
      | expr PLUS expr { 
                         if ($1.ty == gString && $3.ty == gString)
                           $$.ty = gString;
                         else
                           $$.ty = binop($1.ty, $3.ty); }
      | MINUS expr { isnumeric($2.ty); $$.ty = $2.ty; }
      | LPAREN expr RPAREN { $$.ty = $2.ty; }
      | funccall { $$.ty = $1.ty }
      | rid LBRACKET expr RBRACKET {
          const struct typeandname *tan = getVariable($1.str);
          if (!tan->isarray) {
            char ebuf[1024];
            sprintf(ebuf, "%s not an array", $1.str);
            yyerrorex(3, ebuf);
            $$.ty = tan->ty;
          }
          else {
            canconvert($3.ty, gInteger);
            $$.ty = tan->ty;
          }
       }
      | rid {
          $$.ty = getVariable($1.str)->ty;
       }
      | expr EQUALS expr {yyerrorex(0, "Single = in expression, should probably be =="); checkeqtest($1.ty, $3.ty); $$.ty = gBoolean;}
      | LPAREN expr {yyerrorex(0, "')' missing"); $$.ty = $2.ty;}
;

funccall: rid LPAREN exprlistcompl RPAREN {
          struct funcdecl *fd = lookup(&functions, $1.str);
          if (fd == NULL) {
            char ebuf[1024];
            sprintf(ebuf, "Undeclared function %s", $1.str);
            yyerrorex(3, ebuf);
            $$.ty = gNull;
          }
            else {
              checkParameters(fd->p, $3.pl);
              $$.ty = fd->ret;
            }
       }
       |  rid LPAREN exprlistcompl NEWLINE{
          yyerrorex(0, "')' missing");
          struct funcdecl *fd = lookup(&functions, $1.str);
          if (fd == NULL) {
            char ebuf[1024];
            sprintf(ebuf, "Undeclared function %s", $1.str);
            yyerrorex(3, ebuf);
            $$.ty = gNull;
          }
            else {
              checkParameters(fd->p, $3.pl);
              $$.ty = fd->ret;
            }
       }
;

exprlistcompl: /* empty */ { $$.pl = newparamlist(); }
       | exprlist { $$.pl = $1.pl; }
;

exprlist: expr         { $$.pl = newparamlist(); addParam($$.pl, newtypeandname($1.ty, "")); }
       |  expr COMMA exprlist { $$.pl = $3.pl; addParam($$.pl, newtypeandname($1.ty, "")); }
;


stringexpr: STRINGLIT { $$.ty = gString; }
;

realexpr: REALLIT { $$.ty = gReal; }
;

boolexpr: boollit { $$.ty = gBoolean; }
;

boollit: TTRUE
       | TFALSE
;

intexpr:   INTLIT { $$.ty = gInteger; }
         | UNITTYPEINT { $$.ty = gInteger; }
;


funcdecl: nativefuncdecl { $$.fd = $1.fd; }
         | CONSTANT nativefuncdecl { $$.fd = $2.fd; }
;

nativefuncdecl: NATIVE rid TAKES optparam_list RETURNS opttype
{ 
$$.fd = newfuncdecl(); 
  $$.fd->name = strdup($2.str);
  $$.fd->p = $4.pl;
  $$.fd->ret = $6.ty;
  put(&functions, $$.fd->name, $$.fd);
  //showfuncdecl($$.fd);
}
;

funcdefn: NEWLINE
       | CONSTANT funcdefncore
       | funcdefncore
       | statement {yyerrorex(0, "Statement outside of function");}
;

funcdefncore: funcbegin localblock codeblock funcend {if((retval != gNothing) && (!canconvertreturn($3.ty, retval) || ($3.ty == gAny) )) yyerrorex(1, "Return types not correct or nonexistant returns");}
       | funcbegin localblock codeblock {yyerrorex(0, "Missing endfunction"); clear(&params); clear(&locals); curtab = &globals;}
;

funcend: ENDFUNCTION { clear(&params); clear(&locals); curtab = &globals; }
;

funcbegin: FUNCTION rid TAKES optparam_list RETURNS opttype  {
  curtab = &locals;
  $$.fd = newfuncdecl(); 
  $$.fd->name = strdup($2.str);
  $$.fd->p = $4.pl;
  $$.fd->ret = $6.ty;
  put(&functions, $$.fd->name, $$.fd);
  struct typeandname *tan = $4.pl->head;
  for (;tan; tan=tan->next)
    put(&params, strdup(tan->name), newtypeandname(tan->ty, tan->name));
  retval = $$.fd->ret;
  inloop = 0;
  //showfuncdecl($$.fd);
}
;

codeblock: /* empty */ {$$.ty = gAny;}
       | statement codeblock { if($2.ty == gAny) $$.ty = $1.ty; else $$.ty = $2.ty;}
;

statement:  NEWLINE {$$.ty = gAny;}
       | CALL funccall NEWLINE{ $$.ty = gNone;}
       | IF expr THEN NEWLINE codeblock elsifseq elseseq ENDIF NEWLINE { canconvert($2.ty, gBoolean); $$.ty = combinetype($6.ty!=gAny?combinetype($5.ty, $6.ty):$5.ty, $7.ty);}
       | SET rid EQUALS expr NEWLINE { canconvert($4.ty, getVariable($2.str)->ty); $$.ty = gNone; if (getVariable($2.str)->isconst) { char ebuf[1024];
                  sprintf(ebuf, "Cannot assign to constant %s\n", $2.str);
                  yyerrorex(3, ebuf);
									}
}
       | SET rid LBRACKET expr RBRACKET EQUALS expr NEWLINE{ 
           const struct typeandname *tan = getVariable($2.str);
           canconvert($4.ty, gInteger); $$.ty = gNone;
           if (!tan->isarray) {
             char ebuf[1024];
             sprintf(ebuf, "%s not an array", $2.str);
             yyerrorex(3, ebuf);
           }
           canconvert($7.ty, tan->ty); }
       | loopstart NEWLINE codeblock loopend NEWLINE {$$.ty = $3.ty;}
       | loopstart NEWLINE codeblock {$$.ty = $3.ty; yyerrorex(0, "Missing endloop");}
       | EXITWHEN expr NEWLINE { canconvert($2.ty, gBoolean); if (!inloop) yyerrorex(0, "Exitwhen outside of loop"); $$.ty = gNone;}
       | RETURN expr NEWLINE { $$.ty = $2.ty; if(retval == gNothing) yyerrorex(1, "Cannot return value from function that returns nothing");}
       | RETURN NEWLINE { if (retval != gNothing) yyerrorex(1, "Return nothing in function that should return value"); $$.ty = gNone;}
       | DEBUG statement {$$.ty = gNone;}
       | IF expr THEN NEWLINE codeblock elsifseq elseseq {canconvert($2.ty, gBoolean); $$.ty = combinetype($6.ty!=gAny?combinetype($5.ty, $6.ty):$5.ty, $7.ty); yyerrorex(0, "Missing endif");}
       | IF expr NEWLINE{canconvert($2.ty, gBoolean); $$.ty = gAny; yyerrorex(0, "Missing then or non valid expression");}
       | SET funccall NEWLINE{$$.ty = gNone; yyerrorex(0, "Call expected instead of set");}
       | error {$$.ty = gNone; }
;

loopstart: LOOP {inloop++;}
;

loopend: ENDLOOP {inloop--;}
;

elseseq: /* empty */ {$$.ty = gNone;}
        | ELSE NEWLINE codeblock {$$.ty = $3.ty;}
;

elsifseq: /* empty */ {$$.ty = gAny;}
        | ELSEIF expr THEN NEWLINE codeblock elsifseq { canconvert($2.ty, gBoolean); $$.ty = $6.ty!=gAny?combinetype($5.ty, $6.ty):$5.ty;}
;

optparam_list: param_list { $$.pl = $1.pl; }
               | NOTHING { $$.pl = newparamlist(); }
;

opttype: NOTHING { $$.ty = gNothing; }
         | type { $$.ty = $1.ty; }
;

param_list: typeandname { $$.pl = newparamlist(); addParam($$.pl, $1.tan); }
          | typeandname COMMA param_list { addParam($3.pl, $1.tan); $$.pl = $3.pl; }
;

rid: ID
{ $$.str = strdup(yytext); }
;

vartypedecl: type rid {
  struct typeandname *tan = newtypeandname($1.ty, $2.str);
  $$.str = $2.str;
  put(curtab, $2.str, tan);  }
       | CONSTANT type rid {
  if (afterendglobals) {
    yyerrorex(3, "Local constants are not allowed");
  }
  struct typeandname *tan = newtypeandname($2.ty, $3.str);
  $$.str = $3.str;
  tan->isconst = 1;
  put(curtab, $3.str, tan); }
       | type ARRAY rid {
  struct typeandname *tan = newtypeandname($1.ty, $3.str);
  $$.str = $3.str;
  tan->isarray = 1;
  put(curtab, $3.str, tan); }
;

localblock: /* empty */
        | lvardecl localblock
;

lvardecl: LOCAL vardecl { }
        | NEWLINE
;

vardecl:   vartypedecl NEWLINE{ $$.ty = gNothing; }
        |  vartypedecl EQUALS expr NEWLINE{
           const struct typeandname *tan = getVariable($1.str);
           if (tan->isarray) {
             yyerrorex(3, "Arrays cannot be directly initialized");
           }
           canconvert($3.ty, tan->ty);
           $$.ty = gNothing;
           }
        | error
;

typedef: TYPE rid EXTENDS type {
  if (lookup(&types, $2.str)) {
     char buf[1024];
     sprintf(buf, "Multiply defined type: %s", $2.str);
     yyerrorex(3, buf);
  }
  else
    put(&types, $2.str, newtypenode($2.str, $4.ty));
}
;

typeandname: type rid { $$.tan = newtypeandname($1.ty, $2.str); }
;
  
type: primtype { $$.ty = $1.ty; }
  | rid {
   if (lookup(&types, $1.str) == NULL) {
     char buf[1024];
     sprintf(buf, "Undefined type: %s", $1.str);
     yyerrorex(3, buf);
     $$.ty = gNull;
   }
   else
     $$.ty = lookup(&types, $1.str);
}
;

primtype: HANDLE  { $$.ty = lookup(&types, yytext); }
 | INTEGER        { $$.ty = lookup(&types, yytext); }
 | REAL           { $$.ty = lookup(&types, yytext); }
 | BOOLEAN        { $$.ty = lookup(&types, yytext); }
 | STRING         { $$.ty = lookup(&types, yytext); }
 | CODE           { $$.ty = lookup(&types, yytext); }
;

