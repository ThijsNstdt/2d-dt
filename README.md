# 2d-dt

## Compilation
The project is compiled using CMake, see the bash scripts under "./build_scripts/" for 
exact commands. 

## Config
The simulation settings are read from a json file. If no arguments are given, settings 
are from "./config/config.json". 

### Passing arguments
The `ArgumentHandler` class deals with interpreting command line arguments. One can 
pass either 0, 1, 3 or 4 arguments:
- When 0 arguments are passed, behaviour is as described above.
- 1 argument is passed if a config file different from the default is desired. Its 
  path is passed as the only argument.
- 3 arguments are passed if one wants to run simulations with the exact same settings 
  at the same time (for example by submitting a number of them to a job manager using 
  a bash script). To avoid clashes in filenames an index can be passed as follows. The 
  first arguments is again the path to the config file. The second argument must be 
  `repeat` to state the intention of repetition. The third argument is an integer used 
  to keep track of the job, and is added to filenames associated with this job.
- 4 arguments are passed if one wants automatically perform simulations over an array 
  of settings. In that case, one can input an array of settings in place of a single 
  setting in the config file. The first argument is again the path of the config file. 
  The second and third arguments point to the name of the setting one wants to index 
  (which is always two layers deep, due to how the config file is set up). The last 
  argument is an unsigned integer that points to an entry of the array in the config 
  file. The simulation is then run with the settings corresponding to that entry of 
  the array.

### Simulation summary
A simulation summary `.json` file is made upon starting the simulation. The first part 
of this file is a copy of the config file that is used (with the array substituted for 
the indexed entry in case 4 arguments are used). Next, an object `"summary"` is added 
to the file. Here, information such as duration or seed is added. Furthermore, the 
location of the raw measurement output file is added under the `"observables"` object.

## Rough structure
The information above is all one needs to know in order to get something to run. What 
follows here is an outline of how the program is set up in case one would 
want to expand or improve upon it.

An instance of the `Factory` class (or more precisely, one of its derived classes) 
is used to create a `Universe`. A `Universe` consists of `Vertices`, `Edges`, and 
`Triangles`, all labelled. The former and latter store information about who their 
neighbors are, and through which `Edge` they are neighboring them. This way, the connectivity of the 
triangulation is fully captured, even if the triangulation is of one of the degenerate 
ensembles. Currently, the only derived class of `Factory` that is fully implemented is 
`GrowthFactory`, which makes a universe of desired size by starting with a 4 
tetrahedron triangulation and making random insertions.

Edge flips are performed by derived classes of `Updater`. In the current state, one 
updater is implemented for each of the combinatorial, restricted degenerate, and 
maximally degenerate ensembles. An instance of `Updater` is passed a pointer to the 
universe in question upon construction.

Measurements are performed by instances of a class derived from `Observer`.

All of the above is handled by an instance of the `Runner` class. This runner has 
the `Factory`, `Updater`, `Universe`, and the various `Observer`s as its members. It 
also instructs when and how these should perform their actions based on the config 
settings.

