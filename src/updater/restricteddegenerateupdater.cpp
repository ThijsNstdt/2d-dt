//
// Created by Thijs on 6-7-2022.
//

#include "restricteddegenerateupdater.hpp"

void RestrictedDegenerateUpdater::edgeUpdate() {
    addUpdate(); //rejection is also an update

    neighborhoodIdentification();

    if (!degreeCheck(3)) {
        addRejection();
        return;
    }

    if ( hasTwoNeck(cVertex, dVertex) ) {
        addRejection();
        return;
    }

    regularFlip();
}

bool RestrictedDegenerateUpdater::hasTwoNeck(Vertex *vCenter, Vertex *vOther) {
    return vCenter->getLabel() == vOther->getLabel();
}

RestrictedDegenerateUpdater::~RestrictedDegenerateUpdater() {}

RestrictedDegenerateUpdater::RestrictedDegenerateUpdater(Universe *myUniverse) : Updater(myUniverse) {}
