//
// Created by Thijs on 20-7-2022.
//

#ifndef INC_2D_DT_UNORDEREDPAIR_HPP
#define INC_2D_DT_UNORDEREDPAIR_HPP

#include <utility>
#include <algorithm>
#include <stdexcept>

template <typename t>
class UnorderedPair : public std::pair<t, t> {
private:

    // sort the elements
    void sort() {
        bool swapped = this->first > this->second;
        if (swapped) {
            std::swap(this->first, this->second);
        }
    }

    // compare by comparing sorted versions of instances
    std::strong_ordering operator<=>(const UnorderedPair<t>& other) {
        return *this <=> other;
    }

public:
    UnorderedPair(t x, t y) : std::pair<t, t>(x,y) {
        sort();
    };

    bool contains(const t& element) {
        if (this->first == element || this->second == element) {
            return true;
        } else {
            return false;
        }
    }

    void replace(const t& current, const t& next) {
        if(!this->contains(current)) {
            throw std::invalid_argument("In `replace` of struct UnorderedPair: current is not an element of pair");
        } else if (this->first == current) {
            this->first = next;
        } else if (this->second == current) {
            this->second = next;
        }
        sort();
    }

};


#endif //INC_2D_DT_UNORDEREDPAIR_HPP
