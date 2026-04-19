/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_GRAPH_FIND_BUILDER_HH
#define TCHECKER_GRAPH_FIND_BUILDER_HH

#include <tuple>
#include <unordered_map>
#include <vector>
#include <stack>

#include "tchecker/basictypes.hh"
#include "tchecker/graph/builder.hh"

/*!
 \file find_builder.hh
 \brief Find graph builder
 */

namespace tchecker {
  
  namespace graph {
    
    /*!
     \class ts_find_builder_t
     \brief Build a find graph from a transition system
     \tparam TS : type of transition system (see tchecker::ts::ts_t)
     \tparam FIND_GRAPH : type of find graph, should derive from tchecker::graph::find_graph_t, and
     should be a graph allocator (see tchecker::graph::graph_allocator_t)
     */
    template <class TS, class FIND_GRAPH>
    class ts_find_builder_t : protected tchecker::graph::ts_builder_t<TS, FIND_GRAPH> {
    public:
      /*!
       \brief Type of pointer to node
       */
      using node_ptr_t = typename tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::node_ptr_t;
      
      /*!
       \brief Type of pointer to edge
       */
      using edge_ptr_t = typename tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::edge_ptr_t;
      
      /*!
       \brief Constructor
       \param ts : a transition system
       \param find_graph : a find graph with allocation
       \note see tchecker::graph::ts_builder_t::ts_builder_t
       */
      ts_find_builder_t(TS & ts, FIND_GRAPH & find_graph)
      : tchecker::graph::ts_builder_t<TS, FIND_GRAPH>(ts, find_graph)
      {}
      
      /*!
       \brief Copy constructor
       */
      ts_find_builder_t(tchecker::graph::ts_find_builder_t<TS, FIND_GRAPH> const &) = default;
      
      /*!
       \brief Move constructor
       */
      ts_find_builder_t(tchecker::graph::ts_find_builder_t<TS, FIND_GRAPH> &&) = default;
      
      /*!
       \brief Destructor
       */
      ~ts_find_builder_t() = default;
      
      /*!
       \brief Assignment operator
       */
      tchecker::graph::ts_find_builder_t<TS, FIND_GRAPH> &
      operator= (tchecker::graph::ts_find_builder_t<TS, FIND_GRAPH> const &) = default;
      
      /*!
       \brief Move assignment operator
       */
      tchecker::graph::ts_find_builder_t<TS, FIND_GRAPH> &
      operator= (tchecker::graph::ts_find_builder_t<TS, FIND_GRAPH> &&) = default;
      
      /*!
       \brief Compute initial node
       \param v : initial iterator value from the transition system
       \param nargs : parameters to FIND_GRAPH::allocate_node()
       \param eargs : parameters to FIND_GRAPH::allocate_edge()
       \return a quadruple <node, edge, status, is_new_node> where
       status is the tchecker::state_status_t status of node,
       node and edge are the initial nodes and edges computed from v,
       and is_new_node tells if node is a new node of an existing node.
       node points to nullptr if status != tchecker::STATE_OK
       */
      template <class ... NARGS, class ... EARGS>
      std::tuple<node_ptr_t, edge_ptr_t, tchecker::state_status_t, bool>
      initial_node(typename TS::initial_iterator_value_t const & v, std::tuple<NARGS...> && nargs, std::tuple<EARGS...> && eargs)
      {
        node_ptr_t node;
        edge_ptr_t edge;
        tchecker::state_status_t status;
        
        std::tie(node, edge, status)
        = tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::initial_node(v,
                                                                      std::forward<std::tuple<NARGS...>>(nargs),
                                                                      std::forward<std::tuple<EARGS...>>(eargs));
        
        if (status != tchecker::STATE_OK)
          return std::make_tuple(node, edge, status, false);
        
        node_ptr_t actual_node = this->_graph.find(node);
        bool is_new_node = (actual_node == node);
        return std::make_tuple(actual_node, edge, status, is_new_node);
      }
      
      /*!
       \brief Just Compute next node
       \param node : source node
       \param v : outgoing edges iterator value from the transition system
       \param nargs : parameters to FIND_GRAPH::allocate_node()
       \param eargs : parameters to FIND_GRAPH::allocate_edge()
       \return a quadruple <next_node, edge, status, is_new_node> where status
       describes whether next_node is empty or not,
       next_node and edges are the next node and outgoing edge of node
       corresponding to v, and is_new_node does not serve any purpose currently, but was
       added since it was previously part of tchecker.
       status != tchecker::STATE_OK
       */
      template <class ... NARGS, class ... EARGS>
      std::tuple<node_ptr_t, edge_ptr_t, tchecker::state_status_t, bool>
      next_node(node_ptr_t & node,
                typename TS::outgoing_edges_iterator_value_t const & v,
                std::tuple<NARGS...> && nargs,
                std::tuple<EARGS...> && eargs)
      {
        node_ptr_t next_node;
        edge_ptr_t edge;
        tchecker::state_status_t status;
        
        std::tie(next_node, edge, status)
        = tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::next_node(node,
                                                                   v,
                                                                   std::forward<std::tuple<NARGS...>>(nargs),
                                                                   std::forward<std::tuple<EARGS...>>(eargs));
        return std::make_tuple(next_node, edge, status, 1);
      }


      /*!
        \brief Return stack operation details of edge.
        \param v: outgoing edges iterator value from transition system.
        \return a triplet in the form of pair of pair. The first bool, i.e. first of outer pair,
        describes whether the stack operation is NOP or (Push/Pop). The bool inside inner pair,
        describes whether the stack operation if (Push/Pop) is either Push or Pop, and the string
        which is in the inner pair is the string to be either pushed or popped in case operation is
        (Push/Pop).
        Go To graph/builder.hh for more details.
      */
      std::pair<bool, std::pair<bool, std::string>> edge_name(typename TS::outgoing_edges_iterator_value_t const & v){
      	return tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::edge_name(v);
      }


      /*!
        \brief Return name of state of the node.
        \param node: Reference to the node (say, (q,Z)) in question.
        \return Return state q as a string.
      */
      std::string node_name(node_ptr_t & node){
        return tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::node_name(node);
      }

      /**
        \brief Determine if two nodes subsume each other.
        \param n1: Node say, (q,Z)
        \param n2: Node say, (q',Z')
        \return If Z~Z' (regardless of q,q', as global bounds used currently)
        then return 1, else return 0.
      */
      bool equal(const node_ptr_t &n1, const node_ptr_t &n2){
      	return !tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::equal(n1, n2, 0);
      }

      /**
        \brief Determine if two nodes are exactly equal.
        \param n1: Node say, (q,Z)
        \para n2: Node say, (q',Z')
        \return If Z=Z' (every cell in DBMs), return 1, else return 0.
      */
      bool exact_equal(const node_ptr_t &n1, const node_ptr_t &n2){
        return tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::exact_equal(n1, n2);
      }


      /**
        \brief Simply add node to TLM second level map and ToDo list.
        \param state1: q1 in (q1,Z1)
        \param n1: (q1,Z1)
        \param state2: q2 in (q2,Z2)
        \param n2: (q2,Z2)
        \param to_do: ToDo list
        \return Add [(q1,Z1),(q2,Z2)] to ToDo list and add (q2,Z2) to second level map of
        (q1,Z1) unconditionally (i.e. Don't check if the node (root or internal) exists,
        just add).
      */
      void just_add_node(const std::string &state1, const node_ptr_t &n1, const std::string &state2, const node_ptr_t &n2, std::stack<std::pair<node_ptr_t, node_ptr_t>> &to_do){
        bool x = this->_graph.FULL_TLM.find(state1) == this->_graph.FULL_TLM.end();
        if(x){
          this->_graph.FULL_TLM[state1] = std::vector<std::tuple<std::unordered_map<std::string, std::vector<node_ptr_t>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, node_ptr_t>>();
        }
        std::vector<std::tuple<std::unordered_map<std::string, std::vector<node_ptr_t>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, node_ptr_t>> &flm_tgt = this->_graph.FULL_TLM[state1];
        x = 0;
        for(auto & [slm, pushes, pops, nt] : flm_tgt){
          if(tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::exact_equal(nt, n1)){
            if(slm.find(state2) == slm.end()){
              slm[state2] = std::vector<node_ptr_t>();
            }
            slm[state2].push_back(n2);
            to_do.push(std::make_pair(n1, n2));
            x = 1;
            break;
          } else {
          }
        }
        if(x == 0){
          std::unordered_map<std::string, std::vector<node_ptr_t>> mptemp;
          mptemp[state2] = std::vector<node_ptr_t>(1, n2);
          std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>> push_temp, pop_temp;
          flm_tgt.push_back(make_tuple(mptemp, push_temp, pop_temp, n1));
          to_do.push(std::make_pair(n1, n2));
        }
      }

      /**
        \brief Add node to Pushes Map, and then match pop.
        \param state1: q1 in (q1,Z1)
        \param n1: (q1,Z1)
        \param state2: q2 in (q2, Z2)
        \param n2: (q2,Z2)
        \param symbol: a in q1 ->(push a) q2
        \param to_do: ToDo list
        \return Given (q1,Z1) ->(push a) (q2,Z2') with Z2'~Z2, add (q2,Z2) to pushes list of
        (q1,Z1), then add all nodes in pops map of (q2,Z2) to second level
        map of (q1,Z1) (match pop).
      */
      void append_push_and_match_pop(const std::string &state1, const node_ptr_t &n1, const std::string &state2, const node_ptr_t &n2, const std::string &symbol, std::stack<std::pair<node_ptr_t, node_ptr_t>> &to_do, bool eqOrSim){
        bool x = this->_graph.FULL_TLM.find(state2) == this->_graph.FULL_TLM.end();
        if(x){
          this->_graph.FULL_TLM[state2] = std::vector<std::tuple<std::unordered_map<std::string, std::vector<node_ptr_t>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, node_ptr_t>>();
        }
        std::vector<std::tuple<std::unordered_map<std::string, std::vector<node_ptr_t>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, node_ptr_t>> &temp = this->_graph.FULL_TLM[state2];
        x = 0;
        for(auto & [slm, pushes, pops, nt] : temp){
          if(tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::exact_equal(nt, n2) && state2 == node_name(nt)){
            if(pushes.find(symbol) != pushes.end() && pushes[symbol].find(state1) != pushes[symbol].end()){
              for(auto it = pushes[symbol][state1].begin() ; it != pushes[symbol][state1].end() ; it++){
                if(tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::exact_equal(*it, n1)){
                  x = 1;
                  break;
                }
              }
            }
            if(x == 0){
                if(pushes.find(symbol) == pushes.end()) pushes[symbol] = std::unordered_map<std::string, std::vector<node_ptr_t>>();
                if(pushes[symbol].find(state1) == pushes[symbol].end()) pushes[symbol][state1] = std::vector<node_ptr_t>(1, n1);
                else  pushes[symbol][state1].push_back(n1);
                if(pops.find(symbol) != pops.end()){
                  for(auto it = pops[symbol].begin() ; it != pops[symbol].end() ; it++){
                    std::string q2 = it->first;
                    std::vector<node_ptr_t> vec_z2 = it->second;
                    for(auto z2 = vec_z2.begin() ; z2 != vec_z2.end() ; z2++){
                      if(isNewNode(state1, n1, q2, *z2, eqOrSim)){
                        just_add_node(state1, n1, q2, *z2, to_do);
                      }
                    }
                  }
                }
            }
            break;
          }
        }
      }

      /**
        \brief Add node to Pops map, and match pushes.
        \param state1: q1 in (q1,Z1)
        \param n1: (q1,Z1)
        \param state2: q2 in (q2,Z2)
        \param n2: (q2,Z2)
        \param symbol: a in q1' ->(pop a) q2
        \param to_do: ToDo list
        \return Given (q1',Z1') in second level map of (q1,Z1), if we have
        (q1',Z1')->(pop a)(q2,Z2), then add (q2,Z2) to pops map of (q1,Z1),
        and add (q2,Z2) to second level map of all nodes in pushes map of (q1,Z1) (match push).
      */
      void append_pop_and_match_push(const std::string &state1, const node_ptr_t &n1, const std::string &state2, const node_ptr_t &n2, const std::string &symbol, std::stack<std::pair<node_ptr_t, node_ptr_t>> &to_do, bool eqOrSim){
        std::vector<std::tuple<std::unordered_map<std::string, std::vector<node_ptr_t>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, node_ptr_t>> &temp = this->_graph.FULL_TLM[state1];
        for(auto & [slm, pushes, pops, nt] : temp){
          if(tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::exact_equal(nt, n1) && state1 == node_name(nt)){
            bool x = 0;
            if(pops.find(symbol) == pops.end()) pops[symbol] = std::unordered_map<std::string, std::vector<node_ptr_t>>();
            if(pops[symbol].find(state2) == pops[symbol].end()){
              pops[symbol][state2] = std::vector<node_ptr_t>();
            } else {
              for(auto it = pops[symbol][state2].begin() ; it != pops[symbol][state2].end() ; it++){
                if(!tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::equal(*it, n2, 0)){
                  x = 1;
                  break;
                }
              }
            }
            if(x == 0){
              pops[symbol][state2].push_back(n2);
              if(pushes.find(symbol) != pushes.end()){
                for(auto it = pushes[symbol].begin() ; it != pushes[symbol].end() ; it++){
                  std::string q3 = it->first;
                  std::vector<node_ptr_t> vec_z3 = it->second;
                  for(auto z3 = vec_z3.begin() ; z3 != vec_z3.end() ; z3++){
                    if(isNewNode(q3, *z3, state2, n2, eqOrSim)){
                      just_add_node(q3, *z3, state2, n2, to_do);
                    }
                  }
                }
              }
            }
          }
        }
      }

      /**
        \brief Is a node new in a subtree, or does a node that subsumes it present.
        \param state1: q1 in (q1,Z1)
        \param n1: (q1,Z1)
        \param state2: q2 in (q2,Z2)
        \param n2: (q2,Z2)
        \return False if a node (q2,Z2') is in second level map of (q1,Z1), such that,
        Z2' simulates Z2. True otherwise.
      */
      bool isNewNode(const std::string &state1, const node_ptr_t &n1, const std::string &state2, const node_ptr_t &n2, bool eqOrSim){
        if(this->_graph.FULL_TLM.find(state1) == this->_graph.FULL_TLM.end()){
          return 1;
        }
        std::vector<std::tuple<std::unordered_map<std::string, std::vector<node_ptr_t>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, node_ptr_t>> &flm = this->_graph.FULL_TLM[state1];
        for(auto & [slm, pushes, pops, nt] : flm){
          if(tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::exact_equal(nt, n1)){
            if(slm.find(state2) == slm.end()) return 1;
            for(auto i = slm[state2].begin() ; i != slm[state2].end() ; i++){
              if(!tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::equal(*i, n2, eqOrSim)){
                return 0;
              }
            }
          }
        }
        return 1;
      }

      /**
        \brief Is a node new in the first level map or not.
        \param state: q in (q,Z)
        \param node: (q,Z)
        \return If there is a node (q,Z') in first level map, such that Z'~Z, then return
        pair [false, (q,Z)], else return [true, (q,Z')].
      */
      std::pair<bool, node_ptr_t> isNewRoot(const std::string &state, const node_ptr_t &node){
        if(this->_graph.FULL_TLM.find(state) == this->_graph.FULL_TLM.end()){
          return std::make_pair(true, node);
        }
        std::vector<std::tuple<std::unordered_map<std::string, std::vector<node_ptr_t>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<node_ptr_t>>>, node_ptr_t>> &flm = this->_graph.FULL_TLM[state];
        for(auto & [slm, pushes, pops, nt] : flm){
          if(!tchecker::graph::ts_builder_t<TS, FIND_GRAPH>::equal(nt, node, 0))  return std::make_pair(false, nt);
        }
        return std::make_pair(true, node);
      }
    };
    
  } // end of namespace graph
  
} // end of namespace tchecker

#endif // TCHECKER_GRAPH_FIND_BUILDER_HH
