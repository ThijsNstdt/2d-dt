//
// Created by Thijs on 19-7-2022.
//

#ifndef INC_2D_DT_VERTEXLABEL_HPP
#define INC_2D_DT_VERTEXLABEL_HPP

#include <memory>

#include "label.hpp"

class VertexLabel : public Label {
private:

public:
    VertexLabel(int labelValue) : Label(labelValue) {};

    bool operator==(VertexLabel otherLabel) {
        return toInt() == otherLabel.toInt();
    }
};


#endif //INC_2D_DT_VERTEXLABEL_HPP
