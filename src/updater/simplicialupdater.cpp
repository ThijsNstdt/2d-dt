//
// Created by thijs on 25/05/2022.
//

#include "simplicialupdater.hpp"


void SimplicialUpdater::edgeUpdate() {
    addUpdate(); //rejection is also an update

    neighborhoodIdentification();

    if (!degreeCheck(4)) {
        addRejection();
        return;
    }

    if ( hasThreeNeck(cVertex, dVertex, xEdge, aTriangle) ) {
        addRejection();
        return;
    }

    regularFlip();
}

bool SimplicialUpdater::hasThreeNeck(Vertex *vCenter, Vertex *vOther, Edge *startingDirection,
                                       Triangle *startingTriangle) {
    // look for three neck by looping around a center vertex, and seeing whether we encounter the vertex two edges away, and so completing a three-neck.

    int trianglesInCycle = 0;

    auto currentTriangle = startingTriangle;
    auto currentDirection = startingDirection;

    trianglesInCycle++;

    if (currentDirection->getOtherVertex(vCenter->getLabel())->getLabel() == vOther->getLabel()) {
        return true;
    }

    Triangle* nextTriangle = currentTriangle->neighborFromEdge(currentDirection->getLabel());
    Edge* nextDirection = getDirection(vCenter, currentDirection, nextTriangle);

    while( !(nextTriangle->getLabel() == startingTriangle->getLabel()) ) {
        currentTriangle = nextTriangle;
        currentDirection = nextDirection;

        trianglesInCycle++;

        if (currentDirection->getOtherVertex(vCenter->getLabel())->getLabel() == vOther->getLabel()) {
            return true;
        }

        nextTriangle = currentTriangle->neighborFromEdge(currentDirection->getLabel());
        nextDirection = getDirection(vCenter, currentDirection, nextTriangle);
    }

    // length of full cycle should be vertex degree by definition
    if (trianglesInCycle != vCenter->getDegree()) {
        throw std::invalid_argument("In `threeNeckCheck` of SimplicialUpdater: center vertex degree does not match with cycle length");
    }

    return false;
}

Edge *SimplicialUpdater::getDirection(Vertex *vCenter, Edge *currentDirection, Triangle *nextTriangle) {
    auto oppositeVertex = currentDirection->getOtherVertex(vCenter->getLabel());
    return nextTriangle->edgeOppositeTo(oppositeVertex->getLabel());
}

SimplicialUpdater::~SimplicialUpdater() {}

SimplicialUpdater::SimplicialUpdater(Universe *myUniverse) : Updater(myUniverse) {}


