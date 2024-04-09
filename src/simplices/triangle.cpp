//#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "triangle.hpp"
#include "src/rng/global_engine.hpp"

Triangle::Triangle(const int& labelValue) : label(labelValue), edges{nullptr, nullptr, nullptr}, neighbors{nullptr, nullptr, nullptr}, vertices{nullptr, nullptr, nullptr} {}

// TODO:: properly initialize vertices
Triangle::Triangle(const int &labelValue, Edge *e1, Edge *e2, Edge *e3, Triangle *t1, Triangle *t2, Triangle *t3) : label(labelValue), edges{e1,e2,e3},
                                                                                                                    neighbors{t1,t2,t3}, vertices{nullptr, nullptr, nullptr} {}


void Triangle::setNeighbors(Triangle *t1, Triangle *t2, Triangle *t3) {
    neighbors = {t1, t2, t3};
}

void Triangle::setEdges(Edge *e1, Edge *e2, Edge *e3) {
    edges = {e1, e2, e3};
}

TriangleLabel Triangle::getLabel() const {
    return label;
}

std::array<Triangle *, 3> Triangle::getNeighbors() const {
    return neighbors;
}

std::array<Edge *, 3> Triangle::getEdges() {
    return edges;
}

void Triangle::setVertices(Vertex *v1, Vertex *v2, Vertex *v3) {
    vertices = {v1, v2, v3};
}

Vertex *Triangle::vertexOppositeTo(EdgeLabel eLabel) {
    for (int i = 0; i < edges.size(); ++i) {
        if (edges[i]->getLabel() == eLabel) {
            return vertices[i];
        }
    }
    throw std::invalid_argument("In `vertexOppositeTo` of Triangle: given edge is not in triangle");
}

std::array<Vertex *, 3> Triangle::getVertices() {
    return vertices;
}

void Triangle::replaceNeighbor(TriangleLabel oldNeighborLabel, Triangle *newNeighbor) {
    auto condition = [&oldNeighborLabel](Triangle *myTriangle) {
        return myTriangle->getLabel() == oldNeighborLabel;
    };

    std::replace_if(neighbors.begin(), neighbors.end(), condition, newNeighbor);
}

void Triangle::replaceNeighbor(Triangle *oldNeighbor, Triangle *newNeighbor) {
    replaceNeighbor(oldNeighbor->getLabel(), newNeighbor);
}

void Triangle::replaceEdge(EdgeLabel oldEdgeLabel, Edge *newEdge) {
    auto condition = [&oldEdgeLabel](Edge *myEdge) {
        return myEdge->getLabel() == oldEdgeLabel;
    };

    std::replace_if(edges.begin(), edges.end(), condition, newEdge);
}

void Triangle::replaceEdge(Edge *oldEdge, Edge *newEdge) {
    replaceEdge(oldEdge->getLabel(), newEdge);
}

void Triangle::replaceVertex(VertexLabel oldVertexLabel, Vertex *newVertex) {
    auto condition = [&oldVertexLabel](Vertex *myVertex) {
        return myVertex->getLabel() == oldVertexLabel;
    };

    std::replace_if(vertices.begin(), vertices.end(), condition, newVertex);
}

void Triangle::replaceVertex(Vertex *oldVertex, Vertex *newVertex) {
    replaceVertex(oldVertex->getLabel(), newVertex);
}


// Note this will likely fail in maximally degenerate ensemble <- brilliant observation of past me
bool Triangle::checkVertexEdgeOrder() {

    if(isTadpole()) {
        return tadpoleVertexEdgeCheck();
    }

    for (int i = 0; i < 3; ++i) {
        auto myEdge = edges[i];
        auto myVertexLabel = vertices[i]->getLabel();

        // tadpole case has already been eliminated, opposite edge can not contain this vertex
        if(myEdge->hasVertex(myVertexLabel) && !hasOneNeckAround(myVertexLabel)) {
            return false;
        }

        auto edgeVertices = myEdge->getVertices();
        if (!hasVertex(std::get<0>(edgeVertices)->getLabel())){
            return false;
        }

        if (!hasVertex(std::get<1>(edgeVertices)->getLabel())){
            return false;
        }
    }


    return true;
}

/**
 * check if neighbors have current triangle as its neighbor, through their shared edge.
 * @return bool indicating whether above described connectivity is fulfilled.
 */
bool Triangle::checkEdgeNeighborOrder() {
    Edge *myEdge;
    Triangle *neighborTriangle;

    for (int i = 0; i < edges.size(); ++i) {
        myEdge = edges[i];
        neighborTriangle = neighbors[i];

        if ( !(neighborTriangle->neighborFromEdge(myEdge->getLabel())->getLabel() == getLabel()) ) {
            return false;
        }
    }

    return true;
}

std::tuple<Edge *, Triangle *> Triangle::getRandomEdgeNeighborPair() {
    int i = randomBelowThree_(global_engine);

    return {edges[i], neighbors[i]};
}

Edge *Triangle::edgeOppositeTo(VertexLabel vLabel) {
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i]->getLabel() == vLabel) {
            return edges[i];
        }
    }
    throw std::invalid_argument("In `EdgeOppositeTo` of Triangle: given vertex is not in triangle");
}

Triangle *Triangle::neighborFromEdge(EdgeLabel eLabel) {
    for (int i = 0; i < edges.size(); ++i) {
        if (edges[i]->getLabel() == eLabel) {
            return neighbors[i];
        }
    }

    throw std::invalid_argument("In `neighborFromEdge` of Triangle: given edge is not in triangle");
}

void Triangle::replaceEdgeByNeighbor(Edge *newEdge, TriangleLabel neighborInQuestion) {
    for (int i = 0; i < neighbors.size(); ++i) {
        if (neighbors[i]->getLabel() == neighborInQuestion) {
            edges[i] = newEdge;
            return;
        }
    }
    throw std::invalid_argument("In `replaceEdgeByNeighbor` of Triangle: given neighbor is not a neighbor of triangle");
}

void Triangle::replaceVertexByEdge(Vertex *newVertex, EdgeLabel edgeInQuestion) {
    for (int i = 0; i < edges.size(); ++i) {
        if (edges[i]->getLabel() == edgeInQuestion) {
            vertices[i] = newVertex;
        }
    }
    return;
    throw std::invalid_argument("In `replaceVertexByEdge` of Triangle: given edge is not an edge of triangle");
}

void Triangle::replaceNeighborByEdge(Triangle *newTriangle, EdgeLabel edgeInQuestion) {
    for (int i = 0; i < edges.size(); ++i) {
        if (edges[i]->getLabel() == edgeInQuestion) {
            neighbors[i] = newTriangle;
            return;
        }
    }
    throw std::invalid_argument("In `replaceNeighborByEdge` of Triangle: given edge is not an edge of triangle");
}

void Triangle::replaceTripletByEdge(Vertex *newVertex, Edge *newEdge, Triangle *newTriangle, EdgeLabel edgeInQuestion) {
    for (int i = 0; i < edges.size(); ++i) {
        if (edges[i]->getLabel() == edgeInQuestion) {
            vertices[i] = newVertex;
            edges[i] = newEdge;
            neighbors[i] = newTriangle;
            return;
        }
    }
    throw std::invalid_argument("In `replaceTripletByEdge` of Triangle: given edge is not an edge of triangle");
}

bool Triangle::isTadpole() const {
    return tadpoleStatus;
}

void Triangle::toggleTadpole() {
    tadpoleStatus ^= true;
}

bool Triangle::tadpoleVertexEdgeCheck() {
    // Look for which triplets are equivalent, and which one is unique
    std::vector<int> indicesOfDuplicates;
    int indexOfUnique;
    for (int i = 0; i < neighbors.size(); ++i) {
        if (neighbors[i]->getLabel() == getLabel()) {
            indicesOfDuplicates.push_back(i);
        } else {
            indexOfUnique = i;
        }
    }

    // for duplicate triplets check whether edges are equal
    if ( !(edges[indicesOfDuplicates[0]]->getLabel() == edges[indicesOfDuplicates[1]]->getLabel()) ) {
        return false;
    }

    // for duplicate triplets check whether vertices are equal
    if ( !(vertices[indicesOfDuplicates[0]]->getLabel() == vertices[indicesOfDuplicates[1]]->getLabel()) ) {
        return false;
    }

    // for duplicate edges check whether they contain the two actually existing vertices
    for (auto & i : indicesOfDuplicates) {
        if ( !(edges[i]->hasVertex(vertices[i]->getLabel()) && edges[i]->hasVertex(vertices[indexOfUnique]->getLabel())) ) {
            return false;
        }
    }

    // for unique triplet do the usual check
    auto myEdge = edges[indexOfUnique];
    auto myVertexLabel = vertices[indexOfUnique]->getLabel();
    if(myEdge->hasVertex(myVertexLabel)) {
        return false;
    }

    return true;
}

bool Triangle::isPartOfSelfEnergy() const {
    //TODO: fix RD en MD updater such that self energy status is correctly updated, this will make this method easier

    // Look for which neighbors are equivalent, and which one is unique, of none are equivalent then return false
    std::vector<int> indicesOfDuplicates;
    int indexOfUnique;
    if (neighbors[0]->getLabel() == neighbors[1]->getLabel()) {
        indicesOfDuplicates = {0,1};
        indexOfUnique = 2;
    } else if (neighbors[0]->getLabel() == neighbors[2]->getLabel()) {
        indicesOfDuplicates = {0,2};
        indexOfUnique = 1;
    } else if (neighbors[1]->getLabel() == neighbors[2]->getLabel()) {
        indicesOfDuplicates = {1,2};
        indexOfUnique = 0;
    } else {
        return false;
    }

    // check whether the duplicates are trough different edges, otherwise there is either an error or the triangle is tadpole
    if ( edges[indicesOfDuplicates[0]]->getLabel() == edges[indicesOfDuplicates[1]]-> getLabel() ) {
        if ( isTadpole() ) {
            return false;
        } else {
            throw std::runtime_error("In `isPartOfSelfEnergy` of Triangle: duplicate edge-triangle pairs, but not tadpole");
        }
    }

    return true;
}

void Triangle::toggleSelfEnergy() {
    selfEnergyStatus ^= true;
}

bool Triangle::isSelfNeighborThroughEdge(EdgeLabel edge) {
    if ( getLabel() == neighborFromEdge(edge)->getLabel() ) {
        if ( isTadpole() ) {
            return true;
        } else {
            throw std::runtime_error("in `isSelfNeighborThroughEdge` of Triangle: triangle is selfneighbor but not tadpole");
        }
    }

    return false;
}

bool Triangle::hasNeighborTwice(TriangleLabel neighborLabel) {
    int count = 0;

    for (auto nbr : neighbors) {
        if (nbr->getLabel() == neighborLabel) {
            count++;
        }
    }

    if (count == 2) {
        return true;
    }

    return false;
}

std::tuple<Edge *, Edge *> Triangle::getDegenerateEdges() {
//    if (!isPartOfSelfEnergy()) {
//        throw std::runtime_error("in `getDegenerateEdges` of Triangle: triangle is not part of self energy");
//    }

    if (edges[0]->compareVertices(edges[1])) {
        return {edges[0], edges[1]};
    } else if (edges[0]->compareVertices(edges[2])) {
        return {edges[0], edges[2]};
    } else if (edges[1]->compareVertices(edges[2])) {
        return {edges[1], edges[2]};
    } else {
        throw std::runtime_error("in `getDegenerateEdges` of Triangle: no edges have same pair of vertices");
    }
}

bool Triangle::hasVertexTwice(VertexLabel vertexLabel) {
    int count = 0;

    for (auto vertex : vertices) {
        if (vertex->getLabel() == vertexLabel) {
            count++;
        }
    }

    if (count == 2) {
        return true;
    }

    return false;
}

Edge *Triangle::otherEdgeContaining(VertexLabel vertexLabel, EdgeLabel edgeLabel) {
     if (hasVertexTwice(vertexLabel)) {
         throw std::runtime_error("in `otherEdgeContaining` of Triangle: method does not work for twice occurring vertices");
     }

     for (auto edge : edges) {
         // return edge if it's not our starting one, but does contain the looked for vertex
         if (!(edge->getLabel() == edgeLabel) && edge->hasVertex(vertexLabel)) {
             return edge;
         }
     }

     throw std::runtime_error("in `otherEdgeContaining` of Triangle: vertex only appears in one edge (which shouldn't be possible anyway?)");
}

bool Triangle::hasOneNeck() {
    for (auto e : edges) {
        if (e->isOneNeck()) {
            return true;
        }
    }

    return false;
}

Edge *Triangle::getEdgeExcluding(const std::vector<Edge *>& excludedEdges) {
    for (auto e: edges) {
        bool isExcluded = false;
        for (auto excludedEdge : excludedEdges) {
            if (excludedEdge->getLabel() == e->getLabel()) {
                isExcluded = true;
            }
        }
        if (!isExcluded) {
            return e;
        }
    }

    throw std::runtime_error("In `getEdgeExcluding` of Triangle: all edges are excluded");
}

bool Triangle::hasOneNeckAround(VertexLabel vertexLabel) {
    for (auto e : edges) {
        if (e->isOneNeck() and e->hasVertex(vertexLabel)) {
            return true;
        }
    }

    return false;
}

void Triangle::print(const std::string& symbolicName) {
    std::cout << symbolicName << " " << getLabel().toInt() << "\n";
    std::cout << "neighbors: ";
    for (auto n : neighbors) {
        std::cout << n->getLabel().toInt() << "  ";
    }
    std::cout << "\n";

    std::cout << "edges: ";
    for (auto e : edges) {
        std::cout << e->getLabel().toInt() << "  ";
    }
    std::cout << "\n";

    std::cout << "vertices: ";
    for (auto v : vertices) {
        std::cout << v->getLabel().toInt() << "  ";
    }
    std::cout << "\n";

    std::cout << "-------------------\n";
}

bool Triangle::hasNeighbor(TriangleLabel candidateLabel) {
    for (auto n : neighbors) {
        if (n->getLabel() == candidateLabel) {
            return true;
        }
    }

    return false;
}

bool Triangle::hasVertex(VertexLabel candidateVertex) {
    for (auto v : vertices) {
        if (v->getLabel() == candidateVertex) {
            return true;
        }
    }

    return false;
}

std::tuple<Edge *, Edge *> Triangle::getDegenerateEdgesExcluding(Edge *excludedEdge) {
    std::vector<Edge *> includedEdges;
    for (auto e : edges) {
        if (!(e->getLabel() == excludedEdge->getLabel())) {
            includedEdges.push_back(e);
        }
    }

    if (includedEdges.size() != 2) {
        throw std::runtime_error("In `getDegenerateEdgesExcluding` of Triangle: we are not left with 2 edges after excluding one");
    }

    if (includedEdges[0]->compareVertices(includedEdges[0])) {
        return {includedEdges[0], includedEdges[1]};
    } else {
        throw std::runtime_error("In `getDegenerateEdgesExcluding` of Triangle: edges left after excluding do not share the same set of vertices");
    }
}