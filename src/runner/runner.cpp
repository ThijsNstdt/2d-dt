//
// Created by Thijs on 20-6-2022.
//

#include <iomanip>
#include <json/json.h>

#include "runner.hpp"
#include "src/updater/simplicialupdater.hpp"
#include "src/updater/restricteddegenerateupdater.hpp"
#include "src/updater/maximallydegenerateupdater.hpp"
#include "src/factory/growthfactory.hpp"
#include "src/observer/dualhausdorffobserver.hpp"
#include "src/observer/observer.hpp"
#include "src/observer/vertexdegreeobserver.hpp"
#include "src/observer/hausdorffobserver.hpp"
#include "src/observer/averagespheredistance.hpp"
#include "src/observer/selfaveragespheredistance.hpp"
#include "src/observer/vertexgraphobserver.hpp"

/**
 * creates a factory of choice based on the private variables, and sets m_factory to be a
 * smart pointer to that factory.
 */
void Runner::makeFactory() {
    Factory::InitGeometry shape = m_shape;

    if (shape == Factory::Tetrahedron) {
        throw std::invalid_argument("tetrahedronfactory is still under construction");
    } else if (shape == Factory::Growth) {
        std::unique_ptr<Factory> myFactory = std::make_unique<GrowthFactory>();
        m_factory = std::move(myFactory);
    } else {
        throw std::invalid_argument("shape not supported");
    }
}

/**
 * creates an updater of choice based on the private variables, and sets m_updater to be a
 * smart pointer to that updater.
 */
void Runner::makeUpdater() {
    Updater::Ensemble ensemble = m_ensemble;

    if (ensemble == Updater::Simplicial) {
        std::unique_ptr<Updater> myUpdater = std::make_unique<SimplicialUpdater>(&m_universe);
        m_updater = std::move(myUpdater);
    } else if (ensemble == Updater::RestrictedDegenerate) {
        std::unique_ptr<Updater> myUpdater = std::make_unique<RestrictedDegenerateUpdater>(&m_universe);
        m_updater = std::move(myUpdater);
    } else if (ensemble == Updater::MaximallyDegenerate) {
        std::unique_ptr<Updater> myUpdater = std::make_unique<MaximallyDegenerateUpdater>(&m_universe);
        m_updater = std::move(myUpdater);
    } else {
        throw std::invalid_argument("ensemble not supported");
    }
}

/**
 * create an Observer of choice based on the private variables, and sets m_observer to be a
 * smart pointer to that observer.
 */
std::unique_ptr<Observer> Runner::makeObserver(Observer::Observable observable) {
    std::string addition;

    // read json file
    Json::Value root;
    std::ifstream ifs;
    ifs.open(configFile);

    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    Json::String errs;

    if (!parseFromStream(builder, ifs, &root, &errs)) {
        std::cout << errs << std::endl;
        throw std::runtime_error("could not parse baseConfigFile");
    }

    ifs.close();

    if (root.isMember("array_job_info")) {
        if (root["array_job_info"].isMember("index")) {
            addition = std::to_string(root["array_job_info"]["index"].asInt());
        }
    }

    std::unique_ptr<Observer> myObserver;

    if (observable == Observer::DualHausdorffDimension) {
        myObserver = std::make_unique<DualHausdorffObserver>(&m_universe);

    } else if (observable == Observer::VertexDegree) {
        myObserver = std::make_unique<VertexDegreeObserver>(&m_universe);

    } else if (observable == Observer::HausdorffDimension) {
        myObserver = std::make_unique<HausdorffObserver>(&m_universe);

    } else if (observable == Observer::AverageSphereDistance) {
        myObserver = std::make_unique<AverageSphereDistance>(&m_universe);

    } else if (observable == Observer::SelfAverageSphereDistance) {
        myObserver = std::make_unique<SelfAverageSphereDistance>(&m_universe);

    } else if (observable == Observer::VertexGraphObserver) {
        myObserver = std::make_unique<VertexGraphObserver>(&m_universe);

    } else {
        throw std::invalid_argument("In `makeObserver` of Runner: observable not supported");
    }

    myObserver->setFileName(addition);
    root["observables"][Observer::obsToStr(observable)]["result_location"] = myObserver->getFileName();

    // write json tree with filename to new config file
    std::ofstream ofs(configFile, std::ofstream::trunc);

    if (!ofs.is_open()) {
        throw std::runtime_error("can not open new configFile");
    }

    ofs << root.toStyledString() << std::endl;
    ofs.close();

    return myObserver;

}

void Runner::addObserver(Observer::Observable observable) {
    m_observableTypes.push_back(observable);
}

void Runner::thermalize() {
    m_progress.startProcess("Thermalization",m_thermalizationSweeps);
    m_progress.setVerbosityLevel(ProgressManager::VerbosityLevels::Minimal);
    for (int i = 0; i < m_thermalizationSweeps; ++i) {
        m_updater->sweepUpdate(1);
        m_progress.takeStep();
    }
    m_progress.endProcess();
}

void Runner::independentMeasurement() {
    if(m_firstMeasurement) {
        m_firstMeasurement = false;
    } else if (m_subSweepMeasurements) {
        m_updater->seriesOfUpdates(m_subSweepData.m_updatesPerMeasurement);
    } else {
        m_updater->sweepUpdate(m_autocorrelationSweeps);
    }

    for (auto&& observer : m_observers) {
        observer->measurement();
    }
}

void Runner::measurementPhase() {
    m_progress.startProcess("Measurement phase", m_numberOfMeasurements);
    m_progress.setVerbosityLevel(ProgressManager::VerbosityLevels::Minimal);
    for (int measurement = 0; measurement < m_numberOfMeasurements; ++measurement) {
        independentMeasurement();
        m_progress.takeStep();
    }
    m_progress.endProcess();
}

Runner::Runner(const std::string &configLoc, unsigned int seed) {
    settingsFromConfig(configLoc);

    m_totalTimer.startTimer();

    m_outputFileName = makeFileName();

    writeToSummary("seed", Json::Value(seed));

    writeToSummary("updates_per_measurement", Json::Value(m_subSweepData.m_updatesPerMeasurement));

    makeFactory();
    m_universe = m_factory->createUniverse(m_size);
    m_universe.setDistributions();
}

void Runner::printUpdaterStats() {
    std::cout << "simulation ended with " << m_updater->getTotalUpdates() << " updates, " <<
        m_updater->getRejections() << " rejections, for a rejection rate of " << m_updater->getRejectionRate() << "\n";
}

void Runner::conclude() {
    writeToSummary("updates", Json::Value( m_updater->getTotalUpdates() ) );
    writeToSummary("rejections", Json::Value( m_updater->getRejections() ) );
    writeToSummary("rejection_rate", Json::Value( m_updater->getRejectionRate() ) );
    writeToSummary("total_time", Json::Value( m_totalTimer.getTime() ) );

    printUpdaterStats();
}

std::string Runner::makeFileName() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");

    std::string filename = "./simulations/summary/simulation_" + datetime.str() + ".txt";
    return filename;
}

void Runner::settingsFromConfig(const std::string &configLoc) {
    // Set config file, and do the necessary JSON prep
    configFile = configLoc;

    Json::Value root;
    std::ifstream ifs;
    ifs.open(configLoc);

    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    Json::String errs;

    // check if file can be opened en read properly
    if (!parseFromStream(builder, ifs, &root, &errs)) {
        std::cout << errs << std::endl;
        throw std::runtime_error("In `settingsFromConfig` of Runner: problem parsing config " + configFile);
    }

    // define a couple of useful objects in the config JSON
    if (!root.isMember("initialization")) {
        throw std::runtime_error("In `settingsFromConfig` of Runner: no initialization settings in config");
    }
    Json::Value initializationRoot = root["initialization"];

    if (!root.isMember("updating")) {
        throw std::runtime_error("In `settingsFromConfig` of Runner: no updating settings in config");
    }
    Json::Value updatingRoot = root["updating"];

    if (!root.isMember("measurement")) {
        throw std::runtime_error("In `settingsFromConfig` of Runner: no measurement settings in config");
    }
    Json::Value measurementRoot = root["measurement"];

    if (!root.isMember("observables")) {
        throw std::runtime_error("In 'settingsFromConfig` of Runner: not observables settings in config");
    }
    Json::Value observablesRoot = root["observables"];

    // read out initialization parameters
    if (initializationRoot.isMember("size")) {
        m_size = initializationRoot["size"].asInt();
    }
    if (initializationRoot.isMember("shape")) {
        auto shape = initializationRoot["shape"].asString();
        m_shape = Factory::strToInitGeom(shape);
    }

    // read out updating parameters
    if (updatingRoot.isMember("ensemble")) {
        auto ensemble = updatingRoot["ensemble"].asString();
        m_ensemble = Updater::strToEnsemble(ensemble);
    }
    if (updatingRoot.isMember("thermalization_sweeps")) {
        m_thermalizationSweeps = updatingRoot["thermalization_sweeps"].asInt();
    }

    // read out measurement paramters
    if (measurementRoot.isMember("autocorrelation_sweeps")) {
        m_autocorrelationSweeps = measurementRoot["autocorrelation_sweeps"].asInt();
    }
    if (measurementRoot.isMember("number_of_measurements")) {
        m_numberOfMeasurements = measurementRoot["number_of_measurements"].asInt();
    }
    if (measurementRoot.isMember("measurements_per_sweep")) {
        m_subSweepMeasurements = true;
        m_subSweepData = SubSweepMeasurementData(measurementRoot["measurements_per_sweep"].asInt(), m_size);
    }

    // read out observables types
    auto observableNames = observablesRoot.getMemberNames();
    for (const auto& name : observableNames) {
        m_observableTypes.push_back( Observer::strToObs(name) );
    }
}

void Runner::makeObservers() {
    for (const auto &obsType : m_observableTypes) {
        auto myObserver = makeObserver(obsType);
        myObserver->loadSettingsFromConfig(configFile);

        m_observers.push_back(std::move(myObserver));
    }
}

void Runner::writeToSummary(const std::string &key, const Json::Value &value) {
    // read base config file
    Json::Value root;
    std::ifstream ifs;
    ifs.open(configFile);

    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    Json::String errs;

    if (!parseFromStream(builder, ifs, &root, &errs)) {
        std::cout << errs << std::endl;
        throw std::runtime_error("could not parse baseConfigFile");
    }

    ifs.close();

    root["summary"][key] = value;

    // write altered json tree to config file
    std::ofstream ofs(configFile, std::ofstream::trunc);

    if (!ofs.is_open()) {
        throw std::runtime_error("can not open new configFile");
    }

    ofs << root.toStyledString() << std::endl;
    ofs.close();
}



