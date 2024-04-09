//
// Created by thijs on 27/05/2022.
//

#ifndef INC_2D_DT_EDGE_HPP
#define INC_2D_DT_EDGE_HPP

#include <array>
#include <tuple>
#include <memory>

#include "src/structs/unorderedpointerpair.hpp"
#include "src/label/edgelabel.hpp"
#include "vertex.hpp"
#include "src/simplices/simplex.hpp"
//#include "triangle.hpp"

class Edge : public Simplex {
private:
    std::array<Vertex*, 2> vertices;

    const EdgeLabel label;

    bool oneNeckStatus = false;

public:
    Edge(const int& labelValue, Vertex* v1, Vertex* v2);

    bool hasVertex(VertexLabel vLabel);

    EdgeLabel getLabel() {
        return label;
    }

    std::tuple<Vertex*, Vertex*> getVertices();

    Vertex* getOtherVertex(VertexLabel myVertexLabel);

    void setVertices(Vertex* v1, Vertex* v2);

    bool compareVertices(Edge *otherEdge);

    [[nodiscard]] bool isOneNeck() const;

    void print(const std::string& symbolicName);
};


#endif //INC_2D_DT_EDGE_HPP
