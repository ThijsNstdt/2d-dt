#include <stdexcept>
#include <utility>
#include <memory>
#include <fstream>

#include "universe.hpp"
#include "src/rng/global_engine.hpp"

Vertex* Universe::addVertex() {
    int labelValue = numberOfVertices();
    vertices.emplace_back( std::make_unique<Vertex>(labelValue) );
    return getVertex(VertexLabel(labelValue));
}

Triangle *Universe::getTriangle(TriangleLabel label) {
    int labelValue = label.toInt();
    return triangles[labelValue].get();
}

Edge* Universe::addEdge(Vertex *v1, Vertex *v2) {
    int labelValue = numberOfEdges();
    edges.emplace_back( std::make_unique<Edge>(labelValue, v1, v2) );
    return getEdge(EdgeLabel(labelValue));
}

Edge *Universe::getEdge(EdgeLabel label) {
    int labelValue = label.toInt();
    return edges[labelValue].get();
}

Triangle* Universe::addTriangle(Edge *e1, Edge *e2, Edge *e3, Triangle *t1, Triangle *t2, Triangle *t3) {
    int labelValue = numberOfTriangles();
    triangles.emplace_back( std::make_unique<Triangle>(labelValue, e1, e2, e3, t1, t2, t3) );
    return getTriangle(TriangleLabel(labelValue));
}

Vertex *Universe::getVertex(VertexLabel label) {
    int labelValue = label.toInt();
    return vertices[labelValue].get();
}

Triangle* Universe::addBlankTriangle() {
    int labelValue = numberOfTriangles();
    triangles.emplace_back( std::make_unique<Triangle>(labelValue) );
    return getTriangle(TriangleLabel(labelValue));
}

int Universe::numberOfTriangles() const {
    return static_cast<int>(triangles.size());
}

int Universe::numberOfEdges() const {
    return static_cast<int>(edges.size());
}

int Universe::numberOfVertices() const {
    return static_cast<int>(vertices.size());
}

Universe::Universe(Universe&& source) noexcept {
    vertices = std::move(source.vertices);
    edges = std::move(source.edges);
    triangles = std::move(source.triangles);

    uniformTriangleDistribution_ = source.uniformTriangleDistribution_;
    uniformEdgeDistribution_ = source.uniformEdgeDistribution_;
    uniformVertexDistribution_ = source.uniformVertexDistribution_;
}

bool Universe::triangleDataCheck() {
    for (auto & triangle : triangles) {
        auto condition1 = triangle->checkEdgeNeighborOrder();
        auto condition2 = triangle->checkVertexEdgeOrder();
        if ( !(condition1 && condition2) ) {
            return false;
        }
    }

    return true;
}

Triangle *Universe::getRandomTriangle() {
    auto labelValue = uniformTriangleDistribution_(global_engine)   ;
    TriangleLabel label(labelValue);
    return getTriangle(label);
}

Universe &Universe::operator=(Universe &&other) noexcept {
    if (this != &other) {
        vertices = std::move(other.vertices);
        edges = std::move(other.edges);
        triangles = std::move(other.triangles);
    }

    return *this;
}

Vertex *Universe::getRandomVertex() {
    auto labelValue = uniformVertexDistribution_(global_engine);
    VertexLabel label(labelValue);
    return getVertex(label);
}

void Universe::setDistributions() {
    uniformTriangleDistribution_ = std::uniform_int_distribution<int> (0, numberOfTriangles()-1 );
    uniformEdgeDistribution_ = std::uniform_int_distribution<int> (0, numberOfEdges()-1 );
    uniformVertexDistribution_ = std::uniform_int_distribution<int> (0, numberOfVertices()-1 );
}

bool Universe::labelCheck() {
    bool vertexLabelCheck = true;
    bool edgeLabelCheck = true;
    bool triangleLabelCheck = true;

    for (int i = 0; i < numberOfVertices(); ++i) {
        if (vertices[i]->getLabel().toInt() != i) {
            vertexLabelCheck = false;
            break;
        }
    }

    for (int i = 0; i < numberOfEdges(); ++i) {
        if (edges[i]->getLabel().toInt() != i) {
            edgeLabelCheck = false;
            break;
        }
    }

    for (int i = 0; i < numberOfTriangles(); ++i) {
        if (triangles[i]->getLabel().toInt() != i) {
            triangleLabelCheck = false;
            break;
        }
    }

    return ( vertexLabelCheck && edgeLabelCheck && triangleLabelCheck );
}

void Universe::writeVertexGraph(std::string &filename) const {
    std::ofstream outFile(filename, std::ofstream::app);

    if (!outFile.is_open()) {
        throw std::runtime_error("In 'writeVertexGraph' of Universe: problem opening file");
    }

    for (const auto &vertex : vertices) {
        outFile << vertex->getCSVOutput();
    }
}
