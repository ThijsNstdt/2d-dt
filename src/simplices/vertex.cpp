//
// Created by Thijs on 19-7-2022.
//

#include "vertex.hpp"

void Vertex::print(const std::string& symbolicName) const {
    std::cout << symbolicName << " " << getLabel().toInt() << "\n";
    std::cout << "-------------------\n";
}

void Vertex::addNeighbor(Vertex* v, EdgeLabel e) {
    // adding neighbor vertex through same edge is redundant, these are equivalent in any of the ensembles used.
    if ( hasNeighbor(VertexEdgeTuple(v, e)) ) {
        return;
    }

    neighbors.emplace_back(v, e);
}

void Vertex::removeNeighbor(VertexEdgeTuple toRemoveTuple) {
    auto last = std::remove(neighbors.begin(), neighbors.end(), toRemoveTuple);
    neighbors.erase(last, neighbors.end());
}

bool Vertex::hasNeighbor(VertexEdgeTuple candidate) const {
    return std::find(neighbors.begin(), neighbors.end(), candidate) != neighbors.end();
}

unsigned int Vertex::getDegree() const {
    return static_cast<int>(neighbors.size());
}

Vertex::Vertex(int labelValue) : label(labelValue) {
    neighbors.reserve(reservedNeighborSize);
}

bool Vertex::operator==(const Vertex& other) const {
    return getLabel() == other.getLabel();
}

void Vertex::removeNeighbor(Vertex *v, EdgeLabel e) {
    removeNeighbor(VertexEdgeTuple(v, e));
}

std::vector<Vertex::VertexEdgeTuple> Vertex::getNeighbors() const {
    return neighbors;
}

std::string Vertex::getCSVOutput() const {
    std::string output = label.toStr();

    for (auto tuple : neighbors) {
        output += "," + tuple.getVertex()->label.toStr();
    }

    output += "\n";

    return output;
}

Vertex* Vertex::VertexEdgeTuple::getVertex() const {
    return std::get<0>(vertexEdgeTuple);
}

EdgeLabel Vertex::VertexEdgeTuple::getEdgeLabel() const {
    return std::get<1>(vertexEdgeTuple);
}

bool Vertex::VertexEdgeTuple::operator==(VertexEdgeTuple otherTuple) const {
    // compare vertex- and edge labels
    bool vertexEquality = *getVertex() == *otherTuple.getVertex();
    bool edgeEquality = getEdgeLabel() == otherTuple.getEdgeLabel();
    return (vertexEquality and edgeEquality);
}

bool Vertex::VertexEdgeTuple::operator!=(VertexEdgeTuple otherTuple) const {
    return !(*this == otherTuple);
}

Vertex::VertexEdgeTuple::VertexEdgeTuple(Vertex* v, EdgeLabel e) : vertexEdgeTuple(v,e) {}
