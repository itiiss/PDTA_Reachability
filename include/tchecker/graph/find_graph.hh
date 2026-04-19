/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_FIND_GRAPH_HH
#define TCHECKER_FIND_GRAPH_HH

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <tuple>

/*!
 \file find_graph.hh
 \brief Graph with node finding
 */

namespace tchecker {
  
  namespace graph {
    
    /*!
     \class find_graph_t
     \brief Graph with node finding
     \tparam NODE_PTR : type of pointer to node
     \tparam HASH : hash function on NODE_PTR (see std::hash)
     \tparam EQUAL : equality function on NODE_PTR (see std::equal_to)
     \note this graph implementation stores nodes and answers find queries.
     It does not store edges
     \note each node has a unique instance in this graph w.r.t. EQUAL
     */
    template <class NODE_PTR, class HASH, class EQUAL>
    class find_graph_t {
    public:
      /*!
       \brief Type of pointers of node
       */
      using node_ptr_t = NODE_PTR;
      
      /*!
       \brief Type of hash function
       */
      using hash_t = HASH;
      
      /*!
       \brief Type of equality predicate
       */
      using equal_t = EQUAL;
      
      /*!
       \brief Clear
       \post The graph is empty
       \note No destructor call on nodes
       */
      inline void clear()
      {
        _nodes.clear();
        all_nodes.clear();
      }
      
      /*!
       \brief Accessor
       \param n : a node
       \return the node in the graph equivalent to n w.r.t. HASH and EQUAL if any,
       n otherwise
       */
      NODE_PTR const & find(NODE_PTR const & n)
      {
        auto it = _nodes.find(n);
        if (it != _nodes.end())
          return *it;
        return n;
      }
      
      /*!
       \brief Add node
       \param n : a node
       \post n has been added to the graph unless it already contains an equivalent
       node w.r.t. HASH and EQUAL
       \return true if n has been added to the graph, false otherwise
       */
      bool add_node(NODE_PTR const & n)
      {
        try {
          _nodes.insert(n);
          all_nodes.push_back(n);
          return true;
        }
        catch (...)
        {}
        return false;
      }
      
      /*!
       \brief Type of iterator on nodes
       */
      using const_iterator_t = typename std::unordered_set<NODE_PTR, HASH, EQUAL>::const_iterator;
      
      /*!
       \brief Accessor
       \return iterator on first node if any, past-the-end iterator otherwise
       */
      inline tchecker::graph::find_graph_t<NODE_PTR, HASH, EQUAL>::const_iterator_t begin() const
      {
        return _nodes.begin();
      }
      
      /*!
       \brief Accessor
       \return past-the-end iterator
       */
      inline tchecker::graph::find_graph_t<NODE_PTR, HASH, EQUAL>::const_iterator_t end() const
      {
        return _nodes.end();
      }
      std::unordered_set<NODE_PTR, HASH, EQUAL> _nodes;
      std::vector<NODE_PTR> all_nodes;


      /*!
        \brief FULL_TLM as described in the paper. Contains Second Level Map as first pointer, Pushes As second Pointer, Pops as third pointer, and fourth a node pointer.
      */
      std::unordered_map<std::string, std::vector<std::tuple<std::unordered_map<std::string, std::vector<NODE_PTR>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<NODE_PTR>>>, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<NODE_PTR>>>, NODE_PTR>>> FULL_TLM;
    };
    
  } // end of namespace graph
  
} // end of namespace tchecker

#endif // TCHECKER_FIND_GRAPH_HH

