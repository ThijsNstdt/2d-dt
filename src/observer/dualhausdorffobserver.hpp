//
// Created by thijs on 15/06/2022.
//

#ifndef INC_2D_DT_DUALHAUSDORFFOBSERVER_HPP
#define INC_2D_DT_DUALHAUSDORFFOBSERVER_HPP

#include "observer.hpp"

class DualHausdorffObserver : public Observer {
public:
    ~DualHausdorffObserver() override;

    explicit DualHausdorffObserver(Universe *myUniverse);

    void measurement() override;

    void writeVector(const std::vector<int> &data) override;

    void setFileName(const std::string &addition) override;

    std::string getFileName() override;

    std::vector<int> sphereGrowth();

    void loadSettingsFromConfig(const std::string& configLoc) override;

private:
    std::vector<Triangle*> nextLayerNeighbors(const Triangle *triangle);

    std::vector<Triangle*> getNextLayer(const std::vector<Triangle*>& currentLayer);

    std::vector<bool> m_visited;

    std::string m_baseName = "./simulations/results/dualhausdorffobserver/distanceprofile";

    std::string m_fileName;

    void setVisited(const Triangle *triangle);

    bool isVisited(const Triangle *triangle);

    [[maybe_unused]] unsigned int maxRadius;
};


#endif //INC_2D_DT_DUALHAUSDORFFOBSERVER_HPP
