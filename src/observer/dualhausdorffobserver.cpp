//
// Created by thijs on 15/06/2022.
//

#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip> // put_time
#include <sstream> // stringstream
#include <json/json.h>

#include "dualhausdorffobserver.hpp"

void DualHausdorffObserver::writeVector(const std::vector<int> &data) {
    std::ofstream outFile(m_fileName, std::ofstream::app);

    if(!outFile.is_open()) {
        throw std::runtime_error("in `writeVector` of DualHausdorffObserver: Problem with opening file");
    }

    for (int datum : data) {
        outFile << datum << ", ";
    }
    outFile << "\n";

    outFile.close();
}

void DualHausdorffObserver::setFileName(const std::string &addition) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");

    std::string filename = m_baseName + addition + "_" + datetime.str() + ".txt";
    m_fileName = filename;
}


void DualHausdorffObserver::measurement() {
    auto volumeProfile = sphereGrowth();

    writeVector(volumeProfile);
}

/**
 * get those neighbors of triangle that have not been visited
 * @param triangle
 * @return nextLayerTriangles
 */
std::vector<Triangle*> DualHausdorffObserver::nextLayerNeighbors(const Triangle *triangle) {
    std::vector<Triangle*> nextLayerTriangles;
    nextLayerTriangles.reserve(3);

    std::array<Triangle*, 3> candidates = triangle->getNeighbors();
    for (auto candidate : candidates) {
        if (!isVisited(candidate)) {
            nextLayerTriangles.push_back(candidate);
            setVisited(candidate);
        }
    }

    return nextLayerTriangles;
}

/**
 * for each triangle in current layer, add those neighbors that have not been visited to next layer, being careful
 * to mark these as visited immediately, to avoid double counting.
 * @param currentLayer
 * @return
 */
std::vector<Triangle*> DualHausdorffObserver::getNextLayer(const std::vector<Triangle*>& currentLayer) {
    // initialize next layer, which for large r is sure to be smaller than 2 times the current layer size
    std::vector<Triangle*> nextLayer;
    nextLayer.reserve(2*currentLayer.size());

    for (auto triangle : currentLayer) {
        std::vector<Triangle*> additionsToNextLayer = nextLayerNeighbors(triangle);
        nextLayer.insert(nextLayer.end(), additionsToNextLayer.begin(), additionsToNextLayer.end());
    }

    return nextLayer;
}

bool DualHausdorffObserver::isVisited(const Triangle *triangle) {
    int labelValue = triangle->getLabel().toInt();
    return m_visited[labelValue];
}

void DualHausdorffObserver::setVisited(const Triangle *triangle) {
    auto labelValue = triangle->getLabel().toInt();
    if (isVisited(triangle)) {
        throw std::invalid_argument("In `setVisited` of HausdorffObserver: triangle has already been visited");
    }
    m_visited[labelValue] = true;
}

DualHausdorffObserver::~DualHausdorffObserver() = default;

DualHausdorffObserver::DualHausdorffObserver(Universe *myUniverse) : Observer(myUniverse) {}

std::string DualHausdorffObserver::getFileName() {
    return m_fileName;
}

std::vector<int> DualHausdorffObserver::sphereGrowth() {
    // set up visited vector
    int numberOfTriangles = myUniverse->numberOfTriangles();
    std::vector<bool> tempVisited(numberOfTriangles, false);
    m_visited = tempVisited;

    // get starting point and initialize volumeprofile and currentlayer
    Triangle *startingTriangle = myUniverse->getRandomTriangle();
    setVisited(startingTriangle);
    std::vector<int> volumeProfile;
    std::vector<Triangle*> currentLayer;

    currentLayer.push_back(startingTriangle);

    // for each layer, store its size and generate next layer in loop
    int totalInRadius = 0;

    while( (totalInRadius < myUniverse->numberOfTriangles()) && (!currentLayer.empty()) ) {
        int layerSize = static_cast<int>(currentLayer.size());
        volumeProfile.push_back(layerSize);
        totalInRadius += layerSize;

        currentLayer = getNextLayer(currentLayer);
    }

    if (totalInRadius < myUniverse->numberOfTriangles()) {
        throw std::runtime_error("universe is not one connected component");
    }

    return volumeProfile;
}

void DualHausdorffObserver::loadSettingsFromConfig(const std::string &configLoc) {
    Json::Value root;
    std::ifstream ifs;
    ifs.open(configLoc);

    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    Json::String errs;

    if (!parseFromStream(builder, ifs, &root, &errs)) {
        std::cout << errs << std::endl;
        throw std::runtime_error("In `loadSettingsFromConfig` of DualHausdorffObserver: problem parsing config");
    }

    if (!root.isMember("observables")) {
        throw std::runtime_error("In `loadSettingsFromConfig` of DualHausdorffObserver: no observable settings in config");
    }

    Json::Value observableRoot = root["observables"];

    if (!observableRoot.isMember("DualHausdorffDimension")) {
        throw std::runtime_error("In `loadSettingsFromConfig` of DualHausdorffObserver: no DualHausdorffObserver settings in config");
    }

    Json::Value dhoRoot = observableRoot["DualHausdorffDimension"];

    if (dhoRoot.isMember("max_radius")) {
        maxRadius = dhoRoot["max_radius"].asUInt();
    }
}
