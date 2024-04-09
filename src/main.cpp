#include "src/runner/runner.hpp"
#include "src/rng/global_engine.hpp"
#include "src/argumenthandler/argumenthandler.hpp"

// initialize global random engine
std::random_device rd;
unsigned int seed = rd();
std::mt19937 global_engine(seed);

int main( int argc, char *argv[] ) {
    ArgumentHandler myHandler(argc);
    std::string configFile = myHandler.getSimulationConfig(argv);

    Runner myRunner(configFile, seed);

    myRunner.makeUpdater();
    myRunner.thermalize();

    myRunner.makeObservers();
    myRunner.measurementPhase();

    myRunner.conclude();
}