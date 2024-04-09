//
// Created by Thijs on 26-5-2022.
//
#include <vector>
#include <stdexcept>
#include <iostream>

#include "updater.hpp"
#include "src/universe/universe.hpp"
#include "src/simplices/triangle.hpp"


Updater::~Updater() {}

Updater::Updater(Universe *myUniverse) : myUniverse(myUniverse) {}

void Updater::sweepUpdate(int numberOfSweeps) {
    int numberOfUpdates = numberOfSweeps*myUniverse->numberOfTriangles();
    seriesOfUpdates(numberOfUpdates);
}

void Updater::seriesOfUpdates(int numberOfUpdates) {
    for (int i = 0; i < numberOfUpdates; ++i) {
        edgeUpdate();
    }
}

void Updater::neighborhoodIdentification() {
    // identify central triangles and edge
    aTriangle = myUniverse->getRandomTriangle();

    auto myEdgeTriangleTuple = aTriangle->getRandomEdgeNeighborPair();
    zEdge = std::get<0>(myEdgeTriangleTuple);
    bTriangle = std::get<1>(myEdgeTriangleTuple);

    // identify central vertices
    auto myVerticesTuple = zEdge->getVertices();
    aVertex = std::get<0>(myVerticesTuple);
    bVertex = std::get<1>(myVerticesTuple);

    // identify the other important edges
    yEdge = aTriangle->edgeOppositeTo(bVertex->getLabel());
    xEdge = aTriangle->edgeOppositeTo(aVertex->getLabel());
    wEdge = bTriangle->edgeOppositeTo(aVertex->getLabel());
    vEdge = bTriangle->edgeOppositeTo(bVertex->getLabel());

    // identify the other important triangles
    cTriangle = aTriangle->neighborFromEdge(yEdge->getLabel());
    dTriangle = bTriangle->neighborFromEdge(vEdge->getLabel());
    eTriangle = aTriangle->neighborFromEdge(xEdge->getLabel());
    fTriangle = bTriangle->neighborFromEdge(wEdge->getLabel());

    // identify other important vertices
    cVertex = aTriangle->vertexOppositeTo(zEdge->getLabel());
    dVertex = bTriangle->vertexOppositeTo(zEdge->getLabel());
}

bool Updater::degreeCheck(int lowestPermissibleDegree) {
    if(aVertex->getDegree() < lowestPermissibleDegree) {
        return false;
    }

    if(bVertex->getDegree() < lowestPermissibleDegree) {
        return false;
    }

    return true;
}

void Updater::regularFlip() {
    // when all checks are passed, update relevant triangles, edges and vertices in that order
    dTriangle->replaceNeighborByEdge(aTriangle, vEdge->getLabel());
    eTriangle->replaceNeighborByEdge(bTriangle, xEdge->getLabel());

    aTriangle->replaceTripletByEdge(cVertex, vEdge, dTriangle, xEdge->getLabel());
    aTriangle->replaceVertexByEdge(aVertex, zEdge->getLabel());
    aTriangle->replaceVertexByEdge(dVertex, yEdge->getLabel());

    bTriangle->replaceTripletByEdge(dVertex, xEdge, eTriangle, vEdge->getLabel());
    bTriangle->replaceVertexByEdge(bVertex, zEdge->getLabel());
    bTriangle->replaceVertexByEdge(cVertex, wEdge->getLabel());

    zEdge->setVertices(cVertex, dVertex);

    aVertex->removeNeighbor(bVertex, zEdge->getLabel());
    bVertex->removeNeighbor(aVertex, zEdge->getLabel());

    cVertex->addNeighbor(dVertex, zEdge->getLabel());
    dVertex->addNeighbor(cVertex, zEdge->getLabel());
}

void Updater::edgeUpdate() {
    neighborhoodIdentification();
    regularFlip();
}

void Updater::printCurrentIdentification() {
    if (aTriangle != nullptr) {
        aTriangle->print("aTriangle");
    }

    if (bTriangle != nullptr) {
        bTriangle->print("bTriangle");
    }

    if (cTriangle != nullptr) {
        cTriangle->print("cTriangle");
    }

    if (dTriangle != nullptr) {
        dTriangle->print("dTriangle");
    }

    if (eTriangle != nullptr) {
        eTriangle->print("eTriangle");
    }

    if (fTriangle != nullptr) {
        fTriangle->print("fTriangle");
    }

    if (vEdge != nullptr) {
        vEdge->print("vEdge");
    }

    if (wEdge != nullptr) {
        wEdge->print("wEdge");
    }

    if (xEdge != nullptr) {
        xEdge->print("xEdge");
    }

    if (yEdge != nullptr) {
        yEdge->print("yEdge");
    }

    if (zEdge != nullptr) {
        zEdge->print("zEdge");
    }

    if (aVertex != nullptr) {
        aVertex->print("aVertex");
    }

    if (bVertex != nullptr) {
        bVertex->print("bVertex");
    }
    if (cVertex != nullptr) {
        cVertex->print("cVertex");
    }
    if (dVertex != nullptr) {
        dVertex->print("dVertex");
    }

    std::cout.flush();
}

void Updater::printIdentificationOfFlip(int updateNr) {
    if (m_totalUpdates == updateNr) {
        printCurrentIdentification();
    }
}

Updater::Ensemble Updater::strToEnsemble(const std::string &str) {
    if (str == "simplicial") {
        return Simplicial;
    }
    if (str == "restricted_degenerate") {
        return RestrictedDegenerate;
    }
    if (str == "maximally_degenerate") {
        return MaximallyDegenerate;
    }
    else {
        throw std::runtime_error("In `strToEnsemble` of Updater: input " + str +  " is not supported");
    }
}

std::string Updater::ensembleToStr(const Updater::Ensemble &ens) {
    if (ens == Simplicial) {
        return "simplicial";
    }
    if (ens == RestrictedDegenerate) {
        return "restricted_degenerate";
    }
    if (ens == MaximallyDegenerate) {
        return "maximally_degenerate";
    }
    else {
        throw std::runtime_error("In `ensembleToStr` of Updater: input ensemble is not supported");
    }
}


