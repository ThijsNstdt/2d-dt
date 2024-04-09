//
// Created by thijs on 15/06/2022.
//

#ifndef INC_2D_DT_OBSERVER_HPP
#define INC_2D_DT_OBSERVER_HPP

#include <string>
#include <queue>

#include "src/universe/universe.hpp"

class Observer {
public:
    virtual ~Observer();

    explicit Observer(Universe *myUniverse);

    virtual void measurement() {};

    virtual void writeVector(const std::vector<int> &data) {};
    virtual void writeVector(const std::vector<unsigned int> &data) {};

    virtual void setFileName(const std::string &addition);

    virtual std::string getFileName() {return "method called for base class, no filename here";}

    enum Observable {
        DualHausdorffDimension,
        VertexDegree,
        HausdorffDimension,
        AverageSphereDistance,
        SelfAverageSphereDistance,
        VertexGraphObserver
    };

    virtual void loadSettingsFromConfig(const std::string& configLoc) {};

    static Observable strToObs(const std::string& string);

    static std::string obsToStr(const Observable &obs);

protected:
    std::string baseName_ = " ";

    std::string fileName_;

    Universe *myUniverse;


    class SphereGrower {
    public:
        SphereGrower(const Vertex *startingVertex, Universe *universe);

        void growToRadius(const unsigned int& radius);

        void growToNextRadius();

        void growToVertex(const Vertex* vertex);

        void growToEmptyShell();

        [[maybe_unused]] const Vertex *startingVertex_;

        std::deque<const Vertex*> currentLayer_ = {};

        std::vector<unsigned int> shellSizes_ = {};

        const Vertex *getRandomFromShell();

        [[nodiscard]] unsigned int getRadius() const;

        [[nodiscard]] unsigned int getCircumference() const;

        [[nodiscard]] std::vector<const Vertex *> currentLayerVector() const;

    protected:
        Universe *universe_;

        std::vector<bool> visited_ = std::vector<bool>(universe_->numberOfVertices(), false);

        bool isVisited(const Vertex* vertex) const;

        void setVisited(const Vertex* vertex);

        std::vector<const Vertex*> nextLayerNeighbors(const Vertex* vertex);

        std::deque<const Vertex *> nextLayer();
    };

    class DistanceMeasurer : public SphereGrower {
    public:
        DistanceMeasurer(const Vertex *startingVertex, Universe *universe, const std::vector<const Vertex *>& distantVertexObject);

        DistanceMeasurer(const Vertex *startingVertex, Universe *universe, const SphereGrower& otherSphere);

        std::vector<unsigned int> measureDistances();
        std::vector<unsigned int> measureDistances(const unsigned int &maxRadius);

        double measureAverageDistance();
        double measureAverageDistance(const unsigned int &maxRadius);

        void growToNextRadiusWithDetection();

        void setUnwanted(const Vertex* vertex);

    private:
        std::vector<bool> wanted_ = std::vector<bool>(universe_->numberOfVertices(), false);

        const std::vector<const Vertex *> distantVertexObject_;

        std::vector<unsigned int> distances_;

        bool isWanted(const Vertex* vertex) const;

        std::vector<const Vertex*> nextLayerNeighborsWithDetection(const Vertex* vertex);

        std::deque<const Vertex *> nextLayerWithDetection();
    };
};


#endif //INC_2D_DT_OBSERVER_HPP
