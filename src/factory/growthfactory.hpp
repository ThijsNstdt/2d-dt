#ifndef _GROWTHFACTORY_H
#define _GROWTHFACTORY_H

#include <array>

#include "factory.hpp"
#include "src/universe/universe.hpp"
#include "src/simplices/triangle.hpp"


class GrowthFactory : public Factory {
public:
    Universe && createUniverse(int size) override;

    ~GrowthFactory() override = default;

private:
    Universe myUniverse;

    void makeUnitTetrahedron();

    void createVertices();

    void createEdges();

    void createTriangles();

    void createBlankTriangles();

    std::vector<Triangle*> tetrahedronNeighbor(TriangleLabel label);

    void setAllTriangleNeighbors();

    void setTriangleEdges(TriangleLabel tLabel, EdgeLabel eLabel1, EdgeLabel eLabel2, EdgeLabel eLabel3);

    void setTriangleVertices(TriangleLabel tLabel, VertexLabel vLabel1, VertexLabel vLabel2, VertexLabel vLabel3);

    void setAllTriangleEdges();

    void setAllTriangleVertices();

    void insertionMove(TriangleLabel label);

    void insertionMove();

    void setAllVertexNeighbors();

};

#endif