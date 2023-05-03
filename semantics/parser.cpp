/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <cstring>
#include "lexer.hpp"
#include "ast.hpp"


extern int yylineno;
extern char *yytext;


SymbolTable st;

#line 84 "parser.cpp"

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

#include "parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_and = 3,                      /* "and"  */
  YYSYMBOL_T_char = 4,                     /* "char"  */
  YYSYMBOL_T_div = 5,                      /* "div"  */
  YYSYMBOL_T_do = 6,                       /* "do"  */
  YYSYMBOL_T_else = 7,                     /* "else"  */
  YYSYMBOL_T_fun = 8,                      /* "fun"  */
  YYSYMBOL_T_if = 9,                       /* "if"  */
  YYSYMBOL_T_int = 10,                     /* "int"  */
  YYSYMBOL_T_not = 11,                     /* "not"  */
  YYSYMBOL_T_nothing = 12,                 /* "nothing"  */
  YYSYMBOL_T_or = 13,                      /* "or"  */
  YYSYMBOL_T_ref = 14,                     /* "ref"  */
  YYSYMBOL_T_return = 15,                  /* "return"  */
  YYSYMBOL_T_then = 16,                    /* "then"  */
  YYSYMBOL_T_var = 17,                     /* "var"  */
  YYSYMBOL_T_while = 18,                   /* "while"  */
  YYSYMBOL_T_mod = 19,                     /* "mod"  */
  YYSYMBOL_T_id = 20,                      /* T_id  */
  YYSYMBOL_T_const = 21,                   /* T_const  */
  YYSYMBOL_T_constchar = 22,               /* T_constchar  */
  YYSYMBOL_T_string = 23,                  /* T_string  */
  YYSYMBOL_T_leq = 24,                     /* "<="  */
  YYSYMBOL_T_meq = 25,                     /* ">="  */
  YYSYMBOL_T_assign = 26,                  /* "<-"  */
  YYSYMBOL_27_ = 27,                       /* '='  */
  YYSYMBOL_28_ = 28,                       /* '#'  */
  YYSYMBOL_29_ = 29,                       /* '<'  */
  YYSYMBOL_30_ = 30,                       /* '>'  */
  YYSYMBOL_31_ = 31,                       /* '+'  */
  YYSYMBOL_32_ = 32,                       /* '-'  */
  YYSYMBOL_33_ = 33,                       /* '*'  */
  YYSYMBOL_SIGN = 34,                      /* SIGN  */
  YYSYMBOL_35_ = 35,                       /* '('  */
  YYSYMBOL_36_ = 36,                       /* ')'  */
  YYSYMBOL_37_ = 37,                       /* ':'  */
  YYSYMBOL_38_ = 38,                       /* ';'  */
  YYSYMBOL_39_ = 39,                       /* ','  */
  YYSYMBOL_40_ = 40,                       /* '['  */
  YYSYMBOL_41_ = 41,                       /* ']'  */
  YYSYMBOL_42_ = 42,                       /* '{'  */
  YYSYMBOL_43_ = 43,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 44,                  /* $accept  */
  YYSYMBOL_program = 45,                   /* program  */
  YYSYMBOL_46_func_def = 46,               /* func-def  */
  YYSYMBOL_47_local_def_list = 47,         /* local-def_list  */
  YYSYMBOL_header = 48,                    /* header  */
  YYSYMBOL_49_fpar_def_list = 49,          /* fpar-def_list  */
  YYSYMBOL_50_fpar_def = 50,               /* fpar-def  */
  YYSYMBOL_id_list = 51,                   /* id_list  */
  YYSYMBOL_52_data_type = 52,              /* data-type  */
  YYSYMBOL_type = 53,                      /* type  */
  YYSYMBOL_54_int_const_list = 54,         /* int-const_list  */
  YYSYMBOL_55_ret_type = 55,               /* ret-type  */
  YYSYMBOL_56_fpar_type = 56,              /* fpar-type  */
  YYSYMBOL_57_local_def = 57,              /* local-def  */
  YYSYMBOL_58_func_decl = 58,              /* func-decl  */
  YYSYMBOL_59_var_def = 59,                /* var-def  */
  YYSYMBOL_stmt = 60,                      /* stmt  */
  YYSYMBOL_block = 61,                     /* block  */
  YYSYMBOL_stmt_list = 62,                 /* stmt_list  */
  YYSYMBOL_63_func_call = 63,              /* func-call  */
  YYSYMBOL_expr_list = 64,                 /* expr_list  */
  YYSYMBOL_65_l_value = 65,                /* l-value  */
  YYSYMBOL_expr = 66,                      /* expr  */
  YYSYMBOL_cond = 67                       /* cond  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   220

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  69
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  141

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   282


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    28,     2,     2,     2,     2,
      35,    36,    33,    31,    39,    32,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    37,    38,
      29,    27,    30,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    41,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,     2,    43,     2,     2,     2,     2,
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
      25,    26,    34
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    99,    99,   106,   110,   111,   115,   116,   120,   121,
     125,   126,   130,   131,   135,   136,   140,   144,   145,   149,
     150,   154,   155,   156,   160,   161,   162,   166,   170,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   186,   190,
     191,   195,   196,   200,   201,   205,   206,   207,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "\"and\"", "\"char\"",
  "\"div\"", "\"do\"", "\"else\"", "\"fun\"", "\"if\"", "\"int\"",
  "\"not\"", "\"nothing\"", "\"or\"", "\"ref\"", "\"return\"", "\"then\"",
  "\"var\"", "\"while\"", "\"mod\"", "T_id", "T_const", "T_constchar",
  "T_string", "\"<=\"", "\">=\"", "\"<-\"", "'='", "'#'", "'<'", "'>'",
  "'+'", "'-'", "'*'", "SIGN", "'('", "')'", "':'", "';'", "','", "'['",
  "']'", "'{'", "'}'", "$accept", "program", "func-def", "local-def_list",
  "header", "fpar-def_list", "fpar-def", "id_list", "data-type", "type",
  "int-const_list", "ret-type", "fpar-type", "local-def", "func-decl",
  "var-def", "stmt", "block", "stmt_list", "func-call", "expr_list",
  "l-value", "expr", "cond", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-94)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      22,    49,    75,   -94,   -94,    52,   -94,    -1,    32,    60,
     -94,   -94,    46,   -94,   -94,   -94,   -94,    70,   -94,    57,
      81,   -94,   -94,    63,   -94,   -94,    10,   108,    58,    -2,
      85,   155,   160,   155,    67,   -94,   -94,   -94,   -94,   -94,
      69,    24,    92,    11,    90,   -94,   -94,   -94,   -94,   -94,
     108,   -94,    11,   155,   -94,   -94,   185,   185,   155,   -94,
     106,   140,    76,   185,   -94,    66,     7,   179,   -94,   185,
     185,    11,   112,   -94,   -94,   -94,   -94,    83,   -94,   -94,
     -94,   111,     6,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   155,   155,    73,   118,   -94,    73,
     -94,   129,    95,     3,   -94,   -16,   113,   -94,   -94,   -94,
     -94,   -94,   129,   129,   129,   129,   129,   129,    12,    12,
     -94,   -94,   110,   125,   -94,   -25,   -94,   -94,    84,   -94,
     134,    73,   -94,   185,   -94,   113,   115,   -94,   129,   113,
     -94
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     4,     0,     1,     0,     0,     0,
      39,    24,     4,     5,    25,    26,     3,     0,    12,     0,
       0,     8,    12,     0,    27,    12,     0,     0,     0,     0,
       0,     0,     0,     0,    45,    46,    29,    38,    40,    31,
       0,     0,     0,     0,     0,    15,    14,    20,    19,     6,
       0,     9,     0,     0,    48,    49,     0,     0,     0,    52,
      50,     0,     0,     0,    36,     0,     0,     0,    32,     0,
       0,     0,    21,    10,    13,     7,    17,     0,    61,    53,
      54,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
      41,    43,     0,     0,    11,     0,    16,    28,    51,    60,
      58,    59,    68,    69,    64,    65,    66,    67,    55,    56,
      57,    62,    63,    33,    35,     0,    30,    47,     0,    17,
       0,     0,    42,     0,    17,    22,     0,    34,    44,    23,
      18
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -94,   -94,   150,   -94,   151,   -94,   145,    45,   -20,   -94,
     -86,   135,   117,   -94,   -94,   -94,   -93,   156,   -94,   -23,
     -94,   -22,   -30,   -29
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,     7,     4,    20,    21,    26,    48,    77,
     106,    49,    73,    13,    14,    15,    38,    39,    23,    59,
     125,    60,    61,    62
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      40,    41,    65,   123,    66,   128,   124,     1,    83,    94,
      94,   132,    17,    99,   133,    45,     9,    83,    18,    95,
      95,    46,    84,    72,    78,   129,    79,    80,    81,    82,
       1,    84,    76,    97,    91,    92,    93,   101,   137,   102,
     103,    10,   109,   135,   127,    93,    17,    43,   139,    44,
      69,    72,    18,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,    70,   121,   122,    30,    19,     5,
      42,    83,    31,    40,    41,     6,    40,    41,    32,    94,
      22,    33,    31,    34,    24,    84,    35,     8,    32,    95,
      25,    33,    96,    34,    27,    50,    35,    91,    92,    93,
      83,    36,    67,   138,    98,    10,    37,    68,    40,    41,
      74,    36,    45,    94,    84,    10,    83,    28,    46,    29,
      47,   107,    52,    83,    44,   134,    91,    92,    93,    71,
      84,    44,   131,   126,    83,    85,    86,    84,    87,    88,
      89,    90,    91,    92,    93,    83,    70,   108,    84,    91,
      92,    93,   105,   130,   108,   136,   140,    11,    12,    84,
      91,    92,    93,    16,    85,    86,    53,    87,    88,    89,
      90,    91,    92,    93,    51,    34,    54,    55,    35,     0,
      34,    54,    55,    35,     0,    75,    56,    57,   104,     0,
      58,    56,    57,     0,     0,    63,     0,     0,    64,    34,
      54,    55,    35,     0,     0,    34,    54,    55,    35,     0,
      56,    57,     0,     0,    63,   100,    56,    57,     0,     0,
      63
};

static const yytype_int16 yycheck[] =
{
      23,    23,    32,    96,    33,    21,    99,     8,     5,     3,
       3,    36,    14,     6,    39,     4,    17,     5,    20,    13,
      13,    10,    19,    43,    53,    41,    56,    57,    58,    58,
       8,    19,    52,    63,    31,    32,    33,    67,   131,    69,
      70,    42,    36,   129,    41,    33,    14,    37,   134,    39,
      26,    71,    20,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    40,    94,    95,    22,    36,    20,
      25,     5,     9,    96,    96,     0,    99,    99,    15,     3,
      20,    18,     9,    20,    38,    19,    23,    35,    15,    13,
      20,    18,    16,    20,    37,    37,    23,    31,    32,    33,
       5,    38,    35,   133,    38,    42,    43,    38,   131,   131,
      20,    38,     4,     3,    19,    42,     5,    36,    10,    38,
      12,    38,    37,     5,    39,    41,    31,    32,    33,    37,
      19,    39,     7,    38,     5,    24,    25,    19,    27,    28,
      29,    30,    31,    32,    33,     5,    40,    36,    19,    31,
      32,    33,    40,    40,    36,    21,    41,     7,     7,    19,
      31,    32,    33,     7,    24,    25,    11,    27,    28,    29,
      30,    31,    32,    33,    29,    20,    21,    22,    23,    -1,
      20,    21,    22,    23,    -1,    50,    31,    32,    71,    -1,
      35,    31,    32,    -1,    -1,    35,    -1,    -1,    38,    20,
      21,    22,    23,    -1,    -1,    20,    21,    22,    23,    -1,
      31,    32,    -1,    -1,    35,    36,    31,    32,    -1,    -1,
      35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     8,    45,    46,    48,    20,     0,    47,    35,    17,
      42,    46,    48,    57,    58,    59,    61,    14,    20,    36,
      49,    50,    20,    62,    38,    20,    51,    37,    36,    38,
      51,     9,    15,    18,    20,    23,    38,    43,    60,    61,
      63,    65,    51,    37,    39,     4,    10,    12,    52,    55,
      37,    50,    37,    11,    21,    22,    31,    32,    35,    63,
      65,    66,    67,    35,    38,    66,    67,    35,    38,    26,
      40,    37,    52,    56,    20,    55,    52,    53,    67,    66,
      66,    66,    67,     5,    19,    24,    25,    27,    28,    29,
      30,    31,    32,    33,     3,    13,    16,    66,    38,     6,
      36,    66,    66,    66,    56,    40,    54,    38,    36,    36,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    67,    67,    60,    60,    64,    38,    41,    21,    41,
      40,     7,    36,    39,    41,    54,    21,    60,    66,    54,
      41
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    44,    45,    46,    47,    47,    48,    48,    49,    49,
      50,    50,    51,    51,    52,    52,    53,    54,    54,    55,
      55,    56,    56,    56,    57,    57,    57,    58,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    61,    62,
      62,    63,    63,    64,    64,    65,    65,    65,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     0,     2,     6,     7,     1,     3,
       4,     5,     0,     3,     1,     1,     2,     0,     4,     1,
       1,     1,     4,     5,     1,     1,     1,     2,     6,     1,
       4,     1,     2,     4,     6,     4,     2,     3,     3,     0,
       2,     3,     5,     0,     3,     1,     1,     4,     1,     1,
       1,     3,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

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

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


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
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
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

  YYLOCATION_PRINT (yyo, yylocationp);
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
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

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
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

  yychar = YYEMPTY; /* Cause a token to be read.  */

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
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
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
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
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
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
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
  yychar = YYEMPTY;
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
  switch (yyn)
    {
  case 2: /* program: func-def  */
#line 99 "parser.y"
                                        {   FuncDef *parsingTree = (yyvsp[0].funcdef);
                                            //std::cout << "AST: " << *$1 << std::endl; 
                                            printf("\033[1;32m- Successful parsing.\n\033[0m");
                                            parsingTree->sem(); delete (yyvsp[0].funcdef); }
#line 1363 "parser.cpp"
    break;

  case 3: /* func-def: header local-def_list block  */
#line 106 "parser.y"
                                        { (yyval.funcdef) = new FuncDef((yyvsp[-2].header), (yyvsp[-1].localdeflist), (yyvsp[0].block), yylineno); }
#line 1369 "parser.cpp"
    break;

  case 4: /* local-def_list: %empty  */
#line 110 "parser.y"
                                        { (yyval.localdeflist) = new LocalDefList(); }
#line 1375 "parser.cpp"
    break;

  case 5: /* local-def_list: local-def_list local-def  */
#line 111 "parser.y"
                                        { (yyvsp[-1].localdeflist)->append((yyvsp[0].func)); (yyval.localdeflist) = (yyvsp[-1].localdeflist); }
#line 1381 "parser.cpp"
    break;

  case 6: /* header: "fun" T_id '(' ')' ':' ret-type  */
#line 115 "parser.y"
                                                    { (yyval.header) = new Header((yyvsp[-4].str), new FparDefList(), (yyvsp[0].rettype), yylineno); }
#line 1387 "parser.cpp"
    break;

  case 7: /* header: "fun" T_id '(' fpar-def_list ')' ':' ret-type  */
#line 116 "parser.y"
                                                    { (yyval.header) = new Header((yyvsp[-5].str), (yyvsp[-3].fpardeflist), (yyvsp[0].rettype), yylineno); }
#line 1393 "parser.cpp"
    break;

  case 8: /* fpar-def_list: fpar-def  */
#line 120 "parser.y"
                                        { (yyval.fpardeflist) = new FparDefList(); (yyval.fpardeflist)->append((yyvsp[0].fpardef));}
#line 1399 "parser.cpp"
    break;

  case 9: /* fpar-def_list: fpar-def_list ';' fpar-def  */
#line 121 "parser.y"
                                        { (yyvsp[-2].fpardeflist)->append((yyvsp[0].fpardef)); (yyval.fpardeflist) = (yyvsp[-2].fpardeflist); }
#line 1405 "parser.cpp"
    break;

  case 10: /* fpar-def: T_id id_list ':' fpar-type  */
#line 125 "parser.y"
                                        { (yyvsp[-2].idlist)->putinfront((yyvsp[-3].str)); (yyval.fpardef) = new FparDef(yylineno, (yyvsp[-2].idlist), (yyvsp[0].fpartype)); }
#line 1411 "parser.cpp"
    break;

  case 11: /* fpar-def: "ref" T_id id_list ':' fpar-type  */
#line 126 "parser.y"
                                        { (yyvsp[-2].idlist)->putinfront((yyvsp[-3].str)); (yyval.fpardef) = new FparDef(yylineno, (yyvsp[-2].idlist), (yyvsp[0].fpartype), true); }
#line 1417 "parser.cpp"
    break;

  case 12: /* id_list: %empty  */
#line 130 "parser.y"
                                        { (yyval.idlist) = new IdList(); }
#line 1423 "parser.cpp"
    break;

  case 13: /* id_list: id_list ',' T_id  */
#line 131 "parser.y"
                                        { (yyvsp[-2].idlist)->append((yyvsp[0].str)); (yyval.idlist) = (yyvsp[-2].idlist); }
#line 1429 "parser.cpp"
    break;

  case 14: /* data-type: "int"  */
#line 135 "parser.y"
                                        { (yyval.datatype) = TYPE_int; }
#line 1435 "parser.cpp"
    break;

  case 15: /* data-type: "char"  */
#line 136 "parser.y"
                                        { (yyval.datatype) = TYPE_char; }
#line 1441 "parser.cpp"
    break;

  case 16: /* type: data-type int-const_list  */
#line 140 "parser.y"
                                        { (yyval.type) = new Type((yyvsp[-1].datatype), (yyvsp[0].constlist));}
#line 1447 "parser.cpp"
    break;

  case 17: /* int-const_list: %empty  */
#line 144 "parser.y"
                                        { (yyval.constlist) = new ConstList(); }
#line 1453 "parser.cpp"
    break;

  case 18: /* int-const_list: int-const_list '[' T_const ']'  */
#line 145 "parser.y"
                                        { (yyvsp[-3].constlist)->append((yyvsp[-1].num)); (yyval.constlist) = (yyvsp[-3].constlist); }
#line 1459 "parser.cpp"
    break;

  case 19: /* ret-type: data-type  */
#line 149 "parser.y"
                                        { (yyval.rettype) = (yyvsp[0].datatype); }
#line 1465 "parser.cpp"
    break;

  case 20: /* ret-type: "nothing"  */
#line 150 "parser.y"
                                        { (yyval.rettype) = TYPE_nothing; }
#line 1471 "parser.cpp"
    break;

  case 21: /* fpar-type: data-type  */
#line 154 "parser.y"
                                                { (yyval.fpartype) = new FparType((yyvsp[0].datatype)); }
#line 1477 "parser.cpp"
    break;

  case 22: /* fpar-type: data-type '[' ']' int-const_list  */
#line 155 "parser.y"
                                                { (yyvsp[0].constlist)->putinfront(0); (yyval.fpartype) = new FparType((yyvsp[-3].datatype), (yyvsp[0].constlist)); }
#line 1483 "parser.cpp"
    break;

  case 23: /* fpar-type: data-type '[' T_const ']' int-const_list  */
#line 156 "parser.y"
                                                { (yyvsp[0].constlist)->putinfront((yyvsp[-2].num)); (yyval.fpartype) = new FparType((yyvsp[-4].datatype), (yyvsp[0].constlist)); }
#line 1489 "parser.cpp"
    break;

  case 24: /* local-def: func-def  */
#line 160 "parser.y"
                                        { (yyval.func) = (yyvsp[0].funcdef);}
#line 1495 "parser.cpp"
    break;

  case 25: /* local-def: func-decl  */
#line 161 "parser.y"
                                        { (yyval.func) = (yyvsp[0].func);}
#line 1501 "parser.cpp"
    break;

  case 26: /* local-def: var-def  */
#line 162 "parser.y"
                                        { (yyval.func) = (yyvsp[0].func);}
#line 1507 "parser.cpp"
    break;

  case 27: /* func-decl: header ';'  */
#line 166 "parser.y"
                                        { (yyval.func) = new FuncDecl((yyvsp[-1].header)); }
#line 1513 "parser.cpp"
    break;

  case 28: /* var-def: "var" T_id id_list ':' type ';'  */
#line 170 "parser.y"
                                        { (yyvsp[-3].idlist)->putinfront((yyvsp[-4].str)); (yyval.func) = new VarDef((yyvsp[-3].idlist), (yyvsp[-1].type), yylineno); }
#line 1519 "parser.cpp"
    break;

  case 29: /* stmt: ';'  */
#line 174 "parser.y"
                                        { (yyval.stmt) = new BlankStmt(); }
#line 1525 "parser.cpp"
    break;

  case 30: /* stmt: l-value "<-" expr ';'  */
#line 175 "parser.y"
                                        { (yyval.stmt) = new Assignment((yyvsp[-3].expr), (yyvsp[-1].expr), yylineno); }
#line 1531 "parser.cpp"
    break;

  case 31: /* stmt: block  */
#line 176 "parser.y"
                                        { (yyval.stmt) = (yyvsp[0].block); }
#line 1537 "parser.cpp"
    break;

  case 32: /* stmt: func-call ';'  */
#line 177 "parser.y"
                                        { (yyvsp[-1].funccall)->makeStmt(); (yyval.stmt) = (yyvsp[-1].funccall); }
#line 1543 "parser.cpp"
    break;

  case 33: /* stmt: "if" cond "then" stmt  */
#line 178 "parser.y"
                                        { (yyval.stmt) = new If((yyvsp[-2].expr), (yyvsp[0].stmt), yylineno); }
#line 1549 "parser.cpp"
    break;

  case 34: /* stmt: "if" cond "then" stmt "else" stmt  */
#line 179 "parser.y"
                                        { (yyval.stmt) = new If((yyvsp[-4].expr), (yyvsp[-2].stmt), yylineno, (yyvsp[0].stmt)); }
#line 1555 "parser.cpp"
    break;

  case 35: /* stmt: "while" cond "do" stmt  */
#line 180 "parser.y"
                                        { (yyval.stmt) = new While((yyvsp[-2].expr), (yyvsp[0].stmt), yylineno); }
#line 1561 "parser.cpp"
    break;

  case 36: /* stmt: "return" ';'  */
#line 181 "parser.y"
                                        { (yyval.stmt) = new Return(yylineno); }
#line 1567 "parser.cpp"
    break;

  case 37: /* stmt: "return" expr ';'  */
#line 182 "parser.y"
                                        { (yyval.stmt) = new Return(yylineno, (yyvsp[-1].expr)); }
#line 1573 "parser.cpp"
    break;

  case 38: /* block: '{' stmt_list '}'  */
#line 186 "parser.y"
                                        { (yyval.block) = (yyvsp[-1].block); }
#line 1579 "parser.cpp"
    break;

  case 39: /* stmt_list: %empty  */
#line 190 "parser.y"
                                        { (yyval.block) = new Block(); }
#line 1585 "parser.cpp"
    break;

  case 40: /* stmt_list: stmt_list stmt  */
#line 191 "parser.y"
                                        { (yyvsp[-1].block)->append((yyvsp[0].stmt)); (yyval.block) = (yyvsp[-1].block); }
#line 1591 "parser.cpp"
    break;

  case 41: /* func-call: T_id '(' ')'  */
#line 195 "parser.y"
                                        { (yyval.funccall) = new FuncCall((yyvsp[-2].str), yylineno); }
#line 1597 "parser.cpp"
    break;

  case 42: /* func-call: T_id '(' expr expr_list ')'  */
#line 196 "parser.y"
                                        { (yyvsp[-1].exprlist)->putinfront((yyvsp[-2].expr)); (yyval.funccall) = new FuncCall((yyvsp[-4].str), yylineno, (yyvsp[-1].exprlist)); }
#line 1603 "parser.cpp"
    break;

  case 43: /* expr_list: %empty  */
#line 200 "parser.y"
                                        { (yyval.exprlist) = new ExprList(); }
#line 1609 "parser.cpp"
    break;

  case 44: /* expr_list: expr_list ',' expr  */
#line 201 "parser.y"
                                        { (yyvsp[-2].exprlist)->append((yyvsp[0].expr)); (yyval.exprlist) = (yyvsp[-2].exprlist); }
#line 1615 "parser.cpp"
    break;

  case 45: /* l-value: T_id  */
#line 205 "parser.y"
                                        { (yyval.expr) = new Id((yyvsp[0].str), yylineno); }
#line 1621 "parser.cpp"
    break;

  case 46: /* l-value: T_string  */
#line 206 "parser.y"
                                        { (yyval.expr) = new String((yyvsp[0].str)); }
#line 1627 "parser.cpp"
    break;

  case 47: /* l-value: l-value '[' expr ']'  */
#line 207 "parser.y"
                                        { (yyval.expr) = new ArrayCall((yyvsp[-3].expr), (yyvsp[-1].expr), yylineno); }
#line 1633 "parser.cpp"
    break;

  case 48: /* expr: T_const  */
#line 211 "parser.y"
                                        { (yyval.expr) = new Const((yyvsp[0].num)); }
#line 1639 "parser.cpp"
    break;

  case 49: /* expr: T_constchar  */
#line 212 "parser.y"
                                        { (yyval.expr) = new ConstChar((yyvsp[0].constchar)); }
#line 1645 "parser.cpp"
    break;

  case 50: /* expr: l-value  */
#line 213 "parser.y"
                                        { (yyval.expr) = (yyvsp[0].expr); }
#line 1651 "parser.cpp"
    break;

  case 51: /* expr: '(' expr ')'  */
#line 214 "parser.y"
                                        { (yyval.expr) = (yyvsp[-1].expr); }
#line 1657 "parser.cpp"
    break;

  case 52: /* expr: func-call  */
#line 215 "parser.y"
                                        { (yyval.expr) = (yyvsp[0].funccall); }
#line 1663 "parser.cpp"
    break;

  case 53: /* expr: '+' expr  */
#line 216 "parser.y"
                                        { (yyval.expr) = new Plus((yyvsp[0].expr), yylineno); }
#line 1669 "parser.cpp"
    break;

  case 54: /* expr: '-' expr  */
#line 217 "parser.y"
                                        { (yyval.expr) = new Minus((yyvsp[0].expr), yylineno); }
#line 1675 "parser.cpp"
    break;

  case 55: /* expr: expr '+' expr  */
#line 218 "parser.y"
                                        { (yyval.expr) = new BinOp((yyvsp[-2].expr), (yyvsp[-1].binop), (yyvsp[0].expr), yylineno); }
#line 1681 "parser.cpp"
    break;

  case 56: /* expr: expr '-' expr  */
#line 219 "parser.y"
                                        { (yyval.expr) = new BinOp((yyvsp[-2].expr), (yyvsp[-1].binop), (yyvsp[0].expr), yylineno); }
#line 1687 "parser.cpp"
    break;

  case 57: /* expr: expr '*' expr  */
#line 220 "parser.y"
                                        { (yyval.expr) = new BinOp((yyvsp[-2].expr), (yyvsp[-1].binop), (yyvsp[0].expr), yylineno); }
#line 1693 "parser.cpp"
    break;

  case 58: /* expr: expr "div" expr  */
#line 221 "parser.y"
                                        { (yyval.expr) = new BinOp((yyvsp[-2].expr), (yyvsp[-1].binop), (yyvsp[0].expr), yylineno); }
#line 1699 "parser.cpp"
    break;

  case 59: /* expr: expr "mod" expr  */
#line 222 "parser.y"
                                        { (yyval.expr) = new BinOp((yyvsp[-2].expr), (yyvsp[-1].binop), (yyvsp[0].expr), yylineno); }
#line 1705 "parser.cpp"
    break;

  case 60: /* cond: '(' cond ')'  */
#line 226 "parser.y"
                                        { (yyval.expr) = (yyvsp[-1].expr); }
#line 1711 "parser.cpp"
    break;

  case 61: /* cond: "not" cond  */
#line 227 "parser.y"
                                        { (yyval.expr) = new Not((yyvsp[0].expr), yylineno); }
#line 1717 "parser.cpp"
    break;

  case 62: /* cond: cond "and" cond  */
#line 228 "parser.y"
                                        { (yyval.expr) = new CondOp((yyvsp[-2].expr), (yyvsp[-1].condop), (yyvsp[0].expr), yylineno); }
#line 1723 "parser.cpp"
    break;

  case 63: /* cond: cond "or" cond  */
#line 229 "parser.y"
                                        { (yyval.expr) = new CondOp((yyvsp[-2].expr), (yyvsp[-1].condop), (yyvsp[0].expr), yylineno); }
#line 1729 "parser.cpp"
    break;

  case 64: /* cond: expr '=' expr  */
#line 230 "parser.y"
                                        { (yyval.expr) = new CondOp((yyvsp[-2].expr), (yyvsp[-1].condop), (yyvsp[0].expr), yylineno); }
#line 1735 "parser.cpp"
    break;

  case 65: /* cond: expr '#' expr  */
#line 231 "parser.y"
                                        { (yyval.expr) = new CondOp((yyvsp[-2].expr), (yyvsp[-1].condop), (yyvsp[0].expr), yylineno); }
#line 1741 "parser.cpp"
    break;

  case 66: /* cond: expr '<' expr  */
#line 232 "parser.y"
                                        { (yyval.expr) = new CondOp((yyvsp[-2].expr), (yyvsp[-1].condop), (yyvsp[0].expr), yylineno); }
#line 1747 "parser.cpp"
    break;

  case 67: /* cond: expr '>' expr  */
#line 233 "parser.y"
                                        { (yyval.expr) = new CondOp((yyvsp[-2].expr), (yyvsp[-1].condop), (yyvsp[0].expr), yylineno); }
#line 1753 "parser.cpp"
    break;

  case 68: /* cond: expr "<=" expr  */
#line 234 "parser.y"
                                        { (yyval.expr) = new CondOp((yyvsp[-2].expr), (yyvsp[-1].condop), (yyvsp[0].expr), yylineno); }
#line 1759 "parser.cpp"
    break;

  case 69: /* cond: expr ">=" expr  */
#line 235 "parser.y"
                                        { (yyval.expr) = new CondOp((yyvsp[-2].expr), (yyvsp[-1].condop), (yyvsp[0].expr), yylineno); }
#line 1765 "parser.cpp"
    break;


#line 1769 "parser.cpp"

      default: break;
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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
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
  ++yynerrs;

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
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
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

  return yyresult;
}

#line 239 "parser.y"


void yyerror(const char *msg){



  printf("\033[1;31m%s:\n\t\033[0m", msg);
  printf("Cannot parse token: ");
  printf("\033[1;35m%s\033[0m", yytext);
  printf(" -- line: ");
  printf("\033[1;36m%d\n\033[0m", yylineno);
  printf("\n- Compilation \033[1;31mFAILED\033[0m.\n");
  exit(42);
}

int main() {
    int result = yyparse();
    if (result == 0) printf("\033[1;32m- Semantics checked.\n\033[0m");
    return result;
}
