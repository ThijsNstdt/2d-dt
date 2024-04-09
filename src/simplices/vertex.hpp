//
// Created by Thijs on 19-7-2022.
//

#ifndef INC_2D_DT_VERTEX_HPP
#define INC_2D_DT_VERTEX_HPP

#include <memory>
#include <vector>

#include "src/label/vertexlabel.hpp"
#include "src/label/edgelabel.hpp"
#include "src/simplices/simplex.hpp"
//#include "src/structs/vertexedgetuple.hpp"

class Vertex : public Simplex {
public:
    class VertexEdgeTuple {
    private:
        std::tuple<Vertex*, EdgeLabel> vertexEdgeTuple;
    public:
        VertexEdgeTuple(Vertex* v, EdgeLabel e);

        [[nodiscard]] Vertex* getVertex() const;

        [[nodiscard]] EdgeLabel getEdgeLabel() const;

        bool operator==(VertexEdgeTuple otherTuple) const;

        bool operator!=(VertexEdgeTuple otherTuple) const;
    };

private:

    const unsigned int reservedNeighborSize = 10;

    const VertexLabel label;

    std::vector<VertexEdgeTuple> neighbors = {};

public:
    explicit Vertex(int labelValue);

    [[nodiscard]] inline VertexLabel getLabel() const {
        return label;
    }

    void print(const std::string& symbolicName) const;

    void addNeighbor(Vertex* v, EdgeLabel e);

    void removeNeighbor(VertexEdgeTuple toRemoveTuple);

    void removeNeighbor(Vertex* v, EdgeLabel e);

    [[nodiscard]] bool hasNeighbor(VertexEdgeTuple candidate) const;

    [[nodiscard]] unsigned int getDegree() const;

    bool operator==(const Vertex& other) const;

    [[nodiscard]] std::vector<VertexEdgeTuple> getNeighbors() const;

    std::string getCSVOutput() const;
};

#endif //INC_2D_DT_VERTEX_HPP
