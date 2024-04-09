//
// Created by thijs on 21/09/2022.
//

#ifndef INC_2D_DT_PROGRESSMANAGER_HPP
#define INC_2D_DT_PROGRESSMANAGER_HPP

#include <string>
#include <chrono>

class ProgressManager {
private:
    const int barWidth = 70;

    std::chrono::time_point<std::chrono::system_clock> start;

    float progress = 0.0;
    int numberOfSteps = 1;
    std::string name;

    void updateBar() const;

    void talk() const;

    void minimalUpdate() const;

public:
    ProgressManager() = default;

    void startProcess(const std::string& processName = " ", int myNumber = 1);

    void takeStep() {
        progress += static_cast< float >(1)/static_cast< float >(numberOfSteps);
        talk();
    }

    void endProcess();

    double getTime();

    void startTimer();

    enum VerbosityLevels {
        Silent,
        Minimal,
        Full
    };

    void setVerbosityLevel(VerbosityLevels level) {verbosity_ = level;}

private:
    VerbosityLevels verbosity_ = Full;
};


#endif //INC_2D_DT_PROGRESSMANAGER_HPP
