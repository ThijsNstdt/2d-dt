#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <array>
#include <vector>
#include <tuple>
#include <random>
#include "edge.hpp"
#include "vertex.hpp"
#include "src/label/trianglelabel.hpp"


class Triangle {
private:
    const TriangleLabel label;

    //pointers to its vertices, opposite to edges by order
    std::array<Vertex*, 3> vertices;

    // pointers to its edges, in same order as neighbors
    std::array<Edge*, 3> edges;

    // pointers to its neighbors, in same order as edges
    std::array<Triangle*, 3> neighbors;

    bool tadpoleStatus = false;

    bool tadpoleVertexEdgeCheck();

    bool selfEnergyStatus = false;

    std::uniform_int_distribution<int> randomBelowThree_{0, 2};

public:
    Triangle(const int& labelValue, Edge* e1, Edge* e2, Edge* e3, Triangle* t1, Triangle* t2, Triangle* t3);

    explicit Triangle(const int& labelValue);

    void setNeighbors(Triangle* t1, Triangle* t2, Triangle* t3);

    [[nodiscard]] std::array<Triangle*, 3> getNeighbors() const;

    void setVertices(Vertex* v1, Vertex* v2, Vertex* v3);

    void setEdges(Edge* e1, Edge* e2, Edge* e3);

    std::array<Edge*, 3> getEdges();

    TriangleLabel getLabel() const;

    Vertex* vertexOppositeTo(EdgeLabel eLabel);

    std::array<Vertex*, 3> getVertices();

    void replaceNeighbor(TriangleLabel oldNeighborLabel, Triangle *newNeighbor);
    void replaceNeighbor(Triangle *oldNeighbor, Triangle *newNeighbor);

    void replaceEdge(EdgeLabel oldEdgeLabel, Edge *newEdge);
    void replaceEdge(Edge *oldEdge, Edge *newEdge);

    void replaceVertex(VertexLabel oldVertexLabel, Vertex *newVertex);
    void replaceVertex(Vertex *oldVertex, Vertex *newVertex);

    bool checkVertexEdgeOrder();

    bool checkEdgeNeighborOrder();

    std::tuple<Edge*, Triangle*> getRandomEdgeNeighborPair();

    Edge* edgeOppositeTo(VertexLabel vLabel);

    Triangle* neighborFromEdge(EdgeLabel eLabel);

    void replaceEdgeByNeighbor(Edge *newEdge, TriangleLabel neighborInQuestion);

    void replaceVertexByEdge(Vertex *newVertex, EdgeLabel edgeInQuestion);

    void replaceNeighborByEdge(Triangle* newTriangle, EdgeLabel edgeInQuestion);

    void replaceTripletByEdge(Vertex *newVertex, Edge *newEdge, Triangle *newTriangle, EdgeLabel edgeInQuestion);

    [[nodiscard]] bool isTadpole() const;

    void toggleTadpole();

    [[nodiscard]] bool isPartOfSelfEnergy() const;

    void toggleSelfEnergy();

    bool isSelfNeighborThroughEdge(EdgeLabel edge);

    bool hasNeighborTwice(TriangleLabel neighborLabel);

    std::tuple<Edge*, Edge*> getDegenerateEdges();

    bool hasVertexTwice(VertexLabel vertexLabel);

    Edge* otherEdgeContaining(VertexLabel vertexLabel, EdgeLabel edgeLabel);

    bool hasOneNeck();

    bool hasOneNeckAround(VertexLabel vertexLabel);

    Edge* getEdgeExcluding(const std::vector<Edge*>& excludedEdges);

    void print(const std::string& symbolicName);

    bool hasNeighbor(TriangleLabel candidateLabel);

    bool hasVertex(VertexLabel candidateVertex);

    std::tuple<Edge*, Edge*> getDegenerateEdgesExcluding(Edge* excludedEdge);

//    explicit Triangle(int label, std::array<int, 3> nbrLabels = {0, 0, 0}, std::array<int, 3> vertexLabels = {0, 0, 0});
//
//    // retrieve the label of a random neighbor
//    int getRandomNbr();
//
//    void printTriangle();
//
//    void setLabel(int label);
//
//    void setNbr(std::array<int, 3> nbrLabels);
//
//    void setVertex(std::array<int, 3> vertexLabels);
//
//    bool isNbr(int label);
//    bool isDoubleNbr(int label);
//    bool isVertex(int label);
//
//    std::array<int, 3> getNeighbors();
//    std::array<int, 3> getVertices();
//    int getLabel();
//
//    void replaceNeighbor(int oldNeighbor, int newNeighbor);
//    void replaceVertex(int oldVertex, int newVertex);
//
//    int vertexOppositeTo(Edge edge);
//
//    Edge edgeOppositeTo(int vertexLabel);
//
//    bool hasEdge(Edge edge);
};

#endif