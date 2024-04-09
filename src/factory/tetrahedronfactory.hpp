#ifndef _TETRAHEDRONFACTORY_H
#define _TETRAHEDRONFACTORY_H


#include <vector>

#include "factory.hpp"
#include "src/universe/universe.hpp"


class TetrahedronFactory : public Factory {
public:
    Universe createUniverse(int size);

    ~TetrahedronFactory() override = default;

private:
    static std::vector<Triangle> makeTriangles(int size);

    int beginLabel(int size, int face, int row);

    int endLabel(int size, int face, int row);

    void faceSetup(std::vector<Triangle> &universe,
                   int size,
                   int face,
                   std::vector<int> leftAdjacency,
                   std::vector<int> rightAdjacency,
                   std::vector<int> downAdjacency);

    void pairSetup(std::vector<Triangle> &universe,
                   int downLabel,
                   std::vector<int> rhombusNbr,
                   std::vector<int> rhombusVertex);


};

#endif