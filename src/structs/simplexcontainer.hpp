//
// Created by Thijs on 21-7-2022.
//

#ifndef INC_2D_DT_SIMPLEXCONTAINER_HPP
#define INC_2D_DT_SIMPLEXCONTAINER_HPP

#include <array>
#include <memory>

#include "src/simplices/simplex.hpp"

template <unsigned int s>
class SimplexPointerContainer {
private:
    std::array<std::shared_ptr<Simplex>, s> m_simplexPointers;
public:
    explicit SimplexPointerContainer(std::array<std::shared_ptr<Simplex>, s> simplexPointers) : m_simplexPointers(simplexPointers) {}


};


#endif //INC_2D_DT_SIMPLEXCONTAINER_HPP
