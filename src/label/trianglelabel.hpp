//
// Created by Thijs on 19-7-2022.
//

#ifndef INC_2D_DT_TRIANGLELABEL_HPP
#define INC_2D_DT_TRIANGLELABEL_HPP

#include "label.hpp"

class TriangleLabel : public Label {
public:
    explicit TriangleLabel(int label) : Label(label) {};

    bool operator==(TriangleLabel otherLabel) {
        return toInt() == otherLabel.toInt();
    }

};


#endif //INC_2D_DT_TRIANGLELABEL_HPP
