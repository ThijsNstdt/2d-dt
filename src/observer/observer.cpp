//
// Created by thijs on 15/06/2022.
//

#include "src/rng/global_engine.hpp"
#include "observer.hpp"
#include <fstream>
#include <chrono>
#include <vector>
#include <stdexcept>
#include <json/json.h>
#include <iomanip>
#include <numeric>

Observer::Observer(Universe *myUniverse) : myUniverse(myUniverse) {}

Observer::~Observer() = default;

Observer::Observable Observer::strToObs(const std::string& string) {
    if (string == "dual_hausdorff_dimension") {
        return DualHausdorffDimension;
    }
    if (string == "vertex_degree") {
        return VertexDegree;
    }
    if (string == "hausdorff_dimension") {
        return HausdorffDimension;
    }
    if (string == "asd") {
        return AverageSphereDistance;
    }
    if (string == "self_average_sphere_distance") {
        return SelfAverageSphereDistance;
    }
    if (string == "vertex_graph") {
        return VertexGraphObserver;
    }

    throw std::runtime_error("In `strToObs` of Observer: input " + string +  " is not supported");

}

std::string Observer::obsToStr(const Observer::Observable &obs) {
    if (obs == DualHausdorffDimension) {
        return "dual_hausdorff_dimension";
    }
    if (obs == VertexDegree) {
        return "vertex_degree";
    }
    if (obs == HausdorffDimension) {
        return "hausdorff_dimension";
    }
    if (obs == AverageSphereDistance) {
        return "asd";
    }
    if (obs == SelfAverageSphereDistance) {
        return "self_average_sphere_distance";
    }
    if (obs == VertexGraphObserver) {
        return "vertex_graph";
    }

    throw std::runtime_error("In `obsToStr` of Observer: input observable is not supported");
}

void Observer::setFileName(const std::string &addition) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");

    std::string filename = baseName_ + addition + "_" + datetime.str() + ".txt";
    fileName_ = filename;
}

Observer::SphereGrower::SphereGrower(const Vertex *startingVertex, Universe *universe) : startingVertex_(startingVertex), universe_(universe) {
    currentLayer_.push_back(startingVertex);
    shellSizes_.push_back(currentLayer_.size());
    setVisited(startingVertex_);
}

bool Observer::SphereGrower::isVisited(const Vertex *vertex) const {
    return visited_[vertex->getLabel().toInt()];
}

void Observer::SphereGrower::setVisited(const Vertex *vertex) {
    auto labelValue = vertex->getLabel().toInt();
    if (isVisited(vertex)) {
        throw std::runtime_error("In setVisited of Observer::sphereGrower: vertex has already been visited");
    }
    visited_[labelValue] = true;
}

std::vector<const Vertex *> Observer::SphereGrower::nextLayerNeighbors(const Vertex *vertex) {
    std::vector<const Vertex *> chosen;
    chosen.reserve(10);

    auto candidates = vertex->getNeighbors();
    for (auto candidate : candidates) {
        auto candidateVertex = candidate.getVertex();
        if (!isVisited(candidateVertex)) {
            chosen.emplace_back(candidateVertex);
            setVisited(candidateVertex);
        }
    }

    return chosen;
}

std::deque<const Vertex *> Observer::SphereGrower::nextLayer() {
    std::deque<const Vertex *> nextLayer;

    for (auto vertex : currentLayer_) {
        std::vector<const Vertex *> addition = nextLayerNeighbors(vertex);
        for (auto addition_vertex : addition) {
            nextLayer.push_back(addition_vertex);
        }
    }

    return nextLayer;
}

void Observer::SphereGrower::growToNextRadius() {
    currentLayer_ = nextLayer();
    shellSizes_.push_back(currentLayer_.size());
}

void Observer::SphereGrower::growToRadius(const unsigned int &radius) {
    // keep on growing until wanted radius, or all vertices are already included
    while (getRadius() < radius && !currentLayer_.empty()) {
        growToNextRadius();
    }
}

void Observer::SphereGrower::growToVertex(const Vertex *vertex) {
    bool found = false;
    //keep going until vertex found, or all vertices are already included
    while (!found && !currentLayer_.empty()) {
        if (std::find(currentLayer_.begin(), currentLayer_.end(), vertex) != currentLayer_.end()) {
            found = true;
        }
    }
}

void Observer::SphereGrower::growToEmptyShell() {
    while (!currentLayer_.empty()) {
        growToNextRadius();
    }
}

const Vertex *Observer::SphereGrower::getRandomFromShell() {
    if (currentLayer_.empty()) {
        throw std::runtime_error("In `getRandomFromShell` of SphereGrower: sphere has perimeter of 0");
    }

    std::uniform_int_distribution<unsigned int> temp_dist(0, currentLayer_.size() - 1);
    auto index = temp_dist(global_engine);
    return currentLayer_[index];
}

unsigned int Observer::SphereGrower::getRadius() const {
    return static_cast<unsigned int>(shellSizes_.size()-1);
}

unsigned int Observer::SphereGrower::getCircumference() const {
    return static_cast<unsigned int>(currentLayer_.size());
}

std::vector<const Vertex *> Observer::SphereGrower::currentLayerVector() const {
    std::vector<const Vertex *> currentLayerVector;
    currentLayerVector.reserve(currentLayer_.size());
    for (auto vertex : currentLayer_) {
        currentLayerVector.push_back(vertex);
    }
    return currentLayerVector;
}

Observer::DistanceMeasurer::DistanceMeasurer(const Vertex *startingVertex, Universe *universe,
                                             const std::vector<const Vertex *> &distantVertexObject) : SphereGrower(
        startingVertex, universe), distantVertexObject_(distantVertexObject) {

    distances_.reserve(distantVertexObject_.size());

    for (auto wantedVertex : distantVertexObject_) {
        wanted_[wantedVertex->getLabel().toInt()] = true;
        if (wantedVertex->getLabel() == startingVertex->getLabel()) {
//            std::cout << "overlap vertex gevonden" << "\n";
            distances_.push_back(0);
        }
    }
}

bool Observer::DistanceMeasurer::isWanted(const Vertex *vertex) const {
    return wanted_[vertex->getLabel().toInt()];
}

Observer::DistanceMeasurer::DistanceMeasurer(const Vertex *startingVertex, Universe *universe,
                                             const Observer::SphereGrower &otherSphere) :
        DistanceMeasurer(startingVertex, universe, otherSphere.currentLayerVector()) {}

std::vector<const Vertex *> Observer::DistanceMeasurer::nextLayerNeighborsWithDetection(const Vertex *vertex) {
    auto selectedNeighbors = nextLayerNeighbors(vertex);
    for (auto neighbor : selectedNeighbors) {
        if (isWanted(neighbor)) {
            distances_.push_back( static_cast<unsigned int>(getRadius() + 1) );
        }
    }
    return selectedNeighbors;
}

std::deque<const Vertex *> Observer::DistanceMeasurer::nextLayerWithDetection() {
    std::deque<const Vertex*> nextLayer;

    for (auto vertex : currentLayer_) {
        std::vector<const Vertex*> addition = nextLayerNeighborsWithDetection(vertex);
        for (auto addition_vertex : addition) {
            nextLayer.push_back(addition_vertex);
        }
    }

    return nextLayer;
}

std::vector<unsigned int> Observer::DistanceMeasurer::measureDistances() {
    while (distances_.size() < distantVertexObject_.size()) {
        growToNextRadiusWithDetection();
    }

    return distances_;
}

std::vector<unsigned int> Observer::DistanceMeasurer::measureDistances(const unsigned int &maxRadius) {
    while (distances_.size() < distantVertexObject_.size() && getRadius() < maxRadius) {
        growToNextRadiusWithDetection();
    }

    return distances_;
}

double Observer::DistanceMeasurer::measureAverageDistance() {
    measureDistances();
    auto const count = static_cast<double>(distances_.size());

    return static_cast<double>(std::reduce(distances_.begin(), distances_.end())) / count;
}

double Observer::DistanceMeasurer::measureAverageDistance(const unsigned int &maxRadius) {
    measureDistances(maxRadius);
    auto const count = static_cast<double>(distances_.size());

    return static_cast<double>(std::reduce(distances_.begin(), distances_.end())) / count;
}

void Observer::DistanceMeasurer::growToNextRadiusWithDetection() {
    currentLayer_ = nextLayerWithDetection();
    shellSizes_.push_back(currentLayer_.size());
}

void Observer::DistanceMeasurer::setUnwanted(const Vertex *vertex) {
    wanted_[vertex->getLabel().toInt()] = false;
}
