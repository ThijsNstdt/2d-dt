//
// Created by Thijs on 19-7-2022.
//

#ifndef INC_2D_DT_EDGELABEL_HPP
#define INC_2D_DT_EDGELABEL_HPP

#include "label.hpp"

class EdgeLabel : public Label {
public:
    explicit EdgeLabel(int label) : Label(label) {};

    bool operator==(EdgeLabel otherLabel) {
        return toInt() == otherLabel.toInt();
    }

};


#endif //INC_2D_DT_EDGELABEL_HPP
