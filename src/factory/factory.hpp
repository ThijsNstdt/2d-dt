#ifndef FACTORY_H
#define FACTORY_H

#include <iostream>
#include "src/universe/universe.hpp"

class Factory {
public:
    virtual ~Factory() = default;

    virtual Universe && createUniverse(int size) = 0;

    enum InitGeometry {
        Tetrahedron = 0,
        Growth
    };

    static InitGeometry strToInitGeom(const std::string &string);

    static std::string initGeomToStr(const InitGeometry &geom);
};

#endif