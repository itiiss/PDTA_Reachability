/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_ALGORITHMS_EXPLORE_ALGORITHM_HH
#define TCHECKER_ALGORITHMS_EXPLORE_ALGORITHM_HH

#include <tuple>
#include <typeinfo>
#include <stack>
#include <unordered_set>
#include <vector>
#include <chrono>

#include "tchecker/basictypes.hh"
#include "tchecker/algorithms/explore/builder.hh"

/*!
 \file algorithm.hh
 \brief Explore algorithm
 */

namespace tchecker {
  
  namespace explore {
    
    /*!
     \class algorithm_t
     \brief Explore algorithm
     \tparam TS : type of transition system, should derive from tchecker::ts::ts_t
     \tparam GRAPH : type of graph, should derive from tchecker::explore::graph_t
     \tparam WAITING : type of waiting container, should derive from tchecker::utils::waiting_t
     */
    template <class TS, class GRAPH, template <class NPTR> class WAITING>
    class algorithm_t {
    public:
      /*!
       \brief Create a Full TLM (Zone Graph) given a Transition System
       \param ts : a transition system
       \param graph : a graph
       \post TLM is built from a traversal of ts. Currently the entire zone graph/TLM
       is constructed, but we could stop in between by providing breaking when seeing a final
       state.
       */
      void run(TS & ts, GRAPH & graph, bool eqOrSim)
      {
        using namespace std::chrono;
        auto start = high_resolution_clock::now(); 
        using node_ptr_t = typename GRAPH::node_ptr_t;
        using edge_ptr_t = typename GRAPH::edge_ptr_t;

        tchecker::explore::builder_t<TS, GRAPH> builder(ts, graph);
        WAITING<node_ptr_t> waiting;

        std::stack<std::pair<node_ptr_t, node_ptr_t>> to_do;
        
        node_ptr_t initial_node, next_node;
        edge_ptr_t edge;
        tchecker::state_status_t status;
        bool is_new_node;
        int count = 0;

        auto initial_range = ts.initial();
        for (auto it = initial_range.begin(); ! it.at_end(); ++it) {
          std::tie(initial_node, status, is_new_node) = builder.initial_node(*it);
          std::string name = builder.node_name(initial_node);
          if (is_new_node) {
            builder.just_add_node(name, initial_node, name, initial_node, to_do);
            // to_do.push(std::make_pair(initial_node, initial_node));
          }
        }

        std::string init_state = builder.node_name(initial_node);

        //Last Version Start ###################################################################################
        std::vector<std::string> reachable;
        std::unordered_set<std::string> all_reachable;
        while(!to_do.empty()){
          std::pair<node_ptr_t, node_ptr_t> top = to_do.top();
          to_do.pop();
          count++;
          node_ptr_t root = top.first;
          node_ptr_t sub = top.second, tgt_node;
          std::string state1 = builder.node_name(root);
          std::string fin_state = builder.node_name(sub);
          if(state1 == init_state && builder.equal(root, initial_node)){
            if(all_reachable.find(fin_state) == all_reachable.end()){
            	reachable.push_back(fin_state);
            	all_reachable.insert(fin_state);
            }
          }
          auto vedges_range = ts.outgoing_edges(*sub);
          for(auto it = vedges_range.begin() ; !it.at_end() ; ++it){
            std::pair<bool, std::pair<bool, std::string>> ed_details = builder.edge_name(*it);
            std::tie(tgt_node, edge, status, is_new_node) = builder.next_node(sub, *it);
            if(status == tchecker::STATE_OK){
              std::string tgt_name = builder.node_name(tgt_node);
              if(!ed_details.first){
                //NOP Operation
                if(builder.isNewNode(state1, root, tgt_name, tgt_node, eqOrSim)){
                  builder.just_add_node(state1, root, tgt_name, tgt_node, to_do);
                }
              } else {
                if(!ed_details.second.first){
                  //Push Operation
                  std::pair<bool, node_ptr_t> z_1 = builder.isNewRoot(tgt_name, tgt_node);
                  node_ptr_t n = z_1.second;
                  if(z_1.first){
                    builder.just_add_node(tgt_name, tgt_node, tgt_name, tgt_node, to_do);
                  }
                  builder.append_push_and_match_pop(state1, root, tgt_name, n, ed_details.second.second, to_do, eqOrSim);
                } else {
                  //Pop Operation
                  builder.append_pop_and_match_push(state1, root, tgt_name, tgt_node, ed_details.second.second, to_do, eqOrSim);
                }
              }
            }
          }
        }

        // std::cout << "REACHABLE (WELL NESTED) STATES START\n";
        // std::cout << "REACHABLE (WELL NESTED) STATES END\n";
        std::cout << "[nodes, time(microseconds), states] = ";
        std::cout << "[" << count << ", ";
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::cout << duration.count() << ", {";
        int reach_count = 0;
        for(auto it = reachable.begin() ; it != reachable.end() ; ){
        	std::cout << *it;
			it++;
			if(it != reachable.end())	std::cout << ", ";
        }
        std::cout << "}]\n\n";
        std::exit(0);
      }
    };
    
  } // end of namespace explore
  
} // end of namespace tchecker

#endif // TCHECKER_ALGORITHMS_EXPLORE_ALGORITHM_HH
