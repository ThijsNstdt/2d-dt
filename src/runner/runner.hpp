//
// Created by Thijs on 20-6-2022.
//

#ifndef INC_2D_DT_RUNNER_HPP
#define INC_2D_DT_RUNNER_HPP

#include <memory>
#include <stdexcept>
#include <fstream>
#include <string>
#include <json/json.h>

#include "src/factory/factory.hpp"
#include "src/updater/updater.hpp"
#include "src/observer/observer.hpp"
#include "src/universe/universe.hpp"
#include "src/runner/progressmanager.hpp"


class Runner {
public:
    Runner(const std::string &configLoc, unsigned int seed);

    void makeFactory();

    void makeUpdater();

    void setInitializationShape(Factory::InitGeometry shape) {m_shape = shape;}
    void setEnsemble(Updater::Ensemble ensemble) {m_ensemble = ensemble;}
    void addObserver(Observer::Observable observable);
    void setSize(int size) {m_size = size;}
    void setThermalizationSweeps(int thermalizationSweeps);
    void setAutocorrelationSweeps(int autocorrelationSweeps);
    void setNumberOfMeasurements(int numberOfMeasurements);

    void thermalize();

    void independentMeasurement();

    void measurementPhase();

    void printUpdaterStats();

    void conclude();

    void initialStateMeasurements();

    void settingsFromConfig(const std::string &configLoc);

    void makeObservers();

private:
    std::unique_ptr<Factory> m_factory;
    std::unique_ptr<Updater> m_updater;

    std::vector<std::unique_ptr<Observer>> m_observers;
    std::vector<Observer::Observable> m_observableTypes = {};

    ProgressManager m_progress;
    ProgressManager m_totalTimer;

    Universe m_universe;

    Factory::InitGeometry m_shape;
    Updater::Ensemble m_ensemble;
    Observer::Observable m_observable;
    int m_size;
    int m_thermalizationSweeps {};
    int m_autocorrelationSweeps {};
    int m_numberOfMeasurements {};

    bool m_subSweepMeasurements = false;

    struct SubSweepMeasurementData {
    public:
        SubSweepMeasurementData() = default;

        SubSweepMeasurementData(const int& measurementsPerSweep, const int& size)
            : m_measurementsPerSweep(measurementsPerSweep)
            , m_updatesPerMeasurement(size/measurementsPerSweep){}

        int m_measurementsPerSweep;
        int m_updatesPerMeasurement;
    };

    SubSweepMeasurementData m_subSweepData;

    bool m_firstMeasurement = true;

    std::unique_ptr<Observer> makeObserver(Observer::Observable observable);

    static std::string makeFileName();

    std::string m_outputFileName;

    std::string configFile;

    void writeToSummary(const std::string &key, const Json::Value &value);
};


#endif //INC_2D_DT_RUNNER_HPP
