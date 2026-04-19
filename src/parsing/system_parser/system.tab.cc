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
#define yylex   spyylex

// First part of user declarations.

#line 39 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "system.tab.hh"

// User implementation prologue.

#line 53 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 45 "system.yy" // lalr1.cc:413

  // Declare the lexer for the parser's sake.
  tchecker::parsing::system::parser_t::symbol_type spyylex
  (std::string const & filename,
  tchecker::log_t & log,
  tchecker::parsing::system_declaration_t * & system_declaration);
  
  // Error detection
  static unsigned int error_count;

#line 66 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:413


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
#if SPYYDEBUG

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

#else // !SPYYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !SPYYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 13 "system.yy" // lalr1.cc:479
namespace tchecker { namespace parsing { namespace system {
#line 152 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:479

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
  parser_t::parser_t (std::string const & filename_yyarg, tchecker::log_t & log_yyarg, tchecker::parsing::system_declaration_t * & system_declaration_yyarg)
    :
#if SPYYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      filename (filename_yyarg),
      log (log_yyarg),
      system_declaration (system_declaration_yyarg)
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
      case 41: // sync_strength
        value.move< enum tchecker::sync_strength_t > (that.value);
        break;

      case 20: // "identifier"
      case 22: // "integer value"
      case 27: // "text value"
      case 38: // text_or_empty
        value.move< std::string > (that.value);
        break;

      case 39: // sync_constraint_list
        value.move< std::vector<tchecker::parsing::sync_constraint_t const *> > (that.value);
        break;

      case 43: // integer
        value.move< tchecker::integer_t > (that.value);
        break;

      case 37: // attr
        value.move< tchecker::parsing::attr_t * > (that.value);
        break;

      case 34: // attr_list
      case 35: // non_empty_attr_list
        value.move< tchecker::parsing::attributes_t > (that.value);
        break;

      case 36: // sop
        value.move< tchecker::parsing::stack_op * > (that.value);
        break;

      case 40: // sync_constraint
        value.move< tchecker::parsing::sync_constraint_t * > (that.value);
        break;

      case 44: // uinteger
        value.move< unsigned int > (that.value);
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
      case 41: // sync_strength
        value.copy< enum tchecker::sync_strength_t > (that.value);
        break;

      case 20: // "identifier"
      case 22: // "integer value"
      case 27: // "text value"
      case 38: // text_or_empty
        value.copy< std::string > (that.value);
        break;

      case 39: // sync_constraint_list
        value.copy< std::vector<tchecker::parsing::sync_constraint_t const *> > (that.value);
        break;

      case 43: // integer
        value.copy< tchecker::integer_t > (that.value);
        break;

      case 37: // attr
        value.copy< tchecker::parsing::attr_t * > (that.value);
        break;

      case 34: // attr_list
      case 35: // non_empty_attr_list
        value.copy< tchecker::parsing::attributes_t > (that.value);
        break;

      case 36: // sop
        value.copy< tchecker::parsing::stack_op * > (that.value);
        break;

      case 40: // sync_constraint
        value.copy< tchecker::parsing::sync_constraint_t * > (that.value);
        break;

      case 44: // uinteger
        value.copy< unsigned int > (that.value);
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

#if SPYYDEBUG
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
            case 20: // "identifier"

#line 104 "system.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 401 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 22: // "integer value"

#line 104 "system.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 408 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 27: // "text value"

#line 104 "system.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 415 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 34: // attr_list

#line 104 "system.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< tchecker::parsing::attributes_t > (); }
#line 422 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 35: // non_empty_attr_list

#line 104 "system.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< tchecker::parsing::attributes_t > (); }
#line 429 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 36: // sop

#line 104 "system.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< tchecker::parsing::stack_op * > (); }
#line 436 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 37: // attr

#line 105 "system.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::parsing::attr_t * > (); }
#line 443 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 38: // text_or_empty

#line 104 "system.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 450 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 39: // sync_constraint_list

#line 107 "system.yy" // lalr1.cc:636
        {
  for (auto it = yysym.value.template as< std::vector<tchecker::parsing::sync_constraint_t const *> > ().begin(); it != yysym.value.template as< std::vector<tchecker::parsing::sync_constraint_t const *> > ().end(); ++it) {
    if (it != yysym.value.template as< std::vector<tchecker::parsing::sync_constraint_t const *> > ().begin())
    yyoutput << ",";
    yyoutput << **it;
  }
}
#line 463 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 40: // sync_constraint

#line 105 "system.yy" // lalr1.cc:636
        { yyoutput << * yysym.value.template as< tchecker::parsing::sync_constraint_t * > (); }
#line 470 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 41: // sync_strength

#line 104 "system.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< enum tchecker::sync_strength_t > (); }
#line 477 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 43: // integer

#line 104 "system.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< tchecker::integer_t > (); }
#line 484 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
        break;

      case 44: // uinteger

#line 104 "system.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< unsigned int > (); }
#line 491 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:636
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

#if SPYYDEBUG
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
#endif // SPYYDEBUG

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
    #line 56 "system.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &const_cast<std::string &>(filename);
  
  error_count = 0;
}

#line 612 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:741

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
            symbol_type yylookahead (yylex (filename, log, system_declaration));
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
      case 41: // sync_strength
        yylhs.value.build< enum tchecker::sync_strength_t > ();
        break;

      case 20: // "identifier"
      case 22: // "integer value"
      case 27: // "text value"
      case 38: // text_or_empty
        yylhs.value.build< std::string > ();
        break;

      case 39: // sync_constraint_list
        yylhs.value.build< std::vector<tchecker::parsing::sync_constraint_t const *> > ();
        break;

      case 43: // integer
        yylhs.value.build< tchecker::integer_t > ();
        break;

      case 37: // attr
        yylhs.value.build< tchecker::parsing::attr_t * > ();
        break;

      case 34: // attr_list
      case 35: // non_empty_attr_list
        yylhs.value.build< tchecker::parsing::attributes_t > ();
        break;

      case 36: // sop
        yylhs.value.build< tchecker::parsing::stack_op * > ();
        break;

      case 40: // sync_constraint
        yylhs.value.build< tchecker::parsing::sync_constraint_t * > ();
        break;

      case 44: // uinteger
        yylhs.value.build< unsigned int > ();
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
#line 120 "system.yy" // lalr1.cc:859
    {
  system_declaration = new system_declaration_t(yystack_[0].value.as< std::string > ());
}
#line 764 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 124 "system.yy" // lalr1.cc:859
    {
  if (error_count > 0) {
    delete system_declaration;
    system_declaration = nullptr;
  }
}
#line 775 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 131 "system.yy" // lalr1.cc:859
    {
  delete system_declaration;
  system_declaration = nullptr;
}
#line 784 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 140 "system.yy" // lalr1.cc:859
    {}
#line 790 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 142 "system.yy" // lalr1.cc:859
    {}
#line 796 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 148 "system.yy" // lalr1.cc:859
    {}
#line 802 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 150 "system.yy" // lalr1.cc:859
    {}
#line 808 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 156 "system.yy" // lalr1.cc:859
    {
  auto const * d = system_declaration->get_clock_declaration(yystack_[1].value.as< std::string > ());
  if (d != nullptr)
  error(yystack_[1].location, "multiple declarations of clock " + yystack_[1].value.as< std::string > ());
  else {
    auto const * intd = system_declaration->get_int_declaration(yystack_[1].value.as< std::string > ());
    if (intd != nullptr)
    error(yystack_[1].location, "variable " + yystack_[1].value.as< std::string > () + " already declared as an int");
    else {
      try {
        d = new tchecker::parsing::clock_declaration_t(yystack_[1].value.as< std::string > (), yystack_[3].value.as< unsigned int > ());
        if ( ! system_declaration->insert_clock_declaration(d) ) {
          error(yylhs.location, "insertion of clock declaration failed");
          delete d;
        }
      }
      catch (std::exception const & e) {
        error(yylhs.location, e.what());
      }
    }
  }
}
#line 835 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 180 "system.yy" // lalr1.cc:859
    {
  auto const * proc = system_declaration->get_process_declaration(yystack_[9].value.as< std::string > ());
  if (proc == nullptr)
  error(yystack_[9].location, "process " + yystack_[9].value.as< std::string > () + " is not declared");
  else {
    auto const * src = system_declaration->get_location_declaration(yystack_[9].value.as< std::string > (), yystack_[7].value.as< std::string > ());
    if (src == nullptr)
      error(yystack_[7].location, "location " + yystack_[7].value.as< std::string > () + " is not declared in process " +yystack_[9].value.as< std::string > ());
    else {
      auto const * tgt = system_declaration->get_location_declaration(yystack_[9].value.as< std::string > (), yystack_[5].value.as< std::string > ());
      if (tgt == nullptr)
        error(yystack_[5].location, "location " + yystack_[5].value.as< std::string > () + " is not declared in process " + yystack_[9].value.as< std::string > ());
      else {
        auto const * event = system_declaration->get_event_declaration(yystack_[3].value.as< std::string > ());
        if (event == nullptr)
          error(yystack_[3].location, "event " + yystack_[3].value.as< std::string > () + " is not declared");
        else {
          try {
          	tchecker::parsing::stack_op *sp = yystack_[1].value.as< tchecker::parsing::stack_op * > ();
            auto * d = new tchecker::parsing::edge_declaration_t(*proc, *src, *tgt, *event, std::move(yystack_[2].value.as< tchecker::parsing::attributes_t > ()), *sp);
            if ( ! system_declaration->insert_edge_declaration(d) ) {
              error(yylhs.location, "insertion of edge declaration failed");
              delete d;
            }
          }
          catch (std::exception const & e) {
            error(yylhs.location, e.what());
          }
        }
      }
    }
  }
}
#line 873 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 215 "system.yy" // lalr1.cc:859
    {
  auto const * d = system_declaration->get_event_declaration(yystack_[1].value.as< std::string > ());
  if (d != nullptr)
    error(yystack_[1].location, "multiple declarations of event " + yystack_[1].value.as< std::string > ());
  else {
    try {
      d = new tchecker::parsing::event_declaration_t(yystack_[1].value.as< std::string > ());
      if ( ! system_declaration->insert_event_declaration(d) ) {
        error(yylhs.location, "insertion of event declaration failed");
        delete d;
      }
    }
    catch (std::exception const & e) {
      error(yylhs.location, e.what());
    }
  }
}
#line 895 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 234 "system.yy" // lalr1.cc:859
    {
  auto const * d = system_declaration->get_int_declaration(yystack_[1].value.as< std::string > ());
  if (d != nullptr)
    error(yystack_[1].location, "multiple declarations of int variable " + yystack_[1].value.as< std::string > ());
  else {
    auto const * clockd = system_declaration->get_clock_declaration(yystack_[1].value.as< std::string > ());
    if (clockd != nullptr)
      error(yystack_[1].location, "variable " + yystack_[1].value.as< std::string > () + " already declared as a clock");
    else {
      try {
        d = new tchecker::parsing::int_declaration_t(yystack_[1].value.as< std::string > (), yystack_[9].value.as< unsigned int > (), yystack_[7].value.as< tchecker::integer_t > (), yystack_[5].value.as< tchecker::integer_t > (), yystack_[3].value.as< tchecker::integer_t > ());
        if ( ! system_declaration->insert_int_declaration(d) ) {
          error(yylhs.location, "insertion of int declaration failed");
          delete d;
        }
      }
      catch (std::exception const & e) {
        error(yylhs.location, e.what());
      }
    }
  }
}
#line 922 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 258 "system.yy" // lalr1.cc:859
    {
  auto const * d = system_declaration->get_location_declaration(yystack_[4].value.as< std::string > (), yystack_[2].value.as< std::string > ());
  if (d != nullptr)
    error(yystack_[2].location, "multiple declarations of location " + yystack_[2].value.as< std::string > () + " in process " + yystack_[4].value.as< std::string > ());
  else {
    auto const * proc = system_declaration->get_process_declaration(yystack_[4].value.as< std::string > ());
    if (proc == nullptr)
      error(yystack_[4].location, "process " + yystack_[4].value.as< std::string > () + " is not declared");
    else {
      try {
        d = new location_declaration_t(yystack_[2].value.as< std::string > (), *proc, std::move(yystack_[1].value.as< tchecker::parsing::attributes_t > ()));
        if ( ! system_declaration->insert_location_declaration(d) ) {
          error(yylhs.location, "insertion of location declaration failed");
          delete d;
        }
      }
      catch (std::exception const & e) {
        error(yylhs.location, e.what());
      }
    }
  }
}
#line 949 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 282 "system.yy" // lalr1.cc:859
    {
  auto const * d = system_declaration->get_process_declaration(yystack_[1].value.as< std::string > ());
  if (d != nullptr)
    error(yystack_[1].location, "multiple declarations of process " + yystack_[1].value.as< std::string > ());
  else {
    try {
      d = new tchecker::parsing::process_declaration_t(yystack_[1].value.as< std::string > ());
      if ( ! system_declaration->insert_process_declaration(d) ) {
        error(yylhs.location, "insertion of process declaration failed");
        delete d;
      }
    }
    catch (std::exception const & e) {
      error(yylhs.location, e.what());
    }
  }
}
#line 971 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 301 "system.yy" // lalr1.cc:859
    {
  try {
    auto const * d = new tchecker::parsing::sync_declaration_t(std::move(yystack_[1].value.as< std::vector<tchecker::parsing::sync_constraint_t const *> > ()));
    if ( ! system_declaration->insert_sync_declaration(d) ) {
      error(yylhs.location, "insertion of sync declaration failed");
      delete d;
    }
  }
  catch (std::exception const & e) {
    error(yylhs.location, e.what());
  }
}
#line 988 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 320 "system.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::parsing::attributes_t > () = std::move(yystack_[1].value.as< tchecker::parsing::attributes_t > ()); }
#line 994 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 322 "system.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::parsing::attributes_t > ().clear(); }
#line 1000 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 324 "system.yy" // lalr1.cc:859
    { yylhs.value.as< tchecker::parsing::attributes_t > ().clear(); }
#line 1006 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 330 "system.yy" // lalr1.cc:859
    {
  if (yystack_[0].value.as< tchecker::parsing::attr_t * > () != nullptr) {
    if ( ! yylhs.value.as< tchecker::parsing::attributes_t > ().insert(yystack_[0].value.as< tchecker::parsing::attr_t * > ()) ) {
      error(yystack_[0].location, "multiple instances of attribute " + yystack_[0].value.as< tchecker::parsing::attr_t * > ()->key());
      delete yystack_[0].value.as< tchecker::parsing::attr_t * > ();
    }
  }
}
#line 1019 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 339 "system.yy" // lalr1.cc:859
    {
  if (yystack_[0].value.as< tchecker::parsing::attr_t * > () != nullptr) {
    if ( ! yystack_[2].value.as< tchecker::parsing::attributes_t > ().insert(yystack_[0].value.as< tchecker::parsing::attr_t * > ()) ) {
      error(yystack_[0].location, "multiple instances of attribute " + yystack_[0].value.as< tchecker::parsing::attr_t * > ()->key());
      delete yystack_[0].value.as< tchecker::parsing::attr_t * > ();
    }
  }
  yylhs.value.as< tchecker::parsing::attributes_t > () = std::move(yystack_[2].value.as< tchecker::parsing::attributes_t > ());
}
#line 1033 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 351 "system.yy" // lalr1.cc:859
    {
  if(yystack_[1].value.as< std::string > () == "")  error(yystack_[1].location,"empty stack symbol");
  else {
    std::stringstream value_loc;
    value_loc << yystack_[1].location;
    yylhs.value.as< tchecker::parsing::stack_op * > () = new tchecker::parsing::stack_op("push", yystack_[1].value.as< std::string > (), "push", value_loc.str());
  }
}
#line 1046 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 359 "system.yy" // lalr1.cc:859
    {
  if(yystack_[3].value.as< std::string > () == "")  error(yystack_[3].location,"empty stack symbol");
  else {
    std::stringstream value_loc, first, second;
    std::string first1 = "<=";
    second << yystack_[1].location;
    first << first1;
    value_loc << yystack_[3].location;
    value_loc << first.str();
    value_loc << second.str();
    yylhs.value.as< tchecker::parsing::stack_op * > () = new tchecker::parsing::stack_op("pop", yystack_[3].value.as< std::string > () + "<=" + yystack_[1].value.as< std::string > (), "pop", value_loc.str());
  }
}
#line 1064 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 372 "system.yy" // lalr1.cc:859
    {
  if(yystack_[3].value.as< std::string > () == "")  error(yystack_[3].location,"empty stack symbol");
  else {
    std::stringstream value_loc, first, second;
    std::string first1 = ">=";
    second << yystack_[1].location;
    first << first1;
    value_loc << yystack_[3].location;
    value_loc << first.str();
    value_loc << second.str();
    yylhs.value.as< tchecker::parsing::stack_op * > () = new tchecker::parsing::stack_op("pop", yystack_[3].value.as< std::string > () + ">=" + yystack_[1].value.as< std::string > (), "pop", value_loc.str());
  }
}
#line 1082 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 385 "system.yy" // lalr1.cc:859
    {
  if(yystack_[3].value.as< std::string > () == "")  error(yystack_[3].location,"empty stack symbol");
  else {
    std::stringstream value_loc, first, second;
    std::string first1 = "<";
    second << yystack_[1].location;
    first << first1;
    value_loc << yystack_[3].location;
    value_loc << first.str();
    value_loc << second.str();
    yylhs.value.as< tchecker::parsing::stack_op * > () = new tchecker::parsing::stack_op("pop", yystack_[3].value.as< std::string > () + "<" + yystack_[1].value.as< std::string > (), "pop", value_loc.str());
  }
}
#line 1100 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 398 "system.yy" // lalr1.cc:859
    {
  if(yystack_[3].value.as< std::string > () == "")  error(yystack_[3].location,"empty stack symbol");
  else {
    std::stringstream value_loc, first, second;
    std::string first1 = ">";
    second << yystack_[1].location;
    first << first1;
    value_loc << yystack_[3].location;
    value_loc << first.str();
    value_loc << second.str();
    yylhs.value.as< tchecker::parsing::stack_op * > () = new tchecker::parsing::stack_op("pop", yystack_[3].value.as< std::string > () + ">" + yystack_[1].value.as< std::string > (), "pop", value_loc.str());
  }
}
#line 1118 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 411 "system.yy" // lalr1.cc:859
    {
  yylhs.value.as< tchecker::parsing::stack_op * > () = new tchecker::parsing::stack_op("nop", "", "nop", "");
}
#line 1126 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 418 "system.yy" // lalr1.cc:859
    {
  yylhs.value.as< tchecker::parsing::attr_t * > () = nullptr;
  if (yystack_[2].value.as< std::string > () == "")
    error(yystack_[2].location, "empty attribute key");
  else {
    std::stringstream key_loc, value_loc;
    key_loc << yystack_[2].location;
    value_loc << yystack_[0].location;
    boost::trim(yystack_[0].value.as< std::string > ());
    yylhs.value.as< tchecker::parsing::attr_t * > () = new tchecker::parsing::attr_t(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > (), key_loc.str(), value_loc.str());
  }
}
#line 1143 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 435 "system.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () = std::move(yystack_[0].value.as< std::string > ()); }
#line 1149 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 437 "system.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > ().clear(); }
#line 1155 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 443 "system.yy" // lalr1.cc:859
    {
  if (yystack_[0].value.as< tchecker::parsing::sync_constraint_t * > () != nullptr)
  yylhs.value.as< std::vector<tchecker::parsing::sync_constraint_t const *> > ().push_back(yystack_[0].value.as< tchecker::parsing::sync_constraint_t * > ());
}
#line 1164 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 448 "system.yy" // lalr1.cc:859
    {
  if (yystack_[0].value.as< tchecker::parsing::sync_constraint_t * > () != nullptr)
  yystack_[2].value.as< std::vector<tchecker::parsing::sync_constraint_t const *> > ().push_back(yystack_[0].value.as< tchecker::parsing::sync_constraint_t * > ());
  yylhs.value.as< std::vector<tchecker::parsing::sync_constraint_t const *> > () = std::move(yystack_[2].value.as< std::vector<tchecker::parsing::sync_constraint_t const *> > ());
}
#line 1174 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 458 "system.yy" // lalr1.cc:859
    {
  yylhs.value.as< tchecker::parsing::sync_constraint_t * > () = nullptr;
  auto const * proc = system_declaration->get_process_declaration(yystack_[3].value.as< std::string > ());
  if (proc == nullptr)
  error(yystack_[3].location, "process " + yystack_[3].value.as< std::string > () + " is not declared");
  else {
    auto const * event = system_declaration->get_event_declaration(yystack_[1].value.as< std::string > ());
    if (event == nullptr)
    error(yystack_[1].location, "event " + yystack_[1].value.as< std::string > () + " is not declared");
    else {
      try {
        yylhs.value.as< tchecker::parsing::sync_constraint_t * > () = new tchecker::parsing::sync_constraint_t(*proc, *event, yystack_[0].value.as< enum tchecker::sync_strength_t > ());
      }
      catch (std::exception const & e) {
        error(yylhs.location, e.what());
      }
    }
  }
}
#line 1198 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 482 "system.yy" // lalr1.cc:859
    { yylhs.value.as< enum tchecker::sync_strength_t > () = tchecker::SYNC_WEAK; }
#line 1204 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 484 "system.yy" // lalr1.cc:859
    { yylhs.value.as< enum tchecker::sync_strength_t > () = tchecker::SYNC_STRONG; }
#line 1210 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 496 "system.yy" // lalr1.cc:859
    {
  try {
    long long l = std::stoll(yystack_[0].value.as< std::string > (), nullptr, 10);
    if ( (l < std::numeric_limits<tchecker::integer_t>::min()) || (l > std::numeric_limits<tchecker::integer_t>::max()) )
    throw std::out_of_range("integer constant our of range");
    yylhs.value.as< tchecker::integer_t > () = static_cast<tchecker::integer_t>(l);
  }
  catch (std::exception const & e) {
    error(yystack_[0].location, "value " + yystack_[0].value.as< std::string > () + " out of range " + std::to_string(std::numeric_limits<tchecker::integer_t>::min()) +
    "," + std::to_string(std::numeric_limits<tchecker::integer_t>::max()) );
    yylhs.value.as< tchecker::integer_t > () = 0;
  }
}
#line 1228 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 514 "system.yy" // lalr1.cc:859
    {
  try {
    unsigned long l = std::stoul(yystack_[0].value.as< std::string > (), nullptr, 10);
    if (l > std::numeric_limits<unsigned int>::max())
    throw std::out_of_range("unsigned integer out of range");
    yylhs.value.as< unsigned int > () = static_cast<unsigned int>(l);
  }
  catch (std::exception const & e) {
    error(yystack_[0].location, "value " + yystack_[0].value.as< std::string > () + " out of range " + std::to_string(std::numeric_limits<unsigned int>::min()) +
    "," + std::to_string(std::numeric_limits<unsigned int>::max()) );
    yylhs.value.as< unsigned int > () = 0;
  }
}
#line 1246 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
    break;


#line 1250 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:859
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


  const signed char parser_t::yypact_ninf_ = -56;

  const signed char parser_t::yytable_ninf_ = -38;

  const signed char
  parser_t::yypact_[] =
  {
      13,     6,     8,   -13,   -56,   -56,   -56,     7,     2,   -56,
      17,   -56,     0,    20,    44,    45,    46,    47,    48,    49,
      50,   -56,   -56,   -56,   -56,    34,    35,    37,    34,    38,
      39,    40,     9,   -56,    57,    58,    51,    59,    60,    52,
      61,    -2,   -56,   -56,    53,    54,   -56,    43,    55,   -56,
      56,    40,   -56,    62,    63,   -56,    64,    65,    71,   -56,
     -56,    66,    43,    21,    67,   -56,   -56,    74,    75,   -56,
      77,    32,   -56,   -56,    68,    43,    69,    70,   -56,    65,
      78,   -56,   -56,   -56,    80,    72,    27,    73,    76,   -56,
      79,    83,   -56,   -56,    81,    82,    85,    33,   -56,    84,
      86,    87,    88,    89,    90,    93,    94,   -56,   -56,   -56,
     -56
  };

  const unsigned char
  parser_t::yydefact_[] =
  {
       0,     0,     0,     0,     4,     1,    36,     0,     0,     2,
       0,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     3,    37,     7,    16,     0,     0,     0,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,    31,     8,     0,     0,    11,     0,     0,    14,
       0,     0,    15,     0,     0,    38,     0,    19,    35,    32,
       9,     0,     0,     0,     0,    34,    33,     0,     0,    18,
       0,     0,    20,    13,     0,     0,    30,     0,    17,    19,
       0,    29,    28,    21,     0,     0,     0,     0,     0,    27,
       0,     0,    10,    12,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,    25,    26,    23,
      24
  };

  const signed char
  parser_t::yypgoto_[] =
  {
     -56,   -56,   -56,   -56,   -56,    91,    -8,   -56,   -56,    16,
     -56,   -56,    92,   -56,    -7,   -55,    96
  };

  const signed char
  parser_t::yydefgoto_[] =
  {
      -1,     2,    10,    21,    22,    23,    64,    71,    87,    72,
      82,    41,    42,    66,     3,    56,    34
  };

  const signed char
  parser_t::yytable_[] =
  {
      -6,    13,    51,    14,    12,     6,     4,    68,     5,    -5,
      13,     8,    14,     7,     1,    32,    52,    15,     6,    16,
      80,    17,     9,    18,    19,    20,    15,     6,    16,    69,
      17,   -37,    18,    19,    20,    11,    77,    89,    24,   -37,
      78,    70,    90,    91,    99,   100,   101,   102,    25,    26,
      27,    28,    29,    30,    31,    35,    33,    36,    38,    39,
      40,    44,    45,    47,    48,    55,    50,    61,    62,    46,
      49,    84,    63,    53,    54,    57,    58,    65,    74,    75,
      60,    76,    85,    94,     0,    73,    67,    95,    79,    86,
      70,    92,    88,    83,    93,    98,    81,     0,     0,   107,
     108,    96,    97,   109,   110,     0,   103,     0,   104,   105,
     106,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59
  };

  const signed char
  parser_t::yycheck_[] =
  {
       0,     1,     4,     3,    11,    18,     0,    62,     0,     0,
       1,     4,     3,    26,     1,    22,    18,    17,    18,    19,
      75,    21,    20,    23,    24,    25,    17,    18,    19,     8,
      21,    18,    23,    24,    25,    18,     4,    10,    18,    26,
       8,    20,    15,    16,    11,    12,    13,    14,     4,     4,
       4,     4,     4,     4,     4,    20,    22,    20,    20,    20,
      20,     4,     4,     4,     4,    22,     5,     4,     4,    18,
      18,    79,     7,    20,    20,    20,    20,     6,     4,     4,
      18,     4,     4,     4,    -1,    18,    20,     4,    20,     9,
      20,    18,    20,    77,    18,    10,    27,    -1,    -1,    10,
      10,    20,    20,    10,    10,    -1,    22,    -1,    22,    22,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51
  };

  const unsigned char
  parser_t::yystos_[] =
  {
       0,     1,    29,    42,     0,     0,    18,    26,     4,    20,
      30,    18,    42,     1,     3,    17,    19,    21,    23,    24,
      25,    31,    32,    33,    18,     4,     4,     4,     4,     4,
       4,     4,    42,    22,    44,    20,    20,    44,    20,    20,
      20,    39,    40,    33,     4,     4,    18,     4,     4,    18,
       5,     4,    18,    20,    20,    22,    43,    20,    20,    40,
      18,     4,     4,     7,    34,     6,    41,    20,    43,     8,
      20,    35,    37,    18,     4,     4,     4,     4,     8,    20,
      43,    27,    38,    37,    34,     4,     9,    36,    20,    10,
      15,    16,    18,    18,     4,     4,    20,    20,    10,    11,
      12,    13,    14,    22,    22,    22,    22,    10,    10,    10,
      10
  };

  const unsigned char
  parser_t::yyr1_[] =
  {
       0,    28,    30,    29,    29,    31,    31,    32,    32,    33,
      33,    33,    33,    33,    33,    33,    33,    34,    34,    34,
      35,    35,    36,    36,    36,    36,    36,    36,    37,    38,
      38,    39,    39,    40,    41,    41,    42,    42,    43,    44
  };

  const unsigned char
  parser_t::yyr2_[] =
  {
       0,     2,     0,     8,     2,     2,     0,     1,     3,     6,
      12,     4,    12,     7,     4,     4,     2,     3,     2,     0,
       1,     3,     5,     7,     7,     7,     7,     2,     3,     1,
       0,     1,     3,     4,     1,     0,     2,     0,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser_t::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"clock\"", "\":\"", "\"@\"",
  "\"?\"", "\"{\"", "\"}\"", "\"[\"", "\"]\"", "\"<\"", "\">\"", "\"<=\"",
  "\">=\"", "\"push\"", "\"pop\"", "\"edge\"", "\"\\n\"", "\"event\"",
  "\"identifier\"", "\"int\"", "\"integer value\"", "\"location\"",
  "\"process\"", "\"sync\"", "\"system\"", "\"text value\"", "$accept",
  "system", "$@1", "declaration_list", "non_empty_declaration_list",
  "declaration", "attr_list", "non_empty_attr_list", "sop", "attr",
  "text_or_empty", "sync_constraint_list", "sync_constraint",
  "sync_strength", "eol_sequence", "integer", "uinteger", YY_NULLPTR
  };

#if SPYYDEBUG
  const unsigned short int
  parser_t::yyrline_[] =
  {
       0,   120,   120,   120,   130,   139,   142,   147,   149,   155,
     179,   214,   233,   257,   281,   300,   314,   319,   321,   324,
     329,   338,   351,   359,   372,   385,   398,   411,   417,   434,
     437,   442,   447,   457,   481,   484,   489,   490,   495,   513
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
#endif // SPYYDEBUG


#line 13 "system.yy" // lalr1.cc:1167
} } } // tchecker::parsing::system
#line 1692 "/home/karthik/IITB/MTP/tchecker/src/parsing/system_parser/system.tab.cc" // lalr1.cc:1167
#line 530 "system.yy" // lalr1.cc:1168


void tchecker::parsing::system::parser_t::error(location_type const & l, std::string const & msg)
{
  log.error(l, msg);
  ++ error_count;
}
