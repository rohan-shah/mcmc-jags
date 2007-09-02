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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 62 "parser.yy"
{
  int intval;
  double val;
  std::string *stringptr;
  ParseTree *ptree;
  std::vector<ParseTree*> *pvec;
  std::vector<double> *vec;
  std::vector<long> *ivec;
}
/* Line 1489 of yacc.c.  */
#line 137 "../../../JAGS/src/terminal/parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE zzlval;

