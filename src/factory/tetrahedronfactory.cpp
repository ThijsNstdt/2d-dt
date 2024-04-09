#include <vector>
#include <array>

#include "tetrahedronfactory.hpp"
#include "src/simplices/triangle.hpp"
#include "src/universe/universe.hpp"

Universe TetrahedronFactory::createUniverse(int size) {
    std::vector<Triangle> myTriangles = makeTriangles(size);

    int face;
    std::vector<int> leftAdjacency;
    std::vector<int> rightAdjacency;
    std::vector<int> downAdjacency;


    face = 0;
    for (int i = 0; i < myTriangles.size(); i++) {
        leftAdjacency.push_back(endLabel(size, 2, i));
        rightAdjacency.push_back(beginLabel(size, 1, i));
        downAdjacency.push_back(beginLabel(size, 3, 0) + i);
    }
    faceSetup(myTriangles, size, face, leftAdjacency, rightAdjacency, downAdjacency);

    face = 1;
    for (int i = 0; i < myTriangles.size(); i++) {
        leftAdjacency.push_back(endLabel(size, 0, i));
        rightAdjacency.push_back(beginLabel(size, 2, i));
        downAdjacency.push_back(endLabel(size, 3, i));
    }
    faceSetup(myTriangles, size, face, leftAdjacency, rightAdjacency, downAdjacency);

    face = 2;
    for (int i = 0; i < myTriangles.size(); i++) {
        leftAdjacency.push_back(endLabel(size, 1, i));
        rightAdjacency.push_back(beginLabel(size, 0, i));
        downAdjacency.push_back(beginLabel(size, 3, size - 1 - i));
    }
    faceSetup(myTriangles, size, face, leftAdjacency, rightAdjacency, downAdjacency);


    // TODO: vertices on the edges of this face are not correct.
    face = 3;
    for (int i = 0; i < myTriangles.size(); i++) {
        leftAdjacency.push_back(beginLabel(size, 2, 0) + size - 1 - i);
        rightAdjacency.push_back(beginLabel(size, 1, 0) + i);
        downAdjacency.push_back(beginLabel(size, 0, 0) + i);
    }
    faceSetup(myTriangles, size, face, leftAdjacency, rightAdjacency, downAdjacency);

    //temporary, to return something
    std::vector<int> vertexDegrees = {};
    Universe myUniverse(4 * size * size, myTriangles, vertexDegrees);
    return myUniverse;
}

std::vector<Triangle> TetrahedronFactory::makeTriangles(int size) {
    std::vector<Triangle> myTriangles(4 * size * size, Triangle(0));
    return myTriangles;
}

int TetrahedronFactory::beginLabel(int size, int face, int row) {
    return 2 * size * row - row + face * size * size;
}

int TetrahedronFactory::endLabel(int size, int face, int row) {
    return beginLabel(size, face, row + 1) - 1;
}

void TetrahedronFactory::pairSetup(std::vector<Triangle> &myTriangles,
                                   int downLabel,
                                   std::vector<int> rhombusNbr,
                                   std::vector<int> rhombusVertex) {
    // setup all properties of the down triangle
    std::array<int, 3> downNbr = {
            rhombusNbr[0],
            downLabel + 1,
            rhombusNbr[3]
    };

    std::array<int, 3> downVertex = {
            rhombusVertex[0],
            rhombusVertex[2],
            rhombusVertex[3]
    };

    myTriangles[downLabel].setLabel(downLabel);
    myTriangles[downLabel].setNbr(downNbr);
    myTriangles[downLabel].setVertex(downVertex);

    // setup all properties of the up triangle
    int upLabel = downLabel + 1;

    std::array<int, 3> upNbr = {
            downLabel,
            rhombusNbr[1],
            rhombusNbr[2]
    };

    std::array<int, 3> upVertex = {
            rhombusVertex[0],
            rhombusVertex[1],
            rhombusVertex[2]
    };

    myTriangles[upLabel].setLabel(upLabel);
    myTriangles[upLabel].setNbr(upNbr);
    myTriangles[upLabel].setVertex(upVertex);

    return;
}

void TetrahedronFactory::faceSetup(std::vector<Triangle> &myTriangles,
                                   int size,
                                   int face,
                                   std::vector<int> leftAdjacency,
                                   std::vector<int> rightAdjacency,
                                   std::vector<int> downAdjacency) {
    // setup all but the highest layer
    for (int i = 0; i < size - 1; i++) {
        // setup first triangle in each row
        int firstLabel = beginLabel(size, face, i);

        // make array of neighbors for said triangle
        std::array<int, 3> firstNbr = {
                leftAdjacency[i],
                beginLabel(size, face, i - 1),
                firstLabel + 1
        };

        if (i == 0) {
            // make exception for triangle of first row
            firstNbr[1] = downAdjacency[0];
        }

        // make array of vertices for said triangle
        std::array<int, 3> firstVertex = {
                leftAdjacency[i + 1],
                leftAdjacency[i],
                firstLabel
        };

        // set member variables for triangle using arrays from above
        myTriangles[firstLabel].setLabel(firstLabel);
        myTriangles[firstLabel].setNbr(firstNbr);
        myTriangles[firstLabel].setVertex(firstVertex);

        for (int k = 0; k < 2 * size - 2 * i - 1; k += 2) {
            int downLabel = beginLabel(size, face, i) + k;

            std::vector<int> rhombusNbr = {
                    beginLabel(size, face, i + 1) + k - 1,
                    downLabel + 2,
                    beginLabel(size, face, i - 1) + k + 2,
                    downLabel - 1
            };

            std::vector<int> rhombusVertex = {
                    beginLabel(size, face, i + 1) + k - 1,
                    downLabel + 1,
                    downLabel - 1,
                    beginLabel(size, face, i + 1) + k - 3,
            };

            if (i == 0) {
                // make exception for bottom of rhombuses of first row
                rhombusNbr[2] = downAdjacency[(k + 1) / 2];
            }

            if (k == 1) {
                // overwrite what is different for first rhombus
                rhombusVertex[3] = leftAdjacency[i + 1];
            } else if (k == 2 * size - 2 * i - 3) {
                // overwrite what is different for last rhombus
                rhombusNbr[1] = rightAdjacency[i];
            }

            pairSetup(myTriangles, downLabel, rhombusNbr, rhombusVertex);


        }
    }

    // setup top triangle
    int topLabel = beginLabel(size, face, size - 1);

    std::array<int, 3> topNbr = {
            leftAdjacency[size - 1],
            rightAdjacency[size - 1],
            beginLabel(size, face, size - 2) + 1
    };

    int peakVertex;
    if (face == 3) {
        peakVertex = endLabel(size, 1, 0);
    } else {
        peakVertex = 4 * size * size; // TODO: check whether this is correct
    }

    std::array<int, 3> topVertex = {
            leftAdjacency[size - 1],
            topLabel,
            peakVertex
    };

    return;
}