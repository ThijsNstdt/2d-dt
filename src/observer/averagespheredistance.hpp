//// Created by thijs on 15/12/2022.//#ifndef INC_2D_DT_AVERAGESPHEREDISTANCE_HPP#define INC_2D_DT_AVERAGESPHEREDISTANCE_HPP#include <json/json.h>#include "observer.hpp"class AverageSphereDistance : public Observer {public:    ~AverageSphereDistance() override;    explicit AverageSphereDistance(Universe *myUniverse);    void measurement() override;    void writeVector(const std::vector<int> &data) override;    void writeVector(const std::vector<unsigned int> &data) override;    void setFileName(const std::string &addition) override;    std::string getFileName() override;    void loadSettingsFromConfig(const std::string &configLoc) override;private:    std::string baseName_ = "./simulations/results/new_asd/ASD";    std::string fileName_;    unsigned int minRadius_;    unsigned int maxRadius_;    void writeVector(const std::vector<double> &data) const;    static SphereGrower setupSecondSphere(SphereGrower &firstSphere, Universe *myUniverse, const unsigned int &radius) ;    static double singleRadiusMeasurement(SphereGrower &firstSphere, SphereGrower &secondSphere, Universe *myUniverse);    void writeNewMeasurement(const std::vector<double> &asdData, const std::vector<unsigned int> &firstSphereSizes, const std::vector<unsigned int> &secondSphereSizes);    void makeOutputFile();};#endif //INC_2D_DT_AVERAGESPHEREDISTANCE_HPP