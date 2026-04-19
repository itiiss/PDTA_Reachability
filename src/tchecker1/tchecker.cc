/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include <cstring>
#include <fstream>
#include <getopt.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <string>
#include <cmath>
#include <unordered_set>
#include <boost/algorithm/string.hpp>

#include "tchecker/algorithms/covreach/options.hh"
#include "tchecker/algorithms/covreach/run.hh"
#include "tchecker/algorithms/explore/options.hh"
#include "tchecker/algorithms/explore/run.hh"
#include "tchecker/parsing/parsing.hh"
#include "tchecker/utils/log.hh"

/*!
 \file tchecker.cc
 \brief TChecker command-line tool
 */


/*!
 \brief Type of command
 */
enum command_t {
  COMMAND_UNKNOWN,
  COMMAND_EXPLORE,
  COMMAND_COVREACH,
};

/*!
 \brief Type of map of algorithm command-line options
 */
using command_line_options_map_t = std::unordered_map<std::string, std::string>;


/*!
 \brief Parse the type of graph controller.
 \param controller_string : a string indicating type of graph controller.
 \return a boolean variable indicating the type of graph controller used. 0 is returned
 for equivalence (eq) and 1 for simulation (sim).
 \post Exit if unknown type of graph controller is used.
 */
bool parse_graph_controller(char * controller_string)
{
  if (strcmp(controller_string, "eq") == 0)
    return 0;
  if (strcmp(controller_string, "sim") == 0)
    return 1;
  std::cout << "Unknown Type of graph controller\n";
  std::exit(1);
}


/*!
 \brief Parse command line options
 \param argc : number of command-line options
 \param argv : command-line options
 \param options : string of command-line options
 \param long_options : array of long names for command-line options
 \param log : logging facility
 \return A tuple <map, index> where map have entries (option, value) for every option with
 corresponding value from argv, and parsed according to options and long_options.
 value is the empty string if option has no associated value.
 index is the position of the first non-flag option in argv
 \pre options and long_options should follow getopt_long() requirements.
 Moreover, entries in long_options should either have format {long_name, arg, 0, short_name}
 or {long_name, arg, 0, 0} (no corresponding short name). Every short option (in options) must
 have a corresponding long option name in long_options
 argv[] is assumed to start with flags (of the form -s or --long), and then all non-flag
 options (input file names, etc)
 \post all errors and warnings have been reported to log
 */
std::tuple<command_line_options_map_t, int>
parse_options(int argc, char * argv[], char const * options, struct option const * long_options, tchecker::log_t & log)
{
  command_line_options_map_t map;
  int option = 0, option_index = 0;
  
  while (1) {
    option = getopt_long(argc, argv, options, long_options, &option_index);
    
    if (option == -1)
      break;        // All flags have been parsed
    
    if (option == ':')
      log.warning("Missing option parameter");
    else if (option == '?')
      log.warning("Unknown option");
    else if (option != 0) {
      char opt[] = {static_cast<char>(option), 0};
      map[opt] = (optarg == nullptr ? "" : optarg);
    }
    else
      map[long_options[option_index].name] = (optarg == nullptr ? "" : optarg);
  }
  
  return std::make_tuple(map, optind);
}


/*!
 \brief Print usage information
 \param exec_name : name of executable file
 \post Usage information has been output to std::cerr
 */
void usage(std::string const & exec_name)
{
  std::cerr << "Usage: " << exec_name << " -pdta [file] [graph controller]" << std::endl;
  std::cerr << "    with [graph controller]:" << std::endl;
  std::cerr << "        sim      Use simulation to control size of each TLM[q][Z][0]" << std::endl;
  std::cerr << "        eq       Use equivalence to control size of each TLM[q][Z][0]" << std::endl;
}

class Clock;

class Edge{
public:
  std::string src;
  std::string tgt;
  std::string process;
  std::string event;
  std::unordered_set<std::string> resets;
  std::unordered_set<std::string> guards;
  bool stack_op_or_not;
  bool push_or_pop;
  std::string stack_symbol;
  int compare;
  int with;
  Clock *clock;

  Edge(std::string _src, std::string _tgt, std::string _process, std::string _event, std::unordered_set<std::string> _resets, std::unordered_set<std::string> _guards, bool _stack_op_or_not, bool _push_or_pop, std::string _symbol, int _compare, int _with){
    src = _src;
    tgt = _tgt;
    process = _process;
    event = _event;
    resets = _resets;
    guards = _guards;
    stack_op_or_not = _stack_op_or_not;
    push_or_pop = _push_or_pop;
    stack_symbol = _symbol;
    compare = _compare;
    with = _with;
  }
};

class Clock{
public:
  Edge *edge;
  std::string clock_name;  
};

std::string between_brackets(std::string line){
  std::string ret = "";
  bool start = 0;
  for(char ch : line){
    if(ch == '}') return ret;
    if(start && ch != ' ') ret += ch;
    if(ch == '{') start = 1;
  }
  return ret;
}

std::string between_sq_brackets(std::string line){
  std::string ret = "";
  bool start = 0;
  for(char ch : line){
    if(ch == ']') return ret;
    if(start && ch != ' ')  ret += ch;
    if(ch == '[') start = 1;
  }
  return ret;
}

std::string get_reset(std::string full_reset){
  std::string reset = "";
  for(char ch : full_reset){
    if(ch == '=') return reset;
    reset += ch;
  }
  return reset;
}

std::pair<std::unordered_set<std::string>, std::unordered_set<std::string>> get_resets_guards(std::string btbr){
  using namespace std;
  vector<string> broke;
  unordered_set<string> resets;
  unordered_set<string> guards;
  bool reset_flag = 0, guard_flag = 0;
  boost::split(broke, btbr, boost::is_any_of(":"));
  for(int i = 0 ; i < broke.size() ; i++){
    if(broke[i] == "do"){
      guard_flag = 0;
      reset_flag = 1;
      continue;
    } else if(broke[i] == "provided") {
      guard_flag = 1;
      reset_flag = 0;
      continue;
    }
    if(reset_flag)  resets.insert(get_reset(broke[i]));
    if(guard_flag)  guards.insert(broke[i]);
  }
  return make_pair(resets, guards);
}

void get_pop_args(std::string &full, std::string &symb, int &comp, int &with){
  int i;
  symb = "";
  std::string temp = "";
  for(; i < full.size() ; i++){
    if(full[i] == '<'){
      comp = 1;
      if(full[i + 1] == '='){
        comp = 2;
        i++;
      }
      i++;
      break;
    } else if(full[i] == '>'){
      comp = 4;
      if(full[i + 1] == '='){
        comp = 3;
        i++;
      }
      i++;
      break;
    }
    symb += full[i];
  }
  for(; i < full.size() ; i++){
    temp += full[i];
  }
  with = stoi(temp);
}

std::string get_clock_name(int clock_count){
  std::string ret = "xk";
  ret += std::to_string(clock_count);
  return ret;
}

std::string get_event_name(std::string event, int event_count){
  std::string ret = event;
  ret += std::to_string(event_count);
  return ret;
}

std::unordered_set<std::string> get_all_locs(std::vector<Clock*> upper_bound_clocks, std::unordered_set<std::string> locations){
  using namespace std;
  unordered_set<string> new_locs;
  for(unordered_set<string>::iterator it = locations.begin() ; it != locations.end() ; it++){
    long poss = 0;
    long powsetsize = pow(2, upper_bound_clocks.size());
    string temp = *it;
    for(; poss < powsetsize ; poss++){
      for(int j = 0 ; j < upper_bound_clocks.size() ; j++){
        if(poss & (1 << j)){
          temp += upper_bound_clocks[j]->clock_name;
        }
      }
      new_locs.insert(temp);
    }
  }
  return new_locs;
}

std::string get_new_src_from_set(std::string src, std::vector<std::string> myset){
  std::string temp = src;
  for(std::vector<std::string>::iterator it = myset.begin() ; it != myset.end() ; it++){
    temp += *it;
  }
  return temp;
}

std::string get_new_tgt_from_set(std::string tgt, std::vector<std::string> myset, std::string new_clock){
  std::string ret = tgt;
  bool first = 1;
  for(std::vector<std::string>::iterator it = myset.begin() ; it != myset.end() ; it++){
    if(first && new_clock.compare(*it) < 0){
      ret += new_clock;
      first = 0;
    }
    ret += *it;
  }
  if(first) ret += new_clock;
  return ret;
}

std::string get_new_removed_tgt(std::string tgt, std::vector<std::string> myset, std::string new_clock){
  std::string ret = tgt;
  for(std::vector<std::string>::iterator it = myset.begin() ; it != myset.end() ; it++){
    if(new_clock.compare(*it) != 0){
      ret += *it;
    }
  }
  return ret;
}

std::string get_new_expression(std::string new_clock, int compare, int with){
  std::string ret = new_clock;
  switch(compare){
    case 1:
      ret += "<";
      break;
    case 2:
      ret += "<=";
      break;
    case 3:
      ret += ">=";
      break;
    case 4:
      ret += ">";
      break;
  }
  ret += std::to_string(with);
  return ret;
}

void get_union(std::unordered_set<std::string> &all_events, std::unordered_set<std::string> &temp_set){
  for(std::unordered_set<std::string>::iterator it = temp_set.begin() ; it != temp_set.end() ; it++){
    all_events.insert(*it);
  }
}

std::unordered_set<std::string> set_new_edges(std::vector<Edge*> &new_edges, Edge *edge, std::vector<Clock*> &upper_bound_clocks, std::vector<Clock*> &lower_bound_clocks){
  using namespace std;
  string src = edge->src, tgt = edge->tgt;
  long poss = 0;
  long count_event = 1;
  unordered_set<string> all_events;
  long powsetsize = pow(2, upper_bound_clocks.size());
  vector<string> myset;
  for( ; poss < powsetsize ; poss++){
    myset = vector<string>();
    for(int j = 0 ; j < upper_bound_clocks.size() ; j++){
      if(poss & (1 << j)){
        myset.push_back(upper_bound_clocks[j]->clock_name);
      }
    }
    string new_src = get_new_src_from_set(src, myset);
    // cout << new_src << "\n";
    if(edge->stack_op_or_not == 0){
      string new_tgt = get_new_src_from_set(tgt, myset);
      string new_event = get_event_name(edge->event, count_event++);
      new_edges.push_back(new Edge(new_src, new_tgt, edge->process, new_event, edge->resets, edge->guards, edge->stack_op_or_not, edge->push_or_pop, edge->stack_symbol, edge->compare, edge->with));
      all_events.insert(new_event);
    } else {
      if(edge->push_or_pop == 0){
        //Push
        unordered_set<string> temp_set(myset.begin(), myset.end());
        for(vector<Clock*>::iterator it = upper_bound_clocks.begin() ; it != upper_bound_clocks.end() ; it++){
          string new_clock = (*it)->clock_name;
          if(temp_set.find(new_clock) != temp_set.end()){
            string new_tgt = get_new_src_from_set(tgt, myset);
            string new_event = get_event_name(edge->event, count_event++);
            Edge *temp_edge = new Edge(new_src, new_tgt, edge->process, new_event, edge->resets, edge->guards, edge->stack_op_or_not, edge->push_or_pop, edge->stack_symbol, edge->compare, edge->with);
            new_edges.push_back(temp_edge);
            all_events.insert(new_event);
          } else {
            unordered_set<string> new_resets = edge->resets;
            new_resets.insert(new_clock);
            string new_stack_symbol = edge->stack_symbol;
            new_stack_symbol += new_clock;
            string new_tgt = get_new_tgt_from_set(tgt, myset, new_clock);
            string new_event = get_event_name(edge->event, count_event++);
            Edge *temp_edge = new Edge(new_src, new_tgt, edge->process, new_event, new_resets, edge->guards, edge->stack_op_or_not, edge->push_or_pop, new_stack_symbol, edge->compare, edge->with);
            new_edges.push_back(temp_edge);
            all_events.insert(new_event);
          }
        }
        for(vector<Clock*>::iterator it = lower_bound_clocks.begin() ; it != lower_bound_clocks.end() ; it++){
          string new_clock = (*it)->clock_name;
          string new_tgt = get_new_src_from_set(tgt, myset);
          unordered_set<string> new_resets = edge->resets;
          new_resets.insert(new_clock);
          string new_stack_symbol = edge->stack_symbol;
          new_stack_symbol += new_clock;
          string new_event = get_event_name(edge->event, count_event++);
          Edge *temp_edge = new Edge(new_src, new_tgt, edge->process, new_event, new_resets, edge->guards, edge->stack_op_or_not, edge->push_or_pop, new_stack_symbol, edge->compare, edge->with);
          new_edges.push_back(temp_edge);
          all_events.insert(new_event);
        }
      } else {
        //Pop
        string new_clock = (edge->clock)->clock_name;
        if(edge->compare > 2){
          //Lower Bound
          unordered_set<string> new_guards = edge->guards;
          new_guards.insert(get_new_expression(new_clock, edge->compare, edge->with));
          string new_stack_symbol = edge->stack_symbol;
          string new_tgt = get_new_src_from_set(tgt, myset);
          new_stack_symbol += new_clock;
          string new_event = get_event_name(edge->event, count_event++);
          Edge *temp_edge = new Edge(new_src, new_tgt, edge->process, new_event, edge->resets, new_guards, edge->stack_op_or_not, edge->push_or_pop, new_stack_symbol, edge->compare, edge->with);
          new_edges.push_back(temp_edge);
          all_events.insert(new_event);
        } else {
          //Upper Bound
          unordered_set<string> temp_set(myset.begin(), myset.end());
          if(temp_set.find(new_clock) != temp_set.end()){
            unordered_set<string> new_guards = edge->guards;
            new_guards.insert(get_new_expression(new_clock, edge->compare, edge->with));
            string new_stack_symbol = edge->stack_symbol;
            new_stack_symbol += new_clock;
            string new_tgt = get_new_src_from_set(tgt, myset);
            string new_new_tgt = get_new_removed_tgt(tgt, myset, new_clock);
            string new_event = get_event_name(edge->event, count_event++);
            Edge *temp_edge = new Edge(new_src, new_tgt, edge->process, new_event, edge->resets, new_guards, edge->stack_op_or_not, edge->push_or_pop, edge->stack_symbol, edge->compare, edge->with);
            new_edges.push_back(temp_edge);
            all_events.insert(new_event);
            new_event = get_event_name(edge->event, count_event++);
            temp_edge = new Edge(new_src, new_new_tgt, edge->process, new_event, edge->resets, new_guards, edge->stack_op_or_not, edge->push_or_pop, new_stack_symbol, edge->compare, edge->with);
            new_edges.push_back(temp_edge);
            all_events.insert(new_event);
          }
        }
      }
    }
  }
  return all_events;
}

std::string get_bracket_string(std::unordered_set<std::string> &resets, std::unordered_set<std::string> &guards){
  using namespace std;
  string ret = "{";
  if(!resets.empty()){
    ret += "do: ";
    int s = 0;
    for(unordered_set<string>::iterator it = resets.begin() ; it != resets.end() ; it++){
      ret += *it;
      ret += "=0";
      if(s < resets.size() - 1) ret += " ; ";
      s++;
    }
    if(!guards.empty()) ret += " : ";
  }
  if(!guards.empty()){
    ret += "provided: ";
    int s = 0;
    for(unordered_set<string>::iterator it = guards.begin() ; it != guards.end() ; it++){
      ret += *it;
      if(s < guards.size() - 1) ret += " && ";
      s++;
    }
  }
  ret += "}";
  return ret;
}

std::string get_sq_bracket_string(Edge *edge){
  using namespace std;
  string ret = "[";
  if(edge->stack_op_or_not){
    if(edge->push_or_pop == 0){
      ret += "push:";
      ret += edge->stack_symbol;
    } else {
      ret += "pop:";
      ret += edge->stack_symbol;
      switch(edge->compare){
        case 1:
          ret += "<";
          break;
        case 2:
          ret += "<=";
          break;
        case 3:
          ret += ">=";
          break;
        case 4:
          ret += ">";
          break;
      }
      ret += to_string(edge->with);
    }
  }
  ret += "]";
  return ret;
}

void print_edge(Edge *edge, std::ostream &os){
  using namespace std;
  os << "edge:";
  os << edge->process;
  os << ":";
  os << edge->src;
  os << ":";
  os << edge->tgt;
  os << ":";
  os << edge->event;
  os << get_bracket_string(edge->resets, edge->guards);
  os << get_sq_bracket_string(edge);
  os << "\n";
}

std::string read_all(std::string filename){
  using namespace std;
  ofstream of("temp.txt");
  unordered_set<string> clocks;
  unordered_set<string> locations;
  unordered_set<string> events;
  string init_loc;
  vector<Clock*> upper_bound_clocks;
  vector<Clock*> lower_bound_clocks;
  vector<Edge*> new_edges;
  vector<Edge*> edges;
  vector<string> splitted;
  int clock_count = 1;
  ifstream myf(filename);
  string line;
  string sysline;
  while(getline(myf, line)){
    boost::split(splitted, line, boost::is_any_of(":{}"));
    if(splitted[0] == "system"){
      sysline = line;
    } else if(splitted[0] == "clock"){
      clocks.insert(splitted[2]);
    } else if(splitted[0] == "location"){
      locations.insert(splitted[2]);
      if(splitted[3] == "initial"){
        init_loc = splitted[2];
      }
    } else if(splitted[0] == "event"){
      events.insert(splitted[1]);
    } else if(splitted[0] == "edge"){
      string process = splitted[1];
      string src = splitted[2];
      bool stack_op_or_not = 0;
      bool push_or_pop;
      Clock *clock = NULL;
      string tgt = splitted[3];
      string event = splitted[4];
      string btbr = between_brackets(line);
      string btsqbr = between_sq_brackets(line);
      string sop, symbol;
      int compare, with;
      if(btsqbr != ""){
        stack_op_or_not = 1;
        vector<string> temp;
        boost::split(temp, btsqbr, boost::is_any_of(":"));
        sop = temp[0];
        if(sop == "push"){
          push_or_pop = 0;
          symbol = temp[1];
        }
        else if(sop == "pop"){
          push_or_pop = 1;
          get_pop_args(temp[1], symbol, compare, with);
          clock = new Clock();
          cout << symbol << "\n";
          cout << compare << "\n";
          cout << with << "\n";
          string clock_name = get_clock_name(clock_count);
          clock->clock_name = clock_name;
          clocks.insert(clock_name);
          clock_count++;
        }
      }
      pair<unordered_set<string>, unordered_set<string>> rg = get_resets_guards(btbr);
      unordered_set<string> p1 = rg.first;
      unordered_set<string> p2 = rg.second;
      Edge *ed = new Edge(src, tgt, process, event, p1, p2, stack_op_or_not, push_or_pop, symbol, compare, with);
      edges.push_back(ed);
      ed->clock = clock;
      if(clock != NULL){
        if(compare > 2)  lower_bound_clocks.push_back(clock);
        else  upper_bound_clocks.push_back(clock);
        clock->edge = ed;
      }
    }
  }
  // cout << edges.size() << "\n";
  unordered_set<string> all_events;
  for(vector<Edge*>::iterator it = edges.begin() ; it != edges.end() ; it++){
    Edge *ed = *it;
    unordered_set<string> temp_set = set_new_edges(new_edges, ed, upper_bound_clocks, lower_bound_clocks);
    get_union(all_events, temp_set);
  }
  of << sysline << "\n";
  for(unordered_set<string>::iterator it = clocks.begin() ; it != clocks.end() ; it++){
    of << "clock:1:" << *it << "\n";
  }
  cout << all_events.size() << "\n";
  for(unordered_set<string>::iterator it = all_events.begin() ; it != all_events.end() ; it++){
    of << "event:" << *it << "\n";
  }
  of << "process:P\n";
  unordered_set<string> new_locations = get_all_locs(upper_bound_clocks, locations);
  for(unordered_set<string>::iterator it = new_locations.begin() ; it != new_locations.end() ; it++){
    of << "location:P:";
    of << *it;
    of << "{";
    if(init_loc.compare(*it) == 0){
      of << "initial:";
    }
    of << "}\n";
  }
  for(vector<Edge*>::iterator it = new_edges.begin() ; it != new_edges.end() ; it++){
    print_edge(*it, of);
  }
  of.close();
  // cout << new_edges.size() << "\n";
  // cout << upper_bound_clocks.size() << "\n";
  myf.close();
  string filen = "temp.txt";
  return filen;
}


/*!
 \brief Command-line interface to TChecker
 */
int main(int argc, char * argv[])
{
  tchecker::log_t log(&std::cerr);
  
  // no command: list all available commands
  if (argc < 3 || argc > 4) {
    usage(argv[0]);
    return 0;
  }
  if (strcmp(argv[1], "-pdta") != 0){
    usage(argv[0]);
    return 0;
  }
  bool eqOrSim;
  // parse command
  if(argc == 3){
    eqOrSim = 1;
  } else {
    eqOrSim = parse_graph_controller(argv[2]);
  }
  
  // parse options
  command_line_options_map_t map;
  int index;
  
  //Hard Code Options for TChecker
  char *new_argv[4];
  new_argv[0] = (char *)"explore";
  new_argv[1] = (char *)"-m";
  new_argv[2] = (char *)"zg:elapsed:extraLU+g";
  new_argv[3] = (argc == 3) ? argv[2] : argv[3];  //File name
  std::tie(map, index) = parse_options(4, new_argv, tchecker::explore::options_t::getopt_long_options,
                                           tchecker::explore::options_t::getopt_long_options_long, log);

  if (map.find("h") != map.end()) {
    tchecker::explore::options_t::describe(std::cerr);
    return -1;
  }
  
  std::string filename(new_argv[3]);
  
  // run the command
  tchecker::parsing::system_declaration_t const * sysdecl = nullptr;
  
  try {
    // filename = read_all(filename);
    sysdecl = tchecker::parsing::parse_system_declaration(filename, log);
    if (sysdecl == nullptr)
      throw std::runtime_error("nullptr system declaration");
    tchecker::explore::options_t options(tchecker::make_range(map.begin(), map.end()), log);
    tchecker::explore::run(*sysdecl, options, log, eqOrSim);
  }
  catch (std::exception const & e) {
    log.error(e.what());
  }
  
  log.display_counts();
  delete sysdecl;
  
  return EXIT_SUCCESS;
}
