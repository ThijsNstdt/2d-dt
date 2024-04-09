#include "growthfactory.hpp"
#include "src/simplices/triangle.hpp"
#include "src/universe/universe.hpp"
#include "src/rng/global_engine.hpp"

#include <vector>
#include <array>
#include <ctime>
#include <stdexcept>
#include <random>




void GrowthFactory::createVertices() {
    while(myUniverse.numberOfVertices() < 4) {
        myUniverse.addVertex();
    }
}

void GrowthFactory::createEdges() {
    if (myUniverse.numberOfVertices() != 4) {
        throw std::invalid_argument("In `createEdges` of GrowthFactory: universe does not have proper number of vertices");
    }

    // set up edges by hand, this is admittedly a bit clumsy.
    myUniverse.addEdge(
            myUniverse.getVertex(VertexLabel(2)),
            myUniverse.getVertex(VertexLabel(3))
            );

    myUniverse.addEdge(
            myUniverse.getVertex(VertexLabel(1)),
            myUniverse.getVertex(VertexLabel(3))
    );

    myUniverse.addEdge(
            myUniverse.getVertex(VertexLabel(1)),
            myUniverse.getVertex(VertexLabel(2))
    );

    myUniverse.addEdge(
            myUniverse.getVertex(VertexLabel(0)),
            myUniverse.getVertex(VertexLabel(3))
    );

    myUniverse.addEdge(
            myUniverse.getVertex(VertexLabel(0)),
            myUniverse.getVertex(VertexLabel(2))
    );

    myUniverse.addEdge(
            myUniverse.getVertex(VertexLabel(0)),
            myUniverse.getVertex(VertexLabel(1))
    );
}

void GrowthFactory::createBlankTriangles() {
    while(myUniverse.numberOfTriangles() < 4) {
        myUniverse.addBlankTriangle();
    }
}

std::vector<Triangle*> GrowthFactory::tetrahedronNeighbor(TriangleLabel label) {
    if (myUniverse.numberOfTriangles() != 4) {
        throw std::invalid_argument("In `tetrahedronNeighbor` of GrowthFactory: universe does not have proper number of Triangles");
    }

    // vector with all existing triangles
    std::vector<TriangleLabel> neighborLabels = {TriangleLabel(0), TriangleLabel(1), TriangleLabel(2), TriangleLabel(3)};

    // erase the triangle we are considering from the label vector
    neighborLabels.erase(neighborLabels.begin() + label.toInt());

    // get pointers to triangles from their labels, and push them into vector to be returned
    std::vector<Triangle*> neighbors;
    for (TriangleLabel triangleLabel : neighborLabels) {
        neighbors.push_back(myUniverse.getTriangle(triangleLabel));
    }

    return neighbors;
}

void GrowthFactory::setAllTriangleNeighbors() {
    for (int i = 0; i < myUniverse.numberOfTriangles(); ++i) {
        Triangle *t = myUniverse.getTriangle(TriangleLabel(i));

        auto neighbors = tetrahedronNeighbor(t->getLabel());

        t->setNeighbors(neighbors[0], neighbors[1], neighbors[2]);
    }
}

void GrowthFactory::setTriangleEdges(TriangleLabel tLabel, EdgeLabel eLabel1, EdgeLabel eLabel2, EdgeLabel eLabel3) {
    auto t = myUniverse.getTriangle(tLabel);

    t->setEdges(
            myUniverse.getEdge(eLabel1),
            myUniverse.getEdge(eLabel2),
            myUniverse.getEdge(eLabel3)
            );
}

void GrowthFactory::setAllTriangleEdges() {
    setTriangleEdges(TriangleLabel(0), EdgeLabel(0), EdgeLabel(1), EdgeLabel(2));

    setTriangleEdges(TriangleLabel(1), EdgeLabel(0), EdgeLabel(3), EdgeLabel(4));

    setTriangleEdges(TriangleLabel(2), EdgeLabel(1), EdgeLabel(3), EdgeLabel(5));

    setTriangleEdges(TriangleLabel(3), EdgeLabel(2), EdgeLabel(4), EdgeLabel(5));
}

void GrowthFactory::createTriangles() {
    createBlankTriangles();
    setAllTriangleNeighbors();
    setAllTriangleEdges();
    setAllTriangleVertices();
}

void GrowthFactory::makeUnitTetrahedron() {
    createVertices();
    createEdges();
    createTriangles();

    setAllVertexNeighbors();
}

Universe && GrowthFactory::createUniverse(int size) {
    makeUnitTetrahedron();
    while (myUniverse.numberOfTriangles() < size) {
        insertionMove();
    }
//    std::cout << "triangle number check after growth result: " << (myUniverse.numberOfTriangles()==size) << "\n";
//    std::cout << "triangle data check after growth result: " << myUniverse.triangleDataCheck() << "\n";

    myUniverse.setDistributions();

    return std::move(myUniverse);
}

void GrowthFactory::insertionMove(TriangleLabel label) {

    //Identify relevant triangles, edges and vertices already present
    auto dTriangle = myUniverse.getTriangle(label);

    auto neighbors = dTriangle->getNeighbors();
    auto edges = dTriangle->getEdges();
    auto vertices = dTriangle->getVertices();

    auto aTriangle = neighbors[0];
    auto bTriangle = neighbors[1];
    auto cTriangle = neighbors[2];

    auto aEdge = edges[0];
    auto bEdge = edges[1];
    auto cEdge = edges[2];

    auto aVertex = vertices[0];
    auto bVertex = vertices[1];
    auto cVertex = vertices[2];

    // add vertex to universe
    auto dVertex = myUniverse.addVertex();

    // add new edges
    auto dEdge = myUniverse.addEdge(bVertex, dVertex);
    auto eEdge = myUniverse.addEdge(cVertex, dVertex);
    auto fEdge = myUniverse.addEdge(aVertex, dVertex);

    // add new triangles as blanks for now
    auto eTriangle = myUniverse.addBlankTriangle();
    auto fTriangle = myUniverse.addBlankTriangle();

    // update neighbors of surrounding triangles
    bTriangle->replaceNeighbor(dTriangle->getLabel(), eTriangle);
    cTriangle->replaceNeighbor(dTriangle->getLabel(), fTriangle);
    // aTriangle is unchanged

    // updates neighbors and edges of dTriangle
    dTriangle->replaceNeighbor(bTriangle->getLabel(), eTriangle);
    dTriangle->replaceEdge(bEdge->getLabel(), eEdge);

    dTriangle->replaceNeighbor(cTriangle->getLabel(), fTriangle);
    dTriangle->replaceEdge(cEdge->getLabel(), dEdge);

    dTriangle->replaceVertex(aVertex->getLabel(), dVertex);

    // fill in the blanks of E and F triangles
    eTriangle->setNeighbors(bTriangle, fTriangle, dTriangle);
    eTriangle->setEdges(bEdge, fEdge, eEdge);
    eTriangle->setVertices(dVertex, cVertex, aVertex);

    fTriangle->setNeighbors(cTriangle, dTriangle, eTriangle);
    fTriangle->setEdges(cEdge, dEdge, fEdge);
    fTriangle->setVertices(dVertex, aVertex, bVertex);

    aVertex->addNeighbor(dVertex, fEdge->getLabel());
    bVertex->addNeighbor(dVertex, dEdge->getLabel());
    cVertex->addNeighbor(dVertex, eEdge->getLabel());

    dVertex->addNeighbor(aVertex, fEdge->getLabel());
    dVertex->addNeighbor(bVertex, dEdge->getLabel());
    dVertex->addNeighbor(cVertex, eEdge->getLabel());
}

void GrowthFactory::insertionMove() {
    std::uniform_int_distribution<int> temp_triangle_distribution(0, myUniverse.numberOfTriangles() - 1);
    int labelValue = temp_triangle_distribution(global_engine);
    insertionMove(TriangleLabel(labelValue));
}

void GrowthFactory::setTriangleVertices(TriangleLabel tLabel, VertexLabel vLabel1, VertexLabel vLabel2,
                                        VertexLabel vLabel3) {
    auto t = myUniverse.getTriangle(tLabel);

    t->setVertices(
            myUniverse.getVertex(vLabel1),
            myUniverse.getVertex(vLabel2),
            myUniverse.getVertex(vLabel3)
    );
}

void GrowthFactory::setAllTriangleVertices() {
    setTriangleVertices(TriangleLabel(0), VertexLabel(1), VertexLabel(2), VertexLabel(3));

    setTriangleVertices(TriangleLabel(1), VertexLabel(0), VertexLabel(2), VertexLabel(3));

    setTriangleVertices(TriangleLabel(2), VertexLabel(0), VertexLabel(1), VertexLabel(3));

    setTriangleVertices(TriangleLabel(3), VertexLabel(0), VertexLabel(1), VertexLabel(2));
}

void GrowthFactory::setAllVertexNeighbors() {
    auto v0 = myUniverse.getVertex(VertexLabel(0));
    auto v1 = myUniverse.getVertex(VertexLabel(1));
    auto v2 = myUniverse.getVertex(VertexLabel(2));
    auto v3 = myUniverse.getVertex(VertexLabel(3));

    v0->addNeighbor(v1, EdgeLabel(5));
    v0->addNeighbor(v2, EdgeLabel(4));
    v0->addNeighbor(v3, EdgeLabel(3));

    v1->addNeighbor(v0, EdgeLabel(5));
    v1->addNeighbor(v2, EdgeLabel(2));
    v1->addNeighbor(v3, EdgeLabel(1));

    v2->addNeighbor(v0, EdgeLabel(4));
    v2->addNeighbor(v1, EdgeLabel(2));
    v2->addNeighbor(v3, EdgeLabel(0));

    v3->addNeighbor(v0, EdgeLabel(3));
    v3->addNeighbor(v1, EdgeLabel(1));
    v3->addNeighbor(v2, EdgeLabel(0));
}

