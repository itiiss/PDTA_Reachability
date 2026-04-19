# About the Project
This project from the paper "Fast zone-based algorithms for reachability in pushdown timed automata", uses a Zone-Based algorithm in order to determine reachability in PDTAs. Our tool is built on the tool TChecker, and uses many operations that are present in the tool. For more details on the TChecker visit, https://www.labri.fr/perso/herbrete/tchecker/index.html.
Given a Pushdown Timed Automata (PDTA) and an initial control-state, we compute all the control-states reachable with a well-nested run in the PDTA. A control-state q is reachable with a well-nested run in the PDTA if starting from the initial state with an empty stack we can reach the target state q with an empty stack.
If we are only interested in reachability of a specific target state, we can improve our performance in some cases, by stopping as soon as the target state is discovered. Note that the region-based approach does this. For details regarding the algorithm please refer to the paper.

# Requirements
1. A C++ compiler with decent C++17 support (Clang >= 3.6 or GNU g++ >= 6 should work. Apple LLVM >= 10.0.0 works)
2. CMake (>= 2.8)
3. flex (>= 2.5.35)
4. bison (>= 3.0.4)
5. The Boost library (>= 1.65.0 -- probably works with earlier versions)
6. Doxygen (>= 1.8.15 -- probably works with earlier versions)
7. Catch2 (>= 2.7.0), clone branch v2.x and install, as version 3 is not compatible.

# Clone Repo
git clone https://github.com/karthik-314/PDTA_Reachability.git

# Build
Simply run `sudo bash install.sh`, or follow the following steps.
```
	cmake ../PDTA_Reachability -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
```
```
	make -j5
```
```
	make doc
```
```
	sudo make install
```

# Executing
1. In order to execute the benchmarks simply run, 
```
	cd MyExamples/
```
2. For equivalence run,
```
	bash examples.sh eq
```
3. For simulation run,
```
	bash examples.sh sim
```
By default simulation is assumed, therefore we can also run the following for simulation,
```
	bash examples.sh
```
4. For running a specific example, say B2_10.txt, run
```
	tchecker -pdta <graph_controller> B2_10.txt
```
	Where `<graph_controller>` can either be "sim", or "eq" denoting simulation or equivalence respectively. By default "sim" is used, so we can also ommit the `<graph_controller>`.

## Benchmarks
The benchmarks that are smaller can be directly verified using the corresponding automata. For larger parameterized automata, we have used scripts that are present in MyExamples/Scripts.
For example, for B2(x), we can use the script present in B2.sh in the Scripts folder, and change the parameters. For more information refer to each script in MyExamples/Scripts.

# File Format:
1. A documented sample file format has been given in MyExamples/sample.txt
2. The file format is similar to the TChecker file format with a few crucial differences like, no source invariants, clocks must be reset to 0 and not to any other value, and there can be only one process.

# Output Format:
The output will be of the format "[nodes, time(micro), states]", with nodes denoting the number of nodes in the entire graph, time denoting the total execution time in microseconds, and states denoting the set of states that are reachable via some well nested path in the PDTA.
