/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_FSM_DETAILS_EDGE_HH
#define TCHECKER_FSM_DETAILS_EDGE_HH

#include "tchecker/basictypes.hh"
#include "tchecker/expression/expression.hh"
#include "tchecker/statement/statement.hh"
#include "tchecker/system/edge.hh"

/*!
 \file edge.hh
 \brief Edges for finite state machines
 */

namespace tchecker {
  
  namespace fsm {
    
    namespace details {
      
      /*!
       \class edge_t
       \tparam LOC : type of location, should derive from tchecker::loc_t
       \brief An edge
       */
      template <class LOC>
      class edge_t : public tchecker::edge_t<LOC> {
      public:
        /*!
         \brief Constructor
         \param pid : process identifier
         \param id : identifier
         \param src : source location
         \param tgt : target location
         \param event : synchronization event
         \param guard : edge guard
         \param statement : edge statement
         \param stop: Stack Operation type (NOP or Push/Pop)
         \param push_or_pop: Stack Operation type (Push or Pop)
         \param symbol: Push/Pop on symbol
         \param constraint: Comparator if pop operation (<,<=,>=,>)
         \param fin: Value to be compared against in pop opn.
         \pre guard and statement are not nullptr, src and tgt belong to process
         pid
         \note this takes ownership on guard and statement
         \throw std::invalid_argument : if the precondition is violated
         */
        edge_t(tchecker::process_id_t pid,
               tchecker::edge_id_t id,
               LOC const * src,
               LOC const * tgt,
               tchecker::event_id_t event,
               tchecker::expression_t * guard,
               tchecker::statement_t * statement,
               bool stop,
               bool push_or_pop,
               std::string symbol,
               int constraint,
               int fin)
        : tchecker::edge_t<LOC>(pid, id, src, tgt, event),
        _guard(guard),
        _statement(statement),
        _stop(stop),
        _push_or_pop(push_or_pop),
        _symbol(symbol),
        _constraint(constraint),
        _fin(fin)
        {
          // std::cout << "SYMBOL\t" << symbol << "\n";
          if (_guard == nullptr)
            throw std::invalid_argument("nullptr guard");
          
          if (_statement == nullptr)
            throw std::invalid_argument("nullptr statement");
        }
        
        
        /*!
         \brief Copy constructor
         \param e : edge
         \post this is a copy of e
         */
        edge_t(tchecker::fsm::details::edge_t<LOC> const & e)
        : tchecker::edge_t<LOC>(e),
        _guard(e._guard->clone()),
        _statement(e._statement->clone())
        {}
        
        
        /*!
         \brief Move constructor
         \param e : edge
         \post e has been moved to this
         */
        edge_t(tchecker::fsm::details::edge_t<LOC> && e) = default;
        
        
        /*!
         \brief Destructor
         */
        ~edge_t()
        {
          delete _guard;
          delete _statement;
        }
        
        
        /*!
         \brief Assignement operator
         \param e : edge
         \post this is a copy of e
         */
        tchecker::fsm::details::edge_t<LOC> & operator= (tchecker::fsm::details::edge_t<LOC> const & e)
        {
          if (this != &e) {
            this->tchecker::edge_t<LOC>::operator=(e);
            delete _guard;
            _guard = e._guard->clone();
            _statement = e._statement->clone();
          }
          return *this;
        }
        
        
        /*!
         \brief Move assignment operator
         \param e : edge
         \post e has been moved to this
         */
        tchecker::fsm::details::edge_t<LOC> & operator= (tchecker::fsm::details::edge_t<LOC> && e) = default;
        
        
        /*!
         \brief Accessor
         \return Guard
         */
        inline tchecker::expression_t const & guard() const
        {
          return (* _guard);
        }
        
        
        /*!
         \brief Accessor
         \return Statement
         */
        inline tchecker::statement_t const & statement() const
        {
          return (* _statement);
        }
        bool _stop;                         /*!< NOP or Push/Pop */
        bool _push_or_pop;                  /*!< Push or Pop in case NOP */
        std::string _symbol;                /*< Symbol in case Push/Pop */
        int _constraint;                    /*!< Comparator for Pop */
        int _fin;                           /*!< Value in case Pop */
      private:
        tchecker::expression_t * _guard;     /*!< Guard */
        tchecker::statement_t * _statement;  /*!< Statement */
      };
      
    } // end of namespace details
    
  } // end of namespace fsm
  
} // end of namespace tchecker

#endif // TCHECKER_FSM_DETAILS_EDGE_HH

