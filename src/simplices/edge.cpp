//
// Created by thijs on 27/05/2022.
//

#include <array>
#include <bits/stdc++.h>
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include "edge.hpp"


Edge::Edge(const int &labelValue, Vertex* v1, Vertex* v2) : label(labelValue), vertices{v1, v2} {}

bool Edge::hasVertex(VertexLabel vLabel) {
    for(auto myVertex : vertices) {
        if (myVertex->getLabel() == vLabel) {
            return true;
        }
    }
    return false;
}

std::tuple<Vertex *, Vertex *> Edge::getVertices() {
    return {vertices[0], vertices[1]};
}

Vertex *Edge::getOtherVertex(VertexLabel myVertexLabel) {

    if (vertices[0]->getLabel()==myVertexLabel) {
        return vertices[1];
    }

    if (vertices[1]->getLabel()==myVertexLabel) {
        return vertices[0];
    }

    throw std::invalid_argument("In `getOtherVertex` of Edge: argument vertex is not part of edge");
}

void Edge::setVertices(Vertex *v1, Vertex *v2) {
    vertices = {v1, v2};
    if (v1->getLabel() == v2->getLabel()) {
        oneNeckStatus = true;
    } else {
        oneNeckStatus = false;
    }
}

bool Edge::compareVertices(Edge *otherEdge) {
    if (vertices[0]->getLabel() == otherEdge->vertices[0]->getLabel()) {
        return vertices[1]->getLabel() == otherEdge->vertices[1]->getLabel();
    } else if (vertices[0]->getLabel() == otherEdge->vertices[1]->getLabel()) {
        return vertices[1]->getLabel() == otherEdge->vertices[0]->getLabel();
    } else {
        return false;
    }
}

bool Edge::isOneNeck() const {
    return oneNeckStatus;
}

void Edge::print(const std::string& symbolicName) {
    std::cout << symbolicName << " " << getLabel().toInt() << "\n";

    std::cout << "vertices: ";
    for (auto v : vertices) {
        std::cout << v->getLabel().toInt() << "  ";
    }
    std::cout << "\n";

    std::cout << "one-neck status: " << oneNeckStatus << "\n";

    std::cout << "-------------------\n";
}



//
//bool Edge::operator==(Edge edge) {
//    auto myVertices = m_vertices;
//    std::sort(myVertices.begin(), myVertices.end());
//    auto otherVertices = edge.m_vertices;
//    std::sort(otherVertices.begin(), otherVertices.end());
//    return myVertices == otherVertices;
//}
//
//bool Edge::vertexInEdge(int vertex) {
//    bool vertexFound = std::find(m_vertices.begin(), m_vertices.end(), vertex) != m_vertices.end();
//    return vertexFound;
//}
//
//std::tuple<int, int> Edge::getVertices() {
//    return {m_vertices[0], m_vertices[1]};
//}
//
//void Edge::printEdge() {
//    std::cout << "--- Edge ---\n";
//    for (int vertexLabel : m_vertices) {
//        std::cout << "vertex: " << vertexLabel << "\n";
//    }
//    std::cout << "-----------\n";
//}
//
//int Edge::otherVertexLabel(int myVertexLabel) {
//    for (int candidateLabel : m_vertices) {
//        if (candidateLabel != myVertexLabel) {
//            return candidateLabel;
//        }
//    }
//    throw std::invalid_argument("In `otherVertexLabel` of Edge: myVertexLabel is twice in edge");
//}
//
//const std::unordered_set<Vertex> &Edge::getVertices() const {
//    return m_vertices;
//}
