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

/* Substitute the variable and function names.  */
#define yyparse zzparse
#define yylex   zzlex
#define yyerror zzerror
#define yylval  zzlval
#define yychar  zzchar
#define yydebug zzdebug
#define yynerrs zznerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     DOUBLE = 259,
     NAME = 260,
     STRING = 261,
     ENDCMD = 262,
     MODEL = 263,
     DATA = 264,
     IN = 265,
     TO = 266,
     INITS = 267,
     PARAMETERS = 268,
     COMPILE = 269,
     INITIALIZE = 270,
     ADAPT = 271,
     UPDATE = 272,
     BY = 273,
     MONITOR = 274,
     TYPE = 275,
     SET = 276,
     CLEAR = 277,
     THIN = 278,
     CODA = 279,
     STEM = 280,
     EXIT = 281,
     NCHAINS = 282,
     CHAIN = 283,
     LOAD = 284,
     LIST = 285,
     STRUCTURE = 286,
     DIM = 287,
     NA = 288,
     R_NULL = 289,
     DIMNAMES = 290,
     ITER = 291,
     ARROW = 292,
     ENDDATA = 293,
     ASINTEGER = 294
   };
#endif
/* Tokens.  */
#define INT 258
#define DOUBLE 259
#define NAME 260
#define STRING 261
#define ENDCMD 262
#define MODEL 263
#define DATA 264
#define IN 265
#define TO 266
#define INITS 267
#define PARAMETERS 268
#define COMPILE 269
#define INITIALIZE 270
#define ADAPT 271
#define UPDATE 272
#define BY 273
#define MONITOR 274
#define TYPE 275
#define SET 276
#define CLEAR 277
#define THIN 278
#define CODA 279
#define STEM 280
#define EXIT 281
#define NCHAINS 282
#define CHAIN 283
#define LOAD 284
#define LIST 285
#define STRUCTURE 286
#define DIM 287
#define NA 288
#define R_NULL 289
#define DIMNAMES 290
#define ITER 291
#define ARROW 292
#define ENDDATA 293
#define ASINTEGER 294




/* Copy the first part of user declarations.  */
#line 2 "parser.yy"

#include <config.h>

#include <cstdio>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <fstream>
#include <list>

#include <Console.h>
#include <compiler/ParseTree.h>
#include <sarray/nainf.h>
#include <cstdlib>
#include <cstring>
#include <ltdl.h>

//Required for warning about masked distributions after module loading
#include <deque>
#include <distribution/Distribution.h>
#include <compiler/Compiler.h>

#include "ReadData.h"

    typedef void(*pt2Func)();

    int zzerror(const char *);
    int zzlex();
#define YYERROR_VERBOSE 0
    static Console *console;
    static bool interactive = true;
    void setName(ParseTree *p, std::string *name);
    std::map<std::string, SArray> _data_table;
    std::deque<lt_dlhandle> _modules;
    bool open_data_buffer(std::string const *name);
    void return_to_main_buffer();
    void setMonitor(ParseTree const *var, int thin);
    void clearMonitor(ParseTree const *var);
    void doCoda (ParseTree const *var, std::string const &stem);
    void doAllCoda (std::string const &stem);
    void doDump (std::string const &file, DumpType type, unsigned int chain);
    void dumpMonitors(std::string const &file, std::string const &type,
		      unsigned int chain);

    static void errordump();
    static void updatestar(long niter, long refresh, int width);
    static void adaptstar(long niter, long refresh, int width);
    static void setParameters(ParseTree *p, ParseTree *param1);
    static void setParameters(ParseTree *p, std::vector<ParseTree*> *parameters);
    static void setParameters(ParseTree *p, ParseTree *param1, ParseTree *param2);
    static void loadModule(std::string const &name);
    

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
typedef union YYSTYPE
#line 61 "parser.yy"
{
  int intval;
  double val;
  std::string *stringptr;
  ParseTree *ptree;
  std::vector<ParseTree*> *pvec;
  std::vector<double> *vec;
  std::vector<long> *ivec;
}
/* Line 187 of yacc.c.  */
#line 248 "../../../JAGS/src/terminal/parser.cc"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 261 "../../../JAGS/src/terminal/parser.cc"

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   201

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNRULES -- Number of states.  */
#define YYNSTATES  218

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      41,    42,    46,     2,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    45,    47,
       2,    49,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    43,     2,    44,     2,     2,    48,     2,     2,    50,
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
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    12,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    33,    35,    37,    39,
      41,    43,    45,    49,    53,    55,    59,    63,    66,    70,
      79,    81,    85,    94,    98,   102,   104,   111,   113,   116,
     124,   127,   135,   137,   139,   144,   146,   150,   152,   156,
     158,   160,   162,   166,   175,   178,   186,   190,   201,   203,
     205,   208,   216,   219,   227,   230,   232,   235,   239,   243,
     247,   251,   253,   257,   264,   266,   268,   272,   276,   280,
     282,   284,   289,   291,   296,   298,   302,   304,   306,   310,
     312,   316,   318,   322,   324,   329,   331,   336,   341,   343,
     345,   350,   352,   356,   358,   363,   365
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      52,     0,    -1,    -1,    52,    53,    -1,     7,    -1,    54,
       7,    -1,     1,     7,    -1,    55,    -1,    56,    -1,    57,
      -1,    59,    -1,    60,    -1,    61,    -1,    63,    -1,    64,
      -1,    65,    -1,    66,    -1,    72,    -1,    75,    -1,    77,
      -1,    78,    -1,    67,    -1,     8,    10,    76,    -1,    58,
      79,    38,    -1,    58,    -1,     9,    11,    76,    -1,     9,
      10,    76,    -1,     9,    22,    -1,    62,    79,    38,    -1,
      62,    79,    38,    40,    28,    41,     3,    42,    -1,    62,
      -1,    13,    11,    76,    -1,    13,    11,    76,    40,    28,
      41,     3,    42,    -1,    13,    10,    76,    -1,    12,    10,
      76,    -1,    14,    -1,    14,    40,    27,    41,     3,    42,
      -1,    15,    -1,    16,     3,    -1,    16,     3,    40,    18,
      41,     3,    42,    -1,    17,     3,    -1,    17,     3,    40,
      18,    41,     3,    42,    -1,    26,    -1,     5,    -1,     5,
      43,    69,    44,    -1,    70,    -1,    69,    40,    70,    -1,
      71,    -1,    71,    45,    71,    -1,     3,    -1,    73,    -1,
      74,    -1,    19,    21,    68,    -1,    19,    21,    68,    40,
      23,    41,     3,    42,    -1,    19,    68,    -1,    19,    68,
      40,    23,    41,     3,    42,    -1,    19,    22,    68,    -1,
      19,    20,     5,    11,    76,    40,    28,    41,     3,    42,
      -1,     5,    -1,     6,    -1,    24,    68,    -1,    24,    68,
      40,    25,    41,    76,    42,    -1,    24,    46,    -1,    24,
      46,    40,    25,    41,    76,    42,    -1,    29,    76,    -1,
      80,    -1,    79,    80,    -1,    79,    47,    80,    -1,    81,
      37,    82,    -1,    81,    37,    85,    -1,    81,    37,     6,
      -1,     6,    -1,    48,     5,    48,    -1,    31,    41,    85,
      40,    83,    42,    -1,    84,    -1,    90,    -1,    83,    40,
      90,    -1,    83,    40,    84,    -1,    32,    49,    85,    -1,
      86,    -1,    87,    -1,    39,    41,    87,    42,    -1,    89,
      -1,    50,    41,    88,    42,    -1,    89,    -1,    88,    40,
      89,    -1,     4,    -1,    33,    -1,     5,    49,    93,    -1,
      92,    -1,    91,    40,    92,    -1,    93,    -1,     5,    49,
      93,    -1,    94,    -1,    39,    41,    94,    42,    -1,    96,
      -1,    30,    41,    91,    42,    -1,    31,    41,    91,    42,
      -1,    34,    -1,     4,    -1,    50,    41,    95,    42,    -1,
       4,    -1,    95,    40,     4,    -1,     6,    -1,    50,    41,
      97,    42,    -1,     6,    -1,    97,    40,     6,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   124,   124,   125,   128,   129,   130,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   150,   163,   171,   174,   180,   191,   197,   216,
     229,   232,   236,   242,   251,   263,   266,   271,   278,   282,
     287,   292,   297,   300,   303,   309,   312,   317,   320,   326,
     329,   330,   333,   336,   339,   342,   347,   352,   363,   364,
     367,   370,   373,   376,   381,   386,   389,   392,   397,   400,
     405,   413,   414,   419,   430,   431,   432,   433,   436,   441,
     442,   445,   448,   452,   458,   459,   462,   463,   469,   472,
     473,   476,   477,   480,   481,   482,   483,   484,   485,   488,
     489,   492,   493,   496,   497,   500,   501
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "DOUBLE", "NAME", "STRING",
  "ENDCMD", "MODEL", "DATA", "IN", "TO", "INITS", "PARAMETERS", "COMPILE",
  "INITIALIZE", "ADAPT", "UPDATE", "BY", "MONITOR", "TYPE", "SET", "CLEAR",
  "THIN", "CODA", "STEM", "EXIT", "NCHAINS", "CHAIN", "LOAD", "LIST",
  "STRUCTURE", "DIM", "NA", "R_NULL", "DIMNAMES", "ITER", "ARROW",
  "ENDDATA", "ASINTEGER", "','", "'('", "')'", "'['", "']'", "':'", "'*'",
  "';'", "'`'", "'='", "'c'", "$accept", "input", "line", "command",
  "model", "data_in", "data_to", "data", "data_clear", "parameters_in",
  "parameters_to", "parameters", "compile", "initialize", "adapt",
  "update", "exit", "var", "range_list", "range_element", "index",
  "monitor", "monitor_set", "monitor_clear", "monitor_to", "file_name",
  "coda", "load", "r_assignment_list", "r_assignment", "r_name",
  "r_structure", "r_attribute_list", "r_dim", "r_collection",
  "r_integer_collection", "r_value_collection", "r_value_list", "r_value",
  "r_generic_attribute", "r_generic_list", "r_generic_list_element",
  "r_generic_vector", "r_numeric_vector", "r_double_list",
  "r_character_vector", "r_string_list", 0
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
      44,    40,    41,    91,    93,    58,    42,    59,    96,    61,
      99
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    52,    53,    53,    53,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    55,    56,    56,    57,    58,    59,    60,    60,
      60,    61,    61,    62,    62,    63,    63,    64,    65,    65,
      66,    66,    67,    68,    68,    69,    69,    70,    70,    71,
      72,    72,    73,    73,    73,    73,    74,    75,    76,    76,
      77,    77,    77,    77,    78,    79,    79,    79,    80,    80,
      80,    81,    81,    82,    83,    83,    83,    83,    84,    85,
      85,    86,    87,    87,    88,    88,    89,    89,    90,    91,
      91,    92,    92,    93,    93,    93,    93,    93,    93,    94,
      94,    95,    95,    96,    96,    97,    97
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     1,     3,     3,     2,     3,     8,
       1,     3,     8,     3,     3,     1,     6,     1,     2,     7,
       2,     7,     1,     1,     4,     1,     3,     1,     3,     1,
       1,     1,     3,     8,     2,     7,     3,    10,     1,     1,
       2,     7,     2,     7,     2,     1,     2,     3,     3,     3,
       3,     1,     3,     6,     1,     1,     3,     3,     3,     1,
       1,     4,     1,     4,     1,     3,     1,     1,     3,     1,
       3,     1,     3,     1,     4,     1,     4,     4,     1,     1,
       4,     1,     3,     1,     4,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     4,     0,     0,     0,     0,    35,
      37,     0,     0,     0,     0,    42,     0,     3,     0,     7,
       8,     9,    24,    10,    11,    12,    30,    13,    14,    15,
      16,    21,    17,    50,    51,    18,    19,    20,     6,     0,
       0,     0,    27,     0,     0,     0,     0,    38,    40,    43,
       0,     0,     0,    54,    62,    60,    58,    59,    64,     5,
      71,     0,     0,    65,     0,     0,    22,    26,    25,    34,
      33,    31,     0,     0,     0,     0,     0,    52,    56,     0,
       0,     0,     0,    23,     0,    66,     0,    28,     0,     0,
       0,     0,    49,     0,    45,    47,     0,     0,     0,     0,
       0,    72,    67,    86,    70,     0,    87,     0,     0,    68,
      69,    79,    80,    82,     0,     0,     0,     0,     0,     0,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    36,     0,     0,    46,    48,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,     0,    39,    41,
       0,     0,    55,    63,    61,     0,    81,     0,    83,     0,
      32,     0,    53,     0,     0,     0,    74,    75,    85,    29,
       0,     0,     0,     0,    73,    57,    99,   103,     0,     0,
      98,     0,     0,    88,    93,    95,    78,    77,    76,     0,
       0,     0,     0,     0,     0,    89,    91,     0,     0,     0,
     101,   105,     0,     0,     0,     0,    96,    97,     0,    94,
       0,   100,     0,   104,    92,    90,   102,   106
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    53,    93,    94,
      95,    32,    33,    34,    35,    58,    36,    37,    62,    63,
      64,   109,   165,   166,   110,   111,   112,   144,   113,   167,
     194,   195,   196,   184,   202,   185,   203
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -158
static const yytype_int16 yypact[] =
{
    -158,    76,  -158,     4,  -158,    11,    88,    19,    25,   -22,
    -158,    21,    47,    91,    20,  -158,    69,  -158,   101,  -158,
    -158,  -158,    14,  -158,  -158,  -158,    14,  -158,  -158,  -158,
    -158,  -158,  -158,  -158,  -158,  -158,  -158,  -158,  -158,    69,
      69,    69,  -158,    69,    69,    69,    55,    74,    78,    72,
     115,   116,   116,    82,    83,    84,  -158,  -158,  -158,  -158,
    -158,   120,    17,  -158,    89,    22,  -158,  -158,  -158,  -158,
    -158,    87,    90,   110,   111,   127,   121,    93,  -158,   112,
     109,   113,    92,  -158,    14,  -158,    13,    96,   114,   134,
      98,   100,  -158,   -13,  -158,    99,    69,   122,   102,   105,
     106,  -158,  -158,  -158,  -158,   107,  -158,   108,   117,  -158,
    -158,  -158,  -158,  -158,   123,   118,   119,   147,   149,   127,
    -158,   127,   124,   125,   150,    69,    69,    -1,     8,    18,
     126,   151,  -158,   128,   129,  -158,  -158,   132,   152,   130,
     131,   133,   136,   135,    31,  -158,   153,   137,  -158,  -158,
     139,   140,  -158,  -158,  -158,    40,  -158,    18,  -158,   141,
    -158,   154,  -158,   138,   142,    38,  -158,  -158,  -158,  -158,
     143,     9,    -1,    40,  -158,  -158,  -158,  -158,   145,   148,
    -158,   155,   156,  -158,  -158,  -158,  -158,  -158,  -158,     3,
       3,     6,    75,   144,    61,  -158,  -158,    64,   157,   146,
    -158,  -158,    67,    77,     9,     3,  -158,  -158,   158,  -158,
     159,  -158,   162,  -158,  -158,  -158,  -158,  -158
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -158,  -158,  -158,  -158,  -158,  -158,  -158,  -158,  -158,  -158,
    -158,  -158,  -158,  -158,  -158,  -158,  -158,    16,  -158,    46,
      48,  -158,  -158,  -158,  -158,   -39,  -158,  -158,   164,    32,
    -158,  -158,  -158,     1,  -111,  -158,    50,  -158,  -103,    26,
      -9,   -21,  -157,    10,  -158,  -158,  -158
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      66,    67,    68,   103,    69,    70,    71,   176,   193,   177,
     176,    38,   103,   176,   183,   177,   142,   103,    46,   104,
      60,    39,   103,    60,    47,    49,   145,   119,    60,    43,
      55,   120,   106,   178,   179,    44,    45,   180,   107,   178,
     179,   106,   181,   180,   105,   163,   106,   214,   181,   108,
      48,   106,   107,   182,   168,    83,   198,   122,   108,   182,
      87,   186,    61,   108,    84,    61,    54,    77,    78,    84,
      61,   157,   164,   158,    56,    57,     2,     3,   173,   200,
     174,   201,    72,     4,     5,     6,   140,   141,     7,     8,
       9,    10,    11,    12,    85,    13,    49,    85,    40,    41,
      14,   205,    15,   206,   205,    16,   207,   210,    59,   211,
      42,    50,    51,    52,    73,    75,   102,   212,    74,   213,
      76,    49,    79,    80,    81,    82,    86,    88,    90,    91,
      92,    89,    96,    97,    99,    98,   114,   116,   100,   117,
     101,   118,   115,   124,   121,   123,   125,   126,   127,   128,
     133,   130,   134,   139,   147,   151,   159,   170,   129,   131,
     150,   132,   200,   216,   137,   135,   138,   146,   217,   136,
     148,   149,   152,   153,   187,   154,   155,   156,   143,   160,
     161,   197,   162,   169,   215,   175,   189,   171,   209,   190,
      65,   172,     0,   204,     0,     0,   191,   192,   208,   188,
       0,   199
};

static const yytype_int16 yycheck[] =
{
      39,    40,    41,     4,    43,    44,    45,     4,     5,     6,
       4,     7,     4,     4,   171,     6,   127,     4,    40,     6,
       6,    10,     4,     6,     3,     5,   129,    40,     6,    10,
      14,    44,    33,    30,    31,    10,    11,    34,    39,    30,
      31,    33,    39,    34,    31,     5,    33,   204,    39,    50,
       3,    33,    39,    50,   157,    38,    50,    96,    50,    50,
      38,   172,    48,    50,    47,    48,    46,    51,    52,    47,
      48,    40,    32,    42,     5,     6,     0,     1,    40,     4,
      42,     6,    27,     7,     8,     9,   125,   126,    12,    13,
      14,    15,    16,    17,    62,    19,     5,    65,    10,    11,
      24,    40,    26,    42,    40,    29,    42,    40,     7,    42,
      22,    20,    21,    22,    40,    43,    84,    40,    40,    42,
       5,     5,    40,    40,    40,     5,    37,    40,    18,    18,
       3,    41,    11,    40,    25,    23,    40,     3,    25,    41,
      48,    41,    28,    41,    45,    23,    41,    41,    41,    41,
       3,    28,     3,     3,     3,     3,     3,     3,    41,    41,
      28,    42,     4,     4,    40,   119,    41,    41,     6,   121,
      42,    42,    42,    42,   173,    42,    40,    42,   128,    42,
      41,   190,    42,    42,   205,    42,    41,    49,    42,    41,
      26,    49,    -1,    49,    -1,    -1,    41,    41,    41,   173,
      -1,   191
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    52,     0,     1,     7,     8,     9,    12,    13,    14,
      15,    16,    17,    19,    24,    26,    29,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    72,    73,    74,    75,    77,    78,     7,    10,
      10,    11,    22,    10,    10,    11,    40,     3,     3,     5,
      20,    21,    22,    68,    46,    68,     5,     6,    76,     7,
       6,    48,    79,    80,    81,    79,    76,    76,    76,    76,
      76,    76,    27,    40,    40,    43,     5,    68,    68,    40,
      40,    40,     5,    38,    47,    80,    37,    38,    40,    41,
      18,    18,     3,    69,    70,    71,    11,    40,    23,    25,
      25,    48,    80,     4,     6,    31,    33,    39,    50,    82,
      85,    86,    87,    89,    40,    28,     3,    41,    41,    40,
      44,    45,    76,    23,    41,    41,    41,    41,    41,    41,
      28,    41,    42,     3,     3,    70,    71,    40,    41,     3,
      76,    76,    85,    87,    88,    89,    41,     3,    42,    42,
      28,     3,    42,    42,    42,    40,    42,    40,    42,     3,
      42,    41,    42,     5,    32,    83,    84,    90,    89,    42,
       3,    49,    49,    40,    42,    42,     4,     6,    30,    31,
      34,    39,    50,    93,    94,    96,    85,    84,    90,    41,
      41,    41,    41,     5,    91,    92,    93,    91,    50,    94,
       4,     6,    95,    97,    49,    40,    42,    42,    41,    42,
      40,    42,    40,    42,    93,    92,     4,     6
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
        case 2:
#line 124 "parser.yy"
    {if (interactive) std::cout << ". " << std::flush;}
    break;

  case 3:
#line 125 "parser.yy"
    {if (interactive) std::cout << ". " << std::flush;}
    break;

  case 4:
#line 128 "parser.yy"
    {}
    break;

  case 5:
#line 129 "parser.yy"
    {}
    break;

  case 6:
#line 130 "parser.yy"
    {if(interactive) yyerrok; else exit(1);}
    break;

  case 22:
#line 150 "parser.yy"
    {
    std::FILE *file = std::fopen(((yyvsp[(3) - (3)].stringptr))->c_str(), "r");
    if (!file) {
	std::cerr << "Failed to open file " << *((yyvsp[(3) - (3)].stringptr)) << std::endl;
    }
    else {
	console->checkModel(file);
	std::fclose(file);
    }
    delete (yyvsp[(3) - (3)].stringptr);
}
    break;

  case 23:
#line 163 "parser.yy"
    {
  std::string rngname;
  readRData((yyvsp[(2) - (3)].pvec), _data_table, rngname);
  if (rngname.size() != 0) {
     std::cerr << "WARNING: .RNG.name assignment ignored" << std::endl;
  }
  delete (yyvsp[(2) - (3)].pvec);
}
    break;

  case 24:
#line 171 "parser.yy"
    {}
    break;

  case 25:
#line 174 "parser.yy"
    {
    doDump(*(yyvsp[(3) - (3)].stringptr), DUMP_DATA, 1);
    delete (yyvsp[(3) - (3)].stringptr);
}
    break;

  case 26:
#line 180 "parser.yy"
    {
  if(open_data_buffer((yyvsp[(3) - (3)].stringptr))) {
    std::cout << "Reading data file " << *(yyvsp[(3) - (3)].stringptr) << std::endl;
  }
  else {
    std::cerr << "Unable to open file " << *(yyvsp[(3) - (3)].stringptr) << std::endl << std::flush;
  }
  delete (yyvsp[(3) - (3)].stringptr);
}
    break;

  case 27:
#line 191 "parser.yy"
    {
  std::cout << "Clearing data table " << std::endl;
  _data_table.clear();
}
    break;

  case 28:
#line 198 "parser.yy"
    {
    std::map<std::string, SArray> parameter_table;
    std::string rngname;
    readRData((yyvsp[(2) - (3)].pvec), parameter_table, rngname);
    delete (yyvsp[(2) - (3)].pvec);
    /* Set all chains to the same state. If the user sets the
       RNG state in addition to the parameter values then all
       chains will be identical!
    */
    for (unsigned int i = 1; i <= console->nchain(); ++i) {
	/* We have to set the name first, because the state or seed
	   might be embedded in the parameter_table */
	if (rngname.size() != 0) {
	    console->setRNGname(rngname, i);
	}
	console->setParameters(parameter_table, i);
    }
}
    break;

  case 29:
#line 217 "parser.yy"
    {
    std::map<std::string, SArray> parameter_table;
    std::string rngname;
    readRData((yyvsp[(2) - (8)].pvec), parameter_table, rngname);
    delete (yyvsp[(2) - (8)].pvec);
    /* We have to set the name first, because the state or seed
       might be embedded in the parameter_table */
    if (rngname.size() != 0) {
        console->setRNGname(rngname, (yyvsp[(7) - (8)].intval));
    }
    console->setParameters(parameter_table, (yyvsp[(7) - (8)].intval));
}
    break;

  case 30:
#line 229 "parser.yy"
    {}
    break;

  case 31:
#line 232 "parser.yy"
    {
    doDump(*(yyvsp[(3) - (3)].stringptr), DUMP_PARAMETERS, 1);
    delete (yyvsp[(3) - (3)].stringptr);
}
    break;

  case 32:
#line 236 "parser.yy"
    {
    doDump(*(yyvsp[(3) - (8)].stringptr), DUMP_PARAMETERS, (yyvsp[(7) - (8)].intval));
    delete (yyvsp[(3) - (8)].stringptr);
}
    break;

  case 33:
#line 242 "parser.yy"
    {
  if(open_data_buffer((yyvsp[(3) - (3)].stringptr))) {
    std::cout << "Reading parameter file " << *(yyvsp[(3) - (3)].stringptr) << std::endl;
  }
  else {
    std::cerr << "Unable to open file " << *(yyvsp[(3) - (3)].stringptr) << std::endl << std::flush;
  }
  delete (yyvsp[(3) - (3)].stringptr);
}
    break;

  case 34:
#line 251 "parser.yy"
    {
  /* Legacy option to not break existing scripts */
  if(open_data_buffer((yyvsp[(3) - (3)].stringptr))) {
    std::cout << "Reading initial values file " << *(yyvsp[(3) - (3)].stringptr) << std::endl;
  }
  else {
    std::cerr << "Unable to open file " << *(yyvsp[(3) - (3)].stringptr) << std::endl << std::flush;
  }
  delete (yyvsp[(3) - (3)].stringptr);
}
    break;

  case 35:
#line 263 "parser.yy"
    {
  console->compile(_data_table, 1, true);
}
    break;

  case 36:
#line 266 "parser.yy"
    {
    console->compile(_data_table, (yyvsp[(5) - (6)].intval), true);
}
    break;

  case 37:
#line 271 "parser.yy"
    {
  if (!console->initialize()) {
    errordump();
  }
}
    break;

  case 38:
#line 278 "parser.yy"
    {
    long refresh = (yyvsp[(2) - (2)].intval)/40;
    adaptstar((yyvsp[(2) - (2)].intval), refresh, 40);
}
    break;

  case 39:
#line 282 "parser.yy"
    {
    adaptstar((yyvsp[(2) - (7)].intval),(yyvsp[(6) - (7)].intval),40);
}
    break;

  case 40:
#line 287 "parser.yy"
    {
  //fixme: put refresh option
  long refresh = (yyvsp[(2) - (2)].intval)/40;
  updatestar((yyvsp[(2) - (2)].intval), refresh, 40);
}
    break;

  case 41:
#line 292 "parser.yy"
    {
  updatestar((yyvsp[(2) - (7)].intval),(yyvsp[(6) - (7)].intval), 40);
}
    break;

  case 42:
#line 297 "parser.yy"
    {return 0;}
    break;

  case 43:
#line 300 "parser.yy"
    {
  (yyval.ptree) = new ParseTree(P_VAR); setName((yyval.ptree), (yyvsp[(1) - (1)].stringptr));
}
    break;

  case 44:
#line 303 "parser.yy"
    {
  (yyval.ptree) = new ParseTree(P_VAR); setName((yyval.ptree), (yyvsp[(1) - (4)].stringptr));
  setParameters((yyval.ptree), (yyvsp[(3) - (4)].pvec));
}
    break;

  case 45:
#line 309 "parser.yy"
    {
  (yyval.pvec) = new std::vector<ParseTree*>(1, (yyvsp[(1) - (1)].ptree)); 
}
    break;

  case 46:
#line 312 "parser.yy"
    {
  (yyval.pvec)=(yyvsp[(1) - (3)].pvec); (yyval.pvec)->push_back((yyvsp[(3) - (3)].ptree));
}
    break;

  case 47:
#line 317 "parser.yy"
    {
  (yyval.ptree) = new ParseTree(P_RANGE); setParameters((yyval.ptree),(yyvsp[(1) - (1)].ptree));
}
    break;

  case 48:
#line 320 "parser.yy"
    {
  (yyval.ptree) = new ParseTree(P_RANGE); setParameters((yyval.ptree), (yyvsp[(1) - (3)].ptree), (yyvsp[(3) - (3)].ptree));
}
    break;

  case 49:
#line 326 "parser.yy"
    {(yyval.ptree) = new ParseTree(P_VALUE); (yyval.ptree)->setValue((yyvsp[(1) - (1)].intval));}
    break;

  case 52:
#line 333 "parser.yy"
    { 
  setMonitor((yyvsp[(3) - (3)].ptree), 1); delete (yyvsp[(3) - (3)].ptree);
}
    break;

  case 53:
#line 336 "parser.yy"
    { 
  setMonitor((yyvsp[(3) - (8)].ptree), (yyvsp[(7) - (8)].intval)); delete (yyvsp[(3) - (8)].ptree);
}
    break;

  case 54:
#line 339 "parser.yy"
    {
  setMonitor((yyvsp[(2) - (2)].ptree), 1); delete (yyvsp[(2) - (2)].ptree);
}
    break;

  case 55:
#line 342 "parser.yy"
    { 
  setMonitor((yyvsp[(2) - (7)].ptree), (yyvsp[(6) - (7)].intval)); delete (yyvsp[(2) - (7)].ptree);
}
    break;

  case 56:
#line 347 "parser.yy"
    {
  clearMonitor((yyvsp[(3) - (3)].ptree)); delete (yyvsp[(3) - (3)].ptree);
}
    break;

  case 57:
#line 352 "parser.yy"
    {
    dumpMonitors(*(yyvsp[(5) - (10)].stringptr), *(yyvsp[(3) - (10)].stringptr), (yyvsp[(9) - (10)].intval));
    delete (yyvsp[(3) - (10)].stringptr);
    delete (yyvsp[(5) - (10)].stringptr); 
}
    break;

  case 58:
#line 363 "parser.yy"
    { (yyval.stringptr) = (yyvsp[(1) - (1)].stringptr);}
    break;

  case 59:
#line 364 "parser.yy"
    { (yyval.stringptr) = (yyvsp[(1) - (1)].stringptr); }
    break;

  case 60:
#line 367 "parser.yy"
    {
  doCoda ((yyvsp[(2) - (2)].ptree), "CODA"); delete (yyvsp[(2) - (2)].ptree);
}
    break;

  case 61:
#line 370 "parser.yy"
    {
  doCoda ((yyvsp[(2) - (7)].ptree), *(yyvsp[(6) - (7)].stringptr)); delete (yyvsp[(2) - (7)].ptree); delete (yyvsp[(6) - (7)].stringptr);
}
    break;

  case 62:
#line 373 "parser.yy"
    {
  doAllCoda ("CODA"); 
}
    break;

  case 63:
#line 376 "parser.yy"
    {
  doAllCoda (*(yyvsp[(6) - (7)].stringptr)); delete (yyvsp[(6) - (7)].stringptr); 
}
    break;

  case 64:
#line 381 "parser.yy"
    { loadModule(*(yyvsp[(2) - (2)].stringptr)); }
    break;

  case 65:
#line 386 "parser.yy"
    {
  (yyval.pvec) = new std::vector<ParseTree*>(1, (yyvsp[(1) - (1)].ptree));
}
    break;

  case 66:
#line 389 "parser.yy"
    {
  (yyval.pvec) = (yyvsp[(1) - (2)].pvec); (yyval.pvec)->push_back((yyvsp[(2) - (2)].ptree));
}
    break;

  case 67:
#line 392 "parser.yy"
    {
  (yyval.pvec) = (yyvsp[(1) - (3)].pvec); (yyval.pvec)->push_back((yyvsp[(3) - (3)].ptree));
}
    break;

  case 68:
#line 397 "parser.yy"
    {
  (yyval.ptree) = (yyvsp[(3) - (3)].ptree); setName((yyval.ptree), (yyvsp[(1) - (3)].stringptr));
}
    break;

  case 69:
#line 400 "parser.yy"
    {
  (yyval.ptree) = new ParseTree(P_ARRAY);
  setName((yyval.ptree), (yyvsp[(1) - (3)].stringptr));
  setParameters((yyval.ptree), (yyvsp[(3) - (3)].ptree));
}
    break;

  case 70:
#line 405 "parser.yy"
    {
  /* Allow this for setting the NAME of the random number generator */
  (yyval.ptree) = new ParseTree(P_VAR); setName((yyval.ptree), (yyvsp[(1) - (3)].stringptr));
  ParseTree *p = new ParseTree(P_VAR); setName(p, (yyvsp[(3) - (3)].stringptr));
  setParameters((yyval.ptree), p);
}
    break;

  case 72:
#line 414 "parser.yy"
    {
    /* R >= 2.4.0 uses backticks for quoted names */
    (yyval.stringptr) = (yyvsp[(2) - (3)].stringptr);
}
    break;

  case 73:
#line 419 "parser.yy"
    {
  (yyval.ptree) = new ParseTree(P_ARRAY); 
  if ((yyvsp[(5) - (6)].ptree)) 
    setParameters((yyval.ptree), (yyvsp[(3) - (6)].ptree), (yyvsp[(5) - (6)].ptree));
  else
    setParameters((yyval.ptree), (yyvsp[(3) - (6)].ptree));
}
    break;

  case 75:
#line 431 "parser.yy"
    {(yyval.ptree)=0;}
    break;

  case 77:
#line 433 "parser.yy"
    {(yyval.ptree)=(yyvsp[(3) - (3)].ptree);}
    break;

  case 78:
#line 436 "parser.yy"
    {
  (yyval.ptree) = (yyvsp[(3) - (3)].ptree);
}
    break;

  case 81:
#line 445 "parser.yy"
    {(yyval.ptree) = (yyvsp[(3) - (4)].ptree);}
    break;

  case 82:
#line 448 "parser.yy"
    { 
  (yyval.ptree) = new ParseTree(P_VECTOR); 
  setParameters((yyval.ptree), (yyvsp[(1) - (1)].ptree));
}
    break;

  case 83:
#line 452 "parser.yy"
    {
  (yyval.ptree) = new ParseTree(P_VECTOR);
  setParameters((yyval.ptree), (yyvsp[(3) - (4)].pvec));
}
    break;

  case 84:
#line 458 "parser.yy"
    {(yyval.pvec) = new std::vector<ParseTree*>(1, (yyvsp[(1) - (1)].ptree)); }
    break;

  case 85:
#line 459 "parser.yy"
    {(yyval.pvec) = (yyvsp[(1) - (3)].pvec); (yyval.pvec)->push_back((yyvsp[(3) - (3)].ptree));}
    break;

  case 86:
#line 462 "parser.yy"
    {(yyval.ptree) = new ParseTree(P_VALUE); (yyval.ptree)->setValue((yyvsp[(1) - (1)].val));}
    break;

  case 87:
#line 463 "parser.yy"
    {(yyval.ptree) = new ParseTree(P_VALUE); (yyval.ptree)->setValue(JAGS_NA);}
    break;

  case 88:
#line 469 "parser.yy"
    {;}
    break;

  case 89:
#line 472 "parser.yy"
    {;}
    break;

  case 90:
#line 473 "parser.yy"
    {;}
    break;

  case 91:
#line 476 "parser.yy"
    {;}
    break;

  case 92:
#line 477 "parser.yy"
    {;}
    break;

  case 93:
#line 480 "parser.yy"
    {;}
    break;

  case 94:
#line 481 "parser.yy"
    {;}
    break;

  case 95:
#line 482 "parser.yy"
    {;}
    break;

  case 96:
#line 483 "parser.yy"
    {;}
    break;

  case 97:
#line 484 "parser.yy"
    {;}
    break;

  case 98:
#line 485 "parser.yy"
    {;}
    break;

  case 99:
#line 488 "parser.yy"
    {;}
    break;

  case 101:
#line 492 "parser.yy"
    {;}
    break;

  case 102:
#line 493 "parser.yy"
    {;}
    break;

  case 103:
#line 496 "parser.yy"
    {;}
    break;

  case 104:
#line 497 "parser.yy"
    {;}
    break;

  case 105:
#line 500 "parser.yy"
    {;}
    break;

  case 106:
#line 501 "parser.yy"
    {;}
    break;


/* Line 1267 of yacc.c.  */
#line 2231 "../../../JAGS/src/terminal/parser.cc"
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


#line 504 "parser.yy"


int zzerror (const char *s)
{
  return_to_main_buffer();
  std::cerr << s << std::endl;
  return 0;
}

static Range getRange(ParseTree const *var)
{
  /* 
     Blank arguments, e.g. foo[] or bar[,1]  are not allowed.
  */
  unsigned int size = var->parameters().size();

  std::vector<int>  ind_lower(size), ind_upper(size);
  for (unsigned int i = 0; i < size; ++i) {
    ParseTree const *range_element = var->parameters()[i];
    switch(range_element->parameters().size()) {
    case 1:
      ind_lower[i] = (int) (range_element->parameters()[0]->value() + 1.0E-6);
      ind_upper[i] = ind_lower[i];
      break;
    case 2:
      ind_lower[i] = (int) (range_element->parameters()[0]->value() + 1.0E-6);  
      ind_upper[i] = (int) (range_element->parameters()[1]->value() + 1.0E-6);
      break;
    default:
      //Error! FIXME
      break;
    }
  }
  return Range(ind_lower, ind_upper);
}

void setMonitor(ParseTree const *var, int thin)
{
    std::string const &name = var->name();
    if (var->parameters().empty()) {
	/* Requesting the whole node */
	console->setMonitor(name, Range(), thin, "trace");
    }
    else {
	/* Requesting subset of a multivariate node */
	console->setMonitor(name, getRange(var), thin, "trace");
    }
}

void clearMonitor(ParseTree const *var)
{
    std::string const &name = var->name();
    if (var->parameters().empty()) {
	/* Requesting the whole node */
	console->clearMonitor(name, Range(), "trace");
    }
    else {
	/* Requesting subset of a multivariate node */
	console->clearMonitor(name, getRange(var), "trace");
    }
}

/*
void doCoda (ParseTree const *var, std::string const &stem)
{
  unsigned int nchain = console->nchain();

  for (unsigned int n = 0; n < nchain; ++n) {
      std::ostringstream outstream;
    outstream << stem << n << ".out";
    std::string outname = outstream.str();
    std::ofstream out(outname.c_str());
    if (!out) {
      std::cerr << "Failed to open file " << outname << std::endl;
      return;
    }
    std::ostringstream indstream;
    indstream << stem << n << ".ind";
    std::string indname = indstream.str();
    std::ofstream ind(indname.c_str());
    if (!ind) {
      std::cerr << "Failed to open file " << indname << std::endl;
      out.close();
      return;
    }

    if (var->parameters().empty()) {
      console->coda(var->name(), Range(), out, ind, n);
    }
    else {
      console->coda(var->name(), getRange(var), out, ind, n);
    }
    out.close();
    ind.close();
  }
}
*/

void doAllCoda (std::string const &stem)
{

    /* Open index file */
    std::ostringstream indstream;
    indstream << stem << "index.txt";
    std::string indname = indstream.str();
    std::ofstream ind(indname.c_str());
    if (!ind) {
      std::cerr << "Failed to open file " << indname << std::endl;
      return;
    }

    std::vector<std::ofstream*> output;
    for (unsigned int n = 0; n < console->nchain(); ++n) {
	/* Open output file */
	std::ostringstream outstream;
	outstream << stem << "chain" << n + 1 << ".txt";
	std::string outname = outstream.str();
	std::ofstream *out = new std::ofstream(outname.c_str());
        if (*out) {
            output.push_back(out);
        }
	else {
	    ind.close();
	    for (unsigned int i = 0; i < output.size(); ++i) {
		output[i]->close();
	    }
	    std::cerr << "Failed to open file " << outname << std::endl;
	    return;
	}
    }
    console->coda(ind, output);
    ind.close();
    for (unsigned int i = 0; i < output.size(); ++i) {
	output[i]->close();
        delete output[i];
    }
}

void doCoda (ParseTree const *var, std::string const &stem)
{

    /* Open index file */
    std::ostringstream indstream;
    indstream << stem << "index.txt";
    std::string indname = indstream.str();
    std::ofstream ind(indname.c_str());
    if (!ind) {
      std::cerr << "Failed to open file " << indname << std::endl;
      return;
    }

    std::vector<std::ofstream*> output;
    for (unsigned int n = 0; n < console->nchain(); ++n) {
	/* Open output file */
	std::ostringstream outstream;
	outstream << stem << "chain" << n + 1 << ".txt";
	std::string outname = outstream.str();
	std::ofstream *out = new std::ofstream(outname.c_str());

        if (*out) {
            output.push_back(out);
        }
        else {
            ind.close();
            for (unsigned int i = 0; i < output.size(); ++i) {
                output[i]->close();
            }
            std::cerr << "Failed to open file " << outname << std::endl;
            return;
        }
    }

    //FIXME: Allow list of several nodes

    std::vector<std::pair<std::string, Range> > dmp;
    if (var->parameters().empty()) {
	/* Requesting the whole node */
	dmp.push_back(std::pair<std::string,Range>(var->name(), Range()));
    }
    else {
	/* Requesting subset of a multivariate node */
	dmp.push_back(std::pair<std::string,Range>(var->name(), getRange(var)));
    }
    console->coda(dmp, ind, output);

    ind.close();
    for (unsigned int i = 0; i < output.size(); ++i) {
	output[i]->close();
        delete output[i];
    }
}

/* Helper function for doDump that handles all the special cases
   (missing values etc) when writing a double value */
static void writeValue(double x, std::ostream &out, bool isdiscrete)
{
  using namespace std;

  if (x == JAGS_NA) {
    out << "NA";
  }
  else if (jags_isnan(x)) {
    out << "NaN";
  }
  else if (!jags_finite(x)) {
    if (x > 0)
      out << "Inf";
    else
      out << "-Inf";
  }
  else if (isdiscrete) {
      out << static_cast<int>(x);
  }
  else {
    out << x;
  }
}

void doDump(std::string const &file, DumpType type, unsigned int chain)
{
  std::map<std::string,SArray> data_table;
  std::string rng_name;
  if (!console->dumpState(data_table, rng_name, type, chain)) {
    return;
  }

  /* Open output file */
  std::ofstream out(file.c_str());
  if (!out) {
    std::cerr << "Failed to open file " << file << std::endl;
    return;
  }
  
  if (rng_name.size() != 0) {
    out << "\".RNG.name\" <- \"" << rng_name << "\"\n";
  }

  for (std::map<std::string, SArray>::const_iterator p = data_table.begin();
       p != data_table.end(); ++p) {
    std::string const &name = p->first;
    SArray const &sarray = p->second;
    double const *value = sarray.value();
    long length = sarray.length();
    out << "\"" << name << "\" <- " << std::endl;
    std::vector<unsigned int> const &dim = sarray.dim(false);
    bool discrete = sarray.isDiscreteValued();

    if (discrete) {
       out << "as.integer(";
    }
    if (dim.size() == 1) {
      // Vector 
      if (dim[0] == 1) {
	// Scalar
	writeValue(value[0], out, sarray.isDiscreteValued());
      }
      else {
	// Vector of length > 1
	out << "c(";
	for (int i = 0; i < length; ++i) {
	  if (i > 0) {
	    out << ",";
	  }
	  writeValue(value[i], out, sarray.isDiscreteValued());
	}
	out << ")";
      }
    }
    else {
      // Array 
      out << "structure(c(";
      for (int i = 0; i < length; ++i) {
	if (i > 0) {
	  out << ",";
	}
	writeValue(value[i], out, sarray.isDiscreteValued());
      }
      out << "), .Dim = as.integer(c(";
      for (unsigned int j = 0; j < dim.size(); ++j) {
	if (j > 0) {
	  out << ",";
	}
	out << dim[j];
      }
      out << ")))";
    }
    if (discrete) 
      out << ")";
    out << "\n";
  }
  out.close();
}  

void dumpMonitors(std::string const &file, std::string const &type, 
		  unsigned int chain)
{
    std::map<std::string,SArray> data_table;
    std::string rng_name;
    if (!console->dumpMonitors(data_table, type, chain)) {
	return;
    }

    /* Open output file */
    std::ofstream out(file.c_str());
    if (!out) {
	std::cerr << "Failed to open file " << file << std::endl;
	return;
    }

    //FIXME: cut-and-paste from doDump
    for (std::map<std::string, SArray>::const_iterator p = data_table.begin();
	 p != data_table.end(); ++p) {
	std::string const &name = p->first;
	SArray const &sarray = p->second;
	double const *value = sarray.value();
	long length = sarray.length();
	out << "\"" << name << "\" <- " << std::endl;
	std::vector<unsigned int> const &dim = sarray.dim(false);
	bool discrete = sarray.isDiscreteValued();

	if (discrete) {
	    out << "as.integer(";
	}
	if (dim.size() == 1) {
	    // Vector 
	    if (dim[0] == 1) {
		// Scalar
		writeValue(value[0], out, sarray.isDiscreteValued());
	    }
	    else {
		// Vector of length > 1
		out << "c(";
		for (int i = 0; i < length; ++i) {
		    if (i > 0) {
			out << ",";
		    }
		    writeValue(value[i], out, sarray.isDiscreteValued());
		}
		out << ")";
	    }
	}
	else {
	    // Array 
	    out << "structure(c(";
	    for (int i = 0; i < length; ++i) {
		if (i > 0) {
		    out << ",";
		}
		writeValue(value[i], out, sarray.isDiscreteValued());
	    }
	    out << "), .Dim = as.integer(c(";
	    for (unsigned int j = 0; j < dim.size(); ++j) {
		if (j > 0) {
		    out << ",";
		}
		out << dim[j];
	    }
	    out << ")))";
	}
	if (discrete) 
	    out << ")";
	out << "\n";
    }
    out.close();
}

void setParameters(ParseTree *p, std::vector<ParseTree*> *parameters)
{
  /* 
     The parser dynamically allocates vectors of (pointers to)
     parameters. These vectors must be deleted when we are done with
     them.
  */
  p->setParameters(*parameters);
  delete parameters; 
}

void setParameters(ParseTree *p, ParseTree *param1)
{
  /*
    Wrapper function that creates a vector containing param1
    to be passed to ParseTree::setParameters.
  */
  std::vector<ParseTree *> parameters(1, param1);
  p->setParameters(parameters);
}

void setParameters(ParseTree *p, ParseTree *param1, ParseTree *param2)
{
  /*
    Wrapper function that creates a vector containing param1
    and param2, to be passed to ParseTree::setParameters
  */
  std::vector<ParseTree *> parameters;
  parameters.push_back(param1);
  parameters.push_back(param2);
  p->setParameters(parameters);
}

void setName(ParseTree *p, std::string *name)
{
  p->setName(*name);
  delete name;
}

static void errordump()
{
  if (console->model()) {
    std::ostringstream fname;
    for (unsigned int i = 1; i <= console->nchain(); ++i) {
       fname << "jags.dump" << i << ".R";
       std::cout << "Dumping chain " << i << " at iteration " 
                 << console->iter() << " to file " << fname.str() << std::endl;
       doDump(fname.str(), DUMP_ALL, i);
       fname.str("");
    }
  }
}

static void updatestar(long niter, long refresh, int width)
{
    bool adapt = console->isAdapting();
	
    if (refresh == 0) {
	console->update(niter/2);
	bool status = true;
	if (adapt && !console->adaptOff(status)) {
	    errordump();
	    return;
	}
	console->update(niter - niter/2);
	if (!status) {
	    std::cerr << "WARNING: Adaptation incomplete\n";
	}
	return;
    }

    if (width > niter / refresh + 1)
	width = niter / refresh + 1;

    std::cout << "Updating " << niter << std::endl;
    for (int i = 0; i < width - 1; ++i) {
	std::cout << "-";
    }
    std::cout << "| " << std::min(width * refresh, niter) << std::endl 
	      << std::flush;

    int col = 0;
    bool status = true;
    for (long n = niter; n > 0; n -= refresh) {
	if (adapt && n <= niter/2) {
	    // Turn off adaptive mode half way through burnin
	    if (console->adaptOff(status)) {
		adapt = false;
	    }
	    else {
		std::cout << std::endl;
		errordump();
		return;
	    }
	}
	long nupdate = std::min(n, refresh);
	if(console->update(nupdate))
	    std::cout << "*" << std::flush;
	else {
	    std::cout << std::endl;
	    errordump();
	    return;
	}
	col++;
	if (col == width || n <= nupdate) {
	    int percent = 100 - (n-nupdate) * 100/niter;
	    std::cout << " " << percent << "%" << std::endl;
	    if (n > nupdate) {
		col = 0;
	    }
	}
    }
    if (!status) {
	std::cerr << "WARNING: Adaptation incomplete\n";
    }
}

static void adaptstar(long niter, long refresh, int width)
{
    if (!console->isAdapting()) {
	std::cerr << "ERROR: Model is not in adaptive mode\n";
    }
	
    bool status = true;
    if (refresh == 0) {
	console->update(niter);
	if (!console->adaptOff(status)) {
	    errordump();
	    return;
	}
	if (!status) {
	    std::cerr << "WARNING: Adaptation incomplete\n";
	}
	return;
    }

    if (width > niter / refresh + 1)
	width = niter / refresh + 1;

    std::cout << "Adapting " << niter << std::endl;
    for (int i = 0; i < width - 1; ++i) {
	std::cout << "-";
    }
    std::cout << "| " << std::min(width * refresh, niter) << std::endl 
	      << std::flush;

    int col = 0;
    for (long n = niter; n > 0; n -= refresh) {
	long nupdate = std::min(n, refresh);
	if(console->update(nupdate))
	    std::cout << "+" << std::flush;
	else {
	    std::cout << std::endl;
	    errordump();
	    return;
	}
	col++;
	if (col == width || n <= nupdate) {
	    int percent = 100 - (n-nupdate) * 100/niter;
	    std::cout << " " << percent << "%" << std::endl;
	    if (n > nupdate) {
		col = 0;
	    }
	}
    }
    if (!console->adaptOff(status)) {
	std::cout << std::endl;
	errordump();
	return;
    }
    if (!status) {
	std::cerr << "WARNING: Adaptation incomplete\n";
    }
}

static void loadModule(std::string const &name)
{
  //Record number of masked distributions before loading module
  std::list<Distribution const *> const &masked_dist = 
     Compiler::distTab().masked();
  unsigned int n_dist = masked_dist.size(); 
  lt_dlhandle mod = lt_dlopenext(name.c_str());
  if (mod == NULL) {
      std::cout << lt_dlerror() << std::endl;
  }
  else {
      _modules.push_front(mod);
      std::cout << "Loading module: " << name << std::endl;
      //Warn about newly masked distributions
      if (masked_dist.size() > n_dist) {
	  std::cerr << "Warning: the following distributions were masked "
		    << "by module " << name << ":\n";
	  std::list<Distribution const *>::const_iterator p = 
             masked_dist.begin();
	  for(unsigned int i = masked_dist.size(); i > n_dist; --i, ++p) {
	      std::cerr << (*p)->name() << "\n";
	  }
	  std::cerr << std::endl;
      }
  }
}

int main (int argc, char **argv)
{
  extern std::FILE *zzin;

  std::FILE *cmdfile = 0;
  if (argc > 2) {
    std::cerr << "Too many arguments" << std::endl;
  }
  else if (argc == 2) {
    interactive = false;
    cmdfile = std::fopen(argv[1],"r");
    if (cmdfile) {
      zzin = cmdfile;
    }
    else {
      std::cerr << "Unable to open command file " << argv[1] << std::endl;
      return 1;
    }
  }

  if(lt_dlinit()) {
      std::cerr << lt_dlerror() << std::endl;
      return 1;
  }

  /*
  pt2Func load_base = (pt2Func)(lt_dlsym(base, "load"));
  if (load_base == NULL) {
      std::cout << lt_dlerror() << std::endl;
      return 1;
  }
  else{
      (*load_base)();
  }
  */
  
  time_t t;
  time(&t);
  std::cout << "Welcome to " << PACKAGE_STRING << " on " << ctime(&t);
  std::cout << "JAGS is free software and comes with ABSOLUTELY NO WARRANTY" 
            << std::endl;

  if (getenv("JAGS_HOME")) {
     /* Locate configuration file containing names of modules to load */
     std::string conf_name = std::string(getenv("JAGS_HOME")) + 
                             "/etc/modules.conf";
     std::ifstream conf_file(conf_name.c_str());
     if (conf_file) {
        std::string module_name;
        while((conf_file>> module_name)) {
           loadModule(module_name);
        }
     }
  }
  else {
    std::cerr << "Cannot find JAGS_HOME" << std::endl;
  }

  console = new Console(std::cout, std::cerr);

  zzparse();
  if (argc==2) {
      std::fclose(cmdfile);
  }
  delete console;
  //We have to unload modules *AFTER* deleting the console. 
  for (unsigned int i = 0; i < _modules.size(); ++i) {
      lt_dlclose(_modules[i]);
  }
  lt_dlexit();
}


