/* A Bison parser, made by GNU Bison 3.7.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1





# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YY_YYEMPTY = -2,
  YY_YYEOF = 0,                            /* "end of file"  */
  YY_YYerror = 1,                          /* error  */
  YY_YYUNDEF = 2,                          /* "invalid token"  */
  YY_ID = 3,                               /* ID  */
  YY_I32_LIT = 4,                          /* I32_LIT  */
  YY_F32_LIT = 5,                          /* F32_LIT  */
  YY_CHAR_LIT = 6,                         /* CHAR_LIT  */
  YY_BOOL_LIT = 7,                         /* BOOL_LIT  */
  YY_STRING_LIT = 8,                       /* STRING_LIT  */
  YY_ASSIGN = 9,                           /* "="  */
  YY_ADD = 10,                             /* "+"  */
  YY_SUB = 11,                             /* "-"  */
  YY_MUL = 12,                             /* "*"  */
  YY_DIV = 13,                             /* "/"  */
  YY_MOD = 14,                             /* "%"  */
  YY_EQ = 15,                              /* "=="  */
  YY_NEQ = 16,                             /* "!="  */
  YY_GT = 17,                              /* ">"  */
  YY_GTEQ = 18,                            /* ">="  */
  YY_LT = 19,                              /* "<"  */
  YY_LTEQ = 20,                            /* "<="  */
  YY_INC = 21,                             /* "++"  */
  YY_DEC = 22,                             /* "--"  */
  YY_LNOT = 23,                            /* "!"  */
  YY_LAND = 24,                            /* "&&"  */
  YY_LOR = 25,                             /* "||"  */
  YY_BNOT = 26,                            /* "~"  */
  YY_BAND = 27,                            /* "&"  */
  YY_BOR = 28,                             /* "|"  */
  YY_BXOR = 29,                            /* "^"  */
  YY_BLSHIFT = 30,                         /* "<<"  */
  YY_BRSHIFT = 31,                         /* ">>"  */
  YY_POW = 32,                             /* "**"  */
  YY_COMMA = 33,                           /* ","  */
  YY_DOT = 34,                             /* "."  */
  YY_ARROW = 35,                           /* "->"  */
  YY_COLON = 36,                           /* ":"  */
  YY_SEMICOLON = 37,                       /* ";"  */
  YY_OPEN_PAREN = 38,                      /* "("  */
  YY_CLOSE_PAREN = 39,                     /* ")"  */
  YY_OPEN_BRACKET = 40,                    /* "["  */
  YY_CLOSE_BRACKET = 41,                   /* "]"  */
  YY_OPEN_BRACE = 42,                      /* "{"  */
  YY_CLOSE_BRACE = 43,                     /* "}"  */
  YY_KW_INT = 44,                          /* "int"  */
  YY_KW_FLOAT = 45,                        /* "float"  */
  YY_KW_BOOL = 46,                         /* "bool"  */
  YY_KW_FN = 47,                           /* "fn"  */
  YY_KW_MAIN = 48,                         /* "main"  */
  YY_KW_LET = 49,                          /* "let"  */
  YY_KW_PRINT = 50,                        /* "print"  */
  YY_KW_IF = 51,                           /* "if"  */
  YY_KW_ELSE = 52,                         /* "else"  */
  YY_KW_WHILE = 53,                        /* "while"  */
  YY_KW_DO = 54,                           /* "do"  */
  YY_KW_FOR = 55,                          /* "for"  */
  YY_POS = 56,                             /* POS  */
  YY_NEG = 57,                             /* NEG  */
  YY_AR_SUBSCR = 58,                       /* AR_SUBSCR  */
  YY_LAST = 59,                            /* LAST  */
  YY_YYACCEPT = 60,                        /* $accept  */
  YY_ident = 61,                           /* ident  */
  YY_pident = 62,                          /* pident  */
  YY_i32lit = 63,                          /* i32lit  */
  YY_f32lit = 64,                          /* f32lit  */
  YY_charlit = 65,                         /* charlit  */
  YY_boollit = 66,                         /* boollit  */
  YY_stringlit = 67,                       /* stringlit  */
  YY_expr_terminal = 68,                   /* expr_terminal  */
  YY_root = 69,                            /* root  */
  YY_70_1 = 70,                            /* $@1  */
  YY_productions = 71,                     /* productions  */
  YY_main = 72,                            /* main  */
  YY_stmts = 73,                           /* stmts  */
  YY_stmt = 74,                            /* stmt  */
  YY_assignment = 75,                      /* assignment  */
  YY_print_stmt = 76,                      /* print_stmt  */
  YY_decl = 77,                            /* decl  */
  YY_list_init = 78,                       /* list_init  */
  YY_list_elems = 79,                      /* list_elems  */
  YY_list_elem = 80,                       /* list_elem  */
  YY_integral_var_decl = 81,               /* integral_var_decl  */
  YY_array_var_decl = 82,                  /* array_var_decl  */
  YY_integral_type = 83,                   /* integral_type  */
  YY_array_type = 84,                      /* array_type  */
  YY_sized_array_type = 85,                /* sized_array_type  */
  YY_unsized_array_type = 86,              /* unsized_array_type  */
  YY_code_block = 87,                      /* code_block  */
  YY_88_2 = 88,                            /* $@2  */
  YY_for_1 = 89,                           /* for_1  */
  YY_for_2 = 90,                           /* for_2  */
  YY_for_3 = 91,                           /* for_3  */
  YY_if_stmt = 92,                         /* if_stmt  */
  YY_for_stmt = 93,                        /* for_stmt  */
  YY_94_3 = 94,                            /* $@3  */
  YY_while_stmt = 95,                      /* while_stmt  */
  YY_do_while_stmt = 96,                   /* do_while_stmt  */
  YY_expr = 97                             /* expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */

        #include "globals.h"
        #include "yacc_utils.h"
        #include "dpcc.h"
        #include "log.h"
        #define YYANARRAY (ast_node_t*[])


#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   950

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  111
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  197

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   314


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX) YY_CAST (yysymbol_kind_t, YYX)

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   214,   214,   215,   227,   228,   229,   230,   231,   233,
     234,   235,   236,   237,   238,   247,   247,   251,   252,   253,
     257,   260,   261,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   277,   280,   281,   285,   286,   289,   290,
     291,   295,   296,   299,   302,   303,   304,   305,   308,   309,
     310,   313,   314,   315,   318,   319,   323,   326,   329,   329,
     343,   346,   347,   348,   350,   351,   353,   354,   357,   358,
     359,   360,   361,   362,   365,   365,   366,   368,   369,   371,
     372,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "ID", "I32_LIT", "F32_LIT",
  "CHAR_LIT", "BOOL_LIT", "STRING_LIT", "=", "+", "-", "*", "/", "%", "==",
  "!=", ">", ">=", "<", "<=", "++", "--", "!", "&&", "||", "~", "&", "|",
  "^", "<<", ">>", "**", ",", ".", "->", ":", ";", "(", ")", "[", "]", "{",
  "}", "int", "float", "bool", "fn", "main", "let", "print", "if", "else",
  "while", "do", "for", "POS", "NEG", "AR_SUBSCR", "LAST", "$accept",
  "ident", "pident", "i32lit", "f32lit", "charlit", "boollit", "stringlit",
  "expr_terminal", "root", "$@1", "productions", "main", "stmts", "stmt",
  "assignment", "print_stmt", "decl", "list_init", "list_elems",
  "list_elem", "integral_var_decl", "array_var_decl", "integral_type",
  "array_type", "sized_array_type", "unsized_array_type", "code_block",
  "$@2", "for_1", "for_2", "for_3", "if_stmt", "for_stmt", "$@3",
  "while_stmt", "do_while_stmt", "expr", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314
};
#endif

#define YYPACT_NINF (-141)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-75)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -141,    12,   148,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,   609,   609,   609,   609,  -141,   437,   -24,  -141,  -141,
    -141,   -26,    47,    19,    23,    51,    48,    53,    20,  -141,
    -141,  -141,  -141,  -141,  -141,  -141,  -141,   204,  -141,  -141,
      60,    62,  -141,  -141,    64,  -141,  -141,  -141,  -141,  -141,
     866,  -141,  -141,  -141,  -141,    61,   680,  -141,   370,    77,
    -141,    -2,   483,   494,   540,    63,    10,  -141,  -141,   609,
    -141,  -141,  -141,   609,   609,   609,   609,   609,   609,   609,
     609,   609,   609,   609,   609,   609,   609,   609,   609,   609,
     609,   609,   609,   609,  -141,  -141,  -141,   315,    78,   100,
      41,    79,   711,    80,   742,    81,   773,    83,    86,   423,
     647,   804,   895,    17,    17,    90,    90,    90,   206,   206,
     317,   317,   317,   317,   262,   918,   569,    82,   372,    14,
      14,    90,  -141,   370,  -141,   565,  -141,   895,    -3,   118,
    -141,  -141,  -141,  -141,    48,    48,    48,    48,   551,    48,
      95,  -141,    97,   895,  -141,  -141,   260,  -141,    -1,  -141,
     895,   609,     4,     3,    84,    87,  -141,  -141,    96,   835,
    -141,   609,   609,  -141,   895,  -141,   102,  -141,    -7,    -7,
     104,   110,   120,   895,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,   609,   125,   895,    48,  -141
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      15,     0,     0,     1,    32,     3,     4,     5,     6,     7,
       8,     0,     0,     0,     0,    31,     0,    58,    51,    52,
      53,     0,     0,     0,     0,     0,     0,     0,     9,    10,
      11,    12,    13,    14,   111,    16,    17,     0,    22,   110,
       0,     0,    36,    37,     0,    30,    26,    27,    28,    29,
       0,    83,    84,    90,    93,     0,     0,    60,     0,     0,
       2,    44,     0,     0,     0,     0,     0,   106,   107,     0,
      21,    24,    25,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,    81,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,    33,    85,    86,    87,    88,    89,    99,   100,
     103,   104,   101,   102,    91,    92,    94,    95,    96,    97,
      98,   105,    59,     0,    40,     0,    50,    45,    46,    48,
      54,    55,    35,    34,     0,     0,     0,     0,     0,     0,
     110,    61,     0,     0,   108,   109,     0,    39,     0,    42,
      43,     0,     0,     0,    71,    68,    78,    77,     0,     0,
      76,    65,     0,    38,    47,    57,     0,    49,     0,     0,
       0,     0,     0,    64,    41,    56,    72,    73,    69,    70,
      80,    79,    67,     0,    66,     0,    75
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -141,  -141,  -141,     5,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,   -49,   -14,    56,  -141,    57,     6,  -141,
      -4,  -141,  -141,    70,  -141,  -141,  -141,   -16,  -141,  -141,
    -141,  -141,  -140,  -141,  -141,  -141,  -141,   -11
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    61,    28,    29,    30,    31,    32,    33,    34,     1,
       2,    35,    36,    37,    38,    39,    40,    41,   136,   158,
     159,    42,    43,    44,   139,   140,   141,    45,    58,   152,
     182,   193,    46,    47,   109,    48,    49,    50
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      51,    52,    53,    54,   134,    56,   161,    99,     6,    97,
      65,   108,     3,   -74,   -74,   -74,   -74,   -74,   -74,    57,
     -74,   -74,    59,    70,    75,    76,    77,    78,    79,    77,
      78,    79,   172,   -74,   100,    17,   -74,   162,   187,   189,
     173,    67,    68,   135,    24,   175,    93,   -74,   -74,    93,
      60,   102,   104,   106,   -74,   -74,   -74,    62,   110,   -74,
      69,    63,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    70,   156,    18,    19,    20,   137,    64,
      17,    66,    75,    76,    77,    78,    79,    71,   153,    72,
      95,   134,    73,     5,     6,     7,     8,     9,    10,    88,
      11,    12,    91,    92,    93,    98,   107,   133,   142,   144,
     146,   148,    93,    13,   160,   149,    14,   163,   164,   165,
     166,   167,   -62,   170,   171,   180,   178,   169,    16,   179,
     135,   190,    70,   185,    18,    19,    20,   191,   -19,     4,
     174,     5,     6,     7,     8,     9,    10,   192,    11,    12,
     183,   160,   186,   188,   195,   150,   151,   176,   184,   177,
     138,    13,     0,     0,    14,     0,     0,     0,     0,   196,
       0,   194,     0,     0,     0,    15,    16,     0,     0,     0,
      17,     0,    18,    19,    20,    21,     0,    22,    23,    24,
       0,    25,    26,    27,   -18,     4,     0,     5,     6,     7,
       8,     9,    10,     0,    11,    12,    75,    76,    77,    78,
      79,   -75,   -75,    82,    83,    84,    85,    13,     0,     0,
      14,     0,     0,    88,    89,    90,    91,    92,    93,     0,
       0,    15,    16,     0,     0,     0,    17,     0,    18,    19,
      20,     0,     0,    22,    23,    24,     0,    25,    26,    27,
     -20,     4,     0,     5,     6,     7,     8,     9,    10,     0,
      11,    12,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    13,     0,     0,    14,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,    15,    16,     0,
       0,     0,    17,     0,    18,    19,    20,     0,     0,    22,
      23,    24,     0,    25,    26,    27,     4,     0,     5,     6,
       7,     8,     9,    10,     0,    11,    12,    75,    76,    77,
      78,    79,     0,     0,   -75,   -75,   -75,   -75,    13,     0,
       0,    14,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,    15,    16,     0,     0,     0,    17,   132,    18,
      19,    20,     0,     0,    22,    23,    24,     0,    25,    26,
      27,     4,     0,     5,     6,     7,     8,     9,    10,     0,
      11,    12,    75,    76,    77,    78,    79,     0,     0,     0,
       0,     0,     0,    13,     0,     0,    14,     0,     0,    88,
      89,     0,    91,    92,    93,     0,     0,    15,    16,     0,
       0,     0,    17,     0,    18,    19,    20,     0,     0,    22,
      23,    24,     0,    25,    26,    27,     5,     6,     7,     8,
       9,    10,     0,    11,    12,     0,     0,     0,    55,     0,
       5,     6,     7,     8,     9,    10,    13,    11,    12,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      13,    16,     0,    14,     0,     0,     0,    18,    19,    20,
       0,     0,    22,     0,     0,    16,     0,     0,     0,     0,
       0,    18,    19,    20,   101,     0,     5,     6,     7,     8,
       9,    10,     0,    11,    12,   103,     0,     5,     6,     7,
       8,     9,    10,     0,    11,    12,    13,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,    13,     0,     0,
      14,    16,     0,     0,     0,     0,     0,    18,    19,    20,
       0,     0,    16,     0,     0,     0,     0,     0,    18,    19,
      20,   105,     0,     5,     6,     7,     8,     9,    10,     0,
      11,    12,   168,     0,     5,     6,     7,     8,     9,    10,
       0,    11,    12,    13,     0,     0,    14,     0,     5,     6,
       7,     8,     9,    10,    13,    11,    12,    14,    16,    75,
      76,    77,    78,    79,    18,    19,    20,     0,    13,    16,
       0,    14,     0,     0,     0,    18,    19,    20,     0,    91,
      92,    93,     0,    16,     0,     0,   157,     0,     0,    18,
      19,    20,     5,     6,     7,     8,     9,    10,     0,    11,
      12,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    13,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    16,     0,     0,
       0,     0,     0,    18,    19,    20,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,     0,     0,
       0,    86,    87,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,     0,     0,     0,     0,   154,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,     0,     0,     0,    86,    87,     0,    88,    89,    90,
      91,    92,    93,     0,     0,     0,     0,     0,     0,    96,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,     0,     0,     0,    86,    87,     0,    88,    89,
      90,    91,    92,    93,     0,     0,     0,     0,     0,     0,
     143,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,     0,     0,     0,    86,    87,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,     0,
       0,   145,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,     0,     0,     0,    86,    87,     0,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
       0,     0,   147,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,     0,     0,     0,    86,    87,
       0,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,     0,     0,   155,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,     0,     0,     0,    86,
      87,     0,    88,    89,    90,    91,    92,    93,     0,     0,
       0,     0,     0,     0,   181,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,     0,     0,     0,
      86,    87,     0,    88,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,     0,     0,     0,    86,
      87,     0,    88,    89,    90,    91,    92,    93,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,     0,
       0,     0,    86,     0,     0,    88,    89,    90,    91,    92,
      93
};

static const yytype_int16 yycheck[] =
{
      11,    12,    13,    14,     1,    16,     9,     9,     4,    58,
      26,     1,     0,     3,     4,     5,     6,     7,     8,    43,
      10,    11,    48,    37,    10,    11,    12,    13,    14,    12,
      13,    14,    33,    23,    36,    42,    26,    40,   178,   179,
      41,    21,    22,    40,    51,    41,    32,    37,    38,    32,
       3,    62,    63,    64,    44,    45,    46,    38,    69,    49,
      40,    38,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    97,   133,    44,    45,    46,    99,    38,
      42,    38,    10,    11,    12,    13,    14,    37,   109,    37,
      39,     1,    38,     3,     4,     5,     6,     7,     8,    27,
      10,    11,    30,    31,    32,    38,    53,    39,    39,    39,
      39,    38,    32,    23,   135,    39,    26,     9,   144,   145,
     146,   147,    37,   149,    37,    39,    52,   148,    38,    52,
      40,    37,   156,    41,    44,    45,    46,    37,     0,     1,
     161,     3,     4,     5,     6,     7,     8,    37,    10,    11,
     171,   172,   178,   179,    39,   109,   109,   162,   172,   163,
     100,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,   195,
      -1,   192,    -1,    -1,    -1,    37,    38,    -1,    -1,    -1,
      42,    -1,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,     0,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    10,    11,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    23,    -1,    -1,
      26,    -1,    -1,    27,    28,    29,    30,    31,    32,    -1,
      -1,    37,    38,    -1,    -1,    -1,    42,    -1,    44,    45,
      46,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      10,    11,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    23,    -1,    -1,    26,    -1,    -1,    27,
      28,    29,    30,    31,    32,    -1,    -1,    37,    38,    -1,
      -1,    -1,    42,    -1,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    10,    11,    10,    11,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    23,    -1,
      -1,    26,    -1,    -1,    27,    28,    29,    30,    31,    32,
      -1,    -1,    37,    38,    -1,    -1,    -1,    42,    43,    44,
      45,    46,    -1,    -1,    49,    50,    51,    -1,    53,    54,
      55,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      10,    11,    10,    11,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    -1,    27,
      28,    -1,    30,    31,    32,    -1,    -1,    37,    38,    -1,
      -1,    -1,    42,    -1,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,     3,     4,     5,     6,
       7,     8,    -1,    10,    11,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,     8,    23,    10,    11,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    38,    -1,    26,    -1,    -1,    -1,    44,    45,    46,
      -1,    -1,    49,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,     1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    10,    11,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    10,    11,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      26,    38,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    45,
      46,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      10,    11,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    10,    11,    23,    -1,    -1,    26,    -1,     3,     4,
       5,     6,     7,     8,    23,    10,    11,    26,    38,    10,
      11,    12,    13,    14,    44,    45,    46,    -1,    23,    38,
      -1,    26,    -1,    -1,    -1,    44,    45,    46,    -1,    30,
      31,    32,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      45,    46,     3,     4,     5,     6,     7,     8,    -1,    10,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    45,    46,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    24,    25,    -1,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    24,    25,    -1,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    39,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    24,    25,    -1,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      39,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    24,    25,    -1,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    39,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    24,    25,    -1,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    39,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    24,    25,
      -1,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    39,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    24,
      25,    -1,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    39,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      24,    25,    -1,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    -1,    37,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    24,
      25,    -1,    27,    28,    29,    30,    31,    32,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    24,    -1,    -1,    27,    28,    29,    30,    31,
      32
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    69,    70,     0,     1,     3,     4,     5,     6,     7,
       8,    10,    11,    23,    26,    37,    38,    42,    44,    45,
      46,    47,    49,    50,    51,    53,    54,    55,    62,    63,
      64,    65,    66,    67,    68,    71,    72,    73,    74,    75,
      76,    77,    81,    82,    83,    87,    92,    93,    95,    96,
      97,    97,    97,    97,    97,     1,    97,    43,    88,    48,
       3,    61,    38,    38,    38,    87,    38,    21,    22,    40,
      74,    37,    37,    38,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    24,    25,    27,    28,
      29,    30,    31,    32,    37,    39,    39,    73,    38,     9,
      36,     1,    97,     1,    97,     1,    97,    53,     1,    94,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    43,    39,     1,    40,    78,    97,    83,    84,
      85,    86,    39,    39,    39,    39,    39,    39,    38,    39,
      75,    77,    89,    97,    41,    39,    73,    41,    79,    80,
      97,     9,    40,     9,    87,    87,    87,    87,     1,    97,
      87,    37,    33,    41,    97,    41,    63,    78,    52,    52,
      39,    39,    90,    97,    80,    41,    87,    92,    87,    92,
      37,    37,    37,    91,    97,    39,    87
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      68,    68,    68,    68,    68,    70,    69,    71,    71,    71,
      72,    73,    73,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    75,    76,    76,    77,    77,    78,    78,
      78,    79,    79,    80,    81,    81,    81,    81,    82,    82,
      82,    83,    83,    83,    84,    84,    85,    86,    88,    87,
      87,    89,    89,    89,    90,    90,    91,    91,    92,    92,
      92,    92,    92,    92,    94,    93,    93,    95,    95,    96,
      96,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     2,     1,     1,     0,
       5,     2,     1,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     4,     1,     1,     3,     2,
       1,     3,     1,     1,     2,     4,     4,     6,     4,     6,
       4,     1,     1,     1,     1,     1,     4,     3,     0,     4,
       2,     1,     1,     0,     1,     0,     1,     0,     5,     7,
       7,     5,     7,     7,     0,    10,     5,     5,     5,     7,
       7,     3,     3,     2,     2,     3,     3,     3,     3,     3,
       2,     3,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     4,     4,
       1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = TOK_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == TOK_YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use TOK_YYerror or TOK_YYUNDEF. */
#define YYERRCODE TOK_YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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


/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return YYENOMEM if memory is exhausted.  */
static int
yy_lac_stack_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      if (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      if (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      if (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  return 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#define YY_LAC_ESTABLISH                                                \
do {                                                                    \
  if (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      switch (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        case YYENOMEM:                                                  \
          goto yyexhaustedlab;                                          \
        case 1:                                                         \
          goto yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} while (0)

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#if YYDEBUG
# define YY_LAC_DISCARD(Event)                                           \
do {                                                                     \
  if (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (0)
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  if (yytoken == YY_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[+*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          if (yytable_value_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
          if (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              return 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        if (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            if (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            else
              {
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          const int yylhs = yyr1[yyrule] - YYNTOKENS;
          const int yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        else
          {
            if (yy_lac_stack_realloc (yyes_capacity, 1,
#if YYDEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                return YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_capacity;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;

  int yyx;
  for (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      if (yysym != YY_YYerror && yysym != YY_YYUNDEF)
        switch (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          case YYENOMEM:
            return YYENOMEM;
          case 1:
            continue;
          default:
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YY_YYEMPTY;
  return yycount;
}




/* The kind of the lookahead of this context.  */
static yysymbol_kind_t
yypcontext_token (const yypcontext_t *yyctx) YY_ATTRIBUTE_UNUSED;

static yysymbol_kind_t
yypcontext_token (const yypcontext_t *yyctx)
{
  return yyctx->yytoken;
}

/* The location of the lookahead of this context.  */
static YYLTYPE *
yypcontext_location (const yypcontext_t *yyctx) YY_ATTRIBUTE_UNUSED;

static YYLTYPE *
yypcontext_location (const yypcontext_t *yyctx)
{
  return yyctx->yylloc;
}

/* User defined function to report a syntax error.  */
static int
yyreport_syntax_error (const yypcontext_t *yyctx);

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

    yy_state_t yyesa[20];
    yy_state_t *yyes = yyesa;
    YYPTRDIFF_T yyes_capacity = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  int yy_lac_established = 0;
  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YY_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = TOK_YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == TOK_YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= TOK_YYEOF)
    {
      yychar = TOK_YYEOF;
      yytoken = YY_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == TOK_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = TOK_YYUNDEF;
      yytoken = YY_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      YY_LAC_ESTABLISH;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = TOK_YYEMPTY;
  YY_LAC_DISCARD ("shift");
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
  case 2: /* ident: ID  */
          { yyval = yyvsp[0]; NODE_KIND(yyval, Ident); }
    break;

  case 3: /* pident: ID  */
            {
        yyval = yyvsp[0];
        NODE_KIND(yyval, Ident);
        YYSTYPE decl = symtable_lookup(yyval->tok);
        if (!decl) {
                dpcc_log(DPCC_SEVERITY_ERROR, &yyval->tok->loc, "Use of undeclared identifier `%s`", yyval->tok->lexeme);
                PARSE_ERROR();
        } else {
                yyval->decl = decl;
        }
}
    break;

  case 4: /* i32lit: I32_LIT  */
                                                              { yyval = yyvsp[0]; NODE_KIND(yyval, IntLit); INIT_I32(yyval); }
    break;

  case 5: /* f32lit: F32_LIT  */
                                                              { yyval = yyvsp[0]; NODE_KIND(yyval, FloatLit); INIT_F32(yyval); }
    break;

  case 6: /* charlit: CHAR_LIT  */
                                                              { yyval = yyvsp[0]; NODE_KIND(yyval, IntLit); INIT_CHAR(yyval); }
    break;

  case 7: /* boollit: BOOL_LIT  */
                                                              { yyval = yyvsp[0]; NODE_KIND(yyval, BoolLit); INIT_BOOL(yyval); }
    break;

  case 8: /* stringlit: STRING_LIT  */
                                                              { yyval = yyvsp[0]; NODE_KIND(yyval, StringLit); }
    break;

  case 15: /* $@1: %empty  */
      { symtable_begin_block(); }
    break;

  case 16: /* root: $@1 productions  */
                                              { symtable_end_block(); }
    break;

  case 17: /* productions: main  */
                    { NODE_KIND(&G_root_node, RootNode); push_child(&G_root_node, yyvsp[0]); }
    break;

  case 18: /* productions: stmts  */
                    { NODE_KIND(&G_root_node, RootNode); NODE_KIND(yyvsp[0], CodeBlock); push_child(&G_root_node, yyvsp[0]); }
    break;

  case 20: /* main: "fn" "main" "(" ")" stmts  */
                                                             { yyval = NEW_NODE(yyvsp[-3]->tok, MainFn); push_child(yyval, yyvsp[0]); }
    break;

  case 21: /* stmts: stmts stmt  */
                                                             { yyval = yyvsp[-1]; push_child(yyvsp[-1], yyvsp[0]); }
    break;

  case 22: /* stmts: stmt  */
                                                             { yyval = NEW_NODE(yyvsp[0]->tok, NullAstNodeKind); push_child(yyval, yyvsp[0]); }
    break;

  case 23: /* stmt: expr ";"  */
                                                  { yyval = NEW_NODE(yyvsp[0]->tok, Stmt); push_child(yyval, yyvsp[-1]); }
    break;

  case 24: /* stmt: print_stmt ";"  */
                                                  { yyval = NEW_NODE(yyvsp[0]->tok, Stmt); push_child(yyval, yyvsp[-1]); }
    break;

  case 25: /* stmt: decl ";"  */
                                                  { yyval = NEW_NODE(yyvsp[0]->tok, Stmt); push_child(yyval, yyvsp[-1]); }
    break;

  case 26: /* stmt: if_stmt  */
                                                  { yyval = NEW_NODE(yyvsp[0]->tok, Stmt); push_child(yyval, yyvsp[0]); }
    break;

  case 27: /* stmt: for_stmt  */
                                                  { yyval = NEW_NODE(yyvsp[0]->tok, Stmt); push_child(yyval, yyvsp[0]); }
    break;

  case 28: /* stmt: while_stmt  */
                                                  { yyval = NEW_NODE(yyvsp[0]->tok, Stmt); push_child(yyval, yyvsp[0]); }
    break;

  case 29: /* stmt: do_while_stmt  */
                                                  { yyval = NEW_NODE(yyvsp[0]->tok, Stmt); push_child(yyval, yyvsp[0]); }
    break;

  case 30: /* stmt: code_block  */
                                                  { yyval = NEW_NODE(yyvsp[0]->tok, Stmt); push_child(yyval, yyvsp[0]); }
    break;

  case 31: /* stmt: ";"  */
                                                  { yyval = NULL; }
    break;

  case 32: /* stmt: error  */
                                                  {  }
    break;

  case 33: /* assignment: expr "=" expr  */
                                                              { yyval = NEW_NODE(yyvsp[-1]->tok, ExprAssign); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 34: /* print_stmt: "print" "(" expr ")"  */
                                                              { yyval = NEW_NODE(yyvsp[-3]->tok, PrintStmt); push_child(yyval, yyvsp[-1]); }
    break;

  case 35: /* print_stmt: "print" "(" error ")"  */
                                                              {  }
    break;

  case 36: /* decl: integral_var_decl  */
                                                              { yyval = yyvsp[0]; if (!var_decl(yyvsp[0])) { PARSE_ERROR(); } }
    break;

  case 37: /* decl: array_var_decl  */
                                                              { yyval = yyvsp[0]; if (!var_decl(yyvsp[0])) { PARSE_ERROR(); } }
    break;

  case 38: /* list_init: "[" list_elems "]"  */
                                       { yyval = NEW_NODE(yyvsp[-2]->tok, InitializerList); push_childs(yyval, yyvsp[-1]->num_childs, yyvsp[-1]->childs); yyvsp[-1]->num_childs = 0; }
    break;

  case 39: /* list_init: "[" "]"  */
                                       { yyval = NEW_NODE(yyvsp[-1]->tok, InitializerList);  }
    break;

  case 40: /* list_init: error  */
                            { }
    break;

  case 41: /* list_elems: list_elems "," list_elem  */
                                                       { yyval = yyvsp[-2]; push_child(yyvsp[-2], yyvsp[0]); }
    break;

  case 42: /* list_elems: list_elem  */
                                                       { yyval = NEW_NODE(yyvsp[0]->tok, NullAstNodeKind); push_child(yyval, yyvsp[0]); }
    break;

  case 43: /* list_elem: expr  */
                        { yyval = yyvsp[0]; }
    break;

  case 44: /* integral_var_decl: "let" ident  */
                                                                                        { yyval = NEW_NODE(yyvsp[-1]->tok, VarDeclStmt); push_childs(yyval, 3, YYANARRAY {NULL, yyvsp[0], NULL}); }
    break;

  case 45: /* integral_var_decl: "let" ident "=" expr  */
                                                                                        { yyval = NEW_NODE(yyvsp[-3]->tok, VarDeclStmt); push_childs(yyval, 3, YYANARRAY {NULL, yyvsp[-2], yyvsp[0]}); }
    break;

  case 46: /* integral_var_decl: "let" ident ":" integral_type  */
                                                                                        { yyval = NEW_NODE(yyvsp[-3]->tok, VarDeclStmt); push_childs(yyval, 3, YYANARRAY {yyvsp[0], yyvsp[-2], NULL}); }
    break;

  case 47: /* integral_var_decl: "let" ident ":" integral_type "=" expr  */
                                                                                        { yyval = NEW_NODE(yyvsp[-5]->tok, VarDeclStmt); push_childs(yyval, 3, YYANARRAY {yyvsp[-2], yyvsp[-4], yyvsp[0]}); }
    break;

  case 48: /* array_var_decl: "let" ident ":" array_type  */
                                                                                        { yyval = NEW_NODE(yyvsp[-3]->tok, VarDeclStmt); push_childs(yyval, 3, YYANARRAY {yyvsp[0], yyvsp[-2], NULL}); }
    break;

  case 49: /* array_var_decl: "let" ident ":" array_type "=" list_init  */
                                                                                        { yyval = NEW_NODE(yyvsp[-5]->tok, VarDeclStmt); push_childs(yyval, 3, YYANARRAY {yyvsp[-2], yyvsp[-4], yyvsp[0]}); }
    break;

  case 50: /* array_var_decl: "let" ident "=" list_init  */
                                                                                        { YYSTYPE t = NEW_NODE(yyvsp[-3]->tok, TypeInfoArray); yyval = NEW_NODE(yyvsp[-3]->tok, VarDeclStmt); push_childs(yyval, 3, YYANARRAY {t, yyvsp[-2], yyvsp[0]}); }
    break;

  case 51: /* integral_type: "int"  */
                           { yyval = yyvsp[0]; NODE_KIND(yyval, TypeInfoInt); }
    break;

  case 52: /* integral_type: "float"  */
                           { yyval = yyvsp[0]; NODE_KIND(yyval, TypeInfoFloat);}
    break;

  case 53: /* integral_type: "bool"  */
                           { yyval = yyvsp[0]; NODE_KIND(yyval, TypeInfoBool); }
    break;

  case 54: /* array_type: sized_array_type  */
                                 { yyval = yyvsp[0]; }
    break;

  case 55: /* array_type: unsized_array_type  */
                                 { yyval = yyvsp[0]; }
    break;

  case 56: /* sized_array_type: integral_type "[" i32lit "]"  */
                                                                  { yyval = NEW_NODE(yyvsp[-2]->tok, TypeInfoArray); push_childs(yyval, 2, YYANARRAY {yyvsp[-3], yyvsp[-1]}); }
    break;

  case 57: /* unsized_array_type: integral_type "[" "]"  */
                                                        { yyval = NEW_NODE(yyvsp[-1]->tok, TypeInfoArray); push_childs(yyval, 2, YYANARRAY { yyvsp[-2], NULL}); }
    break;

  case 58: /* $@2: %empty  */
                       {symtable_begin_block(); }
    break;

  case 59: /* code_block: "{" $@2 stmts "}"  */
                                                                                         {
                        yyval = NEW_NODE(yyvsp[-3]->tok, CodeBlock);
                        // Keep bison LEFT recursive (faster) and reverse the order of the childs,
                        // only when needed
                        if (0) {
                                for (int32_t i = 0; i < yyvsp[-1]->num_childs / 2; i++) {
                                    YYSTYPE temp = yyvsp[-1]->childs[0];
                                    yyvsp[-1]->childs[0] = yyvsp[-1]->childs[yyvsp[-1]->num_childs - 1 - i];
                                    yyvsp[-1]->childs[yyvsp[-1]->num_childs - 1 - i] = temp;
                                }
                        }
                        push_childs(yyval, yyvsp[-1]->num_childs, yyvsp[-1]->childs);
                        symtable_end_block();
                }
    break;

  case 60: /* code_block: "{" "}"  */
                                                                  { yyval = NEW_NODE(yyvsp[-1]->tok, CodeBlock); }
    break;

  case 63: /* for_1: %empty  */
              { yyval = NULL; }
    break;

  case 65: /* for_2: %empty  */
              { yyval = NULL; }
    break;

  case 67: /* for_3: %empty  */
              { yyval = NULL; }
    break;

  case 68: /* if_stmt: "if" "(" expr ")" code_block  */
                                                                                                      { yyval = NEW_NODE(yyvsp[-4]->tok, IfStmt); push_childs(yyval, 3, YYANARRAY { yyvsp[-2], yyvsp[0], NULL}); }
    break;

  case 69: /* if_stmt: "if" "(" expr ")" code_block "else" code_block  */
                                                                                                      { yyval = NEW_NODE(yyvsp[-6]->tok, IfStmt); push_childs(yyval, 3, YYANARRAY { yyvsp[-4], yyvsp[-2], yyvsp[0]}); }
    break;

  case 70: /* if_stmt: "if" "(" expr ")" code_block "else" if_stmt  */
                                                                                                      { yyval = NEW_NODE(yyvsp[-6]->tok, IfStmt); push_childs(yyval, 3, YYANARRAY { yyvsp[-4], yyvsp[-2], yyvsp[0]}); }
    break;

  case 71: /* if_stmt: "if" "(" error ")" code_block  */
                                                                                                      {  }
    break;

  case 72: /* if_stmt: "if" "(" error ")" code_block "else" code_block  */
                                                                                                      {  }
    break;

  case 73: /* if_stmt: "if" "(" error ")" code_block "else" if_stmt  */
                                                                                                      {  }
    break;

  case 74: /* $@3: %empty  */
                              { symtable_begin_block(); }
    break;

  case 75: /* for_stmt: "for" "(" $@3 for_1 ";" for_2 ";" for_3 ")" code_block  */
                                                                                                                          { symtable_end_block(); yyval = NEW_NODE(yyvsp[-9]->tok, ForStmt); push_childs(yyval, 4, YYANARRAY {yyvsp[-6], yyvsp[-4], yyvsp[0], yyvsp[-2]} ); }
    break;

  case 76: /* for_stmt: "for" "(" error ")" code_block  */
                                                          {  }
    break;

  case 77: /* while_stmt: "while" "(" expr ")" code_block  */
                                                                     { yyval = NEW_NODE(yyvsp[-4]->tok, WhileStmt); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]} ); }
    break;

  case 78: /* while_stmt: "while" "(" error ")" code_block  */
                                                          {  }
    break;

  case 79: /* do_while_stmt: "do" code_block "while" "(" expr ")" ";"  */
                                                                     { yyval = NEW_NODE(yyvsp[-6]->tok, DoWhileStmt); push_childs(yyval, 2, YYANARRAY {yyvsp[-5], yyvsp[-2]} ); }
    break;

  case 80: /* do_while_stmt: "do" code_block "while" "(" error ")" ";"  */
                                                          {  }
    break;

  case 81: /* expr: "(" error ")"  */
                                                                   {  }
    break;

  case 82: /* expr: "(" expr ")"  */
                                                                   { yyval = yyvsp[-1]; }
    break;

  case 83: /* expr: "+" expr  */
                                                                   { yyval = yyvsp[0]; }
    break;

  case 84: /* expr: "-" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprNeg); push_child(yyval, yyvsp[0]); }
    break;

  case 85: /* expr: expr "+" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprAdd); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 86: /* expr: expr "-" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprSub); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 87: /* expr: expr "*" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprMul); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 88: /* expr: expr "/" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprDiv); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 89: /* expr: expr "%" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprMod); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 90: /* expr: "!" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprLNot); push_child(yyval, yyvsp[0]); }
    break;

  case 91: /* expr: expr "&&" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprLAnd); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 92: /* expr: expr "||" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprLOr); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 93: /* expr: "~" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprBNot); push_child(yyval, yyvsp[0]); }
    break;

  case 94: /* expr: expr "&" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprBAnd); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 95: /* expr: expr "|" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprBOr); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 96: /* expr: expr "^" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprBXor); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 97: /* expr: expr "<<" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprBLShift); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 98: /* expr: expr ">>" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprBRShift); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 99: /* expr: expr "==" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprEq); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 100: /* expr: expr "!=" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprNeq); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 101: /* expr: expr "<" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprLt); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 102: /* expr: expr "<=" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprLtEq); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 103: /* expr: expr ">" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprGt); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 104: /* expr: expr ">=" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprGtEq); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 105: /* expr: expr "**" expr  */
                                                                   { yyval = NEW_NODE(yyvsp[-1]->tok, ExprPow); push_childs(yyval, 2, YYANARRAY {yyvsp[-2], yyvsp[0]}); }
    break;

  case 106: /* expr: pident "++"  */
                                                                   { yyval = NEW_NODE(yyvsp[0]->tok, ExprInc); push_child(yyval, yyvsp[-1]); }
    break;

  case 107: /* expr: pident "--"  */
                                                                   { yyval = NEW_NODE(yyvsp[0]->tok, ExprDec); push_child(yyval, yyvsp[-1]); }
    break;

  case 108: /* expr: pident "[" expr "]"  */
                                                                   { yyval = NEW_NODE(yyvsp[-2]->tok, ExprArraySubscript); push_childs(yyval, 2, YYANARRAY {yyvsp[-3], yyvsp[-1]}); }
    break;

  case 109: /* expr: integral_type "(" expr ")"  */
                                                                   { yyval = NEW_NODE(yyvsp[-3]->tok, ExprCast); push_childs(yyval, 2, YYANARRAY { yyvsp[-3], yyvsp[-1] }); }
    break;

  case 110: /* expr: assignment  */
                                                                   { yyval = yyvsp[0]; }
    break;



        default: break;
      }
    if (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
  }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == TOK_YYEMPTY ? YY_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken, &yylloc};
        if (yychar != TOK_YYEMPTY)
          YY_LAC_ESTABLISH;
        if (yyreport_syntax_error (&yyctx) == 2)
          goto yyexhaustedlab;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= TOK_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == TOK_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = TOK_YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YY_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YY_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != TOK_YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);

  return yyresult;
}




void yyerror(char const *s)
{
    dpcc_log(DPCC_SEVERITY_ERROR, &yylloc, "%s", s);
}


int yyreport_syntax_error (const yypcontext_t *ctx)
{
    yysymbol_kind_t unexpected = yypcontext_token(ctx);
    YYLTYPE *loc = yypcontext_location(ctx);

    yysymbol_kind_t *expected = NULL;
    int num_expected = yypcontext_expected_tokens(ctx, NULL, 0);

    expected = malloc(sizeof(*expected) * num_expected);

    if (!expected) {
        return YYENOMEM;
    }

    int result = yypcontext_expected_tokens(ctx, expected, num_expected);

    if (result != num_expected) {
        return YYENOMEM;
    }

    dpcc_log(DPCC_SEVERITY_ERROR, loc, "Syntax error. Unexpected token `%s` found", yysymbol_name(unexpected));


    if (expected[0] != YY_YYEMPTY) {
        str_t expected_str = {0};

        for (int32_t i = 0; i < num_expected; i++) {
            sfcat(&G_allctx, &expected_str, "`%s`%s", yysymbol_name(expected[i]), i == (num_expected - 1) ? "" : "  ") ;
        }
        dpcc_log(DPCC_SEVERITY_INFO, loc, "Expected one of   %s", expected_str.cstr);
        dalldel(&G_allctx, expected_str.cstr);
    }

    free(expected);
    return 0;
}
