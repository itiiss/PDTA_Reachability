/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_ZG_DETAILS_ZG_HH
#define TCHECKER_ZG_DETAILS_ZG_HH

#include <algorithm>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <sstream>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "tchecker/basictypes.hh"
#include "tchecker/clockbounds/clockbounds.hh"
#include "tchecker/dbm/db.hh"
#include "tchecker/utils/iterator.hh"
#include "tchecker/variables/clocks.hh"
#include "tchecker/variables/intvars.hh"
#include "tchecker/vm/vm.hh"

/*!
 \file zg.hh
 \brief Zone graph (details)
 */

namespace tchecker {
  
  namespace zg {
    
    namespace details {
      
      /*!
       \class zg_t
       \brief Zone graph (details)
       \tparam TA : type of timed automaton, should inherit from tchecker::ta::details::ta_t
       \tparam ZONE_SEMANTICS : type of zone semantics, should implement tchecker::zone_semantics_t
       */
      template <class TA, class ZONE_SEMANTICS>
      class zg_t {
      public:
        /*!
         \brief Type of model
         */
        using model_t = typename TA::model_t;
        
        /*!
         \brief Type of tuple of locations
         */
        using vloc_t = typename TA::vloc_t;
        
        /*!
         \brief Type of valuation of bounded integer variables
         */
        using intvars_valuation_t = typename TA::intvars_valuation_t;
        
        /*!
         \brief Type of zones
         */
        using zone_t = typename ZONE_SEMANTICS::zone_t;
        
        
        /*!
         \brief Constructor
         \tparam MODEL : type of model, should derive from tchecker::zg::details::model_t
         \param model : a model
         */
        template <class MODEL>
        explicit zg_t(MODEL & model)
        : _ta(model),
          _zone_semantics(model),
          _vm(model.flattened_integer_variables().size(), model.flattened_clock_variables().size()),
          _clock_nb(model.flattened_clock_variables().size())
        {}
        
        /*!
         \brief Copy constructor
         */
        zg_t(tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> const &) = default;
        
        /*!
         \brief Move constructor
         */
        zg_t(tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> &&) = default;
        
        /*!
         \brief Destructor
         */
        ~zg_t() = default;
        
        /*!
         \brief Assignment operator
         \param zg : a zone graph
         \post this is a copy of zg
         \return this after assignment
         */
        tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> &
        operator= (tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> const & zg) = default;
        
        /*!
         \brief Move-assignment operator
         \param zg : a zone graph
         \post zg has been moved to this
         \return this after assignment
         */
        tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> &
        operator= (tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> && zg) = default;
        
        /*!
         \brief Type of iterator over initial states
         */
        using initial_iterator_t = typename TA::initial_iterator_t;
        
        /*!
         \brief Accessor
         \return iterator over initial states
         */
        inline tchecker::range_t<initial_iterator_t> initial() const
        {
          return _ta.initial();
        }
        
        /*!
         \brief Dereference type for iterator over initial states
         */
        using initial_iterator_value_t = typename TA::initial_iterator_value_t;
        
        /*!
         \brief Initialize state
         \param vloc : tuple of locations
         \param intvars_val : valuation of integer variables
         \param zone : a zone
         \param initial_range : range of locations
         \param invariant : a tchecker::clock_constraint_t container
         \pre vloc, intvars_val, zone, and initial_range have dimensions corresponding to number of processes,
         number of integer variables, number of clocks and number of processes in the timed automaton
         \post vloc has been initialized to initial locations in initial_range. intvars_val has been initialized to
         the initial value of integer variables. zone has been initialized to the initial zone according to the chosen
         zone semantics. All constraints in the invariant of vloc have been added to invariant and taken into account for zone
         according to the chosen zone semantics
         \return STATE_OK if vloc, intvars_val and zone have been updated, see tchecker::ta::details::ta_t::initialize
         for other possible values when initializing the integer variables valuation fails, and see
         tchecker::zone_semantics_t::initialize for possible values when initializing the zone fails
         */
        enum tchecker::state_status_t initialize(vloc_t & vloc,
                                                 intvars_valuation_t & intvars_val,
                                                 zone_t & zone,
                                                 initial_iterator_value_t const & initial_range,
                                                 tchecker::clock_constraint_container_t & invariant)
        {
          auto status = _ta.initialize(vloc, intvars_val, initial_range, invariant);
          if (status != tchecker::STATE_OK)
            return status;
          return _zone_semantics.initialize(zone, tchecker::ta::delay_allowed(vloc), invariant, vloc);
        }
        
        /*!
         \brief Type of iterator over outgoing edges
         */
        using outgoing_edges_iterator_t = typename TA::outgoing_edges_iterator_t;
        
        /*!
         \brief Accessor
         \param vloc : tuple of locations
         \return range of outgoing synchronized and asynchronous edges from vloc
         */
        inline tchecker::range_t<outgoing_edges_iterator_t> outgoing_edges(vloc_t const & vloc) const
        {
          return _ta.outgoing_edges(vloc);
        }
        
        /*!
         \brief Dereference type for iterator over outgoing edges
         */
        using outgoing_edges_iterator_value_t = typename TA::outgoing_edges_iterator_value_t;
        
        /*!
         \brief Compute next state
         \param vloc : tuple of locations
         \param intvars_val : valuation of integer variables
         \param zone : a zone
         \param vedge : range of synchronized edges
         \param src_invariant : a tchecker::clock_constraint_t container
         \param guard : a tchecker::clock_constraint_t container
         \param clkreset : a tchecker::clock_reset_t container
         \param tgt_invariant : a tchecker::clock_constraint_t container
         \pre vloc, intvars_val, zone, and vedge have dimensions corresponding to number of processes,
         number of integer variables, number of clocks and number of processes in the timed automaton
         \post vloc, intvars_val and zone have been updated according to the transition that consists in
         the synchronized edges in vedge and according to the chosen zone semantics. All the clock constraints
         from the invariant of vloc before updating have been added to src_invariant. All the clock
         constraints from the invariant of vloc after updating have been added to tgt_invariant.
         All the clcok constraints from the guards in vedge have been added to guard. All the
         clock resets from the resets in vedge have been added to clkreset.
         \return STATE_OK if vloc, intvars_val and zone have been updated, see tchecker::ta::details::ta_t::next
         for other possible values when updating the integer variables valuation fails, and see
         tchecker::zone_semantics_t::next for possible values when updating the zone fails
         \note this method does not clear src_invariant, guard, clkreset and tgt_invariant. It adds clock
         contraints/resets to these containers. Every clock constraint/reset in the containers when the
         function is called are taken into account to update zone
         */
        enum tchecker::state_status_t next(vloc_t & vloc,
                                           intvars_valuation_t & intvars_val,
                                           zone_t & zone,
                                           outgoing_edges_iterator_value_t const & vedge,
                                           tchecker::clock_constraint_container_t & src_invariant,
                                           tchecker::clock_constraint_container_t & guard,
                                           tchecker::clock_reset_container_t & clkreset,
                                           tchecker::clock_constraint_container_t & tgt_invariant)
        {
            // std::cout << "EDGE TYPE:\t" << typeid(((vedge.begin())._edge)->_symbol).name() << "\n";
            // if(((vedge.begin())._edge)->_stop)    std::cout << "EDGE SYMBOL:\t" << ((vedge.begin())._edge)->_symbol << "\n";
          bool src_delay_allowed = tchecker::ta::delay_allowed(vloc);
          auto status = _ta.next(vloc, intvars_val, vedge, src_invariant, guard, clkreset, tgt_invariant);
          if (status != tchecker::STATE_OK)
            return status;
          bool tgt_delay_allowed = tchecker::ta::delay_allowed(vloc);
          return _zone_semantics.next(zone, src_delay_allowed, src_invariant, guard, clkreset,
                                      tgt_delay_allowed, tgt_invariant, vloc);
        }

        /*!
          \brief Return stack operation of edge
          \param vedge: Range of edges.
          \return a triplet in the form of pair of pair. The first bool, i.e. first of outer pair,
          describes whether the stack operation is NOP or (Push/Pop). The bool inside inner pair,
          describes whether the stack operation if (Push/Pop) is either Push or Pop, and the string
          which is in the inner pair is the string to be either pushed or popped in case operation is
          (Push/Pop).
        */
        std::pair<bool, std::pair<bool, std::string>> edge_name(outgoing_edges_iterator_value_t const & vedge){
            std::string symbol = "";
            bool b = (vedge.begin()._edge)->_stop;
            bool porp = 0;
            if(b){
                symbol = (vedge.begin()._edge)->_symbol;
                porp = (vedge.begin()._edge)->_push_or_pop;
            }
            return std::make_pair(b, make_pair(porp, symbol));
        }

        /*!
          \brief Determine if a node is equal to/subsumes another or not.
          \param z1: (q,Z)
          \param z2: (q',Z')
          \param vloc1: tuple of locations for z1
          \param vloc2: tuple of locations for z2
          \param subsumption: If we need to check for subsumption set this to true, if we need
          to check for equivalence set this value to false.
          \return For equivalence return false if Z~Z', else return true. For subsumption return
          false if Z'<=Z, else return true.
          Go To zone/dbm/semantics.hh for more details.
        */
        bool equal(zone_t & z1, zone_t & z2,
                   vloc_t & vloc1, vloc_t & vloc2,
                   intvars_valuation_t & intvars1, intvars_valuation_t & intvars2,
                   bool subsumption){
            bool lu_not_equal = local_lu_equal(z1, z2, vloc1, intvars1, subsumption);
            if (lu_not_equal)
              return true;

            std::vector<tchecker::clock_constraint_t> g1;
            std::vector<tchecker::clock_constraint_t> g2;
            collect_diagonal_constraints(vloc1, intvars1, g1);
            collect_diagonal_constraints(vloc2, intvars2, g2);
            g1.insert(g1.end(), g2.begin(), g2.end());

            if (g1.empty())
              return false;

            if (subsumption) {
              if (!diagonal_implication(z2, z1, g1))
                return true;
              return false;
            }

            if (!diagonal_implication(z1, z2, g1))
              return true;
            if (!diagonal_implication(z2, z1, g1))
              return true;
            return false;
        }

        /*!
          \brief Determine if two nodes are exactly equal or not.
          \param z1: (q,Z)
          \param z2: (q',Z')
          \return If DBMs of both Z and Z' are exactly the same return true, else return false.
          Go To zone/dbm/semantics.hh for more details.
        */
        bool exact_equal(zone_t & z1, zone_t & z2){
            return _zone_semantics.exact_equal(z1, z2);
        }

        /*!
          \brief Get name of state of node.
          \param vloc1: tuple of locations for (q,Z)
          \return q from (q,Z)
        */
        std::string node_name(vloc_t &vloc1){
            auto begin1 = vloc1.begin(), end1 = vloc1.end();
            std::string s1 = "";
            for(auto it = begin1 ; it != end1 ; it++){
                if(it != begin1)    s1 += ",";
                s1 += (*it)->name();
            }
            return s1;
        }
        
        /*!
         \brief Accessor
         \return Underlying model
         */
        inline constexpr model_t const & model() const
        {
          return _ta.model();
        }

        /*!
          \brief Explicit finite abstraction carrier used by the current abstraction layer
         */
        struct boolean_abstraction_t {
          std::vector<tchecker::clock_constraint_t> diagonal_bits; /*!< Finite diagonal abstraction bits */
          std::vector<tchecker::integer_t> lower_bits;             /*!< Finite lower-bound abstraction bits */
          std::vector<tchecker::integer_t> upper_bits;             /*!< Finite upper-bound abstraction bits */

          explicit boolean_abstraction_t(tchecker::clock_id_t clock_nb)
          : diagonal_bits(),
            lower_bits(clock_nb, tchecker::clockbounds::NO_BOUND),
            upper_bits(clock_nb, tchecker::clockbounds::NO_BOUND)
          {}
        };

        /*!
          \brief Type of abstraction class identifiers
         */
        using class_id_t = std::size_t;

        /*!
          \brief Explicit abstraction-class object
         */
        struct abstraction_class_t {
          class_id_t id;                  /*!< Stable class identifier */
          std::string canonical_key;      /*!< Canonical key of this class */
          boolean_abstraction_t abstraction; /*!< Canonical representative abstraction */

          abstraction_class_t(class_id_t class_id,
                              std::string const & key,
                              boolean_abstraction_t const & repr)
          : id(class_id),
            canonical_key(key),
            abstraction(repr)
          {}
        };

        /*!
          \brief Registry of abstraction classes keyed by canonical keys
         */
        class abstraction_registry_t {
        public:
          abstraction_registry_t() = default;

          class_id_t register_class(std::string const & key,
                                    boolean_abstraction_t const & abstraction)
          {
            auto it = _class_ids.find(key);
            if (it != _class_ids.end())
              return it->second;

            class_id_t id = _classes.size();
            _class_ids.emplace(key, id);
            _classes.emplace_back(id, key, abstraction);
            return id;
          }

          class_id_t class_id_of(std::string const & key) const
          {
            auto it = _class_ids.find(key);
            if (it == _class_ids.end())
              throw std::out_of_range("unknown abstraction class key");
            return it->second;
          }

          std::size_t class_count() const
          {
            return _classes.size();
          }

          abstraction_class_t const & representative(class_id_t id) const
          {
            if (id >= _classes.size())
              throw std::out_of_range("invalid abstraction class id");
            return _classes[id];
          }

        private:
          std::unordered_map<std::string, class_id_t> _class_ids; /*!< canonical_key -> class_id */
          std::vector<abstraction_class_t> _classes;              /*!< Stored abstraction classes */
        };

        /*!
          \brief Minimal offline enumerator of abstraction classes
         */
        class abstraction_enumerator_t {
        public:
          abstraction_enumerator_t() = default;

          class_id_t add(std::string const & location_name,
                         std::string const & key,
                         boolean_abstraction_t const & abstraction)
          {
            class_id_t id = _registry.register_class(key, abstraction);
            if (id >= _locations.size())
              _locations.resize(id + 1);
            _locations[id].insert(location_name);
            return id;
          }

          std::size_t class_count() const
          {
            return _registry.class_count();
          }

          abstraction_class_t const & representative(class_id_t id) const
          {
            return _registry.representative(id);
          }

          std::set<std::string> const & locations(class_id_t id) const
          {
            if (id >= _locations.size())
              throw std::out_of_range("invalid abstraction class id");
            return _locations[id];
          }

          abstraction_registry_t const & registry() const
          {
            return _registry;
          }

        private:
          abstraction_registry_t _registry;              /*!< Enumerated classes */
          std::vector<std::set<std::string>> _locations; /*!< Per-class location names */
        };

        /*!
          \brief Summary of one abstraction class inside a proof-support report
         */
        struct abstraction_class_summary_t {
          class_id_t id;                      /*!< Class identifier */
          std::string canonical_key;          /*!< Canonical key of this class */
          std::vector<std::string> locations; /*!< Sorted location names seen in this class */
        };

        /*!
          \brief Proof-support report built from an abstraction enumerator
         */
        struct proof_report_t {
          std::size_t class_count;                                     /*!< Number of abstraction classes */
          std::vector<abstraction_class_summary_t> classes;            /*!< Per-class summaries */
          std::map<std::string, std::set<class_id_t>> classes_by_location; /*!< Inverted location -> class ids view */
        };

        /*!
          \brief Lightweight observation layer for recording abstraction classes during exploration
         */
        class abstraction_observer_t {
        public:
          abstraction_observer_t() = default;

          class_id_t observe(std::string const & location_name,
                             std::string const & key,
                             boolean_abstraction_t const & abstraction)
          {
            return _enumerator.add(location_name, key, abstraction);
          }

          std::size_t class_count() const
          {
            return _enumerator.class_count();
          }

          abstraction_enumerator_t const & enumerator() const
          {
            return _enumerator;
          }

        private:
          abstraction_enumerator_t _enumerator; /*!< Observed abstraction classes */
        };

        /*!
          \brief Stable textual dump of the boolean abstraction attached to a symbolic control state
         */
        std::string abstraction_string(vloc_t const & vloc,
                                       intvars_valuation_t & intvars_val)
        {
          return boolean_abstraction_string(boolean_abstraction(vloc, intvars_val));
        }

        /*!
          \brief Canonical key of the boolean abstraction attached to a symbolic control state
         */
        std::string abstraction_key(vloc_t const & vloc,
                                    intvars_valuation_t & intvars_val)
        {
          return canonical_key(boolean_abstraction(vloc, intvars_val));
        }

        /*!
          \brief Check if two symbolic control parts belong to the same abstraction class
         */
        bool same_abstraction_class(vloc_t const & vloc1,
                                    intvars_valuation_t & intvars_val1,
                                    vloc_t const & vloc2,
                                    intvars_valuation_t & intvars_val2)
        {
          return same_boolean_abstraction(boolean_abstraction(vloc1, intvars_val1),
                                          boolean_abstraction(vloc2, intvars_val2));
        }

        /*!
          \brief Register or retrieve the abstraction class identifier of a symbolic control state
         */
        class_id_t class_id_of(vloc_t const & vloc,
                               intvars_valuation_t & intvars_val,
                               abstraction_registry_t & registry)
        {
          boolean_abstraction_t abstraction = boolean_abstraction(vloc, intvars_val);
          return registry.register_class(canonical_key(abstraction), abstraction);
        }

        /*!
          \brief Add a symbolic control state to an abstraction enumerator
         */
        class_id_t enumerate(vloc_t & vloc,
                             intvars_valuation_t & intvars_val,
                             abstraction_enumerator_t & enumerator)
        {
          boolean_abstraction_t abstraction = boolean_abstraction(vloc, intvars_val);
          return enumerator.add(node_name(vloc), canonical_key(abstraction), abstraction);
        }

        /*!
          \brief Observe a symbolic control state through the lightweight observation layer
         */
        class_id_t observe(vloc_t & vloc,
                           intvars_valuation_t & intvars_val,
                           abstraction_observer_t & observer)
        {
          boolean_abstraction_t abstraction = boolean_abstraction(vloc, intvars_val);
          return observer.observe(node_name(vloc), canonical_key(abstraction), abstraction);
        }

        /*!
          \brief Build a proof-support report from an abstraction enumerator
         */
        proof_report_t proof_report(abstraction_enumerator_t const & enumerator) const
        {
          proof_report_t report;
          report.class_count = enumerator.class_count();
          report.classes.reserve(report.class_count);

          for (class_id_t id = 0; id < report.class_count; ++id) {
            abstraction_class_summary_t summary;
            summary.id = id;
            summary.canonical_key = enumerator.representative(id).canonical_key;
            summary.locations.assign(enumerator.locations(id).begin(), enumerator.locations(id).end());
            report.classes.push_back(summary);

            for (std::string const & location : summary.locations)
              report.classes_by_location[location].insert(id);
          }

          return report;
        }

        /*!
          \brief Build a proof-support report from an abstraction observer
         */
        proof_report_t proof_report(abstraction_observer_t const & observer) const
        {
          return proof_report(observer.enumerator());
        }

        /*!
          \brief Stable textual dump of a proof-support report
         */
        std::string proof_report_string(proof_report_t const & report) const
        {
          std::ostringstream os;
          os << "class_count=" << report.class_count;

          for (abstraction_class_summary_t const & summary : report.classes) {
            os << "\nclass[" << summary.id << "].key=" << summary.canonical_key;
            os << "\nclass[" << summary.id << "].locations=";
            for (std::size_t i = 0; i < summary.locations.size(); ++i) {
              if (i != 0)
                os << ",";
              os << summary.locations[i];
            }
          }

          for (auto const & entry : report.classes_by_location) {
            os << "\nlocation[" << entry.first << "].classes=";
            bool first = true;
            for (class_id_t id : entry.second) {
              if (!first)
                os << ",";
              os << id;
              first = false;
            }
          }

          return os.str();
        }

        /*!
          \brief Stable textual dump of a proof-support report built from an enumerator
         */
        std::string proof_report_string(abstraction_enumerator_t const & enumerator) const
        {
          return proof_report_string(proof_report(enumerator));
        }

        /*!
          \brief Stable textual dump of a proof-support report built from an observer
         */
        std::string proof_report_string(abstraction_observer_t const & observer) const
        {
          return proof_report_string(proof_report(observer));
        }
      private:
        /*!
          \brief Cached state summary used by the current G-simulation prototype
          \note This is the implementation counterpart of the theory objects:
          - diagonals corresponds to G^+(q)
          - L/U correspond to LU(G(q)) restricted to simple constraints
          The cache key is the current tuple of locations plus the current
          valuation of integer variables, since both may affect guards.
        */
        struct guard_summary_t {
          std::vector<tchecker::clock_constraint_t> diagonals; /*!< Diagonal guard set G^+(q) */
          std::vector<tchecker::integer_t> L;                  /*!< Lower bounds extracted from simple constraints in G(q) */
          std::vector<tchecker::integer_t> U;                  /*!< Upper bounds extracted from simple constraints in G(q) */

          explicit guard_summary_t(tchecker::clock_id_t clock_nb)
          : diagonals(),
            L(clock_nb, tchecker::clockbounds::NO_BOUND),
            U(clock_nb, tchecker::clockbounds::NO_BOUND)
          {
            if (clock_nb > 0) {
              L[0] = 0;
              U[0] = 0;
            }
          }
        };

        /*!
          \brief Check equality of two clock constraints
         */
        static bool same_constraint(tchecker::clock_constraint_t const & lhs,
                                    tchecker::clock_constraint_t const & rhs)
        {
          return lhs.id1() == rhs.id1()
            && lhs.id2() == rhs.id2()
            && lhs.comparator() == rhs.comparator()
            && lhs.value() == rhs.value();
        }

        /*!
          \brief Compare whole boolean abstractions
         */
        static bool same_boolean_abstraction(boolean_abstraction_t const & lhs,
                                             boolean_abstraction_t const & rhs)
        {
          if (lhs.diagonal_bits.size() != rhs.diagonal_bits.size())
            return false;
          for (std::size_t i = 0; i < lhs.diagonal_bits.size(); ++i) {
            if (!same_constraint(lhs.diagonal_bits[i], rhs.diagonal_bits[i]))
              return false;
          }
          return lhs.lower_bits == rhs.lower_bits
            && lhs.upper_bits == rhs.upper_bits;
        }

        /*!
          \brief Access diagonal component of a boolean abstraction
         */
        static std::vector<tchecker::clock_constraint_t> const &
        diagonal_bits(boolean_abstraction_t const & abstraction)
        {
          return abstraction.diagonal_bits;
        }

        /*!
          \brief Access local lower-bound component of a boolean abstraction
         */
        static std::vector<tchecker::integer_t> const &
        lower_bits(boolean_abstraction_t const & abstraction)
        {
          return abstraction.lower_bits;
        }

        /*!
          \brief Access local upper-bound component of a boolean abstraction
         */
        static std::vector<tchecker::integer_t> const &
        upper_bits(boolean_abstraction_t const & abstraction)
        {
          return abstraction.upper_bits;
        }

        /*!
          \brief Compute the explicit boolean abstraction attached to a symbolic control state
         */
        boolean_abstraction_t boolean_abstraction(vloc_t const & vloc,
                                                  intvars_valuation_t & intvars_val)
        {
          guard_summary_t const & summary = guard_summary(vloc, intvars_val);
          boolean_abstraction_t abstraction(_clock_nb);
          abstraction.diagonal_bits = summary.diagonals;
          abstraction.lower_bits = summary.L;
          abstraction.upper_bits = summary.U;
          return abstraction;
        }

        /*!
          \brief Stable textual encoding of a clock constraint used by abstraction keys
         */
        static std::string constraint_string(tchecker::clock_constraint_t const & c)
        {
          std::ostringstream os;
          os << c.id1() << "," << c.id2() << "," << c.comparator() << "," << c.value();
          return os.str();
        }

        /*!
          \brief Stable textual dump of a boolean abstraction
          \note This output is intentionally canonical: equivalent abstractions
          should yield the same text, so later regression tests / strongly-finite
          checks can reuse it directly.
         */
        std::string boolean_abstraction_string(boolean_abstraction_t const & abstraction) const
        {
          std::ostringstream os;

          os << "diag=[";
          for (std::size_t i = 0; i < diagonal_bits(abstraction).size(); ++i) {
            if (i != 0)
              os << ";";
            os << constraint_string(diagonal_bits(abstraction)[i]);
          }
          os << "]";

          os << "|L=[";
          for (std::size_t i = 0; i < lower_bits(abstraction).size(); ++i) {
            if (i != 0)
              os << ",";
            os << lower_bits(abstraction)[i];
          }
          os << "]";

          os << "|U=[";
          for (std::size_t i = 0; i < upper_bits(abstraction).size(); ++i) {
            if (i != 0)
              os << ",";
            os << upper_bits(abstraction)[i];
          }
          os << "]";

          return os.str();
        }

        /*!
          \brief Canonical key of a boolean abstraction
         */
        std::string canonical_key(boolean_abstraction_t const & abstraction) const
        {
          return boolean_abstraction_string(abstraction);
        }

        /*!
          \brief Canonical key of the boolean abstraction attached to a symbolic control state
         */
        std::string canonical_key(vloc_t const & vloc,
                                  intvars_valuation_t & intvars_val)
        {
          return canonical_key(boolean_abstraction(vloc, intvars_val));
        }

        bool diagonal_implication(zone_t const & from,
                                  zone_t const & to,
                                  std::vector<tchecker::clock_constraint_t> const & diags) const
        {
          for (tchecker::clock_constraint_t const & c : diags) {
            if (zone_entails(from, c) && !zone_entails(to, c))
              return false;
          }
          return true;
        }

        bool zone_entails(zone_t const & zone, tchecker::clock_constraint_t const & c) const
        {
          if (!c.diagonal())
            return false;

          tchecker::clock_id_t dim = static_cast<tchecker::clock_id_t>(zone.dim());
          if (c.id1() >= dim || c.id2() >= dim)
            return false;

          tchecker::dbm::db_t const * dbm = zone.dbm();
          tchecker::dbm::db_t bound = tchecker::dbm::db(static_cast<tchecker::dbm::comparator_t>(c.comparator()), c.value());
          tchecker::dbm::db_t current = dbm[c.id1() * dim + c.id2()];
          return (tchecker::dbm::db_cmp(current, bound) <= 0);
        }

        bool local_lu_equal(zone_t & z1,
                            zone_t & z2,
                            vloc_t const & vloc,
                            intvars_valuation_t & intvars_val,
                            bool subsumption)
        {
          boolean_abstraction_t abstraction = boolean_abstraction(vloc, intvars_val);
          return tchecker::dbm::my_lu_plus(z1.dbm(), z2.dbm(), z1.dim(), z2.dim(),
                                           lower_bits(abstraction).data(),
                                           upper_bits(abstraction).data(),
                                           subsumption);
        }

        /*!
          \brief Cache key for the current symbolic control state
          \note Integer valuations are part of the key on purpose: two nodes at
          the same control location may enable different guards, hence different
          local LU(G(q)) summaries. The state_local_lu regression relies on this.
        */
        std::string summary_key(vloc_t const & vloc, intvars_valuation_t const & intvars_val) const
        {
          std::ostringstream os;
          bool first = true;
          for (typename vloc_t::loc_t const * loc : vloc) {
            if (!first)
              os << ",";
            os << loc->id();
            first = false;
          }
          os << "|";
          for (decltype(intvars_val.size()) i = 0; i < intvars_val.size(); ++i) {
            if (i != 0)
              os << ",";
            os << intvars_val[i];
          }
          return os.str();
        }

        /*!
          \brief Access cached summary for the current symbolic control state
          \note The cached summary is the implementation-level summary used by
          the current finite-state engineering of G-simulation:
          - diagonal implication reads summary.diagonals
          - LU inclusion reads summary.L and summary.U
        */
        guard_summary_t const & guard_summary(vloc_t const & vloc,
                                              intvars_valuation_t & intvars_val)
        {
          std::string key = summary_key(vloc, intvars_val);
          auto it = _guard_summary_cache.find(key);
          if (it != _guard_summary_cache.end())
            return it->second;

          auto inserted = _guard_summary_cache.emplace(key, guard_summary_t(_clock_nb));
          build_guard_summary(vloc, intvars_val, inserted.first->second);
          return inserted.first->second;
        }

        /*!
          \brief Build the cached summary associated to the current symbolic control state
          \note This computes in one pass the two parts used by the current
          prototype of G-simulation:
          - G^+(q): diagonal constraints kept in summary.diagonals
          - LU(G(q)): simple constraints projected to summary.L/U
        */
        void build_guard_summary(vloc_t const & vloc,
                                 intvars_valuation_t & intvars_val,
                                 guard_summary_t & summary)
        {
          summary.diagonals.clear();
          std::fill(summary.L.begin(), summary.L.end(), tchecker::clockbounds::NO_BOUND);
          std::fill(summary.U.begin(), summary.U.end(), tchecker::clockbounds::NO_BOUND);
          if (!summary.L.empty()) {
            summary.L[0] = 0;
            summary.U[0] = 0;
          }

          tchecker::clock_constraint_container_t constraints;
          tchecker::clock_reset_container_t resets;
          auto size = intvars_val.size();
          tchecker::intvars_valuation_t * intvars_copy
            = tchecker::intvars_valuation_allocate_and_construct(size, size);
          for (decltype(size) i = 0; i < size; ++i)
            (*intvars_copy)[i] = intvars_val[i];

          auto const & model = _ta.model();

          for (typename vloc_t::loc_t const * loc : vloc) {
            constraints.clear();
            resets.clear();
            _vm.run(model.invariant_bytecode(loc->id()), *intvars_copy, constraints, resets);
            for (tchecker::clock_constraint_t const & c : constraints)
              update_guard_summary(summary, c);
          }

          auto edges = _ta.outgoing_edges(vloc);
          for (auto it = edges.begin(); it != edges.end(); ++it) {
            auto const & vedge = *it;
            auto begin = vedge.begin(), end = vedge.end();
            for (auto eit = begin; eit != end; ++eit) {
              typename model_t::system_t::edge_t const * edge = *eit;
              constraints.clear();
              resets.clear();
              _vm.run(model.guard_bytecode(edge->id()), *intvars_copy, constraints, resets);
              for (tchecker::clock_constraint_t const & c : constraints)
                update_guard_summary(summary, c);
            }
          }

          normalize_diagonal_constraints(summary.diagonals);

          tchecker::intvars_valuation_destruct_and_deallocate(intvars_copy);
        }

        /*!
          \brief Put diagonal constraints in a canonical order and remove duplicates
          \note This keeps the cached summary stable when the same diagonal guard
          is collected multiple times from equivalent syntax, e.g. duplicated
          conjuncts in a guard. This is a small engineering step towards later
          boolean-abstraction style reasoning on summaries.
        */
        void normalize_diagonal_constraints(std::vector<tchecker::clock_constraint_t> & diagonals) const
        {
          auto less = [] (tchecker::clock_constraint_t const & lhs, tchecker::clock_constraint_t const & rhs) {
            if (lhs.id1() != rhs.id1())
              return lhs.id1() < rhs.id1();
            if (lhs.id2() != rhs.id2())
              return lhs.id2() < rhs.id2();
            if (lhs.comparator() != rhs.comparator())
              return lhs.comparator() < rhs.comparator();
            return lhs.value() < rhs.value();
          };

          std::sort(diagonals.begin(), diagonals.end(), less);
          diagonals.erase(std::unique(diagonals.begin(), diagonals.end(), same_constraint), diagonals.end());
        }

        /*!
          \brief Update cached summary with one clock constraint
          \note Diagonal constraints are stored verbatim in summary.diagonals.
          Simple constraints are projected to local LU bounds:
          - x - 0 # c contributes to U(x)
          - 0 - x # c contributes to L(x)
          Repeated simple constraints are harmless: summary.L/U keep the
          strongest bound seen so far through max-updates.
        */
        void update_guard_summary(guard_summary_t & summary, tchecker::clock_constraint_t const & c) const
        {
          if (c.diagonal()) {
            summary.diagonals.push_back(c);
            return;
          }

          if ((c.id1() != tchecker::zero_clock_id) && (c.id2() == tchecker::zero_clock_id)) {
            if (static_cast<std::size_t>(c.id1()) < summary.U.size())
              summary.U[c.id1()] = std::max(summary.U[c.id1()], c.value());
            return;
          }

          if ((c.id1() == tchecker::zero_clock_id) && (c.id2() != tchecker::zero_clock_id)) {
            if (static_cast<std::size_t>(c.id2()) < summary.L.size())
              summary.L[c.id2()] = std::max(summary.L[c.id2()], -c.value());
            return;
          }
        }

        void collect_diagonal_constraints(vloc_t const & vloc,
                                          intvars_valuation_t & intvars_val,
                                          std::vector<tchecker::clock_constraint_t> & out)
        {
          boolean_abstraction_t abstraction = boolean_abstraction(vloc, intvars_val);
          out = diagonal_bits(abstraction);
        }

        void collect_local_lu_bounds(vloc_t const & vloc,
                                     intvars_valuation_t & intvars_val,
                                     tchecker::clockbounds::map_t & L,
                                     tchecker::clockbounds::map_t & U)
        {
          boolean_abstraction_t abstraction = boolean_abstraction(vloc, intvars_val);
          tchecker::clockbounds::clear(L);
          tchecker::clockbounds::clear(U);
          for (tchecker::clock_id_t id = 0; id < static_cast<tchecker::clock_id_t>(lower_bits(abstraction).size()); ++id) {
            tchecker::clockbounds::update(L, id, lower_bits(abstraction)[id]);
            tchecker::clockbounds::update(U, id, upper_bits(abstraction)[id]);
          }
        }
      protected:
        TA _ta;                          /*!< Timed automaton */
        ZONE_SEMANTICS _zone_semantics;  /*!< Zone semantics */
        tchecker::vm_t _vm;              /*!< Bytecode interpreter for guard/invariant extraction */
        tchecker::clock_id_t _clock_nb;  /*!< Number of flattened clocks */
        std::unordered_map<std::string, guard_summary_t> _guard_summary_cache; /*!< Cached state summaries */
      };
      
    } // end of namespace details
    
  } // end of namespace zg
  
} // end of namespace tchecker

#endif // TCHECKER_ZG_DETAILS_ZG_HH
