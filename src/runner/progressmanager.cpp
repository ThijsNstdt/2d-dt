//
// Created by thijs on 21/09/2022.
//
#include <iostream>

#include "progressmanager.hpp"

void ProgressManager::updateBar() const {
    std::cout << "\r[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %";
    std::cout.flush();
}

void ProgressManager::endProcess() {
    if (verbosity_ == Full) {
        // replace in-progress bar with a full one
        std::cout << "\r[";
        for (int i = 0; i < barWidth; ++i) {
            std::cout << "=";
        }
        std::cout << "] " << 100 << " %" << std::flush;
        std::cout << "\n";
    }

    if (verbosity_ == Minimal) {
        std::cout << "Progress is 100%" << "\n";
    }

    // on new line report on the finished process
    std::cout << name << " done in " << getTime() << " s.";
    for (int i = 0; i < barWidth; ++i) {
        std::cout << " ";
    }
    std::cout << "\n" << "\n";
}

void ProgressManager::startProcess(const std::string& processName, int myNumber) {
    progress = 0;
    name = processName;
    numberOfSteps = myNumber;
    std::cout << processName << ":" << "\n" << std::flush;
    startTimer();
}

double ProgressManager::getTime() {
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    return elapsed_seconds.count();
}

void ProgressManager::startTimer() {
    start = std::chrono::system_clock::now();
}

void ProgressManager::talk() const {
    if (verbosity_ == Full) {
        updateBar();
        return;
    }

    if (verbosity_ == Minimal) {
        minimalUpdate();
        return;
    }

    if (verbosity_ == Silent) {
        // intentionally left empty
        return;
    }
}

void ProgressManager::minimalUpdate() const {
    float previous_progress = progress - static_cast< float >(1)/static_cast< float >(numberOfSteps);

    int subdivisions = 5;

    for (int i = 1; i < subdivisions+1; ++i) {
        float threshold = static_cast<float>(i)/static_cast<float>(subdivisions);

        if ( (previous_progress < threshold) && (progress >= threshold) ) {
            std::cout << "Progress is " << static_cast<int>(100*threshold) << "%" << "\n";
        }
    }

    std::cout.flush();
}
