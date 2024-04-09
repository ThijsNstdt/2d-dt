//
// Created by Thijs on 17-4-2023.
//

#ifndef INC_2D_DT_VERTEXGRAPHOBSERVER_HPP
#define INC_2D_DT_VERTEXGRAPHOBSERVER_HPP

#include "observer.hpp"

class VertexGraphObserver : public Observer {
public:
    explicit VertexGraphObserver(Universe *myUniverse);

    ~VertexGraphObserver() override;

    void measurement() override;

    void setFileName(const std::string &addition) override;

    std::string getFileName() override;

private:
    std::string baseName_ = "./simulations/results/vertex_graph/graph";
    std::string unnumberedFileName_;
    std::string numberedFileName_;

    unsigned int measurementNumber_ = 0;

    void setNumberedFileName();
};


#endif //INC_2D_DT_VERTEXGRAPHOBSERVER_HPP
