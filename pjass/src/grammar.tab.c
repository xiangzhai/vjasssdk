/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/*
preperation for gettext
Added by Tamino Dauth.
*/
#include <libintl.h>
#include <locale.h>
#define _(string) gettext(string)

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 6 "src/grammar.y"


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
    printf ("%s:%d: %s.\n", curfile, lineno, s);
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
//2009-11-21 Tamino Dauth <tamino@cdauth.de>
setlocale(LC_ALL, "");
bindtextdomain("pjass", LOCALE_DIR);
textdomain("pjass");

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 300 "grammar.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  19
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   543

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  123
/* YYNRULES -- Number of states.  */
#define YYNSTATES  210

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   317

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    10,    13,    15,    17,    18,    21,
      22,    27,    28,    29,    32,    34,    36,    37,    40,    42,
      44,    45,    48,    50,    52,    54,    56,    58,    60,    63,
      65,    69,    73,    77,    81,    85,    89,    93,    97,   100,
     104,   108,   112,   116,   119,   123,   125,   130,   132,   136,
     139,   144,   149,   150,   152,   154,   158,   160,   162,   164,
     166,   168,   170,   172,   174,   177,   184,   186,   189,   191,
     193,   198,   202,   204,   211,   212,   215,   217,   221,   231,
     237,   246,   252,   256,   260,   264,   267,   270,   278,   282,
     286,   288,   290,   292,   293,   297,   298,   305,   307,   309,
     311,   313,   315,   319,   321,   324,   328,   332,   333,   336,
     339,   341,   344,   349,   351,   356,   359,   361,   363,   365,
     367,   369,   371,   373
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      64,     0,    -1,    65,    68,    65,    67,    -1,    66,    -1,
      65,    66,    -1,    74,    -1,    72,    -1,    -1,    67,    87,
      -1,    -1,     9,    70,    10,    69,    -1,    -1,    -1,    71,
      70,    -1,     8,    -1,   104,    -1,    -1,    73,    72,    -1,
       8,    -1,    85,    -1,    -1,    75,    74,    -1,     8,    -1,
     105,    -1,    84,    -1,    81,    -1,    80,    -1,    82,    -1,
      15,   100,    -1,    30,    -1,    76,    55,    76,    -1,    76,
      56,    76,    -1,    76,    53,    76,    -1,    76,    54,    76,
      -1,    76,    57,    76,    -1,    76,    58,    76,    -1,    76,
      42,    76,    -1,    76,    43,    76,    -1,    29,    76,    -1,
      76,    45,    76,    -1,    76,    46,    76,    -1,    76,    48,
      76,    -1,    76,    47,    76,    -1,    48,    76,    -1,    49,
      76,    50,    -1,    77,    -1,   100,    51,    76,    52,    -1,
     100,    -1,    76,    44,    76,    -1,    49,    76,    -1,   100,
      49,    78,    50,    -1,   100,    49,    78,     8,    -1,    -1,
      79,    -1,    76,    -1,    76,    41,    79,    -1,    59,    -1,
      61,    -1,    83,    -1,    31,    -1,    32,    -1,    60,    -1,
      62,    -1,    86,    -1,    11,    86,    -1,    12,   100,    13,
      97,    14,    98,    -1,     8,    -1,    11,    88,    -1,    88,
      -1,    92,    -1,    90,   102,    91,    89,    -1,    90,   102,
      91,    -1,    16,    -1,    15,   100,    13,    97,    14,    98,
      -1,    -1,    92,    91,    -1,     8,    -1,    20,    77,     8,
      -1,     3,    76,     4,     8,    91,    96,    95,    23,     8,
      -1,    19,   100,    44,    76,     8,    -1,    19,   100,    51,
      76,    52,    44,    76,     8,    -1,    93,     8,    91,    94,
       8,    -1,    93,     8,    91,    -1,    25,    76,     8,    -1,
      26,    76,     8,    -1,    26,     8,    -1,    27,    92,    -1,
       3,    76,     4,     8,    91,    96,    95,    -1,     3,    76,
       8,    -1,    19,    77,     8,    -1,     1,    -1,    24,    -1,
      28,    -1,    -1,    21,     8,    91,    -1,    -1,    22,    76,
       4,     8,    91,    96,    -1,    99,    -1,    38,    -1,    38,
      -1,   107,    -1,   106,    -1,   106,    41,    99,    -1,    39,
      -1,   107,   100,    -1,    11,   107,   100,    -1,   107,    18,
     100,    -1,    -1,   103,   102,    -1,    17,   104,    -1,     8,
      -1,   101,     8,    -1,   101,    44,    76,     8,    -1,     1,
      -1,     5,   100,     6,   107,    -1,   107,   100,    -1,   108,
      -1,   100,    -1,     7,    -1,    35,    -1,    36,    -1,    37,
      -1,    34,    -1,    33,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   149,   149,   152,   153,   156,   157,   160,   161,   164,
     165,   168,   171,   172,   175,   176,   179,   180,   183,   184,
     187,   188,   191,   192,   196,   197,   198,   199,   200,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   222,   223,   224,   225,   238,   241,   242,
     245,   258,   274,   275,   278,   279,   283,   286,   289,   292,
     293,   296,   297,   301,   302,   305,   316,   317,   318,   319,
     322,   323,   326,   329,   345,   346,   349,   350,   351,   352,
     357,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   378,   381,   384,   385,   388,   389,   392,   393,   396,
     397,   400,   401,   404,   408,   412,   420,   427,   428,   431,
     432,   435,   436,   444,   447,   458,   461,   462,   474,   475,
     476,   477,   478,   479
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "THEN", "TYPE", "EXTENDS",
  "HANDLE", "NEWLINE", "GLOBALS", "ENDGLOBALS", "CONSTANT", "NATIVE",
  "TAKES", "RETURNS", "FUNCTION", "ENDFUNCTION", "LOCAL", "ARRAY", "SET",
  "CALL", "ELSE", "ELSEIF", "ENDIF", "LOOP", "EXITWHEN", "RETURN", "DEBUG",
  "ENDLOOP", "NOT", "TNULL", "TTRUE", "TFALSE", "CODE", "STRING",
  "INTEGER", "REAL", "BOOLEAN", "NOTHING", "ID", "COMMENT", "COMMA", "AND",
  "OR", "EQUALS", "TIMES", "DIV", "PLUS", "MINUS", "LPAREN", "RPAREN",
  "LBRACKET", "RBRACKET", "LESS", "GREATER", "LEQ", "GEQ", "EQCOMP", "NEQ",
  "STRINGLIT", "INTLIT", "REALLIT", "UNITTYPEINT", "$accept", "program",
  "topscopes", "topscope", "funcdefns", "globdefs", "endglobalsmarker",
  "vardecls", "vd", "funcdecls", "fd", "typedefs", "td", "expr",
  "funccall", "exprlistcompl", "exprlist", "stringexpr", "realexpr",
  "boolexpr", "boollit", "intexpr", "funcdecl", "nativefuncdecl",
  "funcdefn", "funcdefncore", "funcend", "funcbegin", "codeblock",
  "statement", "loopstart", "loopend", "elseseq", "elsifseq",
  "optparam_list", "opttype", "param_list", "rid", "vartypedecl",
  "localblock", "lvardecl", "vardecl", "typedef", "typeandname", "type",
  "primtype", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    63,    64,    65,    65,    66,    66,    67,    67,    68,
      68,    69,    70,    70,    71,    71,    72,    72,    73,    73,
      74,    74,    75,    75,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      77,    77,    78,    78,    79,    79,    80,    81,    82,    83,
      83,    84,    84,    85,    85,    86,    87,    87,    87,    87,
      88,    88,    89,    90,    91,    91,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    93,    94,    95,    95,    96,    96,    97,    97,    98,
      98,    99,    99,   100,   101,   101,   101,   102,   102,   103,
     103,   104,   104,   104,   105,   106,   107,   107,   108,   108,
     108,   108,   108,   108
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     1,     2,     1,     1,     0,     2,     0,
       4,     0,     0,     2,     1,     1,     0,     2,     1,     1,
       0,     2,     1,     1,     1,     1,     1,     1,     2,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     2,     3,     1,     4,     1,     3,     2,
       4,     4,     0,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     6,     1,     2,     1,     1,
       4,     3,     1,     6,     0,     2,     1,     3,     9,     5,
       8,     5,     3,     3,     3,     2,     2,     7,     3,     3,
       1,     1,     1,     0,     3,     0,     6,     1,     1,     1,
       1,     1,     3,     1,     2,     3,     3,     0,     2,     2,
       1,     2,     4,     1,     4,     2,     1,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      16,     0,    18,     0,     0,     0,     9,     3,     6,    16,
       5,    20,    19,    63,    23,   103,     0,    64,     0,     1,
       0,     4,    16,    18,    17,    22,    21,     0,     0,   113,
     118,    14,     0,   123,   122,   119,   120,   121,     0,     0,
     117,     0,    15,     0,   116,     7,   114,    98,     0,    97,
     101,     0,     0,    11,    13,   111,     0,     0,   104,     0,
       0,     0,   115,   105,    10,     0,     0,    29,    59,    60,
       0,     0,    56,    61,    57,    62,     0,    45,    26,    25,
      27,    58,    24,    47,   106,    90,     0,    66,     0,     0,
       0,     0,    91,     0,     0,     0,     8,    68,   107,    69,
       0,    99,    65,   100,   102,    28,    38,    43,    49,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,    67,     0,     0,     0,
       0,     0,     0,    85,     0,    76,    86,   110,     0,     0,
     107,     0,    44,    36,    37,    48,    39,    40,    42,    41,
      32,    33,    30,    31,    34,    35,    54,     0,    53,     0,
       0,    88,     0,    89,     0,     0,    77,    83,    84,   109,
      71,     0,   108,    82,     0,    51,    50,    46,     0,     0,
       0,     0,    72,    70,    75,    92,     0,    55,    95,     0,
      79,     0,    81,     0,    93,    73,     0,     0,     0,    87,
       0,     0,     0,     0,    80,     0,    94,    78,    95,    96
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,    59,    22,    64,    38,    39,     8,
       9,    10,    11,   156,    77,   157,   158,    78,    79,    80,
      81,    82,    12,    13,    96,    97,   183,    98,   170,   171,
     100,   186,   199,   194,    48,   102,    49,    83,    41,   139,
     140,    42,    14,    50,    43,    44
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -88
static const yytype_int16 yypact[] =
{
      28,   -27,   -88,    16,   -27,    37,     5,   -88,   -88,    12,
     -88,    77,   -88,   -88,   -88,   -88,    38,   -88,    42,   -88,
     292,   -88,    28,   -88,   -88,   -88,   -88,   112,   205,   -88,
     -88,   -88,   112,   -88,   -88,   -88,   -88,   -88,    52,   292,
     -88,    -3,   -88,    -7,   -88,    28,   -88,   -88,    72,   -88,
      26,   -27,   -27,   -88,   -88,   -88,   259,   -27,   -88,   407,
     310,   112,   -88,   -88,   -88,   -27,   259,   -88,   -88,   -88,
     259,   259,   -88,   -88,   -88,   -88,    85,   -88,   -88,   -88,
     -88,   -88,   -88,    17,   -88,   -88,   259,   -88,    79,   -27,
     -27,   -27,   -88,   259,   142,   416,   -88,   -88,     1,   -88,
      84,   -88,   -88,   -88,   -88,   -88,    57,   -24,   420,   -88,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,    53,   -88,    74,   104,    14,
     105,    65,   152,   -88,   174,   -88,   -88,   -88,   303,   379,
       1,   350,   -88,   485,   485,   471,   -88,   -88,   -24,   -24,
      57,    57,    57,    57,    57,    57,   403,    -4,   -88,   437,
     108,   -88,   205,   -88,   259,   259,   -88,   -88,   -88,   -88,
     101,   350,   -88,    92,   259,   -88,   -88,   -88,   350,   107,
     203,   454,   -88,   -88,   -88,   -88,   114,   -88,   103,   310,
     -88,    90,   -88,   259,   123,   -88,   259,   122,   127,   130,
     225,   146,   350,   148,   -88,   350,   -88,   -88,   103,   -88
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -88,   -88,   136,     2,   -88,   -88,   -88,   120,   -88,   153,
     -88,   172,   -88,   -41,   -42,   -88,    10,   -88,   -88,   -88,
     -88,   -88,   -88,   182,   -88,    98,   -88,   -88,   -87,   -52,
     -88,   -88,   -88,   -21,    27,     3,   132,    -1,   -88,    73,
     -88,    76,   -88,   -88,   -26,   -88
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -75
static const yytype_int16 yytable[] =
{
      16,    46,    51,    18,   175,    55,    52,    99,    21,   137,
       1,    57,    15,     2,    20,    76,     3,     4,   138,    40,
      23,   113,   114,     3,     4,   106,    40,    40,     4,   107,
     108,    40,    15,     1,   103,    51,     2,    19,    40,     3,
       4,    56,    58,   136,    27,   125,   176,    21,   128,   130,
      62,    63,   132,   134,   173,    28,    84,   160,   164,    40,
      40,   161,    53,   123,   105,   165,   123,    61,   124,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     1,   159,   184,    25,    60,   162,   127,   129,
     131,   188,   141,   109,    89,   110,   111,   112,   113,   114,
     115,   116,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   163,   166,   123,   206,   178,   182,   208,    30,
     185,   189,   192,   180,   181,   193,   201,   110,   111,   112,
     113,   114,   115,   116,   196,   202,    51,    40,   117,   118,
     119,   120,   121,   122,   198,    33,    34,    35,    36,    37,
     133,    15,   197,   203,   205,   200,   207,    65,    45,    54,
     167,    40,    24,   103,   110,   111,   112,   113,   114,   115,
     116,    66,    67,    68,    69,   117,   118,   119,   120,   121,
     122,    15,   168,    26,   187,    17,   126,   209,    40,   179,
      70,    71,   195,   104,   110,   111,   112,   113,   114,   115,
     116,    72,    73,    74,    75,   117,   118,   119,   120,   121,
     122,   190,    30,   172,   169,     0,   110,   111,   112,   113,
     114,   115,   116,     0,     0,     0,     0,   117,   118,   119,
     120,   121,   122,   204,     0,     0,     0,     0,    33,    34,
      35,    36,    37,    47,    15,   110,   111,   112,   113,   114,
     115,   116,     0,     0,     0,     0,   117,   118,   119,   120,
     121,   122,     0,     0,     0,     0,     0,   110,   111,   112,
     113,   114,   115,   116,    65,     0,     0,     0,   117,   118,
     119,   120,   121,   122,     0,     0,     0,     0,    66,    67,
      68,    69,     0,    29,     0,     0,     0,     0,    15,    30,
      31,     0,   -12,    32,    29,     0,     0,    70,    71,     0,
      30,     0,     0,     0,    32,     0,     0,    30,    72,    73,
      74,    75,     0,     0,     0,    33,    34,    35,    36,    37,
       0,    15,     0,     0,     0,     0,    33,    34,    35,    36,
      37,     0,    15,    33,    34,    35,    36,    37,   101,    15,
     -74,    85,     0,    86,     0,     0,     0,     0,   135,     0,
       0,   -74,     0,     0,     0,   -74,   -74,     0,     0,    90,
      91,   -74,   -74,   -74,    92,    93,    94,    95,   -74,   -74,
      85,     0,    86,     0,     0,     0,     0,   135,     0,     0,
     -74,     0,     0,     0,   -74,   -74,     0,     0,    90,    91,
       0,     0,     0,    92,    93,    94,    95,    -2,    85,     0,
      86,     0,     0,     0,     0,    87,     0,    85,    88,    86,
       0,     0,    89,     0,   135,     0,    90,    91,     0,     0,
       0,    92,    93,    94,    95,    90,    91,     0,     0,     0,
      92,    93,    94,    95,   174,   110,   111,   112,   113,   114,
     115,   116,     0,     0,     0,     0,   117,   118,   119,   120,
     121,   122,   110,   111,   112,   113,   114,   115,   116,     0,
     142,     0,     0,   117,   118,   119,   120,   121,   122,   110,
     111,   112,   113,   114,   115,   116,     0,     0,     0,   177,
     117,   118,   119,   120,   121,   122,   110,   111,   112,   113,
     114,   115,   116,     0,     0,     0,   191,   117,   118,   119,
     120,   121,   122,   110,   111,   112,   113,   114,   115,   116,
       0,     0,     0,     0,   117,   118,   119,   120,   121,   122,
     113,   114,   115,   116,     0,     0,     0,     0,   117,   118,
     119,   120,   121,   122
};

static const yytype_int16 yycheck[] =
{
       1,    27,    28,     4,     8,     8,    32,    59,     6,     8,
       5,    18,    39,     8,     9,    56,    11,    12,    17,    20,
       8,    45,    46,    11,    12,    66,    27,    28,    12,    70,
      71,    32,    39,     5,    60,    61,     8,     0,    39,    11,
      12,    44,    43,    95,     6,    86,    50,    45,    90,    91,
      51,    52,    93,    94,   141,    13,    57,     4,    44,    60,
      61,     8,    10,    49,    65,    51,    49,    41,    51,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,     5,   124,   171,     8,    14,    13,    89,    90,
      91,   178,     8,     8,    15,    42,    43,    44,    45,    46,
      47,    48,    45,    46,    47,    48,    53,    54,    55,    56,
      57,    58,     8,     8,    49,   202,     8,    16,   205,     7,
      28,    14,     8,   164,   165,    22,     4,    42,    43,    44,
      45,    46,    47,    48,    44,     8,   162,   138,    53,    54,
      55,    56,    57,    58,    21,    33,    34,    35,    36,    37,
       8,    39,   193,    23,     8,   196,     8,    15,    22,    39,
       8,   162,     9,   189,    42,    43,    44,    45,    46,    47,
      48,    29,    30,    31,    32,    53,    54,    55,    56,    57,
      58,    39,     8,    11,   174,     3,    88,   208,   189,   162,
      48,    49,   189,    61,    42,    43,    44,    45,    46,    47,
      48,    59,    60,    61,    62,    53,    54,    55,    56,    57,
      58,     8,     7,   140,   138,    -1,    42,    43,    44,    45,
      46,    47,    48,    -1,    -1,    -1,    -1,    53,    54,    55,
      56,    57,    58,     8,    -1,    -1,    -1,    -1,    33,    34,
      35,    36,    37,    38,    39,    42,    43,    44,    45,    46,
      47,    48,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      45,    46,    47,    48,    15,    -1,    -1,    -1,    53,    54,
      55,    56,    57,    58,    -1,    -1,    -1,    -1,    29,    30,
      31,    32,    -1,     1,    -1,    -1,    -1,    -1,    39,     7,
       8,    -1,    10,    11,     1,    -1,    -1,    48,    49,    -1,
       7,    -1,    -1,    -1,    11,    -1,    -1,     7,    59,    60,
      61,    62,    -1,    -1,    -1,    33,    34,    35,    36,    37,
      -1,    39,    -1,    -1,    -1,    -1,    33,    34,    35,    36,
      37,    -1,    39,    33,    34,    35,    36,    37,    38,    39,
       0,     1,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,
      -1,    11,    -1,    -1,    -1,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,     0,
       1,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    -1,
      11,    -1,    -1,    -1,    15,    16,    -1,    -1,    19,    20,
      -1,    -1,    -1,    24,    25,    26,    27,     0,     1,    -1,
       3,    -1,    -1,    -1,    -1,     8,    -1,     1,    11,     3,
      -1,    -1,    15,    -1,     8,    -1,    19,    20,    -1,    -1,
      -1,    24,    25,    26,    27,    19,    20,    -1,    -1,    -1,
      24,    25,    26,    27,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    42,    43,    44,    45,    46,    47,    48,    -1,
      50,    -1,    -1,    53,    54,    55,    56,    57,    58,    42,
      43,    44,    45,    46,    47,    48,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    57,    58,    42,    43,    44,    45,
      46,    47,    48,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    42,    43,    44,    45,    46,    47,    48,
      -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,    58,
      45,    46,    47,    48,    -1,    -1,    -1,    -1,    53,    54,
      55,    56,    57,    58
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     8,    11,    12,    64,    65,    66,    72,    73,
      74,    75,    85,    86,   105,    39,   100,    86,   100,     0,
       9,    66,    68,     8,    72,     8,    74,     6,    13,     1,
       7,     8,    11,    33,    34,    35,    36,    37,    70,    71,
     100,   101,   104,   107,   108,    65,   107,    38,    97,    99,
     106,   107,   107,    10,    70,     8,    44,    18,   100,    67,
      14,    41,   100,   100,    69,    15,    29,    30,    31,    32,
      48,    49,    59,    60,    61,    62,    76,    77,    80,    81,
      82,    83,    84,   100,   100,     1,     3,     8,    11,    15,
      19,    20,    24,    25,    26,    27,    87,    88,    90,    92,
      93,    38,    98,   107,    99,   100,    76,    76,    76,     8,
      42,    43,    44,    45,    46,    47,    48,    53,    54,    55,
      56,    57,    58,    49,    51,    76,    88,   100,    77,   100,
      77,   100,    76,     8,    76,     8,    92,     8,    17,   102,
     103,     8,    50,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    78,    79,    76,
       4,     8,    13,     8,    44,    51,     8,     8,     8,   104,
      91,    92,   102,    91,    41,     8,    50,    52,     8,    97,
      76,    76,    16,    89,    91,    28,    94,    79,    91,    14,
       8,    52,     8,    22,    96,    98,    44,    76,    21,    95,
      76,     4,     8,    23,     8,     8,    91,     8,    91,    96
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 11:
#line 168 "src/grammar.y"
    {afterendglobals = 1;}
    break;

  case 24:
#line 196 "src/grammar.y"
    { (yyval).ty = gInteger; ;}
    break;

  case 25:
#line 197 "src/grammar.y"
    { (yyval).ty = gReal; ;}
    break;

  case 26:
#line 198 "src/grammar.y"
    { (yyval).ty = gString; ;}
    break;

  case 27:
#line 199 "src/grammar.y"
    { (yyval).ty = gBoolean; ;}
    break;

  case 28:
#line 200 "src/grammar.y"
    { if (lookup(&functions, (yyvsp[(2) - (2)]).str) == NULL)
                         yyerrorex(3, "Undefined function.");
                       (yyval).ty = gCode;
                     ;}
    break;

  case 29:
#line 204 "src/grammar.y"
    { (yyval).ty = gNull; ;}
    break;

  case 30:
#line 205 "src/grammar.y"
    { checkcomparison((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); (yyval).ty = gBoolean; ;}
    break;

  case 31:
#line 206 "src/grammar.y"
    { checkcomparison((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); (yyval).ty = gBoolean; ;}
    break;

  case 32:
#line 207 "src/grammar.y"
    { checkcomparison((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); (yyval).ty = gBoolean; ;}
    break;

  case 33:
#line 208 "src/grammar.y"
    { checkcomparison((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); (yyval).ty = gBoolean; ;}
    break;

  case 34:
#line 209 "src/grammar.y"
    { checkeqtest((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); (yyval).ty = gBoolean; ;}
    break;

  case 35:
#line 210 "src/grammar.y"
    { checkeqtest((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); (yyval).ty = gBoolean; ;}
    break;

  case 36:
#line 211 "src/grammar.y"
    { canconvert((yyvsp[(1) - (3)]).ty, gBoolean); canconvert((yyvsp[(3) - (3)]).ty, gBoolean); (yyval).ty = gBoolean; ;}
    break;

  case 37:
#line 212 "src/grammar.y"
    { canconvert((yyvsp[(1) - (3)]).ty, gBoolean); canconvert((yyvsp[(3) - (3)]).ty, gBoolean); (yyval).ty = gBoolean; ;}
    break;

  case 38:
#line 213 "src/grammar.y"
    { canconvert((yyvsp[(2) - (2)]).ty, gBoolean); (yyval).ty = gBoolean; ;}
    break;

  case 39:
#line 214 "src/grammar.y"
    { (yyval).ty = binop((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); ;}
    break;

  case 40:
#line 215 "src/grammar.y"
    { (yyval).ty = binop((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); ;}
    break;

  case 41:
#line 216 "src/grammar.y"
    { (yyval).ty = binop((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); ;}
    break;

  case 42:
#line 217 "src/grammar.y"
    { 
                         if ((yyvsp[(1) - (3)]).ty == gString && (yyvsp[(3) - (3)]).ty == gString)
                           (yyval).ty = gString;
                         else
                           (yyval).ty = binop((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); ;}
    break;

  case 43:
#line 222 "src/grammar.y"
    { isnumeric((yyvsp[(2) - (2)]).ty); (yyval).ty = (yyvsp[(2) - (2)]).ty; ;}
    break;

  case 44:
#line 223 "src/grammar.y"
    { (yyval).ty = (yyvsp[(2) - (3)]).ty; ;}
    break;

  case 45:
#line 224 "src/grammar.y"
    { (yyval).ty = (yyvsp[(1) - (1)]).ty ;}
    break;

  case 46:
#line 225 "src/grammar.y"
    {
          const struct typeandname *tan = getVariable((yyvsp[(1) - (4)]).str);
          if (!tan->isarray) {
            char ebuf[1024];
            sprintf(ebuf, "%s not an array", (yyvsp[(1) - (4)]).str);
            yyerrorex(3, ebuf);
            (yyval).ty = tan->ty;
          }
          else {
            canconvert((yyvsp[(3) - (4)]).ty, gInteger);
            (yyval).ty = tan->ty;
          }
       ;}
    break;

  case 47:
#line 238 "src/grammar.y"
    {
          (yyval).ty = getVariable((yyvsp[(1) - (1)]).str)->ty;
       ;}
    break;

  case 48:
#line 241 "src/grammar.y"
    {yyerrorex(0, "Single = in expression, should probably be =="); checkeqtest((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).ty); (yyval).ty = gBoolean;;}
    break;

  case 49:
#line 242 "src/grammar.y"
    {yyerrorex(0, "')' missing"); (yyval).ty = (yyvsp[(2) - (2)]).ty;;}
    break;

  case 50:
#line 245 "src/grammar.y"
    {
          struct funcdecl *fd = lookup(&functions, (yyvsp[(1) - (4)]).str);
          if (fd == NULL) {
            char ebuf[1024];
            sprintf(ebuf, "Undeclared function %s", (yyvsp[(1) - (4)]).str);
            yyerrorex(3, ebuf);
            (yyval).ty = gNull;
          }
            else {
              checkParameters(fd->p, (yyvsp[(3) - (4)]).pl);
              (yyval).ty = fd->ret;
            }
       ;}
    break;

  case 51:
#line 258 "src/grammar.y"
    {
          yyerrorex(0, "')' missing");
          struct funcdecl *fd = lookup(&functions, (yyvsp[(1) - (4)]).str);
          if (fd == NULL) {
            char ebuf[1024];
            sprintf(ebuf, "Undeclared function %s", (yyvsp[(1) - (4)]).str);
            yyerrorex(3, ebuf);
            (yyval).ty = gNull;
          }
            else {
              checkParameters(fd->p, (yyvsp[(3) - (4)]).pl);
              (yyval).ty = fd->ret;
            }
       ;}
    break;

  case 52:
#line 274 "src/grammar.y"
    { (yyval).pl = newparamlist(); ;}
    break;

  case 53:
#line 275 "src/grammar.y"
    { (yyval).pl = (yyvsp[(1) - (1)]).pl; ;}
    break;

  case 54:
#line 278 "src/grammar.y"
    { (yyval).pl = newparamlist(); addParam((yyval).pl, newtypeandname((yyvsp[(1) - (1)]).ty, "")); ;}
    break;

  case 55:
#line 279 "src/grammar.y"
    { (yyval).pl = (yyvsp[(3) - (3)]).pl; addParam((yyval).pl, newtypeandname((yyvsp[(1) - (3)]).ty, "")); ;}
    break;

  case 56:
#line 283 "src/grammar.y"
    { (yyval).ty = gString; ;}
    break;

  case 57:
#line 286 "src/grammar.y"
    { (yyval).ty = gReal; ;}
    break;

  case 58:
#line 289 "src/grammar.y"
    { (yyval).ty = gBoolean; ;}
    break;

  case 61:
#line 296 "src/grammar.y"
    { (yyval).ty = gInteger; ;}
    break;

  case 62:
#line 297 "src/grammar.y"
    { (yyval).ty = gInteger; ;}
    break;

  case 63:
#line 301 "src/grammar.y"
    { (yyval).fd = (yyvsp[(1) - (1)]).fd; ;}
    break;

  case 64:
#line 302 "src/grammar.y"
    { (yyval).fd = (yyvsp[(2) - (2)]).fd; ;}
    break;

  case 65:
#line 306 "src/grammar.y"
    { 
(yyval).fd = newfuncdecl(); 
  (yyval).fd->name = strdup((yyvsp[(2) - (6)]).str);
  (yyval).fd->p = (yyvsp[(4) - (6)]).pl;
  (yyval).fd->ret = (yyvsp[(6) - (6)]).ty;
  put(&functions, (yyval).fd->name, (yyval).fd);
  //showfuncdecl($$.fd);
;}
    break;

  case 69:
#line 319 "src/grammar.y"
    {yyerrorex(0, "Statement outside of function");;}
    break;

  case 70:
#line 322 "src/grammar.y"
    {if((retval != gNothing) && (!canconvertreturn((yyvsp[(3) - (4)]).ty, retval) || ((yyvsp[(3) - (4)]).ty == gAny) )) yyerrorex(1, "Return types not correct or nonexistant returns");;}
    break;

  case 71:
#line 323 "src/grammar.y"
    {yyerrorex(0, "Missing endfunction"); clear(&params); clear(&locals); curtab = &globals;;}
    break;

  case 72:
#line 326 "src/grammar.y"
    { clear(&params); clear(&locals); curtab = &globals; ;}
    break;

  case 73:
#line 329 "src/grammar.y"
    {
  curtab = &locals;
  (yyval).fd = newfuncdecl(); 
  (yyval).fd->name = strdup((yyvsp[(2) - (6)]).str);
  (yyval).fd->p = (yyvsp[(4) - (6)]).pl;
  (yyval).fd->ret = (yyvsp[(6) - (6)]).ty;
  put(&functions, (yyval).fd->name, (yyval).fd);
  struct typeandname *tan = (yyvsp[(4) - (6)]).pl->head;
  for (;tan; tan=tan->next)
    put(&params, strdup(tan->name), newtypeandname(tan->ty, tan->name));
  retval = (yyval).fd->ret;
  inloop = 0;
  //showfuncdecl($$.fd);
;}
    break;

  case 74:
#line 345 "src/grammar.y"
    {(yyval).ty = gAny;;}
    break;

  case 75:
#line 346 "src/grammar.y"
    { if((yyvsp[(2) - (2)]).ty == gAny) (yyval).ty = (yyvsp[(1) - (2)]).ty; else (yyval).ty = (yyvsp[(2) - (2)]).ty;;}
    break;

  case 76:
#line 349 "src/grammar.y"
    {(yyval).ty = gAny;;}
    break;

  case 77:
#line 350 "src/grammar.y"
    { (yyval).ty = gNone;;}
    break;

  case 78:
#line 351 "src/grammar.y"
    { canconvert((yyvsp[(2) - (9)]).ty, gBoolean); (yyval).ty = combinetype((yyvsp[(6) - (9)]).ty!=gAny?combinetype((yyvsp[(5) - (9)]).ty, (yyvsp[(6) - (9)]).ty):(yyvsp[(5) - (9)]).ty, (yyvsp[(7) - (9)]).ty);;}
    break;

  case 79:
#line 352 "src/grammar.y"
    { canconvert((yyvsp[(4) - (5)]).ty, getVariable((yyvsp[(2) - (5)]).str)->ty); (yyval).ty = gNone; if (getVariable((yyvsp[(2) - (5)]).str)->isconst) { char ebuf[1024];
                  sprintf(ebuf, "Cannot assign to constant %s\n", (yyvsp[(2) - (5)]).str);
                  yyerrorex(3, ebuf);
									}
;}
    break;

  case 80:
#line 357 "src/grammar.y"
    { 
           const struct typeandname *tan = getVariable((yyvsp[(2) - (8)]).str);
           canconvert((yyvsp[(4) - (8)]).ty, gInteger); (yyval).ty = gNone;
           if (!tan->isarray) {
             char ebuf[1024];
             sprintf(ebuf, "%s not an array", (yyvsp[(2) - (8)]).str);
             yyerrorex(3, ebuf);
           }
           canconvert((yyvsp[(7) - (8)]).ty, tan->ty); ;}
    break;

  case 81:
#line 366 "src/grammar.y"
    {(yyval).ty = (yyvsp[(3) - (5)]).ty;;}
    break;

  case 82:
#line 367 "src/grammar.y"
    {(yyval).ty = (yyvsp[(3) - (3)]).ty; yyerrorex(0, "Missing endloop");;}
    break;

  case 83:
#line 368 "src/grammar.y"
    { canconvert((yyvsp[(2) - (3)]).ty, gBoolean); if (!inloop) yyerrorex(0, "Exitwhen outside of loop"); (yyval).ty = gNone;;}
    break;

  case 84:
#line 369 "src/grammar.y"
    { (yyval).ty = (yyvsp[(2) - (3)]).ty; if(retval == gNothing) yyerrorex(1, "Cannot return value from function that returns nothing");;}
    break;

  case 85:
#line 370 "src/grammar.y"
    { if (retval != gNothing) yyerrorex(1, "Return nothing in function that should return value"); (yyval).ty = gNone;;}
    break;

  case 86:
#line 371 "src/grammar.y"
    {(yyval).ty = gNone;;}
    break;

  case 87:
#line 372 "src/grammar.y"
    {canconvert((yyvsp[(2) - (7)]).ty, gBoolean); (yyval).ty = combinetype((yyvsp[(6) - (7)]).ty!=gAny?combinetype((yyvsp[(5) - (7)]).ty, (yyvsp[(6) - (7)]).ty):(yyvsp[(5) - (7)]).ty, (yyvsp[(7) - (7)]).ty); yyerrorex(0, "Missing endif");;}
    break;

  case 88:
#line 373 "src/grammar.y"
    {canconvert((yyvsp[(2) - (3)]).ty, gBoolean); (yyval).ty = gAny; yyerrorex(0, "Missing then or non valid expression");;}
    break;

  case 89:
#line 374 "src/grammar.y"
    {(yyval).ty = gNone; yyerrorex(0, "Call expected instead of set");;}
    break;

  case 90:
#line 375 "src/grammar.y"
    {(yyval).ty = gNone; ;}
    break;

  case 91:
#line 378 "src/grammar.y"
    {inloop++;;}
    break;

  case 92:
#line 381 "src/grammar.y"
    {inloop--;;}
    break;

  case 93:
#line 384 "src/grammar.y"
    {(yyval).ty = gNone;;}
    break;

  case 94:
#line 385 "src/grammar.y"
    {(yyval).ty = (yyvsp[(3) - (3)]).ty;;}
    break;

  case 95:
#line 388 "src/grammar.y"
    {(yyval).ty = gAny;;}
    break;

  case 96:
#line 389 "src/grammar.y"
    { canconvert((yyvsp[(2) - (6)]).ty, gBoolean); (yyval).ty = (yyvsp[(6) - (6)]).ty!=gAny?combinetype((yyvsp[(5) - (6)]).ty, (yyvsp[(6) - (6)]).ty):(yyvsp[(5) - (6)]).ty;;}
    break;

  case 97:
#line 392 "src/grammar.y"
    { (yyval).pl = (yyvsp[(1) - (1)]).pl; ;}
    break;

  case 98:
#line 393 "src/grammar.y"
    { (yyval).pl = newparamlist(); ;}
    break;

  case 99:
#line 396 "src/grammar.y"
    { (yyval).ty = gNothing; ;}
    break;

  case 100:
#line 397 "src/grammar.y"
    { (yyval).ty = (yyvsp[(1) - (1)]).ty; ;}
    break;

  case 101:
#line 400 "src/grammar.y"
    { (yyval).pl = newparamlist(); addParam((yyval).pl, (yyvsp[(1) - (1)]).tan); ;}
    break;

  case 102:
#line 401 "src/grammar.y"
    { addParam((yyvsp[(3) - (3)]).pl, (yyvsp[(1) - (3)]).tan); (yyval).pl = (yyvsp[(3) - (3)]).pl; ;}
    break;

  case 103:
#line 405 "src/grammar.y"
    { (yyval).str = strdup(yytext); ;}
    break;

  case 104:
#line 408 "src/grammar.y"
    {
  struct typeandname *tan = newtypeandname((yyvsp[(1) - (2)]).ty, (yyvsp[(2) - (2)]).str);
  (yyval).str = (yyvsp[(2) - (2)]).str;
  put(curtab, (yyvsp[(2) - (2)]).str, tan);  ;}
    break;

  case 105:
#line 412 "src/grammar.y"
    {
  if (afterendglobals) {
    yyerrorex(3, "Local constants are not allowed");
  }
  struct typeandname *tan = newtypeandname((yyvsp[(2) - (3)]).ty, (yyvsp[(3) - (3)]).str);
  (yyval).str = (yyvsp[(3) - (3)]).str;
  tan->isconst = 1;
  put(curtab, (yyvsp[(3) - (3)]).str, tan); ;}
    break;

  case 106:
#line 420 "src/grammar.y"
    {
  struct typeandname *tan = newtypeandname((yyvsp[(1) - (3)]).ty, (yyvsp[(3) - (3)]).str);
  (yyval).str = (yyvsp[(3) - (3)]).str;
  tan->isarray = 1;
  put(curtab, (yyvsp[(3) - (3)]).str, tan); ;}
    break;

  case 109:
#line 431 "src/grammar.y"
    { ;}
    break;

  case 111:
#line 435 "src/grammar.y"
    { (yyval).ty = gNothing; ;}
    break;

  case 112:
#line 436 "src/grammar.y"
    {
           const struct typeandname *tan = getVariable((yyvsp[(1) - (4)]).str);
           if (tan->isarray) {
             yyerrorex(3, "Arrays cannot be directly initialized");
           }
           canconvert((yyvsp[(3) - (4)]).ty, tan->ty);
           (yyval).ty = gNothing;
           ;}
    break;

  case 114:
#line 447 "src/grammar.y"
    {
  if (lookup(&types, (yyvsp[(2) - (4)]).str)) {
     char buf[1024];
     sprintf(buf, "Multiply defined type: %s", (yyvsp[(2) - (4)]).str);
     yyerrorex(3, buf);
  }
  else
    put(&types, (yyvsp[(2) - (4)]).str, newtypenode((yyvsp[(2) - (4)]).str, (yyvsp[(4) - (4)]).ty));
;}
    break;

  case 115:
#line 458 "src/grammar.y"
    { (yyval).tan = newtypeandname((yyvsp[(1) - (2)]).ty, (yyvsp[(2) - (2)]).str); ;}
    break;

  case 116:
#line 461 "src/grammar.y"
    { (yyval).ty = (yyvsp[(1) - (1)]).ty; ;}
    break;

  case 117:
#line 462 "src/grammar.y"
    {
   if (lookup(&types, (yyvsp[(1) - (1)]).str) == NULL) {
     char buf[1024];
     sprintf(buf, "Undefined type: %s", (yyvsp[(1) - (1)]).str);
     yyerrorex(3, buf);
     (yyval).ty = gNull;
   }
   else
     (yyval).ty = lookup(&types, (yyvsp[(1) - (1)]).str);
;}
    break;

  case 118:
#line 474 "src/grammar.y"
    { (yyval).ty = lookup(&types, yytext); ;}
    break;

  case 119:
#line 475 "src/grammar.y"
    { (yyval).ty = lookup(&types, yytext); ;}
    break;

  case 120:
#line 476 "src/grammar.y"
    { (yyval).ty = lookup(&types, yytext); ;}
    break;

  case 121:
#line 477 "src/grammar.y"
    { (yyval).ty = lookup(&types, yytext); ;}
    break;

  case 122:
#line 478 "src/grammar.y"
    { (yyval).ty = lookup(&types, yytext); ;}
    break;

  case 123:
#line 479 "src/grammar.y"
    { (yyval).ty = lookup(&types, yytext); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2344 "grammar.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



