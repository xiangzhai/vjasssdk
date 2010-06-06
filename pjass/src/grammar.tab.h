/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IF = 258,
     THEN = 259,
     TYPE = 260,
     EXTENDS = 261,
     HANDLE = 262,
     NEWLINE = 263,
     GLOBALS = 264,
     ENDGLOBALS = 265,
     CONSTANT = 266,
     NATIVE = 267,
     TAKES = 268,
     RETURNS = 269,
     FUNCTION = 270,
     ENDFUNCTION = 271,
     LOCAL = 272,
     ARRAY = 273,
     SET = 274,
     CALL = 275,
     ELSE = 276,
     ELSEIF = 277,
     ENDIF = 278,
     LOOP = 279,
     EXITWHEN = 280,
     RETURN = 281,
     DEBUG = 282,
     ENDLOOP = 283,
     NOT = 284,
     TNULL = 285,
     TTRUE = 286,
     TFALSE = 287,
     CODE = 288,
     STRING = 289,
     INTEGER = 290,
     REAL = 291,
     BOOLEAN = 292,
     NOTHING = 293,
     ID = 294,
     COMMENT = 295,
     COMMA = 296,
     AND = 297,
     OR = 298,
     EQUALS = 299,
     TIMES = 300,
     DIV = 301,
     PLUS = 302,
     MINUS = 303,
     LPAREN = 304,
     RPAREN = 305,
     LBRACKET = 306,
     RBRACKET = 307,
     LESS = 308,
     GREATER = 309,
     LEQ = 310,
     GEQ = 311,
     EQCOMP = 312,
     NEQ = 313,
     STRINGLIT = 314,
     INTLIT = 315,
     REALLIT = 316,
     UNITTYPEINT = 317
   };
#endif
/* Tokens.  */
#define IF 258
#define THEN 259
#define TYPE 260
#define EXTENDS 261
#define HANDLE 262
#define NEWLINE 263
#define GLOBALS 264
#define ENDGLOBALS 265
#define CONSTANT 266
#define NATIVE 267
#define TAKES 268
#define RETURNS 269
#define FUNCTION 270
#define ENDFUNCTION 271
#define LOCAL 272
#define ARRAY 273
#define SET 274
#define CALL 275
#define ELSE 276
#define ELSEIF 277
#define ENDIF 278
#define LOOP 279
#define EXITWHEN 280
#define RETURN 281
#define DEBUG 282
#define ENDLOOP 283
#define NOT 284
#define TNULL 285
#define TTRUE 286
#define TFALSE 287
#define CODE 288
#define STRING 289
#define INTEGER 290
#define REAL 291
#define BOOLEAN 292
#define NOTHING 293
#define ID 294
#define COMMENT 295
#define COMMA 296
#define AND 297
#define OR 298
#define EQUALS 299
#define TIMES 300
#define DIV 301
#define PLUS 302
#define MINUS 303
#define LPAREN 304
#define RPAREN 305
#define LBRACKET 306
#define RBRACKET 307
#define LESS 308
#define GREATER 309
#define LEQ 310
#define GEQ 311
#define EQCOMP 312
#define NEQ 313
#define STRINGLIT 314
#define INTLIT 315
#define REALLIT 316
#define UNITTYPEINT 317




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

