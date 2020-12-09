/* A Bison parser, made by GNU Bison 3.7.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_HOME_DPARO_DEVELOP_DPCC_TRAVIS_BUILD_PARSER_H_INCLUDED
# define YY_YY_HOME_DPARO_DEVELOP_DPCC_TRAVIS_BUILD_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */

        #include "types.h"


/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_YYEMPTY = -2,
    TOK_YYEOF = 0,                 /* "end of file"  */
    TOK_YYerror = 1,               /* error  */
    TOK_YYUNDEF = 2,               /* "invalid token"  */
    TOK_ID = 3,                    /* ID  */
    TOK_I32_LIT = 4,               /* I32_LIT  */
    TOK_F32_LIT = 5,               /* F32_LIT  */
    TOK_CHAR_LIT = 6,              /* CHAR_LIT  */
    TOK_BOOL_LIT = 7,              /* BOOL_LIT  */
    TOK_STRING_LIT = 8,            /* STRING_LIT  */
    TOK_ASSIGN = 9,                /* "="  */
    TOK_ADD = 10,                  /* "+"  */
    TOK_SUB = 11,                  /* "-"  */
    TOK_MUL = 12,                  /* "*"  */
    TOK_DIV = 13,                  /* "/"  */
    TOK_MOD = 14,                  /* "%"  */
    TOK_EQ = 15,                   /* "=="  */
    TOK_NEQ = 16,                  /* "!="  */
    TOK_GT = 17,                   /* ">"  */
    TOK_GTEQ = 18,                 /* ">="  */
    TOK_LT = 19,                   /* "<"  */
    TOK_LTEQ = 20,                 /* "<="  */
    TOK_INC = 21,                  /* "++"  */
    TOK_DEC = 22,                  /* "--"  */
    TOK_LNOT = 23,                 /* "!"  */
    TOK_LAND = 24,                 /* "&&"  */
    TOK_LOR = 25,                  /* "||"  */
    TOK_BNOT = 26,                 /* "~"  */
    TOK_BAND = 27,                 /* "&"  */
    TOK_BOR = 28,                  /* "|"  */
    TOK_BXOR = 29,                 /* "^"  */
    TOK_BLSHIFT = 30,              /* "<<"  */
    TOK_BRSHIFT = 31,              /* ">>"  */
    TOK_POW = 32,                  /* "**"  */
    TOK_COMMA = 33,                /* ","  */
    TOK_DOT = 34,                  /* "."  */
    TOK_ARROW = 35,                /* "->"  */
    TOK_COLON = 36,                /* ":"  */
    TOK_SEMICOLON = 37,            /* ";"  */
    TOK_OPEN_PAREN = 38,           /* "("  */
    TOK_CLOSE_PAREN = 39,          /* ")"  */
    TOK_OPEN_BRACKET = 40,         /* "["  */
    TOK_CLOSE_BRACKET = 41,        /* "]"  */
    TOK_OPEN_BRACE = 42,           /* "{"  */
    TOK_CLOSE_BRACE = 43,          /* "}"  */
    TOK_KW_INT = 44,               /* "int"  */
    TOK_KW_FLOAT = 45,             /* "float"  */
    TOK_KW_BOOL = 46,              /* "bool"  */
    TOK_KW_FN = 47,                /* "fn"  */
    TOK_KW_MAIN = 48,              /* "main"  */
    TOK_KW_LET = 49,               /* "let"  */
    TOK_KW_PRINT = 50,             /* "print"  */
    TOK_KW_IF = 51,                /* "if"  */
    TOK_KW_ELSE = 52,              /* "else"  */
    TOK_KW_WHILE = 53,             /* "while"  */
    TOK_KW_DO = 54,                /* "do"  */
    TOK_KW_FOR = 55,               /* "for"  */
    TOK_POS = 56,                  /* POS  */
    TOK_NEG = 57,                  /* NEG  */
    TOK_AR_SUBSCR = 58,            /* AR_SUBSCR  */
    TOK_LAST = 59                  /* LAST  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef ast_node_t* YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
typedef loc_t YYLTYPE;


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_HOME_DPARO_DEVELOP_DPCC_TRAVIS_BUILD_PARSER_H_INCLUDED  */
