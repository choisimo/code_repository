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
#line 8 "subc.y"

/* Prologue section */
#include <stdio.h>
#include <strings.h>

int   yylex ();
int   yyerror (char* s);
void  reduce(char* s);

#line 81 "subc.tab.c"

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

#include "subc.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INTEGER_CONST = 3,              /* INTEGER_CONST  */
  YYSYMBOL_CHAR_CONST = 4,                 /* CHAR_CONST  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_ID = 6,                         /* ID  */
  YYSYMBOL_LOGICAL_OR = 7,                 /* LOGICAL_OR  */
  YYSYMBOL_LOGICAL_AND = 8,                /* LOGICAL_AND  */
  YYSYMBOL_RELOP = 9,                      /* RELOP  */
  YYSYMBOL_EQUOP = 10,                     /* EQUOP  */
  YYSYMBOL_TYPE = 11,                      /* TYPE  */
  YYSYMBOL_STRUCT = 12,                    /* STRUCT  */
  YYSYMBOL_SYM_NULL = 13,                  /* SYM_NULL  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_IF = 15,                        /* IF  */
  YYSYMBOL_ELSE = 16,                      /* ELSE  */
  YYSYMBOL_WHILE = 17,                     /* WHILE  */
  YYSYMBOL_FOR = 18,                       /* FOR  */
  YYSYMBOL_BREAK = 19,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 20,                  /* CONTINUE  */
  YYSYMBOL_INCOP = 21,                     /* INCOP  */
  YYSYMBOL_DECOP = 22,                     /* DECOP  */
  YYSYMBOL_STRUCTOP = 23,                  /* STRUCTOP  */
  YYSYMBOL_LOWER_THAN_ELSE = 24,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_25_ = 25,                       /* ','  */
  YYSYMBOL_26_ = 26,                       /* '='  */
  YYSYMBOL_27_ = 27,                       /* '+'  */
  YYSYMBOL_28_ = 28,                       /* '-'  */
  YYSYMBOL_29_ = 29,                       /* '*'  */
  YYSYMBOL_30_ = 30,                       /* '/'  */
  YYSYMBOL_31_ = 31,                       /* '%'  */
  YYSYMBOL_UMINUS = 32,                    /* UMINUS  */
  YYSYMBOL_33_ = 33,                       /* '!'  */
  YYSYMBOL_34_ = 34,                       /* '&'  */
  YYSYMBOL_35_ = 35,                       /* '['  */
  YYSYMBOL_36_ = 36,                       /* '('  */
  YYSYMBOL_37_ = 37,                       /* '.'  */
  YYSYMBOL_38_ = 38,                       /* ';'  */
  YYSYMBOL_39_ = 39,                       /* ']'  */
  YYSYMBOL_40_ = 40,                       /* '{'  */
  YYSYMBOL_41_ = 41,                       /* '}'  */
  YYSYMBOL_42_ = 42,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 43,                  /* $accept  */
  YYSYMBOL_program = 44,                   /* program  */
  YYSYMBOL_ext_def_list = 45,              /* ext_def_list  */
  YYSYMBOL_ext_def = 46,                   /* ext_def  */
  YYSYMBOL_type_specifier = 47,            /* type_specifier  */
  YYSYMBOL_struct_specifier = 48,          /* struct_specifier  */
  YYSYMBOL_func_decl = 49,                 /* func_decl  */
  YYSYMBOL_pointers = 50,                  /* pointers  */
  YYSYMBOL_param_list = 51,                /* param_list  */
  YYSYMBOL_param_decl = 52,                /* param_decl  */
  YYSYMBOL_def_list = 53,                  /* def_list  */
  YYSYMBOL_def = 54,                       /* def  */
  YYSYMBOL_compound_stmt = 55,             /* compound_stmt  */
  YYSYMBOL_stmt_list = 56,                 /* stmt_list  */
  YYSYMBOL_stmt = 57,                      /* stmt  */
  YYSYMBOL_expr_e = 58,                    /* expr_e  */
  YYSYMBOL_expr = 59,                      /* expr  */
  YYSYMBOL_binary = 60,                    /* binary  */
  YYSYMBOL_unary = 61,                     /* unary  */
  YYSYMBOL_args = 62                       /* args  */
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
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYLAST   228

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  73
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  151

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   280


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
       2,     2,     2,    33,     2,     2,     2,    31,    34,     2,
      36,    42,    29,    27,    25,    28,    37,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    38,
       2,    26,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    35,     2,    39,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,     2,    41,     2,     2,     2,     2,
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
      32
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    94,    94,    99,   101,   106,   108,   110,   112,   117,
     119,   124,   126,   131,   133,   138,   140,   145,   147,   152,
     154,   159,   161,   166,   168,   170,   175,   180,   182,   187,
     189,   191,   193,   195,   197,   199,   201,   203,   205,   210,
     212,   217,   219,   224,   226,   228,   230,   232,   234,   236,
     238,   240,   242,   247,   249,   251,   253,   255,   257,   259,
     261,   263,   265,   267,   269,   271,   273,   275,   277,   279,
     281,   283,   288,   290
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
  "\"end of file\"", "error", "\"invalid token\"", "INTEGER_CONST",
  "CHAR_CONST", "STRING", "ID", "LOGICAL_OR", "LOGICAL_AND", "RELOP",
  "EQUOP", "TYPE", "STRUCT", "SYM_NULL", "RETURN", "IF", "ELSE", "WHILE",
  "FOR", "BREAK", "CONTINUE", "INCOP", "DECOP", "STRUCTOP",
  "LOWER_THAN_ELSE", "','", "'='", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'!'", "'&'", "'['", "'('", "'.'", "';'", "']'", "'{'", "'}'",
  "')'", "$accept", "program", "ext_def_list", "ext_def", "type_specifier",
  "struct_specifier", "func_decl", "pointers", "param_list", "param_decl",
  "def_list", "def", "compound_stmt", "stmt_list", "stmt", "expr_e",
  "expr", "binary", "unary", "args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-115)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -115,    13,    97,  -115,  -115,     9,  -115,   -11,  -115,    -6,
      -4,  -115,    53,  -115,  -115,  -115,    56,    99,    15,    68,
      10,  -115,    73,   -11,  -115,    84,    58,    61,  -115,   -11,
      -5,  -115,    74,   109,  -115,  -115,  -115,  -115,  -115,   161,
      80,    83,    85,    92,    93,   161,   161,   161,   161,   161,
     161,   161,  -115,  -115,  -115,  -115,   101,   171,   -12,  -115,
     115,   117,    99,  -115,  -115,   183,   116,   161,   161,   161,
    -115,  -115,   111,   111,   111,   111,   111,   111,   107,  -115,
     161,   161,   161,   161,   161,   161,   161,   161,   161,  -115,
    -115,   149,   161,   161,    44,   152,  -115,   100,  -115,    21,
     154,   159,  -115,  -115,   131,   133,   138,  -115,  -115,   176,
     111,    54,   195,   141,    38,    38,  -115,  -115,  -115,  -115,
    -115,   148,  -115,  -115,    33,  -115,   174,  -115,   150,   153,
     123,   123,   161,  -115,   161,  -115,   157,  -115,   155,   175,
    -115,   181,  -115,  -115,  -115,   123,   161,  -115,   178,   123,
    -115
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,     9,     0,     3,    16,    10,     0,
      12,    15,     0,    22,     8,    22,     0,    28,     0,     0,
       0,     5,     0,    16,    21,     0,    11,     0,    13,    16,
       0,    17,    12,     0,    54,    55,    56,    57,    71,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    26,    30,    27,     0,    42,    50,     7,
       0,     0,     0,    14,    22,     0,     0,     0,     0,    40,
      37,    38,    62,    63,    58,    65,    59,    64,     0,    29,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,     0,     6,    19,    18,     0,
       0,     0,    23,    31,     0,     0,     0,    39,    53,    52,
      50,    51,    43,    44,    45,    46,    47,    48,    49,    68,
      41,     0,    70,    72,     0,    67,     0,    11,     0,     0,
       0,     0,    40,    66,     0,    69,     0,    25,     0,    33,
      35,     0,    73,    20,    24,     0,    40,    34,     0,     0,
      36
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -115,  -115,  -115,  -115,    -1,  -115,  -115,    47,  -115,   165,
     -13,  -115,   219,  -115,  -114,   -72,   -39,   129,   -42,  -115
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     6,    23,     8,     9,    12,    30,    31,
      17,    24,    54,    25,    55,   106,    56,    57,    58,   124
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      66,     7,    18,    72,    73,    74,    75,    76,    77,    89,
      90,    91,    78,     3,    92,    10,   139,   140,    11,    29,
      62,     4,    22,    93,    94,    95,     4,    22,   104,   105,
     107,   147,     4,    22,    13,   150,    15,    63,   110,   110,
     110,   110,   110,   110,   110,   110,   110,    34,    35,    36,
      37,    99,    28,   120,   121,   123,    26,    38,   134,    16,
     141,    29,   127,    82,    83,    45,    46,    86,    87,    88,
      33,    27,    47,    48,   148,   135,    61,    49,    50,    32,
      51,    84,    85,    86,    87,    88,   122,    34,    35,    36,
      37,    19,    20,   107,    21,   142,    59,    38,    39,    40,
      60,    41,    42,    43,    44,    45,    46,   107,     4,     5,
       4,    22,    47,    48,    64,    65,    67,    49,    50,    68,
      51,    69,    52,    97,    13,    53,    34,    35,    36,    37,
      70,    71,    89,    90,    91,   126,    38,    39,    40,    79,
      41,    42,    43,    44,    45,    46,    93,    94,    95,   108,
      82,    47,    48,    96,   103,   119,    49,    50,   125,    51,
     128,    52,   129,    13,    34,    35,    36,    37,    84,    85,
      86,    87,    88,   130,    38,   131,   132,   136,    80,    81,
      82,    83,    45,    46,    81,    82,    83,   133,   137,    47,
      48,   145,   138,   144,    49,    50,   143,    51,    84,    85,
      86,    87,    88,    84,    85,    86,    87,    88,   100,   109,
     111,   112,   113,   114,   115,   116,   117,   118,   101,   146,
     149,   102,    84,    85,    86,    87,    88,    98,    14
};

static const yytype_uint8 yycheck[] =
{
      39,     2,    15,    45,    46,    47,    48,    49,    50,    21,
      22,    23,    51,     0,    26,     6,   130,   131,    29,    20,
      25,    11,    12,    35,    36,    37,    11,    12,    67,    68,
      69,   145,    11,    12,    40,   149,    40,    42,    80,    81,
      82,    83,    84,    85,    86,    87,    88,     3,     4,     5,
       6,    64,    42,    92,    93,    94,    41,    13,    25,     6,
     132,    62,    41,     9,    10,    21,    22,    29,    30,    31,
      23,     3,    28,    29,   146,    42,    29,    33,    34,     6,
      36,    27,    28,    29,    30,    31,    42,     3,     4,     5,
       6,    35,    36,   132,    38,   134,    38,    13,    14,    15,
      39,    17,    18,    19,    20,    21,    22,   146,    11,    12,
      11,    12,    28,    29,    40,     6,    36,    33,    34,    36,
      36,    36,    38,     6,    40,    41,     3,     4,     5,     6,
      38,    38,    21,    22,    23,    35,    13,    14,    15,    38,
      17,    18,    19,    20,    21,    22,    35,    36,    37,    42,
       9,    28,    29,    38,    38,     6,    33,    34,     6,    36,
       6,    38,     3,    40,     3,     4,     5,     6,    27,    28,
      29,    30,    31,    42,    13,    42,    38,     3,     7,     8,
       9,    10,    21,    22,     8,     9,    10,    39,    38,    28,
      29,    16,    39,    38,    33,    34,    39,    36,    27,    28,
      29,    30,    31,    27,    28,    29,    30,    31,    25,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    35,    38,
      42,    38,    27,    28,    29,    30,    31,    62,     9
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    44,    45,     0,    11,    12,    46,    47,    48,    49,
       6,    29,    50,    40,    55,    40,     6,    53,    53,    35,
      36,    38,    12,    47,    54,    56,    41,     3,    42,    47,
      51,    52,     6,    50,     3,     4,     5,     6,    13,    14,
      15,    17,    18,    19,    20,    21,    22,    28,    29,    33,
      34,    36,    38,    41,    55,    57,    59,    60,    61,    38,
      39,    50,    25,    42,    40,     6,    59,    36,    36,    36,
      38,    38,    61,    61,    61,    61,    61,    61,    59,    38,
       7,     8,     9,    10,    27,    28,    29,    30,    31,    21,
      22,    23,    26,    35,    36,    37,    38,     6,    52,    53,
      25,    35,    38,    38,    59,    59,    58,    59,    42,    60,
      61,    60,    60,    60,    60,    60,    60,    60,    60,     6,
      59,    59,    42,    59,    62,     6,    35,    41,     6,     3,
      42,    42,    38,    39,    25,    42,     3,    38,    39,    57,
      57,    58,    59,    39,    38,    16,    38,    57,    58,    42,
      57
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    43,    44,    45,    45,    46,    46,    46,    46,    47,
      47,    48,    48,    49,    49,    50,    50,    51,    51,    52,
      52,    53,    53,    54,    54,    54,    55,    56,    56,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    58,
      58,    59,    59,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    62,    62
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     4,     7,     6,     2,     1,
       1,     5,     2,     5,     6,     1,     0,     1,     3,     3,
       6,     2,     0,     4,     7,     6,     4,     2,     0,     2,
       1,     3,     1,     5,     7,     5,     9,     2,     2,     1,
       0,     3,     1,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     3,     1,     1,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     4,     3,     3,     4,
       3,     1,     1,     3
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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
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

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: ext_def_list  */
#line 95 "subc.y"
    { reduce("program->ext_def_list"); }
#line 1239 "subc.tab.c"
    break;

  case 3: /* ext_def_list: ext_def_list ext_def  */
#line 100 "subc.y"
    { reduce("ext_def_list->ext_def_list ext_def"); }
#line 1245 "subc.tab.c"
    break;

  case 4: /* ext_def_list: %empty  */
#line 102 "subc.y"
    { reduce("ext_def_list->epsilon"); }
#line 1251 "subc.tab.c"
    break;

  case 5: /* ext_def: type_specifier pointers ID ';'  */
#line 107 "subc.y"
    { reduce("ext_def->type_specifier pointers ID ';'"); }
#line 1257 "subc.tab.c"
    break;

  case 6: /* ext_def: type_specifier pointers ID '[' INTEGER_CONST ']' ';'  */
#line 109 "subc.y"
    { reduce("ext_def->type_specifier pointers ID '[' INTEGER_CONST ']' ';'"); }
#line 1263 "subc.tab.c"
    break;

  case 7: /* ext_def: STRUCT ID '{' def_list '}' ';'  */
#line 111 "subc.y"
    { reduce("ext_def->STRUCT ID '{' def_list '}' ';'"); }
#line 1269 "subc.tab.c"
    break;

  case 8: /* ext_def: func_decl compound_stmt  */
#line 113 "subc.y"
    { reduce("ext_def->func_decl compound_stmt"); }
#line 1275 "subc.tab.c"
    break;

  case 9: /* type_specifier: TYPE  */
#line 118 "subc.y"
    { reduce("type_specifier->TYPE"); }
#line 1281 "subc.tab.c"
    break;

  case 10: /* type_specifier: struct_specifier  */
#line 120 "subc.y"
    { reduce("type_specifier->struct_specifier"); }
#line 1287 "subc.tab.c"
    break;

  case 11: /* struct_specifier: STRUCT ID '{' def_list '}'  */
#line 125 "subc.y"
    { reduce("struct_specifier->STRUCT ID '{' def_list '}'"); }
#line 1293 "subc.tab.c"
    break;

  case 12: /* struct_specifier: STRUCT ID  */
#line 127 "subc.y"
    { reduce("struct_specifier->STRUCT ID"); }
#line 1299 "subc.tab.c"
    break;

  case 13: /* func_decl: type_specifier pointers ID '(' ')'  */
#line 132 "subc.y"
    { reduce("func_decl->type_specifier pointers ID '(' ')'"); }
#line 1305 "subc.tab.c"
    break;

  case 14: /* func_decl: type_specifier pointers ID '(' param_list ')'  */
#line 134 "subc.y"
    { reduce("func_decl->type_specifier pointers ID '(' param_list ')'"); }
#line 1311 "subc.tab.c"
    break;

  case 15: /* pointers: '*'  */
#line 139 "subc.y"
    { reduce("pointers->'*'"); }
#line 1317 "subc.tab.c"
    break;

  case 16: /* pointers: %empty  */
#line 141 "subc.y"
    { reduce("pointers->epsilon"); }
#line 1323 "subc.tab.c"
    break;

  case 17: /* param_list: param_decl  */
#line 146 "subc.y"
    { reduce("param_list->param_decl"); }
#line 1329 "subc.tab.c"
    break;

  case 18: /* param_list: param_list ',' param_decl  */
#line 148 "subc.y"
    { reduce("param_list->param_list ',' param_decl"); }
#line 1335 "subc.tab.c"
    break;

  case 19: /* param_decl: type_specifier pointers ID  */
#line 153 "subc.y"
    { reduce("param_decl->type_specifier pointers ID"); }
#line 1341 "subc.tab.c"
    break;

  case 20: /* param_decl: type_specifier pointers ID '[' INTEGER_CONST ']'  */
#line 155 "subc.y"
    { reduce("param_decl->type_specifier pointers ID '[' INTEGER_CONST ']' "); }
#line 1347 "subc.tab.c"
    break;

  case 21: /* def_list: def_list def  */
#line 160 "subc.y"
    { reduce("def_list->def_list def"); }
#line 1353 "subc.tab.c"
    break;

  case 22: /* def_list: %empty  */
#line 162 "subc.y"
    { reduce("def_list->epsilon"); }
#line 1359 "subc.tab.c"
    break;

  case 23: /* def: type_specifier pointers ID ';'  */
#line 167 "subc.y"
    { reduce("def->type_specifier pointers ID ';'"); }
#line 1365 "subc.tab.c"
    break;

  case 24: /* def: type_specifier pointers ID '[' INTEGER_CONST ']' ';'  */
#line 169 "subc.y"
    { reduce("def->type_specifier pointers ID '[' INTEGER_CONST ']' ';'"); }
#line 1371 "subc.tab.c"
    break;

  case 25: /* def: type_specifier pointers ID ',' ID ';'  */
#line 171 "subc.y"
    { reduce("def->type_specifier pointers ID ',' ID ';'"); }
#line 1377 "subc.tab.c"
    break;

  case 26: /* compound_stmt: '{' def_list stmt_list '}'  */
#line 176 "subc.y"
    { reduce("compound_stmt->'{' def_list stmt_list '}'"); }
#line 1383 "subc.tab.c"
    break;

  case 27: /* stmt_list: stmt_list stmt  */
#line 181 "subc.y"
    { reduce("stmt_list->stmt_list stmt"); }
#line 1389 "subc.tab.c"
    break;

  case 28: /* stmt_list: %empty  */
#line 183 "subc.y"
    { reduce("stmt_list->epsilon"); }
#line 1395 "subc.tab.c"
    break;

  case 29: /* stmt: expr ';'  */
#line 188 "subc.y"
    { reduce("stmt->expr ';'"); }
#line 1401 "subc.tab.c"
    break;

  case 30: /* stmt: compound_stmt  */
#line 190 "subc.y"
    { reduce("stmt->compound_stmt"); }
#line 1407 "subc.tab.c"
    break;

  case 31: /* stmt: RETURN expr ';'  */
#line 192 "subc.y"
    { reduce("stmt->RETURN expr ';'"); }
#line 1413 "subc.tab.c"
    break;

  case 32: /* stmt: ';'  */
#line 194 "subc.y"
    { reduce("stmt->;"); }
#line 1419 "subc.tab.c"
    break;

  case 33: /* stmt: IF '(' expr ')' stmt  */
#line 196 "subc.y"
    { reduce("stmt->IF '(' expr ')' stmt"); }
#line 1425 "subc.tab.c"
    break;

  case 34: /* stmt: IF '(' expr ')' stmt ELSE stmt  */
#line 198 "subc.y"
    { reduce("stmt->IF '(' expr ')' stmt ELSE stmt"); }
#line 1431 "subc.tab.c"
    break;

  case 35: /* stmt: WHILE '(' expr ')' stmt  */
#line 200 "subc.y"
    { reduce("stmt->WHILE '(' expr ')' stmt"); }
#line 1437 "subc.tab.c"
    break;

  case 36: /* stmt: FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt  */
#line 202 "subc.y"
    { reduce("stmt->FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt"); }
#line 1443 "subc.tab.c"
    break;

  case 37: /* stmt: BREAK ';'  */
#line 204 "subc.y"
    { reduce("stmt->BREAK ';'"); }
#line 1449 "subc.tab.c"
    break;

  case 38: /* stmt: CONTINUE ';'  */
#line 206 "subc.y"
    { reduce("stmt->CONTINUE ';'"); }
#line 1455 "subc.tab.c"
    break;

  case 39: /* expr_e: expr  */
#line 211 "subc.y"
    { reduce("expr_e->expr"); }
#line 1461 "subc.tab.c"
    break;

  case 40: /* expr_e: %empty  */
#line 213 "subc.y"
    { reduce("expr_e->epsilon"); }
#line 1467 "subc.tab.c"
    break;

  case 41: /* expr: unary '=' expr  */
#line 218 "subc.y"
    { reduce("expr->unary '=' expr"); }
#line 1473 "subc.tab.c"
    break;

  case 42: /* expr: binary  */
#line 220 "subc.y"
    { reduce("expr->binary"); }
#line 1479 "subc.tab.c"
    break;

  case 43: /* binary: binary RELOP binary  */
#line 225 "subc.y"
    { reduce("binary->binary RELOP binary"); }
#line 1485 "subc.tab.c"
    break;

  case 44: /* binary: binary EQUOP binary  */
#line 227 "subc.y"
    { reduce("binary->binary EQUOP binary"); }
#line 1491 "subc.tab.c"
    break;

  case 45: /* binary: binary '+' binary  */
#line 229 "subc.y"
    { reduce("binary->binary '+' binary"); }
#line 1497 "subc.tab.c"
    break;

  case 46: /* binary: binary '-' binary  */
#line 231 "subc.y"
    { reduce("binary->binary '-' binary"); }
#line 1503 "subc.tab.c"
    break;

  case 47: /* binary: binary '*' binary  */
#line 233 "subc.y"
    { reduce("binary->binary '*' binary"); }
#line 1509 "subc.tab.c"
    break;

  case 48: /* binary: binary '/' binary  */
#line 235 "subc.y"
    { reduce("binary->binary '/' binary"); }
#line 1515 "subc.tab.c"
    break;

  case 49: /* binary: binary '%' binary  */
#line 237 "subc.y"
    { reduce("binary->binary '%' binary"); }
#line 1521 "subc.tab.c"
    break;

  case 50: /* binary: unary  */
#line 239 "subc.y"
    { reduce("binary->unary"); }
#line 1527 "subc.tab.c"
    break;

  case 51: /* binary: binary LOGICAL_AND binary  */
#line 241 "subc.y"
    { reduce("binary->binary LOGICAL_AND binary"); }
#line 1533 "subc.tab.c"
    break;

  case 52: /* binary: binary LOGICAL_OR binary  */
#line 243 "subc.y"
    { reduce("binary->binary LOGICAL_OR binary"); }
#line 1539 "subc.tab.c"
    break;

  case 53: /* unary: '(' expr ')'  */
#line 248 "subc.y"
    { reduce("unary->'(' expr ')'"); }
#line 1545 "subc.tab.c"
    break;

  case 54: /* unary: INTEGER_CONST  */
#line 250 "subc.y"
    { reduce("unary->INTEGER_CONST"); }
#line 1551 "subc.tab.c"
    break;

  case 55: /* unary: CHAR_CONST  */
#line 252 "subc.y"
    { reduce("unary->CHAR_CONST"); }
#line 1557 "subc.tab.c"
    break;

  case 56: /* unary: STRING  */
#line 254 "subc.y"
    { reduce("unary->STRING"); }
#line 1563 "subc.tab.c"
    break;

  case 57: /* unary: ID  */
#line 256 "subc.y"
    { reduce("unary->ID"); }
#line 1569 "subc.tab.c"
    break;

  case 58: /* unary: '-' unary  */
#line 258 "subc.y"
    { reduce("unary->'-' unary"); }
#line 1575 "subc.tab.c"
    break;

  case 59: /* unary: '!' unary  */
#line 260 "subc.y"
    { reduce("unary->'!' unary"); }
#line 1581 "subc.tab.c"
    break;

  case 60: /* unary: unary INCOP  */
#line 262 "subc.y"
    { reduce("unary->unary INCOP"); }
#line 1587 "subc.tab.c"
    break;

  case 61: /* unary: unary DECOP  */
#line 264 "subc.y"
    { reduce("unary->unary DECOP"); }
#line 1593 "subc.tab.c"
    break;

  case 62: /* unary: INCOP unary  */
#line 266 "subc.y"
    { reduce("unary->INCOP unary"); }
#line 1599 "subc.tab.c"
    break;

  case 63: /* unary: DECOP unary  */
#line 268 "subc.y"
    { reduce("unary->DECOP unary"); }
#line 1605 "subc.tab.c"
    break;

  case 64: /* unary: '&' unary  */
#line 270 "subc.y"
    { reduce("unary->'&' unary"); }
#line 1611 "subc.tab.c"
    break;

  case 65: /* unary: '*' unary  */
#line 272 "subc.y"
    { reduce("unary->'*' unary"); }
#line 1617 "subc.tab.c"
    break;

  case 66: /* unary: unary '[' expr ']'  */
#line 274 "subc.y"
    { reduce("unary->unary '[' expr ']'"); }
#line 1623 "subc.tab.c"
    break;

  case 67: /* unary: unary '.' ID  */
#line 276 "subc.y"
    { reduce("unary->unary '.' ID"); }
#line 1629 "subc.tab.c"
    break;

  case 68: /* unary: unary STRUCTOP ID  */
#line 278 "subc.y"
    { reduce("unary->unary STRUCTOP ID"); }
#line 1635 "subc.tab.c"
    break;

  case 69: /* unary: unary '(' args ')'  */
#line 280 "subc.y"
    { reduce("unary->unary '(' args ')' "); }
#line 1641 "subc.tab.c"
    break;

  case 70: /* unary: unary '(' ')'  */
#line 282 "subc.y"
    { reduce("unary->unary '(' ')' "); }
#line 1647 "subc.tab.c"
    break;

  case 71: /* unary: SYM_NULL  */
#line 284 "subc.y"
    { reduce("unary->SYM_NULL"); }
#line 1653 "subc.tab.c"
    break;

  case 72: /* args: expr  */
#line 289 "subc.y"
    { reduce("args->expr"); }
#line 1659 "subc.tab.c"
    break;

  case 73: /* args: args ',' expr  */
#line 291 "subc.y"
    { reduce("args->args ',' expr"); }
#line 1665 "subc.tab.c"
    break;


#line 1669 "subc.tab.c"

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
                      yytoken, &yylval);
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


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 294 "subc.y"


/* Epilogue section */

int yyerror (char* s) {
  fprintf (stderr, "yyerror: %s\n", s);
}

void reduce(char* s) {
  printf("%s\n", s);
}
