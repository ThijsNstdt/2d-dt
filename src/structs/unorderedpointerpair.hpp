//
// Created by Thijs on 20-7-2022.
//

#ifndef INC_2D_DT_UNORDEREDPOINTERPAIR_HPP
#define INC_2D_DT_UNORDEREDPOINTERPAIR_HPP

#include <array>
#include <memory>
#include <iostream>
#include <stdexcept>

/**
 * this doesn't work
 * @tparam t
 */

template<typename t>
class UnorderedPointerPair {
private:
    std::shared_ptr<t> first;
    std::shared_ptr<t> second;

    void sort() {
        bool swapped = (*first > *second);
        if (swapped) {
            first.swap(second);
        }
    }

public:

    UnorderedPointerPair(std::shared_ptr<t>& x, std::shared_ptr<t>& y) : first(std::move(x)), second(std::move(y)) {
        throw std::invalid_argument("Unordered Pointer Pair should not be used in its current form");
        sort();
    };

    bool pointsToValue(const t& element) {
        if (*first == element || *second == element) {
            return true;
        } else {
            return false;
        }
    }

    void replaceOnePointerTo(const t& current, const t& next) {
        if(!pointsToValue(current)) {
            throw std::invalid_argument("In `replace` of struct UnorderedPointerPair: current is not pointed to by pair");
        } else if (*first == current) {
            first = std::make_shared<t>(next);
            return;
        } else if (*second == current) {
            second = std::make_shared<t>(next);
        }
    }

    std::array<std::shared_ptr<t>, 2> toArray() {
        std::array<std::shared_ptr<t>, 2> myArray;
        myArray[0] = first;
        myArray[1] = second;
        return myArray;
    }
};


#endif //INC_2D_DT_UNORDEREDPOINTERPAIR_HPP
