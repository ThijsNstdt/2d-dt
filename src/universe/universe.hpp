#ifndef _UNIVERSE_H
#define _UNIVERSE_H

#include <vector>
#include <tuple>
#include <memory>
#include <random>

#include "src/simplices/vertex.hpp"
#include "src/simplices/edge.hpp"
#include "src/simplices/triangle.hpp"

class Universe {
private:
    std::vector<std::unique_ptr<Triangle>> triangles;

    std::vector<std::unique_ptr<Edge>> edges;

    std::vector<std::unique_ptr<Vertex>> vertices;

    std::uniform_int_distribution<> uniformTriangleDistribution_;

    std::uniform_int_distribution<> uniformEdgeDistribution_;

    std::uniform_int_distribution<> uniformVertexDistribution_;

public:
    Universe() = default;

    Universe(Universe&& source) noexcept ;

    Universe& operator=(Universe&& other) noexcept;

    Triangle* addBlankTriangle();

    Triangle* addTriangle(Edge* e1, Edge* e2, Edge* e3, Triangle* t1, Triangle* t2, Triangle* t3);

    Triangle* getTriangle(TriangleLabel label);

    Edge* addEdge(Vertex* v1, Vertex* v2);

    Edge* getEdge(EdgeLabel label);

    Vertex* addVertex();

    Vertex* getVertex(VertexLabel label);

    int numberOfTriangles() const;

    int numberOfEdges() const;

    int numberOfVertices() const;

    bool triangleDataCheck();

    Triangle* getRandomTriangle();

    Vertex* getRandomVertex();

    void setDistributions();

    bool labelCheck();

    void writeVertexGraph(std::string &filename) const;
};

#endif