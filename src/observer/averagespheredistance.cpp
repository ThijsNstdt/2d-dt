//// Created by thijs on 15/12/2022.//#include <fstream>#include <chrono>#include <vector>#include <stdexcept>#include <json/json.h>#include <iomanip>#include <numeric>#include "averagespheredistance.hpp"#include "src/jsonutilities/jsonutilities.hpp"AverageSphereDistance::~AverageSphereDistance() = default;AverageSphereDistance::AverageSphereDistance(Universe *myUniverse) : Observer(myUniverse) {}void AverageSphereDistance::measurement() {    const auto *center = static_cast<const Vertex*>(myUniverse->getRandomVertex());    SphereGrower firstSphere(center, myUniverse);    std::vector<double> averageSphereDistances;    averageSphereDistances.reserve(maxRadius_-minRadius_);    std::vector<unsigned int> firstShellSizes;    firstShellSizes.reserve(maxRadius_-minRadius_);    std::vector<unsigned int> secondShellSizes;    secondShellSizes.reserve(maxRadius_-minRadius_);    for (unsigned int radius = minRadius_; radius < maxRadius_; ++radius) {        firstSphere.growToRadius(radius);        auto secondSphere = setupSecondSphere(firstSphere, myUniverse, radius);        auto asd = singleRadiusMeasurement(firstSphere, secondSphere, myUniverse);        averageSphereDistances.push_back(asd);        firstShellSizes.push_back(static_cast<unsigned int>(firstSphere.currentLayer_.size()));        secondShellSizes.push_back(static_cast<unsigned int>(secondSphere.currentLayer_.size()));    }    writeNewMeasurement(averageSphereDistances, firstShellSizes, secondShellSizes);}void AverageSphereDistance::writeVector(const std::vector<int> &data) {    std::ofstream outFile(fileName_, std::ofstream::app);    if(!outFile.is_open()) {        throw std::runtime_error("in `writeVector` of AverageSphereDistance: Problem with opening file");    }    for (int datum : data) {        outFile << datum << ", ";    }    outFile << "\n";    outFile.close();}void AverageSphereDistance::writeVector(const std::vector<unsigned int> &data) {    std::ofstream outFile(fileName_, std::ofstream::app);    if(!outFile.is_open()) {        throw std::runtime_error("in `writeVector` of AverageSphereDistance: Problem with opening file");    }    for (unsigned int datum : data) {        outFile << datum << ", ";    }    outFile << "\n";    outFile.close();}void AverageSphereDistance::setFileName(const std::string &addition) {    auto now = std::chrono::system_clock::now();    auto in_time_t = std::chrono::system_clock::to_time_t(now);    std::stringstream datetime;    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");    std::string filename = baseName_ + "_" + datetime.str() + "_" + addition + ".json";    fileName_ = filename;}std::string AverageSphereDistance::getFileName() {    return fileName_;}void AverageSphereDistance::loadSettingsFromConfig(const std::string &configLoc) {    Json::Value root;    std::ifstream ifs;    ifs.open(configLoc);    Json::CharReaderBuilder builder;    builder["collectComments"] = false;    Json::String errs;    if (!parseFromStream(builder, ifs, &root, &errs)) {        std::cout << errs << std::endl;        throw std::runtime_error("In `loadSettingsFromConfig` of AverageSphereDistance: problem parsing config");    }    if (!root.isMember("observables")) {        throw std::runtime_error("In `loadSettingsFromConfig` of AverageSphereDistance: no observable settings in config");    }    Json::Value observableRoot = root["observables"];    if (!observableRoot.isMember("asd")) {        throw std::runtime_error("In `loadSettingsFromConfig` of AverageSphereDistance: no ASD settings in config");    }    Json::Value asdRoot = observableRoot["asd"];    if (asdRoot.isMember("min_radius")) {        minRadius_ = asdRoot["min_radius"].asUInt();    }    if (asdRoot.isMember("max_radius")) {        maxRadius_ = asdRoot["max_radius"].asUInt();    }    makeOutputFile();}void AverageSphereDistance::writeVector(const std::vector<double> &data) const {    std::ofstream outFile(fileName_, std::ofstream::app);    if(!outFile.is_open()) {        throw std::runtime_error("in `writeVector` of AverageSphereDistance: Problem with opening file");    }    for (double datum : data) {        outFile << datum << ", ";    }    outFile << "\n";    outFile.close();}Observer::SphereGrower AverageSphereDistance::setupSecondSphere(SphereGrower &firstSphere, Universe *myUniverse, const unsigned int &radius) {    const Vertex *otherCenter = firstSphere.getRandomFromShell();    SphereGrower secondSphere(otherCenter, myUniverse);    secondSphere.growToRadius(radius);    return secondSphere;}double AverageSphereDistance::singleRadiusMeasurement(SphereGrower &firstSphere, SphereGrower &secondSphere,                                                      Universe *myUniverse) {    std::vector<double> distances;    distances.reserve(secondSphere.currentLayer_.size());    for (const Vertex *vertexOnShell : secondSphere.currentLayer_) {        DistanceMeasurer distancer(vertexOnShell, myUniverse, firstSphere);        // by triangle inequality, this is the maximum radius at which vertices of firstsphere can be found        unsigned int maxSize = 3*firstSphere.getRadius();        double averageDistance = distancer.measureAverageDistance(maxSize);        distances.push_back(averageDistance);    }    auto const count = static_cast<double>(distances.size());    return (static_cast<double>(std::reduce(distances.begin(), distances.end())) / count);}void AverageSphereDistance::writeNewMeasurement(const std::vector<double> &asdData,                                                const std::vector<unsigned int> &firstSphereSizes,                                                const std::vector<unsigned int> &secondSphereSizes) {    Json::Value root;    std::fstream fs;    fs.open(fileName_);    Json::CharReaderBuilder builder;    builder["collectComments"] = false;    Json::String errs;    if (!parseFromStream(builder, fs, &root, &errs)) {        std::cout << errs << std::endl;        throw std::runtime_error("In `writeNewMeasurement` of AverageSphereDistance: problem parsing config");    }    fs.close();    Json::Value measurement;    // convert vectors to json arrays and add to measurement json value    measurement["asd"] = jsonUtilities::iterable2json(asdData);    measurement["shell1"] = jsonUtilities::iterable2json(firstSphereSizes);    measurement["shell2"] = jsonUtilities::iterable2json(secondSphereSizes);    root["measurements"].append(measurement);    fs.open(fileName_, std::ios::out);    fs << root.toStyledString() << std::endl;    fs.close();}void AverageSphereDistance::makeOutputFile() {    // make json file    Json::StreamWriterBuilder builder;    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());    Json::Value root;    root["measurements"] = Json::arrayValue;    root["min_max_radius"][0] = minRadius_;    root["min_max_radius"][1] = maxRadius_;    std::ofstream outFile(fileName_, std::ofstream::app);    if(!outFile.is_open()) {        throw std::runtime_error("in `makeOutputFile` of AverageSphereDistance: Problem with opening file");    }    writer->write(root, &outFile);    outFile.close();}