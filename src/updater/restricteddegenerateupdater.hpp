//
// Created by Thijs on 6-7-2022.
//

#ifndef INC_2D_DT_RESTRICTEDDEGENERATEUPDATER_HPP
#define INC_2D_DT_RESTRICTEDDEGENERATEUPDATER_HPP

#include "updater.hpp"
#include "src/universe/universe.hpp"

class RestrictedDegenerateUpdater : public Updater {
public:
    ~RestrictedDegenerateUpdater() override;

    RestrictedDegenerateUpdater(Universe* myUniverse);

    void edgeUpdate() override;

private:

    bool hasTwoNeck(Vertex* vCenter, Vertex* vOther);
};


#endif //INC_2D_DT_RESTRICTEDDEGENERATEUPDATER_HPP
