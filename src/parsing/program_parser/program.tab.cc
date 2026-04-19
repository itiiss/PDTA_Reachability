// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Take the name prefix into account.
#define yylex   ppyylex

// First part of user declarations.

#line 39 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "program.tab.hh"

// User implementation prologue.

#line 53 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 45 "program.yy" // lalr1.cc:413

  // Declare the lexer for the parser's sake.
  tchecker::parsing::program::parser_t::symbol_type ppyylex
  (std::string const & program_context,
  tchecker::log_t & log,
  tchecker::expression_t * & expr,
  tchecker::statement_t * & stmt);
  
  
  // Global variables
  static unsigned int error_count;
  
  
  // Fake expression used is case of syntax error to allow parsing of the
  // entire expression
  class fake_expression_t final : public tchecker::lvalue_expression_t {
    public:
    virtual ~fake_expression_t() = default;
    private:
    virtual std::ostream & do_output(std::ostream & os) const
    {
      return os;
    }
    
    virtual tchecker::expression_t * do_clone() const
    {
      return new fake_expression_t();
    }
    
    virtual void do_visit(tchecker::expression_visitor_t & v) const
    {}
  };
  
  
  // Fake variable expression used is case of syntax error to allow parsing of
  // the entire expression
  class fake_var_expression_t final : public tchecker::var_expression_t {
    public:
    fake_var_expression_t() : tchecker::var_expression_t("fake") {}
    
    virtual ~fake_var_expression_t() = default;
    private:
    virtual std::ostream & do_output(std::ostream & os) const
    {
      return os;
    }
    
    virtual tchecker::expression_t * do_clone() const
    {
      return new fake_var_expression_t();
    }
    
    virtual void do_visit(tchecker::expression_visitor_t & v) const
    {}
  };
  
  
  
  // Fake statement used in case of syntax error to allow parsing of the
  // entire statement
  class fake_statement_t final : public tchecker::statement_t {
    public:
    virtual ~fake_statement_t() = default;
    private:
    virtual std::ostream & do_output(std::ostream & os) const
    {
      return os;
    }
    
    virtual tchecker::statement_t * do_clone() const
    {
      return new fake_statement_t();
    }
    
    virtual void do_visit(tchecker::statement_visitor_t & v) const
    {}
  };

#line 134 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if PPYYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !PPYYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !PPYYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 13 "program.yy" // lalr1.cc:479
namespace tchecker { namespace parsing { namespace program {
#line 220 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser_t::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
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
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser_t::parser_t (std::string const & program_context_yyarg, tchecker::log_t & log_yyarg, tchecker::expression_t * & expr_yyarg, tchecker::statement_t * & stmt_yyarg)
    :
#if PPYYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      program_context (program_context_yyarg),
      log (log_yyarg),
      expr (expr_yyarg),
      stmt (stmt_yyarg)
  {}

  parser_t::~parser_t ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  parser_t::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  parser_t::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  parser_t::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  parser_t::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  parser_t::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  parser_t::symbol_number_type
  parser_t::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  parser_t::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  parser_t::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 47: // predicate_operator
        value.move< enum tchecker::binary_operator_t > (that.value);
        break;

      case 30: // "identifier"
      case 31: // "integer"
        value.move< std::string > (that.value);
        break;

      case 43: // conjunctive_formula
      case 44: // non_atomic_conjunctive_formula
      case 45: // atomic_formula
      case 46: // predicate_formula
      case 48: // term
        value.move< tchecker::expression_t * > (that.value);
        break;

      case 51: // integer
        value.move< tchecker::integer_t > (that.value);
        break;

      case 49: // lvalue_term
        value.move< tchecker::lvalue_expression_t * > (that.value);
        break;

      case 36: // sequence_statement
      case 37: // statement
      case 38: // if_statement
      case 39: // loop_statement
      case 40: // local_statement
      case 41: // simple_statement
      case 42: // assignment
        value.move< tchecker::statement_t * > (that.value);
        break;

      case 50: // variable_term
        value.move< tchecker::var_expression_t * > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  parser_t::stack_symbol_type&
  parser_t::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 47: // predicate_operator
        value.copy< enum tchecker::binary_operator_t > (that.value);
        break;

      case 30: // "identifier"
      case 31: // "integer"
        value.copy< std::string > (that.value);
        break;

      case 43: // conjunctive_formula
      case 44: // non_atomic_conjunctive_formula
      case 45: // atomic_formula
      case 46: // predicate_formula
      case 48: // term
        value.copy< tchecker::expression_t * > (that.value);
        break;

      case 51: // integer
        value.copy< tchecker::integer_t > (that.value);
        break;

      case 49: // lvalue_term
        value.copy< tchecker::lvalue_expression_t * > (that.value);
        break;

      case 36: // sequence_statement
      case 37: // statement
      case 38: // if_statement
      case 39: // loop_statement
      case 40: // local_statement
      case 41: // simple_statement
      case 42: // assignment
        value.copy< tchecker::statement_t * > (that.value);
        break;

      case 50: // variable_term
        value.copy< tchecker::var_expression_t * > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  parser_t::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if PPYYDEBUG
  template <typename Base>
  void
  parser_t::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
            case 30: // "identifier"

#line 192 "program.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 468 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 31: // "integer"

#line 192 "program.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 475 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 36: // sequence_statement

#line 193 "program.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::statement_t * > (); }
#line 482 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 37: // statement

#line 193 "program.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::statement_t * > (); }
#line 489 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 38: // if_statement

#line 192 "program.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< tchecker::statement_t * > (); }
#line 496 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 39: // loop_statement

#line 192 "program.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< tchecker::statement_t * > (); }
#line 503 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 40: // local_statement

#line 192 "program.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< tchecker::statement_t * > (); }
#line 510 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 41: // simple_statement

#line 192 "program.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< tchecker::statement_t * > (); }
#line 517 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 42: // assignment

#line 193 "program.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::statement_t * > (); }
#line 524 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 43: // conjunctive_formula

#line 193 "program.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::expression_t * > (); }
#line 531 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 44: // non_atomic_conjunctive_formula

#line 193 "program.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::expression_t * > (); }
#line 538 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 45: // atomic_formula

#line 193 "program.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::expression_t * > (); }
#line 545 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 46: // predicate_formula

#line 193 "program.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::expression_t * > (); }
#line 552 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 47: // predicate_operator

#line 192 "program.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< enum tchecker::binary_operator_t > (); }
#line 559 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 48: // term

#line 193 "program.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::expression_t * > (); }
#line 566 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 49: // lvalue_term

#line 193 "program.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::lvalue_expression_t * > (); }
#line 573 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 50: // variable_term

#line 193 "program.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::var_expression_t * > (); }
#line 580 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;

      case 51: // integer

#line 192 "program.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< tchecker::integer_t > (); }
#line 587 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:636
        break;


      default:
        break;
    }
    yyo << ')';
  }
#endif

  inline
  void
  parser_t::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  parser_t::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  parser_t::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if PPYYDEBUG
  std::ostream&
  parser_t::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser_t::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser_t::debug_level_type
  parser_t::debug_level () const
  {
    return yydebug_;
  }

  void
  parser_t::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // PPYYDEBUG

  inline parser_t::state_type
  parser_t::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  parser_t::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  parser_t::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser_t::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 125 "program.yy" // lalr1.cc:741
{
  error_count = 0;
}

#line 705 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:741

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (program_context, log, expr, stmt));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 47: // predicate_operator
        yylhs.value.build< enum tchecker::binary_operator_t > ();
        break;

      case 30: // "identifier"
      case 31: // "integer"
        yylhs.value.build< std::string > ();
        break;

      case 43: // conjunctive_formula
      case 44: // non_atomic_conjunctive_formula
      case 45: // atomic_formula
      case 46: // predicate_formula
      case 48: // term
        yylhs.value.build< tchecker::expression_t * > ();
        break;

      case 51: // integer
        yylhs.value.build< tchecker::integer_t > ();
        break;

      case 49: // lvalue_term
        yylhs.value.build< tchecker::lvalue_expression_t * > ();
        break;

      case 36: // sequence_statement
      case 37: // statement
      case 38: // if_statement
      case 39: // loop_statement
      case 40: // local_statement
      case 41: // simple_statement
      case 42: // assignment
        yylhs.value.build< tchecker::statement_t * > ();
        break;

      case 50: // variable_term
        yylhs.value.build< tchecker::var_expression_t * > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 212 "program.yy" // lalr1.cc:859
    { expr = nullptr;
  if (error_count > 0) {
    stmt = nullptr;
    delete yystack_[0].value.as< tchecker::statement_t * > ();
  }
  else
    stmt = yystack_[0].value.as< tchecker::statement_t * > ();
}
#line 861 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 221 "program.yy" // lalr1.cc:859
    {
  if (error_count > 0) {
    expr = nullptr;
    delete yystack_[0].value.as< tchecker::expression_t * > ();
  }
  else
    expr = yystack_[0].value.as< tchecker::expression_t * > ();
  stmt = nullptr;
}
#line 875 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 239 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = yystack_[1].value.as< tchecker::statement_t * > (); }
#line 881 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 241 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::statement_t * > () = new tchecker::sequence_statement_t(yystack_[2].value.as< tchecker::statement_t * > (), yystack_[0].value.as< tchecker::statement_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::statement_t * > () = new fake_statement_t();
  }
}
#line 895 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 254 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = yystack_[0].value.as< tchecker::statement_t * > (); }
#line 901 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 256 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = yystack_[0].value.as< tchecker::statement_t * > (); }
#line 907 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 258 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = yystack_[0].value.as< tchecker::statement_t * > (); }
#line 913 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 263 "program.yy" // lalr1.cc:859
    {  yylhs.value.as< tchecker::statement_t * > () = new tchecker::if_statement_t(yystack_[3].value.as< tchecker::expression_t * > (), yystack_[1].value.as< tchecker::statement_t * > (), new tchecker::nop_statement_t()); }
#line 919 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 265 "program.yy" // lalr1.cc:859
    {  yylhs.value.as< tchecker::statement_t * > () = new tchecker::if_statement_t(yystack_[5].value.as< tchecker::expression_t * > (), yystack_[3].value.as< tchecker::statement_t * > (), yystack_[1].value.as< tchecker::statement_t * > ()); }
#line 925 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 270 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = new tchecker::while_statement_t(yystack_[3].value.as< tchecker::expression_t * > (), yystack_[1].value.as< tchecker::statement_t * > ()); }
#line 931 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 275 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = new tchecker::local_var_statement_t(yystack_[0].value.as< tchecker::var_expression_t * > ()); }
#line 937 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 277 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = new tchecker::local_var_statement_t(yystack_[2].value.as< tchecker::var_expression_t * > (), yystack_[0].value.as< tchecker::expression_t * > ()); }
#line 943 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 279 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = new tchecker::local_array_statement_t(yystack_[3].value.as< tchecker::var_expression_t * > (), yystack_[1].value.as< tchecker::expression_t * > ()); }
#line 949 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 284 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = yystack_[0].value.as< tchecker::statement_t * > (); }
#line 955 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 286 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = new tchecker::nop_statement_t(); }
#line 961 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 288 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::statement_t * > () = yystack_[0].value.as< tchecker::statement_t * > (); }
#line 967 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 294 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::statement_t * > () = new tchecker::assign_statement_t(yystack_[2].value.as< tchecker::lvalue_expression_t * > (), yystack_[0].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::statement_t * > () = new fake_statement_t();
  }
}
#line 981 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 308 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::expression_t * > () = yystack_[0].value.as< tchecker::expression_t * > (); }
#line 987 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 310 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::expression_t * > () =yystack_[0].value.as< tchecker::expression_t * > (); }
#line 993 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 316 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::par_expression_t(yystack_[1].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1007 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 326 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::binary_expression_t(tchecker::EXPR_OP_LAND, yystack_[2].value.as< tchecker::expression_t * > (), yystack_[0].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1021 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 340 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::expression_t * > () = yystack_[0].value.as< tchecker::expression_t * > (); }
#line 1027 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 342 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::expression_t * > () = yystack_[0].value.as< tchecker::expression_t * > (); }
#line 1033 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 344 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::unary_expression_t(tchecker::EXPR_OP_LNOT, yystack_[0].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1047 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 358 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::par_expression_t(yystack_[1].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1061 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 368 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::binary_expression_t(yystack_[1].value.as< enum tchecker::binary_operator_t > (), yystack_[2].value.as< tchecker::expression_t * > (), yystack_[0].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1075 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 381 "program.yy" // lalr1.cc:859
    { yylhs.value.as< enum tchecker::binary_operator_t > () = tchecker::EXPR_OP_EQ; }
#line 1081 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 382 "program.yy" // lalr1.cc:859
    { yylhs.value.as< enum tchecker::binary_operator_t > () = tchecker::EXPR_OP_NEQ; }
#line 1087 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 383 "program.yy" // lalr1.cc:859
    { yylhs.value.as< enum tchecker::binary_operator_t > () = tchecker::EXPR_OP_LT; }
#line 1093 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 384 "program.yy" // lalr1.cc:859
    { yylhs.value.as< enum tchecker::binary_operator_t > () = tchecker::EXPR_OP_LE; }
#line 1099 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 385 "program.yy" // lalr1.cc:859
    { yylhs.value.as< enum tchecker::binary_operator_t > () = tchecker::EXPR_OP_GE; }
#line 1105 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 386 "program.yy" // lalr1.cc:859
    { yylhs.value.as< enum tchecker::binary_operator_t > () = tchecker::EXPR_OP_GT; }
#line 1111 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 392 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::int_expression_t(yystack_[0].value.as< tchecker::integer_t > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1125 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 402 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::expression_t * > () = yystack_[0].value.as< tchecker::lvalue_expression_t * > (); }
#line 1131 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 404 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::par_expression_t(yystack_[1].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1145 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 414 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::unary_expression_t(tchecker::EXPR_OP_NEG, yystack_[0].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1159 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 424 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::binary_expression_t(tchecker::EXPR_OP_PLUS, yystack_[2].value.as< tchecker::expression_t * > (), yystack_[0].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1173 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 434 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::binary_expression_t(tchecker::EXPR_OP_MINUS, yystack_[2].value.as< tchecker::expression_t * > (), yystack_[0].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1187 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 444 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::binary_expression_t(tchecker::EXPR_OP_TIMES, yystack_[2].value.as< tchecker::expression_t * > (), yystack_[0].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1201 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 454 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::binary_expression_t(tchecker::EXPR_OP_DIV, yystack_[2].value.as< tchecker::expression_t * > (), yystack_[0].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1215 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 464 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::binary_expression_t(tchecker::EXPR_OP_MOD, yystack_[2].value.as< tchecker::expression_t * > (), yystack_[0].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1229 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 474 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::expression_t * > () = new tchecker::ite_expression_t(yystack_[5].value.as< tchecker::expression_t * > (), yystack_[3].value.as< tchecker::expression_t * > (), yystack_[1].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::expression_t * > () = new fake_expression_t();
  }
}
#line 1243 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 488 "program.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::lvalue_expression_t * > () = yystack_[0].value.as< tchecker::var_expression_t * > (); }
#line 1249 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 490 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::lvalue_expression_t * > () = new tchecker::array_expression_t(yystack_[3].value.as< tchecker::var_expression_t * > (), yystack_[1].value.as< tchecker::expression_t * > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::lvalue_expression_t * > () = new fake_expression_t();
  }
}
#line 1263 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 504 "program.yy" // lalr1.cc:859
    {
  try {
    yylhs.value.as< tchecker::var_expression_t * > () = new tchecker::var_expression_t(yystack_[0].value.as< std::string > ());
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
    yylhs.value.as< tchecker::var_expression_t * > () = new fake_var_expression_t();
  }
}
#line 1277 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 518 "program.yy" // lalr1.cc:859
    {
  try {
    long long l = std::stoll(yystack_[0].value.as< std::string > (), nullptr, 10);
    if ( (l < std::numeric_limits<integer_t>::min()) || (l > std::numeric_limits<integer_t>::max()) )
    throw std::out_of_range("");
    yylhs.value.as< tchecker::integer_t > () = static_cast<tchecker::integer_t>(l);
  }
  catch (std::exception const & e) {
    error(yystack_[0].location, "value " + yystack_[0].value.as< std::string > () + " out of range " + std::to_string(std::numeric_limits<integer_t>::min()) + "," +
    std::to_string(std::numeric_limits<integer_t>::max()) );
    yylhs.value.as< tchecker::integer_t > () = 0;
  }
}
#line 1295 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
    break;


#line 1299 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  parser_t::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  parser_t::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser_t::yypact_ninf_ = -39;

  const signed char parser_t::yytable_ninf_ = -1;

  const short int
  parser_t::yypact_[] =
  {
      53,     8,    83,     4,    46,    46,   -39,    -5,   -39,   -39,
      28,   -39,    21,   -39,   -39,   -39,   -39,   -39,   -39,   -39,
      23,   -39,   133,    37,    35,   -39,    46,    47,    23,    54,
     110,   -39,    70,   -39,    81,   -39,    27,    40,    18,   -39,
     -13,   -39,    46,    83,    83,    83,    83,    83,   -39,   -39,
     -39,   -39,   -39,   -39,    83,    83,    83,    38,   -39,   -39,
     -39,    97,    54,   110,   -13,   -13,    83,    83,   -39,    37,
     -39,    -7,    -7,   -39,   -39,   -39,    86,    86,   157,    83,
      -3,    41,    86,   164,   -39,   123,   -39,   -13,   -39,   -39,
      83,    50,   150,   -39,   -39
  };

  const unsigned char
  parser_t::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,    18,     0,    48,    49,
       0,     2,     5,     9,    10,    19,     8,    17,     3,    22,
      21,    26,    25,    37,    46,    36,     0,     0,     0,    26,
      25,    37,     0,    39,     0,    27,     0,     0,    14,     1,
       4,     6,     0,     0,     0,     0,     0,     0,    30,    31,
      33,    34,    32,    35,     0,     0,     0,     0,    23,    28,
      38,     0,     0,     0,     0,     0,     0,     0,     7,     0,
      24,    40,    41,    42,    43,    44,    29,    20,     0,     0,
       0,     0,    15,     0,    47,     0,    11,     0,    13,    16,
       0,     0,     0,    12,    45
  };

  const signed char
  parser_t::yypgoto_[] =
  {
     -39,   -39,   -39,   -38,   -39,   -39,   -39,   -39,   -39,   -39,
       6,    68,    57,     2,   -39,    -1,     7,    63,   -39
  };

  const signed char
  parser_t::yydefgoto_[] =
  {
      -1,    10,    41,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    54,    22,    31,    24,    25
  };

  const unsigned char
  parser_t::yytable_[] =
  {
      30,    33,    68,    29,    45,    46,    47,    23,    34,     4,
      36,    37,     1,     5,     2,     6,     7,     8,     2,     3,
      86,    66,    87,     3,    67,     8,    80,    81,    39,    40,
      26,    61,    57,    63,     8,     9,    62,    42,     8,     9,
      55,    56,    71,    72,    73,    74,    75,    69,    70,    91,
       1,    64,    58,    76,    77,    78,     2,     1,    28,    59,
      35,     3,    79,     2,    88,    82,    83,    65,     3,    27,
      38,    69,    69,    93,    32,     4,     8,     9,    85,     5,
       2,     6,     7,     8,     9,    34,     0,    32,     0,    92,
       0,     2,    26,     2,    69,    43,    44,    45,    46,    47,
       8,     9,    60,    26,     0,     0,    43,    44,    45,    46,
      47,     8,     9,     8,     9,    60,     0,     0,     0,    43,
      44,    45,    46,    47,     0,     0,    48,    49,    50,    51,
      52,    53,    43,    44,    45,    46,    47,     0,     0,     0,
       0,     0,    43,    44,    45,    46,    47,     0,    90,    48,
      49,    50,    51,    52,    53,    94,     0,     0,     0,    43,
      44,    45,    46,    47,    84,     0,    43,    44,    45,    46,
      47,    89,     0,    43,    44,    45,    46,    47
  };

  const signed char
  parser_t::yycheck_[] =
  {
       1,     2,    40,     1,    11,    12,    13,     0,     4,    22,
       4,     5,     4,    26,    10,    28,    29,    30,    10,    15,
      23,     3,    25,    15,     6,    30,    64,    65,     0,     8,
      22,    32,    26,    34,    30,    31,    34,    14,    30,    31,
       3,     6,    43,    44,    45,    46,    47,    40,    42,    87,
       4,    24,     5,    54,    55,    56,    10,     4,     1,     5,
       3,    15,    24,    10,    23,    66,    67,    27,    15,     1,
       7,    64,    65,    23,     4,    22,    30,    31,    79,    26,
      10,    28,    29,    30,    31,     4,    -1,     4,    -1,    90,
      -1,    10,    22,    10,    87,     9,    10,    11,    12,    13,
      30,    31,     5,    22,    -1,    -1,     9,    10,    11,    12,
      13,    30,    31,    30,    31,     5,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    -1,    -1,    16,    17,    18,    19,
      20,    21,     9,    10,    11,    12,    13,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    -1,    25,    16,
      17,    18,    19,    20,    21,     5,    -1,    -1,    -1,     9,
      10,    11,    12,    13,     7,    -1,     9,    10,    11,    12,
      13,     7,    -1,     9,    10,    11,    12,    13
  };

  const unsigned char
  parser_t::yystos_[] =
  {
       0,     4,    10,    15,    22,    26,    28,    29,    30,    31,
      34,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    48,    49,    50,    51,    22,    44,    45,    46,
      48,    49,     4,    48,     4,    45,    43,    43,    50,     0,
       8,    35,    14,     9,    10,    11,    12,    13,    16,    17,
      18,    19,    20,    21,    47,     3,     6,    43,     5,     5,
       5,    48,    46,    48,    24,    27,     3,     6,    36,    49,
      43,    48,    48,    48,    48,    48,    48,    48,    48,    24,
      36,    36,    48,    48,     7,    48,    23,    25,    23,     7,
      25,    36,    48,    23,     5
  };

  const unsigned char
  parser_t::yyr1_[] =
  {
       0,    33,    34,    34,    35,    35,    36,    36,    37,    37,
      37,    38,    38,    39,    40,    40,    40,    41,    41,    41,
      42,    43,    43,    44,    44,    45,    45,    45,    46,    46,
      47,    47,    47,    47,    47,    47,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    49,    49,    50,    51
  };

  const unsigned char
  parser_t::yyr2_[] =
  {
       0,     2,     1,     1,     1,     0,     2,     3,     1,     1,
       1,     5,     7,     5,     2,     4,     5,     1,     1,     1,
       3,     1,     1,     3,     3,     1,     1,     2,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     2,
       3,     3,     3,     3,     3,     8,     1,     4,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser_t::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"=\"", "\"(\"", "\")\"",
  "\"[\"", "\"]\"", "\";\"", "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"%\"",
  "\"&&\"", "\"!\"", "\"==\"", "\"!=\"", "\"<=\"", "\">=\"", "\"<\"",
  "\">\"", "\"if\"", "\"end\"", "\"then\"", "\"else\"", "\"while\"",
  "\"do\"", "\"nop\"", "\"local\"", "\"identifier\"", "\"integer\"",
  "UMINUS", "$accept", "program", "opt_semicolon", "sequence_statement",
  "statement", "if_statement", "loop_statement", "local_statement",
  "simple_statement", "assignment", "conjunctive_formula",
  "non_atomic_conjunctive_formula", "atomic_formula", "predicate_formula",
  "predicate_operator", "term", "lvalue_term", "variable_term", "integer", YY_NULLPTR
  };

#if PPYYDEBUG
  const unsigned short int
  parser_t::yyrline_[] =
  {
       0,   211,   211,   220,   233,   234,   238,   240,   253,   255,
     257,   262,   264,   269,   274,   276,   278,   283,   285,   287,
     293,   307,   309,   315,   325,   339,   341,   343,   357,   367,
     381,   382,   383,   384,   385,   386,   391,   401,   403,   413,
     423,   433,   443,   453,   463,   473,   487,   489,   503,   517
  };

  // Print the state stack on the debug stream.
  void
  parser_t::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser_t::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // PPYYDEBUG


#line 13 "program.yy" // lalr1.cc:1167
} } } // tchecker::parsing::program
#line 1745 "/home/karthik/IITB/MTP/tchecker/src/parsing/program_parser/program.tab.cc" // lalr1.cc:1167
#line 534 "program.yy" // lalr1.cc:1168



void tchecker::parsing::program::parser_t::error(location_type const & l, std::string const & msg)
{
  if (program_context.empty()) {
    log.error(l, msg);
  }
  else {
    std::stringstream strl;
    strl << l;
    log.error(program_context, msg + " (at " + strl.str() + ")");
  }
  ++ error_count;
}
