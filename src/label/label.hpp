//
// Created by Thijs on 19-7-2022.
//

#ifndef INC_2D_DT_LABEL_HPP
#define INC_2D_DT_LABEL_HPP

#include <utility>
#include <algorithm>
#include <string>


class Label {
private:
    int m_labelValue;
public:
    explicit Label(int label) : m_labelValue(label) {};

    void setLabel(int label) { m_labelValue = label;}

    [[nodiscard]] inline int toInt() const {return m_labelValue;}

    [[nodiscard]] std::string toStr() const {return std::to_string(toInt());}

};


#endif //INC_2D_DT_LABEL_HPP
