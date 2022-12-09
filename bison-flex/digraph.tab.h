/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_BISON_FLEX_DIGRAPH_TAB_H_INCLUDED
# define YY_YY_BISON_FLEX_DIGRAPH_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    DIGRAPH = 259,
    NODE = 260,
    EDGE = 261,
    FONT_NAME = 262,
    SHAPE = 263,
    COLOR_SCHEME = 264,
    FILL_COLOR = 265,
    LABEL = 266,
    STYLE = 267,
    EPS = 268,
    COMMA = 269,
    SEMI_COLON = 270,
    EDGE_OP = 271,
    EQU_OP = 272,
    LEFT_SQUARE_BRAKET = 273,
    RIGHT_SQUARE_BRAKET = 274,
    LEFT_CURLY_BRAKET = 275,
    RIGHT_CURLY_BRAKET = 276
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 50 "bison-flex/digraph.y"

    struct ID *id;
    int id_val;
    char *attr_name;
    struct LIST *attr_list;

#line 86 "bison-flex/digraph.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void *args);

#endif /* !YY_YY_BISON_FLEX_DIGRAPH_TAB_H_INCLUDED  */
