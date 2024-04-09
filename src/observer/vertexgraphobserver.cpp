//
// Created by Thijs on 17-4-2023.
//

#include <chrono>
#include <iomanip>

#include "vertexgraphobserver.hpp"

VertexGraphObserver::VertexGraphObserver(Universe *myUniverse) : Observer(myUniverse) {}

VertexGraphObserver::~VertexGraphObserver() = default;

void VertexGraphObserver::measurement() {
    setNumberedFileName();
    myUniverse->writeVertexGraph(numberedFileName_);
    measurementNumber_++;
}

void VertexGraphObserver::setNumberedFileName() {
    numberedFileName_ = unnumberedFileName_ + std::to_string(measurementNumber_) + ".csv";
}

void VertexGraphObserver::setFileName(const std::string &addition) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");

    std::string filename = baseName_ + "_" + datetime.str() + "_" + addition;

    unnumberedFileName_ = filename;
}

std::string VertexGraphObserver::getFileName() {
    return unnumberedFileName_;
}
