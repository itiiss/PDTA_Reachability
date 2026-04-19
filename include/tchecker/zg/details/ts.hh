/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_ZG_DETAILS_TS_HH
#define TCHECKER_ZG_DETAILS_TS_HH

#include <typeinfo>

#include "tchecker/ts/ts.hh"
#include "tchecker/utils/iterator.hh"

/*!
 \file ts.hh
 \brief Transition system interface to zone graphs
 */

namespace tchecker {
  
  namespace zg {
    
    namespace details {
      
      /*!
       \class ts_t
       \brief Transition system for zone graphs
       \tparam STATE : type of states, should inherit from tchecker::zg::details::state_t
       \tparam TRANSITION : type of transitions, should inherit from tchecker::zg::details::transition_t
       \tparam ZG : type of zone graph, should instantiate tchecker::zg::details::zg_t
       \note STATE::zone_t and ZG::zone_t should coincide but not necessarily be the same. For instance,
       STATE::zone_t can be tchecker::make_shared_t<Z> for some zone implementation Z whereas ZG::zone_t
       is Z itself as STATE::zone_t needs to take into account memory management contingencies.
       Nevertheless, tchecker::make_shared_t<Z> and Z are compatible types for the operations done by
       ZG on the zones in STATE
       */
      template <class STATE, class TRANSITION, class ZG>
      class ts_t : public tchecker::ts::ts_t<
      STATE,
      TRANSITION,
      typename ZG::initial_iterator_t,
      typename ZG::outgoing_edges_iterator_t,
      typename ZG::initial_iterator_value_t,
      typename ZG::outgoing_edges_iterator_value_t>
      {
      public:
        /*!
         \brief Constructor
         \param model : a model to build ZG
         */
        template <class MODEL>
        explicit ts_t(MODEL & model)
        : _zg(model)
        {}
        
        /*!
         \brief Copy constructor
         \param ts : ZG transition system
         \post this is a copy of ts
         */
        ts_t(tchecker::zg::details::ts_t<STATE, TRANSITION, ZG> const & ts) = default;
        
        /*!
         \brief Move constructor
         \param ts : ZG transition system
         \post ts has been moved to this
         */
        ts_t(tchecker::zg::details::ts_t<STATE, TRANSITION, ZG> && ts) = default;
        
        /*!
         \brief Destructor
         */
        virtual ~ts_t() = default;
        
        /*!
         \brief Assignment operator
         \param ts : ZG transition system
         \post this is a copy of zg
         \return this after assignment
         */
        tchecker::zg::details::ts_t<STATE, TRANSITION, ZG> &
        operator= (tchecker::zg::details::ts_t<STATE, TRANSITION, ZG> const & ts) = default;
        
        /*!
         \brief Move-assignment operator
         \param ts : ZG transition system
         \post ts has been moved to this
         \return this after assignment
         */
        tchecker::zg::details::ts_t<STATE, TRANSITION, ZG> &
        operator= (tchecker::zg::details::ts_t<STATE, TRANSITION, ZG> && ts) = default;
        
        /*!
         \brief Accessor (see tchecker::ts::ts_t::initial)
         */
        virtual tchecker::range_t<typename ZG::initial_iterator_t> initial()
        {
          return _zg.initial();
        }
        
        /*!
         \brief Initialize state (see tchecker::ts::ts_t::initialize)
         */
        virtual enum tchecker::state_status_t initialize(STATE & s,
                                                         TRANSITION & t,
                                                         typename ZG::initial_iterator_value_t const & v)
        {
          return _zg.initialize(*s.vloc_ptr(), *s.intvars_valuation_ptr(), *s.zone_ptr(), v,
                                t.src_invariant_container());
        }
        
        /*!
         \brief Accessor (see tchecker::ts::ts_t::outgoing_edges)
         */
        virtual tchecker::range_t<typename ZG::outgoing_edges_iterator_t> outgoing_edges(STATE const & s)
        {
          return _zg.outgoing_edges(s.vloc());
        }
        
        /*!
         \brief Next state computation (see tchecker::ts::ts_t::next)
         */
        virtual enum tchecker::state_status_t next(STATE & s,
                                                   TRANSITION & t,
                                                   typename ZG::outgoing_edges_iterator_value_t const & v)
        {
          return _zg.next(*s.vloc_ptr(), *s.intvars_valuation_ptr(), *s.zone_ptr(), v,
                          t.src_invariant_container(), t.guard_container(), t.reset_container(),
                          t.tgt_invariant_container());
        }
        
        /*!
          \brief Determine if a node is equal to/subsumes another or not.
          \param s1: (q,Z)
          \param s2: (q',Z')
          \param subsumption: If we need to check for subsumption set this to true, if we need
          to check for equivalence set this value to false.
          \return For equivalence return false if Z~Z', else return true. For subsumption return
          false if Z'<=Z, else return true.
          Go To zg/details/zg.hh for more details.
        */
        virtual bool equal(STATE & s1, STATE & s2, bool subsumption){
          return _zg.equal(*s1.zone_ptr(), *s2.zone_ptr(),
                           *s1.vloc_ptr(), *s2.vloc_ptr(),
                           *s1.intvars_valuation_ptr(), *s2.intvars_valuation_ptr(),
                           subsumption);
        }

        /*!
          \brief Determine if two nodes are exactly equal or not.
          \param s1: (q,Z)
          \param s2: (q',Z')
          \return If DBMs of both Z and Z' are exactly the same return true, else return false.
          Go To zg/details/zg.hh for more details.
        */
        virtual bool exact_equal(STATE & s1, STATE &s2){
          return _zg.exact_equal(*s1.zone_ptr(), *s2.zone_ptr());
        }

        /*!
          \brief Get name of state of node.
          \param state: (q,Z)
          \return q from (q,Z)
          Go To zg/details/zg.hh for more details.
        */
        virtual std::string node_name(STATE &state){
          return _zg.node_name(*state.vloc_ptr());
        }

        /*!
          \brief Stable textual dump of the abstraction attached to a state
         */
        virtual std::string abstraction_string(STATE & state)
        {
          return _zg.abstraction_string(*state.vloc_ptr(), *state.intvars_valuation_ptr());
        }

        /*!
          \brief Canonical abstraction key attached to a state
         */
        virtual std::string abstraction_key(STATE & state)
        {
          return _zg.abstraction_key(*state.vloc_ptr(), *state.intvars_valuation_ptr());
        }

        /*!
          \brief Check if two states belong to the same abstraction class
         */
        virtual bool same_abstraction_class(STATE & s1, STATE & s2)
        {
          return _zg.same_abstraction_class(*s1.vloc_ptr(), *s1.intvars_valuation_ptr(),
                                            *s2.vloc_ptr(), *s2.intvars_valuation_ptr());
        }

        /*!
          \brief Type of abstraction class identifiers
         */
        using class_id_t = typename ZG::class_id_t;

        /*!
          \brief Type of abstraction classes
         */
        using abstraction_class_t = typename ZG::abstraction_class_t;

        /*!
          \brief Type of abstraction registries
         */
        using abstraction_registry_t = typename ZG::abstraction_registry_t;

        /*!
          \brief Type of abstraction enumerators
         */
        using abstraction_enumerator_t = typename ZG::abstraction_enumerator_t;

        /*!
          \brief Type of proof-support reports
         */
        using proof_report_t = typename ZG::proof_report_t;

        /*!
          \brief Type of abstraction observers
         */
        using abstraction_observer_t = typename ZG::abstraction_observer_t;

        /*!
          \brief Register or retrieve the abstraction class identifier of a state
         */
        virtual class_id_t class_id_of(STATE & state, abstraction_registry_t & registry)
        {
          return _zg.class_id_of(*state.vloc_ptr(), *state.intvars_valuation_ptr(), registry);
        }

        /*!
          \brief Add a state to an abstraction enumerator
         */
        virtual class_id_t enumerate_state(STATE & state, abstraction_enumerator_t & enumerator)
        {
          return _zg.enumerate(*state.vloc_ptr(), *state.intvars_valuation_ptr(), enumerator);
        }

        /*!
          \brief Observe a state through the lightweight observation layer
         */
        virtual class_id_t observe_state(STATE & state, abstraction_observer_t & observer)
        {
          return _zg.observe(*state.vloc_ptr(), *state.intvars_valuation_ptr(), observer);
        }

        /*!
          \brief Build a proof-support report from an enumerator
         */
        virtual proof_report_t proof_report(abstraction_enumerator_t const & enumerator) const
        {
          return _zg.proof_report(enumerator);
        }

        /*!
          \brief Build a proof-support report from an observer
         */
        virtual proof_report_t proof_report(abstraction_observer_t const & observer) const
        {
          return _zg.proof_report(observer);
        }

        /*!
          \brief Stable textual dump of a proof-support report built from an enumerator
         */
        virtual std::string proof_report_string(abstraction_enumerator_t const & enumerator) const
        {
          return _zg.proof_report_string(enumerator);
        }

        /*!
          \brief Stable textual dump of a proof-support report built from an observer
         */
        virtual std::string proof_report_string(abstraction_observer_t const & observer) const
        {
          return _zg.proof_report_string(observer);
        }

        /*!
          \brief Return stack operation of edge
          \param v: outgoing edges iterator value from transition system.
          \return a triplet in the form of pair of pair. The first bool, i.e. first of outer pair,
          describes whether the stack operation is NOP or (Push/Pop). The bool inside inner pair,
          describes whether the stack operation if (Push/Pop) is either Push or Pop, and the string
          which is in the inner pair is the string to be either pushed or popped in case operation is
          (Push/Pop).
          Go To zg/details/zg.hh for more details.
        */
        virtual std::pair<bool, std::pair<bool, std::string>> edge_name(typename ZG::outgoing_edges_iterator_value_t const & v){
          return _zg.edge_name(v);
        }

        /*!
         \brief Accessor
         \return Underlying zone graph
         */
        inline constexpr ZG const & zg() const
        {
          return _zg;
        }
        
        /*!
         \brief Type of model
         */
        using model_t = typename ZG::model_t;
        
        /*!
         \brief Accessor
         \return Underlying model
         */
        inline constexpr model_t const & model() const
        {
          return _zg.model();
        }
      protected:
        ZG _zg;   /*!< Zone graph */
      };
      
    } // end of namespace details
    
  } // end of namespace zg
  
} // end of namespace tchecker

#endif // TCHECKER_ZG_DETAILS_TS_HH
