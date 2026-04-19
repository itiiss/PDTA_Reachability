/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_ALGORITHMS_EXPLORE_BUILDER_HH
#define TCHECKER_ALGORITHMS_EXPLORE_BUILDER_HH

#include <tuple>
#include <stack>

#include "tchecker/basictypes.hh"
#include "tchecker/graph/find_builder.hh"

/*!
 \file builder.hh
 \brief Graph builder for explore algorithm
 */

namespace tchecker {
  
  namespace explore {
    
    /*!
     \class builder_t
     \brief Graph builder for explore algorithm
     \tparam TS : type of transition system, should derive from tchecker::ts::ts_t
     \tparam GRAPH : type of graph, should derive from tchecker::explore::graph_t
     */
    template <class TS, class GRAPH>
    class builder_t : public tchecker::graph::ts_find_builder_t<TS, GRAPH> {
    public:
      /*!
       \brief Type of pointer to node
       */
      using node_ptr_t = typename tchecker::graph::ts_find_builder_t<TS, GRAPH>::node_ptr_t;
      
      /*!
       \brief Type of pointer to edge
       */
      using edge_ptr_t = typename tchecker::graph::ts_find_builder_t<TS, GRAPH>::edge_ptr_t;
      
      /*!
       \brief Constructor
       \param ts : a transition system
       \param graph : a graph
       \post this keeps a reference on ts and a reference on graph
       */
      builder_t(TS & ts, GRAPH & graph)
      : tchecker::graph::ts_find_builder_t<TS, GRAPH>(ts, graph), _node_id(0)
      {}
      
      /*!
       \brief Copy constructor
       */
      builder_t(tchecker::explore::builder_t<TS, GRAPH> const &) = default;
      
      /*!
       \brief Move constructor
       */
      builder_t(tchecker::explore::builder_t<TS, GRAPH> &&) = default;
      
      /*!
       \brief Destructor
       */
      ~builder_t() = default;
      
      /*!
       \brief Assignment oeprator
       */
      tchecker::explore::builder_t<TS, GRAPH> & operator= (tchecker::explore::builder_t<TS, GRAPH> const &) = default;
      
      /*!
       \brief Move-assignment oeprator
       */
      tchecker::explore::builder_t<TS, GRAPH> & operator= (tchecker::explore::builder_t<TS, GRAPH> &&) = default;
      
      /*!
       \brief Build initial node
       \param v : TS initial iterator value
       \return a tuple <node, status, is_new_node> where node is an initial node built from the initial state of TS
       corresponding to v, status is the state status of node, and is_new_node is true if node is a new node, false
       if it is an existing node.
       \note node points to nullptr if status != tchecker::STATE_OK
       */
      std::tuple<node_ptr_t, tchecker::state_status_t, bool> initial_node(typename TS::initial_iterator_value_t const & v)
      {
        node_ptr_t node;
        edge_ptr_t edge;
        tchecker::state_status_t status;
        bool is_new_node;
        
        std::tie(node, edge, status, is_new_node)
        = tchecker::graph::ts_find_builder_t<TS, GRAPH>::initial_node(v,
            std::make_tuple(_node_id), std::tuple<>());
        
        _node_id += (is_new_node ? 1 : 0);
        
        return std::make_tuple(node, status, is_new_node);
      }
      
      /*!
       \brief Build next node
       \param node : a node
       \param v : TS outgoing edges iterator value
       \return a tuple <next_node, edge, status, is_new_node> where next_node is the successor of node along with
       the outgoing edge corresponing to v, edge is the corresponding edge, status is the state status of next_node
       and is_new_node is true if node is a new node, false if it is an existing node
       \note next_node points to nullptr if status != tchecker::STATE_OK
       */
      std::tuple<node_ptr_t, edge_ptr_t, tchecker::state_status_t, bool>
      next_node(node_ptr_t & node, typename TS::outgoing_edges_iterator_value_t const & v)
      {
        node_ptr_t next_node;
        edge_ptr_t edge;
        tchecker::state_status_t status;
        bool is_new_node;
        
        std::tie(next_node, edge, status, is_new_node)
        = tchecker::graph::ts_find_builder_t<TS, GRAPH>::next_node(node, v,
            std::make_tuple(_node_id), std::tuple<>());
        
        _node_id += (is_new_node ? 1 : 0);
        
        return std::make_tuple(next_node, edge, status, is_new_node);
      }

      bool equal(const node_ptr_t &n1, const node_ptr_t &n2){
        return tchecker::graph::ts_find_builder_t<TS, GRAPH>::equal(n1, n2);
      }

      void match_push(const std::string &state1, const node_ptr_t &n1, const std::string &state2, const node_ptr_t &n2, const std::string &symbol, std::stack<std::pair<node_ptr_t, node_ptr_t>> &to_do){
        tchecker::graph::ts_find_builder_t<TS, GRAPH>::match_push(state1, n1, state2, n2, symbol, to_do);
      }

      void match_pop(const std::string &state1, const node_ptr_t &n1, const std::string &state2, const node_ptr_t &n2, const std::string &symbol, std::stack<std::pair<node_ptr_t, node_ptr_t>> &to_do){
        tchecker::graph::ts_find_builder_t<TS, GRAPH>::match_pop(state1, n1, state2, n2, symbol, to_do);
      }

      bool new_second_node(const std::string &state1, const node_ptr_t &n1, const std::string &state2, const node_ptr_t &n2){
        return tchecker::graph::ts_find_builder_t<TS, GRAPH>::new_second_node(state1, n1, state2, n2);
      }

      void add_push(const std::string &state1, const node_ptr_t &n1, const std::string &state2, const node_ptr_t &n2, const std::string &symbol){
        tchecker::graph::ts_find_builder_t<TS, GRAPH>::add_push(state1, n1, state2, n2, symbol);
      }

      void add_pop(const std::string &state1, const node_ptr_t &n1, const std::string &state2, const node_ptr_t &n2, const std::string &symbol){
        tchecker::graph::ts_find_builder_t<TS, GRAPH>::add_pop(state1, n1, state2, n2, symbol);
      }

      std::pair<bool, std::unordered_map<std::string, std::vector<node_ptr_t>>> get_second_level_map(const std::string &state, const node_ptr_t &n){
        return tchecker::graph::ts_find_builder_t<TS, GRAPH>::get_second_level_map(state, n);
      }

      std::pair<bool, std::pair<bool, std::string>> edge_name(typename TS::outgoing_edges_iterator_value_t const & v){
        return tchecker::graph::ts_find_builder_t<TS, GRAPH>::edge_name(v);
      }


      std::string node_name(node_ptr_t & node){
        // std::cout << "builder.hh/algo/explore\t";
        // std::cout << "builder.hh/algo/explore\t";
        return tchecker::graph::ts_find_builder_t<TS, GRAPH>::node_name(node);
      }

    private:
      tchecker::node_id_t _node_id;   /*!< Node identifier */
    };
    
  } // end of namespace explore
  
} // end of namespace tchecker

#endif // TCHECKER_ALGORITHMS_EXPLORE_BUILDER_HH
