//
// Created by thijs on 27/05/2022.
//

#ifndef INC_2D_DT_UPDATER_HPP
#define INC_2D_DT_UPDATER_HPP

#include <iostream>

#include "src/simplices/edge.hpp"
#include "src/universe/universe.hpp"

class Updater {
public:
    virtual ~Updater();

    Updater(Universe *myUniverse);

    // TODO: all derived classes can use this one instead of all the overrides
    virtual void sweepUpdate(int numberOfSweeps);

    virtual void seriesOfUpdates(int numberOfUpdates);

    [[nodiscard]] int getRejections() const {return m_rejections;}
    void addRejection() {m_rejections++;}

    [[nodiscard]] float getRejectionRate() {
        updateRejectionRate();
        return m_rejectionRate;
    }
    void updateRejectionRate() {
        m_rejectionRate = static_cast<float>(m_rejections)/static_cast<float>(m_totalUpdates);
    }

    [[nodiscard]] std::uint64_t getTotalUpdates() const {return m_totalUpdates;}
    void addUpdate() {m_totalUpdates++;}

    void resetCounters() {
        m_totalUpdates = 0;
        m_rejections = 0;
        m_rejectionRate = 0;
    }

    enum Ensemble {
        Simplicial,
        RestrictedDegenerate,
        MaximallyDegenerate
    };

    static Ensemble strToEnsemble(const std::string &str);

    static std::string ensembleToStr(const Ensemble &ens);

protected:
    virtual void edgeUpdate();

    Universe* myUniverse;

    int m_rejections {0};

    float m_rejectionRate {0};

    std::uint64_t m_totalUpdates {0};

    void neighborhoodIdentification();

    Triangle * aTriangle;
    Triangle * bTriangle;
    Triangle * cTriangle;
    Triangle * dTriangle;
    Triangle * eTriangle;
    Triangle * fTriangle;

    Edge * vEdge;
    Edge * wEdge;
    Edge * xEdge;
    Edge * yEdge;
    Edge * zEdge;

    Vertex * aVertex;
    Vertex * bVertex;
    Vertex * cVertex;
    Vertex * dVertex;

    bool degreeCheck(int lowestPermissibleDegree);

    void regularFlip();

    void printCurrentIdentification();

    void printIdentificationOfFlip(int updateNr);
};

#endif //INC_2D_DT_UPDATER_HPP
