//
// Created by thijs on 25/05/2022.
//

#ifndef INC_2D_DT_SIMPLICIALUPDATER_HPP
#define INC_2D_DT_SIMPLICIALUPDATER_HPP

#include "updater.hpp"
#include "src/universe/universe.hpp"


class SimplicialUpdater : public Updater {
public:
    ~SimplicialUpdater() override;

    SimplicialUpdater(Universe* myUniverse);

    void edgeUpdate() override;

private:

    bool hasThreeNeck(Vertex* vCenter, Vertex* vOther, Edge* startingDirection, Triangle* startingTriangle);

    Edge* getDirection(Vertex* vCenter, Edge* currentDirection, Triangle* nextTriangle);
};


#endif //INC_2D_DT_SIMPLICIALUPDATER_HPP
